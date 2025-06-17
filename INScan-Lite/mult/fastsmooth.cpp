//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fastsmooth.cpp
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
#include "combineVectorElements.h"
#include "sum.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *Y
//                emxArray_real_T *SmoothY
// Return Type  : void
//
void b_sa(const emxArray_real_T *Y, emxArray_real_T *SmoothY)
{
  double SumPoints;
  unsigned int unnamed_idx_0;
  int i11;
  int loop_ub;
  int i12;
  emxArray_real_T *b_Y;
  int i13;
  SumPoints = Y->data[0];
  SumPoints += Y->data[1];
  SumPoints += Y->data[2];
  unnamed_idx_0 = static_cast<unsigned int>(Y->size[0]);
  i11 = SmoothY->size[0];
  SmoothY->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(SmoothY, i11);
  loop_ub = static_cast<int>(unnamed_idx_0);
  for (i11 = 0; i11 < loop_ub; i11++) {
    SmoothY->data[i11] = 0.0;
  }

  i11 = Y->size[0];
  for (loop_ub = 0; loop_ub <= i11 - 4; loop_ub++) {
    SmoothY->data[loop_ub + 1] = SumPoints;
    SumPoints -= Y->data[loop_ub];
    SumPoints += Y->data[loop_ub + 3];
  }

  if (Y->size[0] - 2 > Y->size[0]) {
    i11 = 0;
    i12 = 0;
  } else {
    i11 = Y->size[0] - 3;
    i12 = Y->size[0];
  }

  emxInit_real_T(&b_Y, 1);
  i13 = b_Y->size[0];
  loop_ub = i12 - i11;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i13);
  for (i12 = 0; i12 < loop_ub; i12++) {
    b_Y->data[i12] = Y->data[i11 + i12];
  }

  SmoothY->data[Y->size[0] - 2] = sum(b_Y);
  i11 = SmoothY->size[0];
  emxEnsureCapacity_real_T(SmoothY, i11);
  loop_ub = SmoothY->size[0];
  for (i11 = 0; i11 < loop_ub; i11++) {
    SmoothY->data[i11] /= 3.0;
  }

  //  Taper the ends of the signal if ends=1.
  SmoothY->data[0] = (Y->data[0] + Y->data[1]) / 2.0;
  SumPoints = Y->data[0];
  SumPoints += Y->data[1];
  SumPoints += Y->data[2];
  SmoothY->data[1] = SumPoints / 3.0;
  if (Y->size[0] - 2 > Y->size[0]) {
    i11 = 0;
    i12 = 0;
  } else {
    i11 = Y->size[0] - 3;
    i12 = Y->size[0];
  }

  i13 = b_Y->size[0];
  loop_ub = i12 - i11;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i13);
  for (i12 = 0; i12 < loop_ub; i12++) {
    b_Y->data[i12] = Y->data[i11 + i12];
  }

  SmoothY->data[Y->size[0] - 2] = combineVectorElements(b_Y) / (static_cast<
    double>((loop_ub - 1)) + 1.0);
  SmoothY->data[Y->size[0] - 1] = (Y->data[Y->size[0] - 1] + Y->data[Y->size[0]
    - 2]) / 2.0;
  emxFree_real_T(&b_Y);
}

