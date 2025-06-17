//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortIdx.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include "rt_nonfinite.h"
#include "MixtureFeatureExtract.h"
#include "PureFeatureExtract.h"
#include "analyseMixture.h"
#include "analysePure.h"
#include "autoestibl.h"
#include "baseline_correct.h"
#include "cal_noiselevel.h"
#include "cal_snr.h"
#include "calkafang.h"
#include "cwt1.h"
#include "deri2.h"
#include "fastsmooth.h"
#include "geto_new.h"
#include "offsetCorrect.h"
#include "resolution.h"
#include "resolution_factor.h"
#include "selectitem_m.h"
#include "selectitem_p.h"
#include "zl_fullspectrumcontrast.h"
#include "zl_lsqnonneg1.h"
#include "sortIdx.h"
#include "analyseMixture_emxutil.h"

// Function Declarations
static void b_merge(emxArray_int32_T *idx, emxArray_int32_T *x, int offset, int
                    np, int nq, emxArray_int32_T *iwork, emxArray_int32_T *xwork);
static void b_merge_block(emxArray_int32_T *idx, emxArray_int32_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_int32_T *xwork);
static void c_merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int
                    np, int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork);
static void c_merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T *xwork);
static void merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int np,
                  int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork);

// Function Definitions

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_int32_T *x
//                int offset
//                int np
//                int nq
//                emxArray_int32_T *iwork
//                emxArray_int32_T *xwork
// Return Type  : void
//
static void b_merge(emxArray_int32_T *idx, emxArray_int32_T *x, int offset, int
                    np, int nq, emxArray_int32_T *iwork, emxArray_int32_T *xwork)
{
  int n_tmp;
  int iout;
  int p;
  int i80;
  int q;
  int exitg1;
  if (nq != 0) {
    n_tmp = np + nq;
    for (iout = 0; iout < n_tmp; iout++) {
      i80 = offset + iout;
      iwork->data[iout] = idx->data[i80];
      xwork->data[iout] = x->data[i80];
    }

    p = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork->data[p] <= xwork->data[q]) {
        idx->data[iout] = iwork->data[p];
        x->data[iout] = xwork->data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx->data[iout] = iwork->data[q];
        x->data[iout] = xwork->data[q];
        if (q + 1 < n_tmp) {
          q++;
        } else {
          q = iout - p;
          for (iout = p + 1; iout <= np; iout++) {
            i80 = q + iout;
            idx->data[i80] = iwork->data[iout - 1];
            x->data[i80] = xwork->data[iout - 1];
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_int32_T *x
//                int offset
//                int n
//                int preSortLevel
//                emxArray_int32_T *iwork
//                emxArray_int32_T *xwork
// Return Type  : void
//
static void b_merge_block(emxArray_int32_T *idx, emxArray_int32_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_int32_T *xwork)
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        b_merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 0; nTail < nPairs; nTail++) {
      b_merge(idx, x, offset + nTail * tailOffset, bLen, bLen, iwork, xwork);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    b_merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int np
//                int nq
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
static void c_merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int
                    np, int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  int n_tmp;
  int iout;
  int p;
  int i83;
  int q;
  int exitg1;
  if (nq != 0) {
    n_tmp = np + nq;
    for (iout = 0; iout < n_tmp; iout++) {
      i83 = offset + iout;
      iwork->data[iout] = idx->data[i83];
      xwork->data[iout] = x->data[i83];
    }

    p = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork->data[p] >= xwork->data[q]) {
        idx->data[iout] = iwork->data[p];
        x->data[iout] = xwork->data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx->data[iout] = iwork->data[q];
        x->data[iout] = xwork->data[q];
        if (q + 1 < n_tmp) {
          q++;
        } else {
          q = iout - p;
          for (iout = p + 1; iout <= np; iout++) {
            i83 = q + iout;
            idx->data[i83] = iwork->data[iout - 1];
            x->data[i83] = xwork->data[iout - 1];
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int n
//                int preSortLevel
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
static void c_merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        c_merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 0; nTail < nPairs; nTail++) {
      c_merge(idx, x, offset + nTail * tailOffset, bLen, bLen, iwork, xwork);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    c_merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int np
//                int nq
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
static void merge(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int np,
                  int nq, emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  int n_tmp;
  int iout;
  int p;
  int i68;
  int q;
  int exitg1;
  if (nq != 0) {
    n_tmp = np + nq;
    for (iout = 0; iout < n_tmp; iout++) {
      i68 = offset + iout;
      iwork->data[iout] = idx->data[i68];
      xwork->data[iout] = x->data[i68];
    }

    p = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork->data[p] <= xwork->data[q]) {
        idx->data[iout] = iwork->data[p];
        x->data[iout] = xwork->data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx->data[iout] = iwork->data[q];
        x->data[iout] = xwork->data[q];
        if (q + 1 < n_tmp) {
          q++;
        } else {
          q = iout - p;
          for (iout = p + 1; iout <= np; iout++) {
            i68 = q + iout;
            idx->data[i68] = iwork->data[iout - 1];
            x->data[i68] = xwork->data[iout - 1];
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : emxArray_int32_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void b_sortIdx(emxArray_int32_T *x, emxArray_int32_T *idx)
{
  int i1;
  int i79;
  int n;
  int b_n;
  int x4[4];
  int idx4[4];
  emxArray_int32_T *iwork;
  emxArray_int32_T *xwork;
  int nQuartets;
  int j;
  int i3;
  int i;
  int i2;
  int k;
  signed char perm[4];
  int offset;
  int bLen;
  int bLen2;
  int nPairs;
  int i4;
  int b_iwork[256];
  int b_xwork[256];
  int exitg1;
  i1 = x->size[0];
  i79 = idx->size[0];
  idx->size[0] = i1;
  emxEnsureCapacity_int32_T(idx, i79);
  for (i79 = 0; i79 < i1; i79++) {
    idx->data[i79] = 0;
  }

  if (x->size[0] != 0) {
    n = x->size[0];
    b_n = x->size[0];
    x4[0] = 0;
    idx4[0] = 0;
    x4[1] = 0;
    idx4[1] = 0;
    x4[2] = 0;
    idx4[2] = 0;
    x4[3] = 0;
    idx4[3] = 0;
    emxInit_int32_T(&iwork, 1);
    i79 = iwork->size[0];
    iwork->size[0] = i1;
    emxEnsureCapacity_int32_T(iwork, i79);
    for (i79 = 0; i79 < i1; i79++) {
      iwork->data[i79] = 0;
    }

    emxInit_int32_T(&xwork, 1);
    i1 = x->size[0];
    i79 = xwork->size[0];
    xwork->size[0] = i1;
    emxEnsureCapacity_int32_T(xwork, i79);
    for (i79 = 0; i79 < i1; i79++) {
      xwork->data[i79] = 0;
    }

    nQuartets = x->size[0] >> 2;
    for (j = 0; j < nQuartets; j++) {
      i = j << 2;
      idx4[0] = i + 1;
      idx4[1] = i + 2;
      idx4[2] = i + 3;
      idx4[3] = i + 4;
      x4[0] = x->data[i];
      x4[1] = x->data[i + 1];
      x4[2] = x->data[i + 2];
      x4[3] = x->data[i + 3];
      if (x->data[i] <= x->data[i + 1]) {
        i1 = 1;
        i2 = 2;
      } else {
        i1 = 2;
        i2 = 1;
      }

      if (x->data[i + 2] <= x->data[i + 3]) {
        i3 = 3;
        i4 = 4;
      } else {
        i3 = 4;
        i4 = 3;
      }

      i79 = x4[i1 - 1];
      offset = x4[i3 - 1];
      if (i79 <= offset) {
        i79 = x4[i2 - 1];
        if (i79 <= offset) {
          perm[0] = static_cast<signed char>(i1);
          perm[1] = static_cast<signed char>(i2);
          perm[2] = static_cast<signed char>(i3);
          perm[3] = static_cast<signed char>(i4);
        } else if (i79 <= x4[i4 - 1]) {
          perm[0] = static_cast<signed char>(i1);
          perm[1] = static_cast<signed char>(i3);
          perm[2] = static_cast<signed char>(i2);
          perm[3] = static_cast<signed char>(i4);
        } else {
          perm[0] = static_cast<signed char>(i1);
          perm[1] = static_cast<signed char>(i3);
          perm[2] = static_cast<signed char>(i4);
          perm[3] = static_cast<signed char>(i2);
        }
      } else {
        offset = x4[i4 - 1];
        if (i79 <= offset) {
          if (x4[i2 - 1] <= offset) {
            perm[0] = static_cast<signed char>(i3);
            perm[1] = static_cast<signed char>(i1);
            perm[2] = static_cast<signed char>(i2);
            perm[3] = static_cast<signed char>(i4);
          } else {
            perm[0] = static_cast<signed char>(i3);
            perm[1] = static_cast<signed char>(i1);
            perm[2] = static_cast<signed char>(i4);
            perm[3] = static_cast<signed char>(i2);
          }
        } else {
          perm[0] = static_cast<signed char>(i3);
          perm[1] = static_cast<signed char>(i4);
          perm[2] = static_cast<signed char>(i1);
          perm[3] = static_cast<signed char>(i2);
        }
      }

      i79 = perm[0] - 1;
      idx->data[i] = idx4[i79];
      offset = perm[1] - 1;
      idx->data[i + 1] = idx4[offset];
      i1 = perm[2] - 1;
      idx->data[i + 2] = idx4[i1];
      i2 = perm[3] - 1;
      idx->data[i + 3] = idx4[i2];
      x->data[i] = x4[i79];
      x->data[i + 1] = x4[offset];
      x->data[i + 2] = x4[i1];
      x->data[i + 3] = x4[i2];
    }

    i3 = nQuartets << 2;
    i2 = b_n - i3;
    if (i2 > 0) {
      for (k = 0; k < i2; k++) {
        i1 = i3 + k;
        idx4[k] = i1 + 1;
        x4[k] = x->data[i1];
      }

      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      if (i2 == 1) {
        perm[0] = 1;
      } else if (i2 == 2) {
        if (x4[0] <= x4[1]) {
          perm[0] = 1;
          perm[1] = 2;
        } else {
          perm[0] = 2;
          perm[1] = 1;
        }
      } else if (x4[0] <= x4[1]) {
        if (x4[1] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 2;
          perm[2] = 3;
        } else if (x4[0] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 3;
          perm[2] = 2;
        } else {
          perm[0] = 3;
          perm[1] = 1;
          perm[2] = 2;
        }
      } else if (x4[0] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 1;
        perm[2] = 3;
      } else if (x4[1] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 3;
        perm[2] = 1;
      } else {
        perm[0] = 3;
        perm[1] = 2;
        perm[2] = 1;
      }

      for (k = 0; k < i2; k++) {
        i79 = i3 + k;
        idx->data[i79] = idx4[perm[k] - 1];
        x->data[i79] = x4[perm[k] - 1];
      }
    }

    i1 = 2;
    if (n > 1) {
      if (n >= 256) {
        nQuartets = n >> 8;
        for (i = 0; i < nQuartets; i++) {
          offset = i << 8;
          for (b_n = 0; b_n < 6; b_n++) {
            bLen = 1 << (b_n + 2);
            bLen2 = bLen << 1;
            nPairs = 256 >> (b_n + 3);
            for (k = 0; k < nPairs; k++) {
              i3 = offset + k * bLen2;
              for (j = 0; j < bLen2; j++) {
                i1 = i3 + j;
                b_iwork[j] = idx->data[i1];
                b_xwork[j] = x->data[i1];
              }

              i4 = 0;
              i2 = bLen;
              i1 = i3 - 1;
              do {
                exitg1 = 0;
                i1++;
                if (b_xwork[i4] <= b_xwork[i2]) {
                  idx->data[i1] = b_iwork[i4];
                  x->data[i1] = b_xwork[i4];
                  if (i4 + 1 < bLen) {
                    i4++;
                  } else {
                    exitg1 = 1;
                  }
                } else {
                  idx->data[i1] = b_iwork[i2];
                  x->data[i1] = b_xwork[i2];
                  if (i2 + 1 < bLen2) {
                    i2++;
                  } else {
                    i1 -= i4;
                    for (j = i4 + 1; j <= bLen; j++) {
                      i79 = i1 + j;
                      idx->data[i79] = b_iwork[j - 1];
                      x->data[i79] = b_xwork[j - 1];
                    }

                    exitg1 = 1;
                  }
                }
              } while (exitg1 == 0);
            }
          }
        }

        i1 = nQuartets << 8;
        i2 = n - i1;
        if (i2 > 0) {
          b_merge_block(idx, x, i1, i2, 2, iwork, xwork);
        }

        i1 = 8;
      }

      b_merge_block(idx, x, 0, n, i1, iwork, xwork);
    }

    emxFree_int32_T(&xwork);
    emxFree_int32_T(&iwork);
  }
}

//
// Arguments    : emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void c_sortIdx(emxArray_real_T *x, emxArray_int32_T *idx)
{
  int i1;
  int i82;
  int n;
  int b_n;
  double x4[4];
  int idx4[4];
  emxArray_int32_T *iwork;
  emxArray_real_T *xwork;
  int nNaNs;
  int ib;
  int k;
  int i4;
  signed char perm[4];
  int quartetOffset;
  int i3;
  int nNonNaN;
  int nBlocks;
  double d15;
  double d16;
  int bLen;
  int bLen2;
  int nPairs;
  int b_iwork[256];
  double b_xwork[256];
  int exitg1;
  i1 = x->size[0];
  i82 = idx->size[0];
  idx->size[0] = i1;
  emxEnsureCapacity_int32_T(idx, i82);
  for (i82 = 0; i82 < i1; i82++) {
    idx->data[i82] = 0;
  }

  n = x->size[0];
  b_n = x->size[0];
  x4[0] = 0.0;
  idx4[0] = 0;
  x4[1] = 0.0;
  idx4[1] = 0;
  x4[2] = 0.0;
  idx4[2] = 0;
  x4[3] = 0.0;
  idx4[3] = 0;
  emxInit_int32_T(&iwork, 1);
  i82 = iwork->size[0];
  iwork->size[0] = i1;
  emxEnsureCapacity_int32_T(iwork, i82);
  for (i82 = 0; i82 < i1; i82++) {
    iwork->data[i82] = 0;
  }

  emxInit_real_T(&xwork, 1);
  i1 = x->size[0];
  i82 = xwork->size[0];
  xwork->size[0] = i1;
  emxEnsureCapacity_real_T(xwork, i82);
  for (i82 = 0; i82 < i1; i82++) {
    xwork->data[i82] = 0.0;
  }

  nNaNs = 0;
  ib = -1;
  for (k = 0; k < b_n; k++) {
    if (rtIsNaN(x->data[k])) {
      idx->data[(b_n - nNaNs) - 1] = k + 1;
      xwork->data[(b_n - nNaNs) - 1] = x->data[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib] = k + 1;
      x4[ib] = x->data[k];
      if (ib + 1 == 4) {
        quartetOffset = k - nNaNs;
        if (x4[0] >= x4[1]) {
          i1 = 1;
          ib = 2;
        } else {
          i1 = 2;
          ib = 1;
        }

        if (x4[2] >= x4[3]) {
          i3 = 3;
          i4 = 4;
        } else {
          i3 = 4;
          i4 = 3;
        }

        d15 = x4[i1 - 1];
        d16 = x4[i3 - 1];
        if (d15 >= d16) {
          d15 = x4[ib - 1];
          if (d15 >= d16) {
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(ib);
            perm[2] = static_cast<signed char>(i3);
            perm[3] = static_cast<signed char>(i4);
          } else if (d15 >= x4[i4 - 1]) {
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(i3);
            perm[2] = static_cast<signed char>(ib);
            perm[3] = static_cast<signed char>(i4);
          } else {
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(i3);
            perm[2] = static_cast<signed char>(i4);
            perm[3] = static_cast<signed char>(ib);
          }
        } else {
          d16 = x4[i4 - 1];
          if (d15 >= d16) {
            if (x4[ib - 1] >= d16) {
              perm[0] = static_cast<signed char>(i3);
              perm[1] = static_cast<signed char>(i1);
              perm[2] = static_cast<signed char>(ib);
              perm[3] = static_cast<signed char>(i4);
            } else {
              perm[0] = static_cast<signed char>(i3);
              perm[1] = static_cast<signed char>(i1);
              perm[2] = static_cast<signed char>(i4);
              perm[3] = static_cast<signed char>(ib);
            }
          } else {
            perm[0] = static_cast<signed char>(i3);
            perm[1] = static_cast<signed char>(i4);
            perm[2] = static_cast<signed char>(i1);
            perm[3] = static_cast<signed char>(ib);
          }
        }

        i82 = perm[0] - 1;
        idx->data[quartetOffset - 3] = idx4[i82];
        ib = perm[1] - 1;
        idx->data[quartetOffset - 2] = idx4[ib];
        i1 = perm[2] - 1;
        idx->data[quartetOffset - 1] = idx4[i1];
        i3 = perm[3] - 1;
        idx->data[quartetOffset] = idx4[i3];
        x->data[quartetOffset - 3] = x4[i82];
        x->data[quartetOffset - 2] = x4[ib];
        x->data[quartetOffset - 1] = x4[i1];
        x->data[quartetOffset] = x4[i3];
        ib = -1;
      }
    }
  }

  i4 = (b_n - nNaNs) - 1;
  if (ib + 1 > 0) {
    perm[1] = 0;
    perm[2] = 0;
    perm[3] = 0;
    if (ib + 1 == 1) {
      perm[0] = 1;
    } else if (ib + 1 == 2) {
      if (x4[0] >= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] >= x4[1]) {
      if (x4[1] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (k = 0; k <= ib; k++) {
      i82 = (i4 - ib) + k;
      idx->data[i82] = idx4[perm[k] - 1];
      x->data[i82] = x4[perm[k] - 1];
    }
  }

  ib = (nNaNs >> 1) + 1;
  for (k = 0; k <= ib - 2; k++) {
    i1 = (i4 + k) + 1;
    i3 = idx->data[i1];
    i82 = (b_n - k) - 1;
    idx->data[i1] = idx->data[i82];
    idx->data[i82] = i3;
    x->data[i1] = xwork->data[i82];
    x->data[i82] = xwork->data[i1];
  }

  if ((nNaNs & 1) != 0) {
    i82 = i4 + ib;
    x->data[i82] = xwork->data[i82];
  }

  nNonNaN = n - nNaNs;
  i1 = 2;
  if (nNonNaN > 1) {
    if (n >= 256) {
      nBlocks = nNonNaN >> 8;
      if (nBlocks > 0) {
        for (quartetOffset = 0; quartetOffset < nBlocks; quartetOffset++) {
          b_n = (quartetOffset << 8) - 1;
          for (n = 0; n < 6; n++) {
            bLen = 1 << (n + 2);
            bLen2 = bLen << 1;
            nPairs = 256 >> (n + 3);
            for (k = 0; k < nPairs; k++) {
              i3 = (b_n + k * bLen2) + 1;
              for (i1 = 0; i1 < bLen2; i1++) {
                ib = i3 + i1;
                b_iwork[i1] = idx->data[ib];
                b_xwork[i1] = x->data[ib];
              }

              i4 = 0;
              i1 = bLen;
              ib = i3 - 1;
              do {
                exitg1 = 0;
                ib++;
                if (b_xwork[i4] >= b_xwork[i1]) {
                  idx->data[ib] = b_iwork[i4];
                  x->data[ib] = b_xwork[i4];
                  if (i4 + 1 < bLen) {
                    i4++;
                  } else {
                    exitg1 = 1;
                  }
                } else {
                  idx->data[ib] = b_iwork[i1];
                  x->data[ib] = b_xwork[i1];
                  if (i1 + 1 < bLen2) {
                    i1++;
                  } else {
                    ib -= i4;
                    for (i1 = i4 + 1; i1 <= bLen; i1++) {
                      i82 = ib + i1;
                      idx->data[i82] = b_iwork[i1 - 1];
                      x->data[i82] = b_xwork[i1 - 1];
                    }

                    exitg1 = 1;
                  }
                }
              } while (exitg1 == 0);
            }
          }
        }

        i1 = nBlocks << 8;
        ib = nNonNaN - i1;
        if (ib > 0) {
          c_merge_block(idx, x, i1, ib, 2, iwork, xwork);
        }

        i1 = 8;
      }
    }

    c_merge_block(idx, x, 0, nNonNaN, i1, iwork, xwork);
  }

  if ((nNaNs > 0) && (nNonNaN > 0)) {
    for (k = 0; k < nNaNs; k++) {
      i82 = nNonNaN + k;
      xwork->data[k] = x->data[i82];
      iwork->data[k] = idx->data[i82];
    }

    for (k = nNonNaN; k >= 1; k--) {
      i82 = (nNaNs + k) - 1;
      x->data[i82] = x->data[k - 1];
      idx->data[i82] = idx->data[k - 1];
    }

    for (k = 0; k < nNaNs; k++) {
      x->data[k] = xwork->data[k];
      idx->data[k] = iwork->data[k];
    }
  }

  emxFree_real_T(&xwork);
  emxFree_int32_T(&iwork);
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real_T *x
//                int offset
//                int n
//                int preSortLevel
//                emxArray_int32_T *iwork
//                emxArray_real_T *xwork
// Return Type  : void
//
void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int n,
                 int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T
                 *xwork)
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 0; nTail < nPairs; nTail++) {
      merge(idx, x, offset + nTail * tailOffset, bLen, bLen, iwork, xwork);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

//
// Arguments    : const emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void sortIdx(const emxArray_real_T *x, emxArray_int32_T *idx)
{
  int n;
  unsigned int unnamed_idx_0;
  int i2;
  int i;
  emxArray_int32_T *iwork;
  int k;
  int j;
  int pEnd;
  int p;
  int q;
  int qEnd;
  int kEnd;
  n = x->size[0] + 1;
  unnamed_idx_0 = static_cast<unsigned int>(x->size[0]);
  i2 = idx->size[0];
  idx->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_int32_T(idx, i2);
  i = static_cast<int>(unnamed_idx_0);
  for (i2 = 0; i2 < i; i2++) {
    idx->data[i2] = 0;
  }

  emxInit_int32_T(&iwork, 1);
  i2 = iwork->size[0];
  iwork->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_int32_T(iwork, i2);
  i2 = x->size[0] - 1;
  for (k = 1; k <= i2; k += 2) {
    if ((x->data[k - 1] >= x->data[k]) || rtIsNaN(x->data[k - 1])) {
      idx->data[k - 1] = k;
      idx->data[k] = k + 1;
    } else {
      idx->data[k - 1] = k + 1;
      idx->data[k] = k;
    }
  }

  if ((x->size[0] & 1) != 0) {
    idx->data[x->size[0] - 1] = x->size[0];
  }

  i = 2;
  while (i < n - 1) {
    i2 = i << 1;
    j = 1;
    for (pEnd = 1 + i; pEnd < n; pEnd = qEnd + i) {
      p = j - 1;
      q = pEnd;
      qEnd = j + i2;
      if (qEnd > n) {
        qEnd = n;
      }

      k = 0;
      kEnd = qEnd - j;
      while (k + 1 <= kEnd) {
        if ((x->data[idx->data[p] - 1] >= x->data[idx->data[q - 1] - 1]) ||
            rtIsNaN(x->data[idx->data[p] - 1])) {
          iwork->data[k] = idx->data[p];
          p++;
          if (p + 1 == pEnd) {
            while (q < qEnd) {
              k++;
              iwork->data[k] = idx->data[q - 1];
              q++;
            }
          }
        } else {
          iwork->data[k] = idx->data[q - 1];
          q++;
          if (q == qEnd) {
            while (p + 1 < pEnd) {
              k++;
              iwork->data[k] = idx->data[p];
              p++;
            }
          }
        }

        k++;
      }

      for (k = 0; k < kEnd; k++) {
        idx->data[(j + k) - 1] = iwork->data[k];
      }

      j = qEnd;
    }

    i = i2;
  }

  emxFree_int32_T(&iwork);
}

//
// File trailer for sortIdx.cpp
//
// [EOF]
//
