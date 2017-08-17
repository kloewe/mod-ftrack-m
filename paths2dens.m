function d = paths2dens(p,varargin)
%PATHS2DENS Compute path density map.
%
%   D = PATHS2DENS(P) computes the path density map D corresponding to
%   the paths in P, where P is a structure as generated by FTRACK.
%
%   See also: READPATHS, WRITEPATHS, FILTPATHS, FTRACK.
%
%   Author: Kristian Loewe

%% defaults
distcorr = 'none';
doNormalize = 0;

%% parse arguments
assert(nargin >= 1 && mod(numel(varargin),2) == 0);
assert(nargout == 1);

assert(isstruct(p));
assert(all(isfield(p, {'paths','dims','vsiz'})));
assert(iscell(p.paths));
assert(isempty(p.paths) || isa(p.paths{1}, 'int32'));
% assert(all(cellfun(@(x) strcmp(class(x), 'int32'), p.paths)));
assert(isnumeric(p.dims) && numel(p.dims) == 3);
assert(isnumeric(p.vsiz) && numel(p.vsiz) == 3);

for i = 1:2:numel(varargin)
  pn = varargin{i};   % parameter name
  assert(ischar(pn), 'Parameter names must be of type char.');
  pv = varargin{i+1}; % parameter value
  switch pn
    case 'DistCorr'
      assert(ismember(pv, {'none','linear','quadratic'}));
      distcorr = pv;
    case 'Normalize'
      assert(isscalar(pv) && isnumeric(pv) && ismember(pv, [0 1]));
      doNormalize = pv;
    otherwise
      error('Unexpected parameter name.');
  end
end

%%
d = zeros(p.dims, 'single');
if strcmp(distcorr, 'none')
  if 0
    a = cell2mat(p.paths);
    for i = 1:numel(a)
      d(a(i)) = d(a(i)) + 1;
    end
  else
    d = mxPaths2dens(p.paths, int32(p.dims));
  end
else
  a = cell2mat(p.paths);
  len2 = pathProps(p, 'Length2');
  b = cell2mat(len2);
  if strcmp(distcorr, 'linear')
    for i = 1:numel(a)
      d(a(i)) = d(a(i)) + b(i);
    end
  elseif strcmp(distcorr, 'quadratic')
    for i = 1:numel(a)
      d(a(i)) = d(a(i)) + b(i)^2;
    end
  else
    error('Unexpected parameter value for ''distcorr''.');
  end
end

if doNormalize
  d = d./sum(d(:));
end

d = single(d);

end