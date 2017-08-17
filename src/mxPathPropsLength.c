/*----------------------------------------------------------------------------
 *  File    : mxPathPropsLength.c
 *  Contents: Get path lengths in mm.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxPathPropsLength.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include <math.h>
#include "mex.h"
#include "macros.h"

/*--------------------------------------------------------------------------*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  ASSERT(nrhs == 1);

  ASSERT(mxIsCell(prhs[0]));
  mwSize nCells = mxGetNumberOfElements(prhs[0]);

  plhs[0] = mxCreateNumericMatrix(nCells, 1, mxSINGLE_CLASS, mxREAL);
  ASSERT(plhs[0]);
  float *props = mxGetData(plhs[0]);
  ASSERT(props);

  for (mwIndex i = 0; i < nCells; i++) {
    mxArray *cell = mxGetCell(prhs[0], i);
    ASSERT(cell);
    ASSERT(mxIsClass(cell, "single"));
    float *a = mxGetData(cell);
    for (mwIndex j = 1; j < mxGetN(cell); j++) {
      float d1 = a[j*3]   - a[(j-1)*3];
      float d2 = a[j*3+1] - a[(j-1)*3+1];
      float d3 = a[j*3+2] - a[(j-1)*3+2];
      props[i] += sqrtf(d1*d1 + d2*d2 + d3*d3);
    }
  }
}  // mexFunction()
