function coords = applyAffineTransform(coords,A)
%APPLYAFFINETRANSFORM Apply 4 x 4 affine transform to path coordinates
%
%   Example 1:
%   p = readPaths('paths.dat');
%   h = readImgHdr('image.nii.gz');
%   t = struct;
%   t.data = applyAffineTransform(pathProps(p, 'Coords'), h.mat);
%   write_mrtrix_tracks(t, 'paths_t.tck');
%
%   Example 2:
%   t = read_mrtrix_tracks('tracks.tck');
%   h = readImgHdr('image.nii.gz');
%   t.data = applyAffineTransform(t.data, h.mat);
%   write_mrtrix_tracks(t, 'tracks_t.tck');
%
%   Author: Kristian Loewe

assert(nargin == 2, 'Unexpected number of input arguments.');

if isstruct(coords)
  if all(isfield(coords, {'paths','dims','vsiz'}))
    coords = pathProps(coords, 'Coords');      % input: ftrack format
  elseif all(isfield(coords, {'datatype','count','data'}))
    coords = coords.data;                      % input: mrtrix format
  end
elseif iscell(coords)                          % input: cell array of coords
  assert(all(cellfun(@isnumeric, coords)));
else
  error('Unexpected input argument.');
end

fmt = 1; % format (of the coords of each path/track): 1 -> n x 3;  2 -> 3 x n

% transpose arrays if necessary
assert(all(cellfun(@(c) any(size(c) == 3), coords)));
idx = find(cellfun(@(c) sum(size(c) ~= 3), coords), 1);
fmt = find(size(coords{idx}) ~= 3);
if fmt == 2
  coords = cellfun(@(c) c', coords, 'UniformOutput', false);
end

% transpose transformation matrix if necessary
if all(A(4,1:3) == 0)
  A = A';
end

% append a column of ones to each path's coordinates
coords = cellfun(@(c) [c ones(size(c,1),1)], coords, 'UniformOutput', false);

% apply the transformation to each path
for i = 1:numel(coords);
  coords{i} = coords{i} * A;
end

% remove the column of ones again
coords = cellfun(@(c) c(:,1:3), coords, 'UniformOutput', false);

% revert transpose if necessary
if fmt == 2
  coords = cellfun(@(c) c', coords, 'UniformOutput', false);
end

end
