//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: calkafang.cpp
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
#include "sum.h"
#include "power.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *y
//                double noise
// Return Type  : double
//
double calkafang(const emxArray_real_T *x, const emxArray_real_T *y, double
                 noise)
{
  double result;
  emxArray_real_T *b_x;
  double c;
  int i10;
  int loop_ub;
  emxArray_real_T *r3;
  emxInit_real_T(&b_x, 1);
  c = noise * noise;
  i10 = b_x->size[0];
  b_x->size[0] = x->size[0];
  emxEnsureCapacity_real_T(b_x, i10);
  loop_ub = x->size[0];
  for (i10 = 0; i10 < loop_ub; i10++) {
    b_x->data[i10] = x->data[i10] - y->data[i10];
  }

  emxInit_real_T(&r3, 1);
  power(b_x, r3);
  i10 = r3->size[0];
  emxEnsureCapacity_real_T(r3, i10);
  loop_ub = r3->size[0];
  emxFree_real_T(&b_x);
  for (i10 = 0; i10 < loop_ub; i10++) {
    r3->data[i10] /= c;
  }

  result = sum(r3);
  emxFree_real_T(&r3);
  return result;
}

//
// File trailer for calkafang.cpp
//
// [EOF]
//
