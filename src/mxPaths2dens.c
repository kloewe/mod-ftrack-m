/*----------------------------------------------------------------------------
 *  File    : mxPaths2dens.c
 *  Contents: Compute path density map.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxPaths2dens.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include "mex.h"
#include "macros.h"

/*--------------------------------------------------------------------------*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  ASSERT(nrhs == 2);

  ASSERT(mxIsCell(prhs[0]));
  mwSize nCells = mxGetNumberOfElements(prhs[0]);

  ASSERT(mxIsClass(prhs[1], "int32"));
  ASSERT(mxGetNumberOfElements(prhs[1]) == 3);
  int *dims2 = (int *)mxGetData(prhs[1]);
  mwSize dims[3] = { (mwSize)dims2[0], (mwSize)dims2[1], (mwSize)dims2[2] };
  ASSERT((dims[0] > 0) && (dims[1] > 0) && (dims[2] > 0));

  plhs[0] = mxCreateNumericArray(3, dims, mxSINGLE_CLASS, mxREAL);
  ASSERT(plhs[0]);
  float *densmap = (float *)mxGetData(plhs[0]);
  ASSERT(densmap);

  for (mwIndex i = 0; i < nCells; i++) {
    mxArray *cell = mxGetCell(prhs[0], i);
    ASSERT(cell);
    ASSERT(mxIsClass(cell, "int32"));
    int *a = mxGetData(cell);
    for (mwIndex j = 0; j < mxGetNumberOfElements(cell); j++) {
      densmap[a[j]-1] += 1.0f; 
      ASSERT(a[j] < (int)(dims[0] *dims[1] *dims[2]));
    }
  }
}  // mexFunction()
