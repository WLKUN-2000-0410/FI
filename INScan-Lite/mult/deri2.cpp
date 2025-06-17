//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: deri2.cpp
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
// lcz?????????¡Á??????
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void deri2(const emxArray_real_T *x, emxArray_real_T *y)
{
  int loop_ub;
  emxArray_real_T *a1;
  int i3;
  int i4;
  int i5;
  emxArray_real_T *b_a1;
  int i6;
  if (1 > x->size[0] - 1) {
    loop_ub = 1;
  } else {
    loop_ub = x->size[0];
  }

  emxInit_real_T(&a1, 1);
  i3 = a1->size[0];
  a1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(a1, i3);
  a1->data[0] = 0.0;
  for (i3 = 0; i3 <= loop_ub - 2; i3++) {
    a1->data[i3 + 1] = x->data[i3];
  }

  if (1 > x->size[0] - 2) {
    loop_ub = 2;
  } else {
    loop_ub = x->size[0];
  }

  i3 = y->size[0];
  y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(y, i3);
  y->data[0] = 0.0;
  y->data[1] = 0.0;
  for (i3 = 0; i3 <= loop_ub - 3; i3++) {
    y->data[i3 + 2] = x->data[i3];
  }

  //
  if (3 > x->size[0]) {
    i3 = 0;
  } else {
    i3 = 2;
  }

  if (3 > a1->size[0]) {
    i4 = 0;
    i5 = 0;
  } else {
    i4 = 2;
    i5 = a1->size[0];
  }

  emxInit_real_T(&b_a1, 1);
  i6 = b_a1->size[0];
  loop_ub = i5 - i4;
  b_a1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_a1, i6);
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_a1->data[i5] = a1->data[i4 + i5];
  }

  i4 = a1->size[0];
  a1->size[0] = b_a1->size[0];
  emxEnsureCapacity_real_T(a1, i4);
  loop_ub = b_a1->size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    a1->data[i4] = b_a1->data[i4];
  }

  if (3 > y->size[0]) {
    i4 = 0;
    i5 = 0;
  } else {
    i4 = 2;
    i5 = y->size[0];
  }

  i6 = b_a1->size[0];
  loop_ub = i5 - i4;
  b_a1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_a1, i6);
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_a1->data[i5] = y->data[i4 + i5];
  }

  i4 = y->size[0];
  y->size[0] = b_a1->size[0];
  emxEnsureCapacity_real_T(y, i4);
  loop_ub = b_a1->size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    y->data[i4] = b_a1->data[i4];
  }

  emxFree_real_T(&b_a1);
  i4 = y->size[0];
  emxEnsureCapacity_real_T(y, i4);
  loop_ub = y->size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    y->data[i4] = (y->data[i4] - a1->data[i4]) - (a1->data[i4] - x->data[i3 + i4]);
  }

  emxFree_real_T(&a1);
}

//
// File trailer for deri2.cpp
//
// [EOF]
//
