//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sort1.cpp
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
#include "sort1.h"
#include "analyseMixture_emxutil.h"
#include "sortIdx.h"

// Function Definitions

//
// Arguments    : emxArray_int32_T *x
// Return Type  : void
//
void b_sort(emxArray_int32_T *x)
{
  int dim;
  int j;
  emxArray_int32_T *vwork;
  int vlen;
  int vstride;
  int k;
  emxArray_int32_T *b_vwork;
  dim = 0;
  if (x->size[0] != 1) {
    dim = -1;
  }

  if (dim + 2 <= 1) {
    j = x->size[0];
  } else {
    j = 1;
  }

  emxInit_int32_T(&vwork, 1);
  vlen = j - 1;
  vstride = vwork->size[0];
  vwork->size[0] = j;
  emxEnsureCapacity_int32_T(vwork, vstride);
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x->size[0];
  }

  emxInit_int32_T(&b_vwork, 1);
  for (j = 0; j < vstride; j++) {
    for (k = 0; k <= vlen; k++) {
      vwork->data[k] = x->data[j + k * vstride];
    }

    b_sortIdx(vwork, b_vwork);
    for (k = 0; k <= vlen; k++) {
      x->data[j + k * vstride] = vwork->data[k];
    }
  }

  emxFree_int32_T(&b_vwork);
  emxFree_int32_T(&vwork);
}

//
// Arguments    : emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void c_sort(emxArray_real_T *x, emxArray_int32_T *idx)
{
  int dim;
  int i81;
  emxArray_real_T *vwork;
  int vlen;
  int x_idx_0;
  int vstride;
  int k;
  emxArray_int32_T *iidx;
  dim = 0;
  if (x->size[0] != 1) {
    dim = -1;
  }

  if (dim + 2 <= 1) {
    i81 = x->size[0];
  } else {
    i81 = 1;
  }

  emxInit_real_T(&vwork, 1);
  vlen = i81 - 1;
  x_idx_0 = vwork->size[0];
  vwork->size[0] = i81;
  emxEnsureCapacity_real_T(vwork, x_idx_0);
  x_idx_0 = x->size[0];
  i81 = idx->size[0];
  idx->size[0] = x_idx_0;
  emxEnsureCapacity_int32_T(idx, i81);
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x->size[0];
  }

  emxInit_int32_T(&iidx, 1);
  for (x_idx_0 = 0; x_idx_0 < vstride; x_idx_0++) {
    for (k = 0; k <= vlen; k++) {
      vwork->data[k] = x->data[x_idx_0 + k * vstride];
    }

    c_sortIdx(vwork, iidx);
    for (k = 0; k <= vlen; k++) {
      i81 = x_idx_0 + k * vstride;
      x->data[i81] = vwork->data[k];
      idx->data[i81] = iidx->data[k];
    }
  }

  emxFree_int32_T(&iidx);
  emxFree_real_T(&vwork);
}

