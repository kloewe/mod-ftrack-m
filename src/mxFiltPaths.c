/*----------------------------------------------------------------------------
 *  File    : mxFiltPaths.c
 *  Contents: Filter paths.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxFiltPaths.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include "mex.h"
#include "macros.h"

/*--------------------------------------------------------------------------*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  ASSERT(nrhs == 2);

  ASSERT(mxIsCell(prhs[0]));
  mwSize nCells = mxGetNumberOfElements(prhs[0]);

  ASSERT(mxIsClass(prhs[1], "logical"));
  mxLogical *roi = mxGetData(prhs[1]);

  plhs[0] = mxCreateLogicalMatrix(nCells, 1);
  ASSERT(plhs[0]);
  mxLogical *keepidx = mxGetData(plhs[0]);
  ASSERT(keepidx);

  for (mwIndex i = 0; i < nCells; i++) {
    mxArray *cell = mxGetCell(prhs[0], i);
    ASSERT(cell);
    ASSERT(mxIsClass(cell, "int32"));
    int *a = mxGetData(cell);
    for (mwIndex j = 0; j < mxGetNumberOfElements(cell); j++) {
      if (roi[a[j]-1]) {
        keepidx[i] = true;
        break;
      }
    }
  }
}  // mexFunction()
