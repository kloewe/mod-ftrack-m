/*----------------------------------------------------------------------------
 *  File    : mxWritePaths.c
 *  Contents: Write paths to file.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxWritePaths.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include "mex.h"
#include "macros.h"

/*--------------------------------------------------------------------------*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  ASSERT(nrhs == 4);
  ASSERT(mxIsCell(prhs[0]));
  mwSize nCells = mxGetNumberOfElements(prhs[0]);
  int *dims = (int *)mxGetData(prhs[1]);
  float *vsiz = (float *)mxGetData(prhs[2]);

  // get filename
  mwSize len = mxGetNumberOfElements(prhs[3]) + 1;
  char* fname_paths = (char*)mxCalloc(len, sizeof(char));
  if (!fname_paths)
    ERROR("Out of memory.");
  if (mxGetString(prhs[3], fname_paths, len)) {
    mxFree(fname_paths);
    ERROR("Couldn't read filename.");
  }

  // open output file for paths
  FILE *fp_paths = NULL;
  if (fname_paths) {
    fp_paths = fopen(fname_paths, "w");
    if (!fp_paths)
      ERROR("Can't open output file.");
    fprintf(fp_paths, "# dims: %d %d %d\n",
        dims[0], dims[1], dims[2]);
    fprintf(fp_paths, "# vsiz: %f %f %f\n",
        vsiz[0], vsiz[1], vsiz[2]);
  }

  // write date to file
  for (mwIndex i = 0; i < nCells; i++) {
    mxArray *cell = mxGetCell(prhs[0], i);
    ASSERT(cell);
    ASSERT(mxIsClass(cell, "int32"));
    int *a = mxGetData(cell);
    for (mwIndex j = 0; j < mxGetNumberOfElements(cell)-1; j++) {
      fprintf(fp_paths, "%d ", a[j]);
    }
    fprintf(fp_paths, "%d\n", a[mxGetNumberOfElements(cell)-1]);
  }

  // close file
  fclose(fp_paths);
}  // mexFunction()