//
// Arguments    : emxArray_real_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void sort(emxArray_real_T *x, emxArray_int32_T *idx)
{
  int i67;
  int i1;
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
  double d13;
  double d14;
  int bLen2;
  int nPairs;
  int b_iwork[256];
  double b_xwork[256];
  int exitg1;
  i67 = idx->size[0] * idx->size[1];
  idx->size[0] = 1;
  idx->size[1] = x->size[1];
  emxEnsureCapacity_int32_T(idx, i67);
  i1 = x->size[1];
  for (i67 = 0; i67 < i1; i67++) {
    idx->data[i67] = 0;
  }

  if (x->size[1] != 0) {
    n = x->size[1];
    b_n = x->size[1];
    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    emxInit_int32_T(&iwork, 1);
    i1 = x->size[1];
    i67 = iwork->size[0];
    iwork->size[0] = i1;
    emxEnsureCapacity_int32_T(iwork, i67);
    for (i67 = 0; i67 < i1; i67++) {
      iwork->data[i67] = 0;
    }

    emxInit_real_T(&xwork, 1);
    i1 = x->size[1];
    i67 = xwork->size[0];
    xwork->size[0] = i1;
    emxEnsureCapacity_real_T(xwork, i67);
    for (i67 = 0; i67 < i1; i67++) {
      xwork->data[i67] = 0.0;
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
          if (x4[0] <= x4[1]) {
            i1 = 1;
            ib = 2;
          } else {
            i1 = 2;
            ib = 1;
          }

          if (x4[2] <= x4[3]) {
            i3 = 3;
            i4 = 4;
          } else {
            i3 = 4;
            i4 = 3;
          }

          d13 = x4[i1 - 1];
          d14 = x4[i3 - 1];
          if (d13 <= d14) {
            d13 = x4[ib - 1];
            if (d13 <= d14) {
              perm[0] = static_cast<signed char>(i1);
              perm[1] = static_cast<signed char>(ib);
              perm[2] = static_cast<signed char>(i3);
              perm[3] = static_cast<signed char>(i4);
            } else if (d13 <= x4[i4 - 1]) {
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
            d14 = x4[i4 - 1];
            if (d13 <= d14) {
              if (x4[ib - 1] <= d14) {
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

          i67 = perm[0] - 1;
          idx->data[quartetOffset - 3] = idx4[i67];
          ib = perm[1] - 1;
          idx->data[quartetOffset - 2] = idx4[ib];
          i1 = perm[2] - 1;
          idx->data[quartetOffset - 1] = idx4[i1];
          i3 = perm[3] - 1;
          idx->data[quartetOffset] = idx4[i3];
          x->data[quartetOffset - 3] = x4[i67];
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

      for (k = 0; k <= ib; k++) {
        i67 = (i4 - ib) + k;
        idx->data[i67] = idx4[perm[k] - 1];
        x->data[i67] = x4[perm[k] - 1];
      }
    }

    ib = (nNaNs >> 1) + 1;
    for (k = 0; k <= ib - 2; k++) {
      i1 = (i4 + k) + 1;
      i3 = idx->data[i1];
      i67 = (b_n - k) - 1;
      idx->data[i1] = idx->data[i67];
      idx->data[i67] = i3;
      x->data[i1] = xwork->data[i67];
      x->data[i67] = xwork->data[i1];
    }

    if ((nNaNs & 1) != 0) {
      i67 = i4 + ib;
      x->data[i67] = xwork->data[i67];
    }

    nNonNaN = n - nNaNs;
    i1 = 2;
    if (nNonNaN > 1) {
      if (n >= 256) {
        nBlocks = nNonNaN >> 8;
        if (nBlocks > 0) {
          for (quartetOffset = 0; quartetOffset < nBlocks; quartetOffset++) {
            nNaNs = (quartetOffset << 8) - 1;
            for (b_n = 0; b_n < 6; b_n++) {
              n = 1 << (b_n + 2);
              bLen2 = n << 1;
              nPairs = 256 >> (b_n + 3);
              for (k = 0; k < nPairs; k++) {
                i3 = (nNaNs + k * bLen2) + 1;
                for (i1 = 0; i1 < bLen2; i1++) {
                  ib = i3 + i1;
                  b_iwork[i1] = idx->data[ib];
                  b_xwork[i1] = x->data[ib];
                }

                i4 = 0;
                i1 = n;
                ib = i3 - 1;
                do {
                  exitg1 = 0;
                  ib++;
                  if (b_xwork[i4] <= b_xwork[i1]) {
                    idx->data[ib] = b_iwork[i4];
                    x->data[ib] = b_xwork[i4];
                    if (i4 + 1 < n) {
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
                      for (i1 = i4 + 1; i1 <= n; i1++) {
                        i67 = ib + i1;
                        idx->data[i67] = b_iwork[i1 - 1];
                        x->data[i67] = b_xwork[i1 - 1];
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
            merge_block(idx, x, i1, ib, 2, iwork, xwork);
          }

          i1 = 8;
        }
      }

      merge_block(idx, x, 0, nNonNaN, i1, iwork, xwork);
    }

    emxFree_real_T(&xwork);
    emxFree_int32_T(&iwork);
  }
}

//
// File trailer for sort1.cpp
//
// [EOF]
//
