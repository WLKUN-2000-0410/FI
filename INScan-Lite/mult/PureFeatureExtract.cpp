//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PureFeatureExtract.cpp
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
#include "analyseMixture_emxutil.h"
#include "sort1.h"
#include "findpeaks.h"

// Function Definitions

//
// Arguments    : const double PureSubstance[1650]
//                emxArray_real_T *PureFeatureArray
//                double *flag_ft
// Return Type  : void
//
void PureFeatureExtract(const double PureSubstance[1650], emxArray_real_T
  *PureFeatureArray, double *flag_ft)
{
  int b_flag_ft;
  int idx;
  int k;
  boolean_T exitg1;
  double ex;
  int i40;
  double d4;
  double yl[1650];
  double b_ex;
  emxArray_real_T *locs;
  emxArray_real_T *yy;
  emxArray_real_T *unusedU1;
  emxArray_boolean_T *b_idx;
  int nxin;
  int n;
  int nxout;
  emxArray_int16_T *xx1;
  emxArray_int32_T *iidx;
  double b_xx1[14];

  //   lcz:提取纯净物特征向量，输入为预处理后的纯净物数据，输出为m行两列的数组。不同纯净物得到的数组的维度不一样。 
  b_flag_ft = 0;
  if (!rtIsNaN(PureSubstance[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 1651)) {
      if (!rtIsNaN(PureSubstance[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    ex = PureSubstance[0];
  } else {
    ex = PureSubstance[idx - 1];
    i40 = idx + 1;
    for (k = i40; k < 1651; k++) {
      d4 = PureSubstance[k - 1];
      if (ex < d4) {
        ex = d4;
      }
    }
  }

  for (idx = 0; idx < 1650; idx++) {
    yl[idx] = PureSubstance[idx] / ex;
  }

  if (!rtIsNaN(yl[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 1651)) {
      if (!rtIsNaN(yl[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    ex = yl[0];
  } else {
    ex = yl[idx - 1];
    i40 = idx + 1;
    for (k = i40; k < 1651; k++) {
      d4 = yl[k - 1];
      if (ex < d4) {
        ex = d4;
      }
    }
  }

  if (!rtIsNaN(yl[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 1651)) {
      if (!rtIsNaN(yl[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    b_ex = yl[0];
  } else {
    b_ex = yl[idx - 1];
    i40 = idx + 1;
    for (k = i40; k < 1651; k++) {
      d4 = yl[k - 1];
      if (b_ex < d4) {
        b_ex = d4;
      }
    }
  }

  emxInit_real_T(&locs, 1);
  emxInit_real_T(&yy, 1);
  emxInit_real_T(&unusedU1, 1);
  findpeaks(yl, 0.05 * ex, 0.005 * b_ex, yy, locs, unusedU1);

  //  plot(xl,yl)
  //  axis([351 2000 0 1]);
  emxFree_real_T(&unusedU1);
  if (locs->size[0] != 0) {
    emxInit_boolean_T(&b_idx, 1);
    i40 = b_idx->size[0];
    b_idx->size[0] = locs->size[0];
    emxEnsureCapacity_boolean_T(b_idx, i40);
    idx = locs->size[0];
    for (i40 = 0; i40 < idx; i40++) {
      b_idx->data[i40] = (static_cast<short>((351 + static_cast<short>
        ((static_cast<int>(locs->data[i40]) - 1)))) > 1800);
    }

    nxin = locs->size[0];
    n = 0;
    i40 = b_idx->size[0];
    for (k = 0; k < i40; k++) {
      n += b_idx->data[k];
    }

    nxout = locs->size[0] - n;
    idx = -1;
    for (k = 0; k < nxin; k++) {
      if ((k + 1 > b_idx->size[0]) || (!b_idx->data[k])) {
        idx++;
        locs->data[idx] = locs->data[k];
      }
    }

    emxFree_boolean_T(&b_idx);
    if (1 > nxout) {
      locs->size[0] = 0;
    } else {
      i40 = locs->size[0];
      locs->size[0] = nxout;
      emxEnsureCapacity_real_T(locs, i40);
    }

    if (locs->size[0] != 0) {
      n = locs->size[0];
      if (locs->size[0] <= 2) {
        if (locs->size[0] == 1) {
          ex = yl[static_cast<int>(locs->data[0]) - 1];
        } else if ((yl[static_cast<int>(locs->data[0]) - 1] < yl[static_cast<int>
                    (locs->data[1]) - 1]) || (rtIsNaN(yl[static_cast<int>
                     (locs->data[0]) - 1]) && (!rtIsNaN(yl[static_cast<int>
                      (locs->data[1]) - 1])))) {
          ex = yl[static_cast<int>(locs->data[1]) - 1];
        } else {
          ex = yl[static_cast<int>(locs->data[0]) - 1];
        }
      } else {
        if (!rtIsNaN(yl[static_cast<int>(locs->data[0]) - 1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= locs->size[0])) {
            if (!rtIsNaN(yl[static_cast<int>(locs->data[k - 1]) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = yl[static_cast<int>(locs->data[0]) - 1];
        } else {
          ex = yl[static_cast<int>(locs->data[idx - 1]) - 1];
          i40 = idx + 1;
          for (k = i40; k <= n; k++) {
            if (ex < yl[static_cast<int>(locs->data[k - 1]) - 1]) {
              ex = yl[static_cast<int>(locs->data[k - 1]) - 1];
            }
          }
        }
      }

      i40 = yy->size[0];
      yy->size[0] = locs->size[0];
      emxEnsureCapacity_real_T(yy, i40);
      idx = locs->size[0];
      for (i40 = 0; i40 < idx; i40++) {
        yy->data[i40] = yl[static_cast<int>(locs->data[i40]) - 1] / ex;
      }

      emxInit_int16_T(&xx1, 1);
      emxInit_int32_T(&iidx, 1);
      c_sort(yy, iidx);
      i40 = xx1->size[0];
      xx1->size[0] = iidx->size[0];
      emxEnsureCapacity_int16_T(xx1, i40);
      idx = iidx->size[0];
      for (i40 = 0; i40 < idx; i40++) {
        xx1->data[i40] = static_cast<short>((351 + static_cast<short>
          ((static_cast<int>(locs->data[iidx->data[i40] - 1]) - 1))));
      }

      emxFree_int32_T(&iidx);
      if (yy->size[0] < 7) {
        n = 7 - yy->size[0];
        nxout = 7 - yy->size[0];
        i40 = PureFeatureArray->size[0] * PureFeatureArray->size[1];
        PureFeatureArray->size[0] = xx1->size[0] + n;
        PureFeatureArray->size[1] = 2;
        emxEnsureCapacity_real_T(PureFeatureArray, i40);
        idx = xx1->size[0];
        for (i40 = 0; i40 < idx; i40++) {
          PureFeatureArray->data[i40] = xx1->data[i40];
        }

        for (i40 = 0; i40 < n; i40++) {
          PureFeatureArray->data[i40 + xx1->size[0]] = 0.0;
        }

        idx = yy->size[0];
        for (i40 = 0; i40 < idx; i40++) {
          PureFeatureArray->data[i40 + PureFeatureArray->size[0]] = yy->data[i40];
        }

        for (i40 = 0; i40 < nxout; i40++) {
          PureFeatureArray->data[(i40 + yy->size[0]) + PureFeatureArray->size[0]]
            = 0.0;
        }
      } else {
        for (i40 = 0; i40 < 7; i40++) {
          b_xx1[i40] = xx1->data[i40];
        }

        for (i40 = 0; i40 < 7; i40++) {
          b_xx1[7 + i40] = yy->data[i40];
        }

        i40 = PureFeatureArray->size[0] * PureFeatureArray->size[1];
        PureFeatureArray->size[0] = 7;
        PureFeatureArray->size[1] = 2;
        emxEnsureCapacity_real_T(PureFeatureArray, i40);
        for (i40 = 0; i40 < 14; i40++) {
          PureFeatureArray->data[i40] = b_xx1[i40];
        }
      }

      emxFree_int16_T(&xx1);
    } else {
      PureFeatureArray->size[0] = 0;
      PureFeatureArray->size[1] = 2;
      b_flag_ft = 1;
    }
  } else {
    PureFeatureArray->size[0] = 0;
    PureFeatureArray->size[1] = 2;
    b_flag_ft = 1;
  }

  emxFree_real_T(&yy);
  emxFree_real_T(&locs);
  *flag_ft = b_flag_ft;
}

//
// File trailer for PureFeatureExtract.cpp
//
// [EOF]
//
