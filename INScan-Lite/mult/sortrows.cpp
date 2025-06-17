//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortrows.cpp
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
#include "sortrows.h"
#include "analyseMixture_emxutil.h"
#include "sortLE.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *y
// Return Type  : void
//
void sortrows(emxArray_real_T *y)
{
  emxArray_int32_T *idx;
  int n;
  int p;
  int i2;
  emxArray_int32_T *iwork;
  int k;
  emxArray_real_T *ycol;
  int pEnd;
  int i;
  int j;
  int q;
  int qEnd;
  int kEnd;
  emxInit_int32_T(&idx, 1);
  n = y->size[0] + 1;
  p = idx->size[0];
  idx->size[0] = y->size[0];
  emxEnsureCapacity_int32_T(idx, p);
  i2 = y->size[0];
  for (p = 0; p < i2; p++) {
    idx->data[p] = 0;
  }

  if (y->size[0] == 0) {
    for (k = 0; k <= n - 2; k++) {
      idx->data[k] = k + 1;
    }
  } else {
    emxInit_int32_T(&iwork, 1);
    p = iwork->size[0];
    iwork->size[0] = y->size[0];
    emxEnsureCapacity_int32_T(iwork, p);
    p = y->size[0] - 1;
    for (k = 1; k <= p; k += 2) {
      if (sortLE(y, k, k + 1)) {
        idx->data[k - 1] = k;
        idx->data[k] = k + 1;
      } else {
        idx->data[k - 1] = k + 1;
        idx->data[k] = k;
      }
    }

    if ((y->size[0] & 1) != 0) {
      idx->data[y->size[0] - 1] = y->size[0];
    }

    i = 2;
    while (i < n - 1) {
      i2 = i << 1;
      j = 1;
      for (pEnd = 1 + i; pEnd < n; pEnd = qEnd + i) {
        p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > n) {
          qEnd = n;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          if (sortLE(y, idx->data[p - 1], idx->data[q - 1])) {
            iwork->data[k] = idx->data[p - 1];
            p++;
            if (p == pEnd) {
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
              while (p < pEnd) {
                k++;
                iwork->data[k] = idx->data[p - 1];
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

  emxInit_real_T(&ycol, 1);
  pEnd = y->size[0] - 1;
  i2 = y->size[0];
  p = ycol->size[0];
  ycol->size[0] = i2;
  emxEnsureCapacity_real_T(ycol, p);
  for (j = 0; j < 3; j++) {
    for (i = 0; i <= pEnd; i++) {
      ycol->data[i] = y->data[(idx->data[i] + y->size[0] * j) - 1];
    }

    for (i = 0; i <= pEnd; i++) {
      y->data[i + y->size[0] * j] = ycol->data[i];
    }
  }

  emxFree_real_T(&ycol);
  emxFree_int32_T(&idx);
}

//
// File trailer for sortrows.cpp
//
// [EOF]
//