//
// Arguments    : const emxArray_real_T *Y
//                double smoothwidth
//                emxArray_real_T *SmoothY
// Return Type  : void
//
void c_sa(const emxArray_real_T *Y, double smoothwidth, emxArray_real_T *SmoothY)
{
  double w;
  int loop_ub;
  emxArray_real_T *b_Y;
  int i14;
  double SumPoints;
  unsigned int unnamed_idx_0;
  double halfw;
  int varargin_2;
  int k;
  int i15;
  int i16;
  int i17;
  w = rt_roundd_snf(smoothwidth);
  if (1.0 > w) {
    loop_ub = 0;
  } else {
    loop_ub = static_cast<int>(w);
  }

  emxInit_real_T(&b_Y, 1);
  i14 = b_Y->size[0];
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i14);
  for (i14 = 0; i14 < loop_ub; i14++) {
    b_Y->data[i14] = Y->data[i14];
  }

  SumPoints = sum(b_Y);
  unnamed_idx_0 = static_cast<unsigned int>(Y->size[0]);
  i14 = SmoothY->size[0];
  SmoothY->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(SmoothY, i14);
  loop_ub = static_cast<int>(unnamed_idx_0);
  for (i14 = 0; i14 < loop_ub; i14++) {
    SmoothY->data[i14] = 0.0;
  }

  halfw = rt_roundd_snf(w / 2.0);
  varargin_2 = Y->size[0];
  i14 = static_cast<int>((static_cast<double>(Y->size[0]) - w));
  for (k = 0; k < i14; k++) {
    SmoothY->data[static_cast<int>((((1.0 + static_cast<double>(k)) + halfw) -
      1.0)) - 1] = SumPoints;
    SumPoints -= Y->data[k];
    SumPoints += Y->data[static_cast<int>(((1.0 + static_cast<double>(k)) + w))
      - 1];
  }

  SumPoints = (static_cast<double>(Y->size[0]) - w) + 1.0;
  if (SumPoints > Y->size[0]) {
    i14 = 0;
    i15 = 0;
  } else {
    i14 = static_cast<int>(SumPoints) - 1;
    i15 = Y->size[0];
  }

  i16 = b_Y->size[0];
  loop_ub = i15 - i14;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i16);
  for (i15 = 0; i15 < loop_ub; i15++) {
    b_Y->data[i15] = Y->data[i14 + i15];
  }

  SmoothY->data[static_cast<int>(((static_cast<double>(Y->size[0]) - w) + halfw))
    - 1] = sum(b_Y);
  i14 = SmoothY->size[0];
  emxEnsureCapacity_real_T(SmoothY, i14);
  loop_ub = SmoothY->size[0];
  for (i14 = 0; i14 < loop_ub; i14++) {
    SmoothY->data[i14] /= w;
  }

  //  Taper the ends of the signal if ends=1.
  SmoothY->data[0] = (Y->data[0] + Y->data[1]) / 2.0;
  i14 = static_cast<int>(((smoothwidth + 1.0) / 2.0 + -1.0));
  for (k = 0; k < i14; k++) {
    loop_ub = 2 * (2 + k) - 1;
    i15 = b_Y->size[0];
    b_Y->size[0] = loop_ub;
    emxEnsureCapacity_real_T(b_Y, i15);
    for (i15 = 0; i15 < loop_ub; i15++) {
      b_Y->data[i15] = Y->data[i15];
    }

    SmoothY->data[k + 1] = combineVectorElements(b_Y) / (static_cast<double>
      ((loop_ub - 1)) + 1.0);
    SumPoints = (static_cast<double>(varargin_2) - 2.0 * (2.0 + static_cast<
      double>(k))) + 2.0;
    if (SumPoints > Y->size[0]) {
      i15 = 0;
      i16 = 0;
    } else {
      i15 = static_cast<int>(SumPoints) - 1;
      i16 = varargin_2;
    }

    i17 = b_Y->size[0];
    loop_ub = i16 - i15;
    b_Y->size[0] = loop_ub;
    emxEnsureCapacity_real_T(b_Y, i17);
    for (i16 = 0; i16 < loop_ub; i16++) {
      b_Y->data[i16] = Y->data[i15 + i16];
    }

    SmoothY->data[(varargin_2 - k) - 2] = combineVectorElements(b_Y) /
      (static_cast<double>((loop_ub - 1)) + 1.0);
  }

  emxFree_real_T(&b_Y);
  SmoothY->data[Y->size[0] - 1] = (Y->data[Y->size[0] - 1] + Y->data[Y->size[0]
    - 2]) / 2.0;
}

//
// lcz???им??????
//  fastbsmooth(Y,w,type,ends) smooths vector Y with smooth
//   of width w. Version 2.0, May 2008.
//  The argument "type" determines the smooth type:
//    If type=1, rectangular (sliding-average or boxcar)
//    If type=2, triangular (2 passes of sliding-average)
//    If type=3, pseudo-Gaussian (3 passes of sliding-average)
//  The argument "ends" controls how the "ends" of the signal
//  (the first w/2 points and the last w/2 points) are handled.
//    If ends=0, the ends are zero.  (In this mode the elapsed
//      time is independent of the smooth width). The fastest.
//    If ends=1, the ends are smoothed with progressively
//      smaller smooths the closer to the end. (In this mode the
//      elapsed time increases with increasing smooth widths).
//  fastsmooth(Y,w,type) smooths with ends=0.
//  fastsmooth(Y,w) smooths with type=1 and ends=0.
//  Example:
//  fastsmooth([1 1 1 10 10 10 1 1 1 1],3)= [0 1 4 7 10 7 4 1 1 0]
//  fastsmooth([1 1 1 10 10 10 1 1 1 1],3,1,1)= [1 1 4 7 10 7 4 1 1 1]
//   T. C. O'Haver, May, 2008.
//  if nargin==2, ends=0; type=1; end
//  if nargin==3, ends=0; end
// Arguments    : const emxArray_real_T *Y
//                double w
//                double type
//                emxArray_real_T *SmoothY
// Return Type  : void
//
void fastsmooth(const emxArray_real_T *Y, double w, double type, emxArray_real_T
                *SmoothY)
{
  emxArray_real_T *r4;
  int i26;
  emxArray_real_T *r5;
  emxInit_real_T(&r4, 1);
  switch (static_cast<int>(type)) {
   case 1:
    c_sa(Y, w, SmoothY);
    break;

   case 2:
    c_sa(Y, w, r4);
    c_sa(r4, w, SmoothY);
    break;

   case 3:
    emxInit_real_T(&r5, 1);
    c_sa(Y, w, r4);
    c_sa(r4, w, r5);
    c_sa(r5, w, SmoothY);
    emxFree_real_T(&r5);
    break;

   default:
    i26 = SmoothY->size[0];
    SmoothY->size[0] = 1;
    emxEnsureCapacity_real_T(SmoothY, i26);
    SmoothY->data[0] = 0.0;
    break;
  }

  emxFree_real_T(&r4);
}

