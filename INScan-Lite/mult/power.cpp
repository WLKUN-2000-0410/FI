//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: power.cpp
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
#include "power.h"
#include "analyseMixture_emxutil.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// Arguments    : const double a_data[]
//                const int a_size[1]
//                double b
//                double y_data[]
//                int y_size[1]
// Return Type  : void
//
void b_power(const double a_data[], const int a_size[1], double b, double
             y_data[], int y_size[1])
{
  int nx;
  int k;
  y_size[0] = static_cast<signed char>(a_size[0]);
  nx = static_cast<signed char>(a_size[0]);
  for (k = 0; k < nx; k++) {
    y_data[k] = rt_powd_snf(a_data[k], b);
  }
}

//
// Arguments    : const emxArray_real_T *a
//                double b
//                emxArray_real_T *y
// Return Type  : void
//
void c_power(const emxArray_real_T *a, double b, emxArray_real_T *y)
{
  int nx;
  int k;
  nx = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = a->size[1];
  emxEnsureCapacity_real_T(y, nx);
  nx = a->size[1];
  for (k = 0; k < nx; k++) {
    y->data[k] = rt_powd_snf(a->data[k], b);
  }
}

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *y
// Return Type  : void
//
void power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int a_idx_0;
  int nx;
  int k;
  a_idx_0 = static_cast<unsigned int>(a->size[0]);
  nx = y->size[0];
  y->size[0] = static_cast<int>(a_idx_0);
  emxEnsureCapacity_real_T(y, nx);
  a_idx_0 = static_cast<unsigned int>(a->size[0]);
  nx = static_cast<int>(a_idx_0);
  for (k = 0; k < nx; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

//
// File trailer for power.cpp
//
// [EOF]
//
