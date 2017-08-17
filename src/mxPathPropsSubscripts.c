/*----------------------------------------------------------------------------
 *  File    : mxPathPropsSubscripts.c
 *  Contents: Get 3D array subscripts (voxel coordinates) from linear indices.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxPathPropsSubscripts.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include <math.h>
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
  int *dims = (int *)mxGetData(prhs[1]);
  ASSERT((dims[0] > 0) && (dims[1] > 0) && (dims[2] > 0));

  plhs[0] = mxCreateCellMatrix(nCells, 1);
  ASSERT(plhs[0]);

  for (mwIndex i = 0; i < nCells; i++) {
    mxArray *cell = mxGetCell(prhs[0], i); // input cell
    ASSERT(cell);
    ASSERT(mxIsClass(cell, "int32"));
    int *a = mxGetData(cell);              // input cell content: lin. indices    
    mxArray *mxs = mxCreateNumericMatrix(
            3, mxGetNumberOfElements(cell), mxINT32_CLASS, mxREAL);
    int *s = mxGetData(mxs);               // output cell content: subscripts
    for (mwIndex j = 0; j < mxGetNumberOfElements(cell); j++) {
      int ind = a[j]-1;
      int z = ind / (dims[0] * dims[1]);
      int y = (ind - z * dims[0] *dims[1]) / dims[1];
      int x = ind - z * dims[0] *dims[1] - y * dims[1];
      s[j*3]   = x+1;
      s[j*3+1] = y+1;
      s[j*3+2] = z+1;
    }
    mxSetCell(plhs[0], i, mxs);
  }
}  // mexFunction()