//
// Arguments    : const emxArray_real_T *Y
//                emxArray_real_T *SmoothY
// Return Type  : void
//
void sa(const emxArray_real_T *Y, emxArray_real_T *SmoothY)
{
  double SumPoints;
  unsigned int unnamed_idx_0;
  int i7;
  int loop_ub;
  int varargin_2;
  int i8;
  emxArray_real_T *b_Y;
  int i9;
  int Y_size[1];
  emxArray_real_T Y_data;
  double b_Y_data[5];
  SumPoints = Y->data[0];
  SumPoints += Y->data[1];
  SumPoints += Y->data[2];
  SumPoints += Y->data[3];
  SumPoints += Y->data[4];
  unnamed_idx_0 = static_cast<unsigned int>(Y->size[0]);
  i7 = SmoothY->size[0];
  SmoothY->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(SmoothY, i7);
  loop_ub = static_cast<int>(unnamed_idx_0);
  for (i7 = 0; i7 < loop_ub; i7++) {
    SmoothY->data[i7] = 0.0;
  }

  varargin_2 = Y->size[0];
  i7 = Y->size[0];
  for (loop_ub = 0; loop_ub <= i7 - 6; loop_ub++) {
    SmoothY->data[loop_ub + 2] = SumPoints;
    SumPoints -= Y->data[loop_ub];
    SumPoints += Y->data[loop_ub + 5];
  }

  if (Y->size[0] - 4 > Y->size[0]) {
    i7 = 0;
    i8 = 0;
  } else {
    i7 = Y->size[0] - 5;
    i8 = Y->size[0];
  }

  emxInit_real_T(&b_Y, 1);
  i9 = b_Y->size[0];
  loop_ub = i8 - i7;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i9);
  for (i8 = 0; i8 < loop_ub; i8++) {
    b_Y->data[i8] = Y->data[i7 + i8];
  }

  SmoothY->data[Y->size[0] - 3] = sum(b_Y);
  i7 = SmoothY->size[0];
  emxEnsureCapacity_real_T(SmoothY, i7);
  loop_ub = SmoothY->size[0];
  for (i7 = 0; i7 < loop_ub; i7++) {
    SmoothY->data[i7] /= 5.0;
  }

  //  Taper the ends of the signal if ends=1.
  SmoothY->data[0] = (Y->data[0] + Y->data[1]) / 2.0;
  Y_size[0] = 3;
  for (i7 = 0; i7 < 3; i7++) {
    b_Y_data[i7] = Y->data[i7];
  }

  Y_data.data = &b_Y_data[0];
  Y_data.size = &Y_size[0];
  Y_data.allocatedSize = 5;
  Y_data.numDimensions = 1;
  Y_data.canFreeData = false;
  SmoothY->data[1] = combineVectorElements(&Y_data) / 3.0;
  i7 = varargin_2 - 4;
  if (i7 + 2 > Y->size[0]) {
    i7 = 0;
    i8 = 0;
  } else {
    i7++;
    i8 = varargin_2;
  }

  i9 = b_Y->size[0];
  loop_ub = i8 - i7;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i9);
  for (i8 = 0; i8 < loop_ub; i8++) {
    b_Y->data[i8] = Y->data[i7 + i8];
  }

  SmoothY->data[varargin_2 - 2] = combineVectorElements(b_Y) / (static_cast<
    double>((loop_ub - 1)) + 1.0);
  Y_size[0] = 5;
  for (i7 = 0; i7 < 5; i7++) {
    b_Y_data[i7] = Y->data[i7];
  }

  Y_data.data = &b_Y_data[0];
  Y_data.size = &Y_size[0];
  Y_data.allocatedSize = 5;
  Y_data.numDimensions = 1;
  Y_data.canFreeData = false;
  SmoothY->data[2] = combineVectorElements(&Y_data) / 5.0;
  i7 = varargin_2 - 6;
  if (i7 + 2 > Y->size[0]) {
    i7 = 0;
    i8 = 0;
  } else {
    i7++;
    i8 = varargin_2;
  }

  i9 = b_Y->size[0];
  loop_ub = i8 - i7;
  b_Y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_Y, i9);
  for (i8 = 0; i8 < loop_ub; i8++) {
    b_Y->data[i8] = Y->data[i7 + i8];
  }

  SmoothY->data[varargin_2 - 3] = combineVectorElements(b_Y) / (static_cast<
    double>((loop_ub - 1)) + 1.0);
  emxFree_real_T(&b_Y);
  SmoothY->data[Y->size[0] - 1] = (Y->data[Y->size[0] - 1] + Y->data[Y->size[0]
    - 2]) / 2.0;
}

//
// File trailer for fastsmooth.cpp
//
// [EOF]
//
