/*----------------------------------------------------------------------------
 *  File    : mxReadPaths.c
 *  Contents: Read paths from file.
 *  Author  : Kristian Loewe
 *
 *  mex -largeArrayDims -O CFLAGS="-std=c99 -fPIC
 *      -Wall -Wextra -Wno-unused-parameter -Wconversion -Wshadow -pedantic"
 *      mxReadPaths.c -outdir ../private
 * -------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "mex.h"
#include "macros.h"

/*--------------------------------------------------------------------------*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  // get filename
  mwSize len = mxGetNumberOfElements(prhs[0]) + 1;
  char* fname = (char*)mxCalloc(len, sizeof(char));
  if (!fname)
    ERROR("Out of memory.");
  if (mxGetString(prhs[0], fname, len)) {
    mxFree(fname);
    ERROR("Couldn't read filename.");
  }

  // read entire file into buffer
  FILE *fp = fopen(fname, "rb");
  if (!fp) {
    mxFree(fname);
    ERROR("Couldn't open file.");
  }
  fseek(fp, 0L, SEEK_END);
  long lSize = ftell(fp);
  rewind(fp);
  char *buf = mxCalloc(1, (mwSize)lSize + 1);
  if (!buf) {
    mxFree(fname);
    fclose(fp);
    ERROR("Out of memory.");
  }
  if (fread(buf, (mwSize)lSize, 1, fp) != 1) {
    mxFree(fname);
    fclose(fp);
    mxFree(buf);
    ERROR("Couldn't read file.");
  }
  mxFree(fname);
  fclose(fp);

  // skip comment/header lines
  int first = 0;
  int cnt = 0;
  int is_comment = 0;
  while (1) {
    if (buf[cnt++] == '#') {
      is_comment = 1; }
    else if (buf[cnt] == '\n') {
      if (is_comment) {
        first = cnt+1;
        is_comment = 0; }
      else
        break;
    }
  }

  // get number of (remaining) lines
  mwSize nLines = 0;
  for (long i = first; i < lSize; i++)
    if (buf[i] == '\n')
      nLines++;

  // create output cell array
  plhs[0] = mxCreateCellMatrix(nLines, 1);

  // create buffer array to store the numbers parsed from each line
  int asize = 100;
  int *array = mxMalloc((mwSize)asize *sizeof(int));

  // loop over characters to retrieve the numbers (ad-hoc parsing for speed)
  mwIndex m = 0;               // idx to iterate over lines
  int k = 0;                   // idx to iterate over numbers
  int n = 0;                   // current number being parsed
  for (long i = first; i < lSize; i++) {
    if (k >= asize) {          // increase array size, if necessary
      asize = 2 *asize;
      array = mxRealloc(array, (mwSize)asize *sizeof(int));
    }
    if      (buf[i] == '\n') { // '\n' -> line end
      array[k] = n;            // add this line's last number to the array
      mxArray *ma = mxCreateNumericMatrix((mwSize)(k+1), 1,
                                                    mxINT32_CLASS, mxREAL);
      int *a = mxGetData(ma);
      for (int j = 0; j < k+1; j++)
        a[j] = array[j];
      mxSetCell(plhs[0], m, ma);
      k = 0; n = 0;
      m++; }
    else if (buf[i] == ' ') {  // ' ' -> end of current number
      if (buf[i-1] != ' ') {
        array[k] = n;
        n = 0;
        k++; }
      else                     // unless previous char was ' ' too
        continue; }            // in that case, skip
    else {
      n *= 10;
      n += buf[i]-'0';
    }
  }

  // clean up
  mxFree(array);
  mxFree(buf);

}  // mexFunction()
