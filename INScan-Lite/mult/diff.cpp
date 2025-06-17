//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: diff.cpp
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
#include "diff.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void b_diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int dimSize;
  int orderForDim;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp1;
  double tmp2;
  dimSize = x->size[1];
  if (x->size[1] == 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    orderForDim = x->size[1] - 1;
    if (orderForDim >= 1) {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      y->size[0] = 1;
      y->size[1] = 0;
    } else {
      orderForDim = x->size[1] - 1;
      iyLead = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = orderForDim;
      emxEnsureCapacity_real_T(y, iyLead);
      if (y->size[1] != 0) {
        orderForDim = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= dimSize; m++) {
          tmp1 = x->data[orderForDim];
          tmp2 = work_data_idx_0;
          work_data_idx_0 = tmp1;
          tmp1 -= tmp2;
          orderForDim++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void c_diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int stride;
  int ix;
  int iy;
  int s;
  stride = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_real_T(y, stride);
  if (y->size[0] != 0) {
    stride = x->size[0];
    ix = 0;
    iy = 0;
    for (s = 0; s < stride; s++) {
      y->data[iy] = x->data[ix + stride] - x->data[ix];
      ix++;
      iy++;
    }
  }
}

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int dimSize;
  int orderForDim;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp1;
  double tmp2;
  dimSize = x->size[0];
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    orderForDim = x->size[0] - 1;
    if (orderForDim >= 1) {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      y->size[0] = 0;
    } else {
      orderForDim = x->size[0] - 1;
      iyLead = y->size[0];
      y->size[0] = orderForDim;
      emxEnsureCapacity_real_T(y, iyLead);
      if (y->size[0] != 0) {
        orderForDim = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= dimSize; m++) {
          tmp1 = x->data[orderForDim];
          tmp2 = work_data_idx_0;
          work_data_idx_0 = tmp1;
          tmp1 -= tmp2;
          orderForDim++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

//
// File trailer for diff.cpp
//
// [EOF]
//
