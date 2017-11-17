function exportPaths(d,fname)
%EXPORTPATHS Export paths to an MRtrix tck-file.
%
%   EXPORTPATHS(P,FNAME) exports the paths in P to the tck-file FNAME.
%
%   EXPORTPATHS(C,FNAME) exports the paths specified by the cell array of
%   coordinates C to the tck-file FNAME.
%
%   EXPORTPATHS(T,FNAME) exports the paths in T to the tck-file FNAME,
%   where T is a structure of the kind returned by READ_MRTRIX_TRACKS. This
%   is equivalent to using WRITE_MRTRIX_TRACKS(T,FNAME) directly.
%
%   This function depends on the function WRITE_MRTRIX_TRACKS, which is part
%   part of the MRtrix3 software package (http://www.mrtrix.org).
%
%   Author: Kristian Loewe

if isstruct(d)
  if all(isfield(d, {'paths','dims','vsiz'}))
    tracks = struct;                           % input: ftrack format
    tracks.data = pathProps(d, 'Coords');
  elseif all(isfield(d, {'datatype','count','data'}))
    write_mrtrix_tracks(d);                    % input: mrtrix format
  end
elseif iscell(d)                               % input: cell array of coords
  assert(all(cellfun(@isnumeric, d)));
  tracks = struct;
  tracks.data = d;
else
  error('Unexpected input argument.');
end

% transpose arrays if necessary
assert(all(cellfun(@(c) any(size(c) == 3), tracks.data)));
idx = find(cellfun(@(c) sum(size(c) ~= 3), tracks.data), 1);
fmt = find(size(tracks.data{idx}) ~= 3);
if fmt == 2
  tracks.data = cellfun(@(c) c', tracks.data, 'UniformOutput', false);
end

write_mrtrix_tracks(tracks, fname);

end
