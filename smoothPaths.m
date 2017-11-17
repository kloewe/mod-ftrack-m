function xyzp = smoothPaths(p,fac)
%SMOOTHPATHS Smooth paths using splines.
%
%   COUT = SMOOTHPATHS(P) smooths the paths in P by fitting for each path a
%   smoothing spline for each dimension separately. COUT is a cell array of
%   coordinates (each cell contains the 3D coordinates of one path).
%
%   COUT = SMOOTHPATHS(CIN) specifies the input paths using the cell array of
%   coordinates CIN.
%
%   COUT = SMOOTHPATHS(T) specifies the input paths using T, which is a
%   structure of the kind returned by READ_MRTRIX_TRACKS.
%
%   Author: Kristian Loewe

if ~exist('fac','var')
  fac = 1;
end

if isstruct(p)
  if all(isfield(p, {'paths','dims','vsiz'}))
    coords = pathProps(p, 'Coords');           % input: ftrack format
  elseif all(isfield(p, {'datatype','count','data'}))
    coords = p.data;                           % input: mrtrix format
  end
elseif iscell(p)
  assert(all(cellfun(@isnumeric, p)));         % input: cell array of coords
  coords = p;
else
  error('Unexpected input argument.');
end
assert(iscell(coords));

xyzp = cell(numel(coords), 1);

if isempty(coords)
  return
end

% transpose arrays if necessary
assert(all(cellfun(@(c) any(size(c) == 3), coords)));
idx = find(cellfun(@(c) sum(size(c) ~= 3), coords), 1);
fmt = find(size(coords{idx}) ~= 3);
if fmt == 1
  coords = cellfun(@(c) c', coords, 'UniformOutput', false);
end

% fit a smoothing spline for each dimension
maxDistSE = sqrt(sum((coords{1}(:,2) - coords{1}(:,1)).^2)) / 2;
for i = 1:numel(coords)
  xyz = double(coords{i});
  [ndim,npts] = size(xyz);
  xyzp{i} = zeros(size(xyz,1),fac*npts);
  for k=1:ndim
    xyzp{i}(k,:) = ppval(csaps(1:npts, xyz(k,:)), linspace(1,npts,fac*npts));
  end
  assert(sqrt(sum((xyz(:,1)   - xyzp{i}(:,1)).^2))   < maxDistSE);
  assert(sqrt(sum((xyz(:,end) - xyzp{i}(:,end)).^2)) < maxDistSE);
end
% This is based on the answer by Bryan Howell found here:
%   https://de.mathworks.com/matlabcentral/answers/
%     220095-using-csaps-or-similar-to-create-a-3d-smoothing-spline

end