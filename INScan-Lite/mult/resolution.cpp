//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: resolution.cpp
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

// Function Definitions

//
// Arguments    : const emxArray_real_T *aa
//                double y_data[]
//                int y_size[2]
//                double *flag_ns
// Return Type  : void
//
void resolution(const emxArray_real_T *aa, double y_data[], int y_size[2],
                double *flag_ns)
{
  int i42;
  int i43;
  int idx;
  int k;
  double ex;
  int exitg1;
  emxArray_real_T *ns2;
  emxArray_real_T *b_ns2;
  emxArray_real_T *b_aa;
  boolean_T empty_non_axis_sizes;
  signed char input_sizes_idx_1;
  signed char sizes_idx_1;
  emxArray_real_T *c_aa;
  *flag_ns = 0.0;

  //   lcz:对数据进行nier次小波去噪，然后利用resolution_factor.m函数来进行基线校正和统一维度 
  i42 = aa->size[0];
  i43 = aa->size[0];
  if (i43 <= 2) {
    i42 = aa->size[0];
    if (i42 == 1) {
      ex = aa->data[aa->size[0]];
    } else if ((aa->data[aa->size[0]] < aa->data[1 + aa->size[0]]) || (rtIsNaN
                (aa->data[aa->size[0]]) && (!rtIsNaN(aa->data[1 + aa->size[0]]))))
    {
      ex = aa->data[1 + aa->size[0]];
    } else {
      ex = aa->data[aa->size[0]];
    }
  } else {
    if (!rtIsNaN(aa->data[aa->size[0]])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      do {
        exitg1 = 0;
        i43 = aa->size[0];
        if (k <= i43) {
          if (!rtIsNaN(aa->data[(k + aa->size[0]) - 1])) {
            idx = k;
            exitg1 = 1;
          } else {
            k++;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    if (idx == 0) {
      ex = aa->data[aa->size[0]];
    } else {
      ex = aa->data[(idx + aa->size[0]) - 1];
      i43 = idx + 1;
      for (k = i43; k <= i42; k++) {
        if (ex < aa->data[(k + aa->size[0]) - 1]) {
          ex = aa->data[(k + aa->size[0]) - 1];
        }
      }
    }
  }

  if (ex == 65535.0) {
    y_size[0] = 0;
    y_size[1] = 0;
    *flag_ns = 1.0;
  } else {
    emxInit_real_T(&ns2, 2);
    ns2->size[0] = 0;
    ns2->size[1] = 0;
    emxInit_real_T(&b_ns2, 1);
    emxInit_real_T(&b_aa, 1);
    for (idx = 0; idx < 25; idx++) {
      k = aa->size[0];
      i42 = b_aa->size[0];
      b_aa->size[0] = k;
      emxEnsureCapacity_real_T(b_aa, i42);
      for (i42 = 0; i42 < k; i42++) {
        b_aa->data[i42] = aa->data[i42 + aa->size[0]];
      }

      geto_new(b_aa, b_ns2);
      i42 = ns2->size[0] * ns2->size[1];
      ns2->size[0] = b_ns2->size[0];
      ns2->size[1] = 1;
      emxEnsureCapacity_real_T(ns2, i42);
      k = b_ns2->size[0];
      for (i42 = 0; i42 < k; i42++) {
        ns2->data[i42] = b_ns2->data[i42];
      }

      //  降噪算法
    }

    emxFree_real_T(&b_ns2);
    i42 = aa->size[0];
    if (i42 != 0) {
      idx = aa->size[0];
    } else if ((ns2->size[0] != 0) && (ns2->size[1] != 0)) {
      idx = ns2->size[0];
    } else {
      i42 = aa->size[0];
      if (i42 > 0) {
        idx = aa->size[0];
      } else {
        idx = 0;
      }

      if (ns2->size[0] > idx) {
        idx = ns2->size[0];
      }
    }

    empty_non_axis_sizes = (idx == 0);
    if (empty_non_axis_sizes) {
      input_sizes_idx_1 = 1;
    } else {
      i42 = aa->size[0];
      if (i42 != 0) {
        input_sizes_idx_1 = 1;
      } else {
        input_sizes_idx_1 = 0;
      }
    }

    if (empty_non_axis_sizes || ((ns2->size[0] != 0) && (ns2->size[1] != 0))) {
      sizes_idx_1 = static_cast<signed char>(ns2->size[1]);
    } else {
      sizes_idx_1 = 0;
    }

    k = aa->size[0];
    i42 = b_aa->size[0];
    b_aa->size[0] = k;
    emxEnsureCapacity_real_T(b_aa, i42);
    for (i42 = 0; i42 < k; i42++) {
      b_aa->data[i42] = aa->data[i42];
    }

    emxInit_real_T(&c_aa, 2);
    i42 = c_aa->size[0] * c_aa->size[1];
    c_aa->size[0] = idx;
    c_aa->size[1] = input_sizes_idx_1 + sizes_idx_1;
    emxEnsureCapacity_real_T(c_aa, i42);
    k = input_sizes_idx_1;
    for (i42 = 0; i42 < k; i42++) {
      for (i43 = 0; i43 < idx; i43++) {
        c_aa->data[i43] = b_aa->data[i43];
      }
    }

    emxFree_real_T(&b_aa);
    k = sizes_idx_1;
    for (i42 = 0; i42 < k; i42++) {
      for (i43 = 0; i43 < idx; i43++) {
        c_aa->data[i43 + c_aa->size[0] * (i42 + input_sizes_idx_1)] = ns2->
          data[i43 + ns2->size[0] * i42];
      }
    }

    emxFree_real_T(&ns2);
    b_resolution_factor(c_aa, y_data, y_size);
    emxFree_real_T(&c_aa);
  }
}

//
// File trailer for resolution.cpp
//
// [EOF]
//
