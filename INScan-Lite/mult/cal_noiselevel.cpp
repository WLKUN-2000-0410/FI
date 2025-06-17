//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: cal_noiselevel.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
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
#include "combineVectorElements.h"
#include "analyseMixture_emxutil.h"
#include "std.h"

// Function Definitions

//
// lcz:计算噪声水平
// Arguments    : emxArray_real_T *data
// Return Type  : double
//
double b_cal_noiselevel(emxArray_real_T *data)
{
  emxArray_real_T *noise;
  int numsegments;
  int i60;
  int idx;
  emxArray_real_T *b_data;
  int nx;
  int i61;
  unsigned int u0;
  double kk;
  int i62;
  int exitg1;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  boolean_T exitg2;
  double a;
  double b;
  double i;
  double d11;
  emxInit_real_T(&noise, 2);

  //  window=window;
  numsegments = static_cast<int>(std::ceil(static_cast<double>(data->size[0]) /
    16.0));
  i60 = noise->size[0] * noise->size[1];
  noise->size[0] = numsegments;
  noise->size[1] = 2;
  emxEnsureCapacity_real_T(noise, i60);
  idx = numsegments << 1;
  for (i60 = 0; i60 < idx; i60++) {
    noise->data[i60] = 0.0;
  }

  emxInit_real_T(&b_data, 1);
  for (nx = 0; nx < numsegments; nx++) {
    if (1 + nx == numsegments) {
      i60 = nx << 4;
      if (i60 + 1 > data->size[0]) {
        i60 = 0;
        i61 = 0;
      } else {
        i61 = data->size[0];
      }

      i62 = b_data->size[0];
      idx = i61 - i60;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i62);
      for (i61 = 0; i61 < idx; i61++) {
        b_data->data[i61] = data->data[i60 + i61];
      }

      noise->data[nx] = b_std(b_data);
      i61 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i61);
      for (i61 = 0; i61 < idx; i61++) {
        b_data->data[i61] = data->data[i60 + i61];
      }

      noise->data[nx + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    } else {
      i60 = nx << 4;
      u0 = static_cast<unsigned int>((1 + nx)) << 4;
      if (static_cast<unsigned int>((i60 + 1)) > u0) {
        i60 = 0;
        i61 = 0;
      } else {
        i61 = static_cast<int>(u0);
      }

      i62 = b_data->size[0];
      idx = i61 - i60;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i62);
      for (i61 = 0; i61 < idx; i61++) {
        b_data->data[i61] = data->data[i60 + i61];
      }

      noise->data[nx] = b_std(b_data);
      i61 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i61);
      for (i61 = 0; i61 < idx; i61++) {
        b_data->data[i61] = data->data[i60 + i61];
      }

      noise->data[nx + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    }
  }

  i60 = noise->size[0];
  i61 = noise->size[0];
  if (i61 <= 2) {
    i60 = noise->size[0];
    if (i60 == 1) {
      kk = noise->data[0];
    } else if ((noise->data[0] > noise->data[1]) || (rtIsNaN(noise->data[0]) &&
                (!rtIsNaN(noise->data[1])))) {
      kk = noise->data[1];
    } else {
      kk = noise->data[0];
    }
  } else {
    if (!rtIsNaN(noise->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      numsegments = 2;
      do {
        exitg1 = 0;
        i61 = noise->size[0];
        if (numsegments <= i61) {
          if (!rtIsNaN(noise->data[numsegments - 1])) {
            idx = numsegments;
            exitg1 = 1;
          } else {
            numsegments++;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    if (idx == 0) {
      kk = noise->data[0];
    } else {
      kk = noise->data[idx - 1];
      i61 = idx + 1;
      for (numsegments = i61; numsegments <= i60; numsegments++) {
        if (kk > noise->data[numsegments - 1]) {
          kk = noise->data[numsegments - 1];
        }
      }
    }
  }

  emxInit_boolean_T(&x, 1);
  idx = noise->size[0];
  i60 = x->size[0];
  x->size[0] = idx;
  emxEnsureCapacity_boolean_T(x, i60);
  for (i60 = 0; i60 < idx; i60++) {
    x->data[i60] = (noise->data[i60] == kk);
  }

  emxInit_int32_T(&ii, 1);
  nx = x->size[0];
  idx = 0;
  i60 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(ii, i60);
  numsegments = 0;
  exitg2 = false;
  while ((!exitg2) && (numsegments <= nx - 1)) {
    if (x->data[numsegments]) {
      idx++;
      ii->data[idx - 1] = numsegments + 1;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        numsegments++;
      }
    } else {
      numsegments++;
    }
  }

  if (x->size[0] == 1) {
    if (idx == 0) {
      ii->size[0] = 0;
    }
  } else if (1 > idx) {
    ii->size[0] = 0;
  } else {
    i60 = ii->size[0];
    ii->size[0] = idx;
    emxEnsureCapacity_int32_T(ii, i60);
  }

  emxFree_boolean_T(&x);
  nx = ii->data[0];
  a = noise->data[(ii->data[0] + noise->size[0]) - 1];
  b = noise->data[ii->data[0] - 1];

  //  checkrightnum=(length(c0)-window*keysegmentnum);
  //  checkrightnum=checkrightnum(1);
  //  关键窗口向右搜索
  i = 1.0;
  kk = 16.0 * static_cast<double>(ii->data[0]) + 1.0;
  emxFree_int32_T(&ii);
  emxFree_real_T(&noise);
  while (kk <= data->size[0]) {
    i60 = static_cast<int>(kk) - 1;
    if ((data->data[i60] <= a - 7.0 * b) || (data->data[i60] >= a + 7.0 * b)) {
      //        kk;
      kk = ((1.0 + i) - 1.0) + 16.0 * (static_cast<double>(nx) - 1.0);
      d11 = (16.0 * static_cast<double>(nx) + i) - 1.0;
      if (kk > d11) {
        i61 = 0;
        i62 = 0;
      } else {
        i61 = static_cast<int>(kk) - 1;
        i62 = static_cast<int>(d11);
      }

      numsegments = b_data->size[0];
      idx = i62 - i61;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, numsegments);
      for (i62 = 0; i62 < idx; i62++) {
        b_data->data[i62] = data->data[i61 + i62];
      }

      data->data[i60] = combineVectorElements(b_data) / (static_cast<double>
        ((idx - 1)) + 1.0);
      i++;
      kk = 16.0 * static_cast<double>(nx) + i;
    } else {
      i++;
      kk = 16.0 * static_cast<double>(nx) + i;
    }
  }

  emxFree_real_T(&b_data);

  //  关键窗口向左搜索
  return b_std(data) / 2.4494897427831779;
}

//
// lcz:计算噪声水平
// Arguments    : emxArray_real_T *data
// Return Type  : double
//
double c_cal_noiselevel(emxArray_real_T *data)
{
  emxArray_real_T *noise;
  int numsegments;
  int i64;
  int idx;
  emxArray_real_T *b_data;
  int nx;
  emxArray_real_T *stdtemp;
  unsigned int u1;
  int i65;
  int i66;
  double kk;
  int exitg1;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  boolean_T exitg2;
  double a;
  double b;
  double i;
  double d12;
  emxInit_real_T(&noise, 2);

  //  window=window;
  numsegments = static_cast<int>(std::ceil(static_cast<double>(data->size[0]) /
    21.0));
  i64 = noise->size[0] * noise->size[1];
  noise->size[0] = numsegments;
  noise->size[1] = 2;
  emxEnsureCapacity_real_T(noise, i64);
  idx = numsegments << 1;
  for (i64 = 0; i64 < idx; i64++) {
    noise->data[i64] = 0.0;
  }

  emxInit_real_T(&b_data, 1);
  for (nx = 0; nx < numsegments; nx++) {
    if (1 + nx == numsegments) {
      i64 = 21 * nx;
      if (i64 + 1 > data->size[0]) {
        i64 = 0;
        i65 = 0;
      } else {
        i65 = data->size[0];
      }

      i66 = b_data->size[0];
      idx = i65 - i64;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i66);
      for (i65 = 0; i65 < idx; i65++) {
        b_data->data[i65] = data->data[i64 + i65];
      }

      noise->data[nx] = b_std(b_data);
      i65 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i65);
      for (i65 = 0; i65 < idx; i65++) {
        b_data->data[i65] = data->data[i64 + i65];
      }

      noise->data[nx + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    } else {
      i64 = 21 * nx;
      u1 = 21U * (1 + nx);
      if (static_cast<unsigned int>((i64 + 1)) > u1) {
        i64 = 0;
        i65 = 0;
      } else {
        i65 = static_cast<int>(u1);
      }

      i66 = b_data->size[0];
      idx = i65 - i64;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i66);
      for (i65 = 0; i65 < idx; i65++) {
        b_data->data[i65] = data->data[i64 + i65];
      }

      noise->data[nx] = b_std(b_data);
      i65 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i65);
      for (i65 = 0; i65 < idx; i65++) {
        b_data->data[i65] = data->data[i64 + i65];
      }

      noise->data[nx + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    }
  }

  emxInit_real_T(&stdtemp, 1);
  idx = noise->size[0];
  i64 = stdtemp->size[0];
  stdtemp->size[0] = idx;
  emxEnsureCapacity_real_T(stdtemp, i64);
  for (i64 = 0; i64 < idx; i64++) {
    stdtemp->data[i64] = noise->data[i64];
  }

  i64 = noise->size[0];
  i65 = noise->size[0];
  if (i65 <= 2) {
    i64 = noise->size[0];
    if (i64 == 1) {
      kk = noise->data[0];
    } else if ((noise->data[0] > noise->data[1]) || (rtIsNaN(noise->data[0]) &&
                (!rtIsNaN(noise->data[1])))) {
      kk = noise->data[1];
    } else {
      kk = noise->data[0];
    }
  } else {
    if (!rtIsNaN(noise->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      numsegments = 2;
      do {
        exitg1 = 0;
        i65 = noise->size[0];
        if (numsegments <= i65) {
          if (!rtIsNaN(noise->data[numsegments - 1])) {
            idx = numsegments;
            exitg1 = 1;
          } else {
            numsegments++;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    if (idx == 0) {
      kk = noise->data[0];
    } else {
      kk = noise->data[idx - 1];
      i65 = idx + 1;
      for (numsegments = i65; numsegments <= i64; numsegments++) {
        if (kk > stdtemp->data[numsegments - 1]) {
          kk = stdtemp->data[numsegments - 1];
        }
      }
    }
  }

  emxFree_real_T(&stdtemp);
  emxInit_boolean_T(&x, 1);
  idx = noise->size[0];
  i64 = x->size[0];
  x->size[0] = idx;
  emxEnsureCapacity_boolean_T(x, i64);
  for (i64 = 0; i64 < idx; i64++) {
    x->data[i64] = (noise->data[i64] == kk);
  }

  emxInit_int32_T(&ii, 1);
  nx = x->size[0];
  idx = 0;
  i64 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(ii, i64);
  numsegments = 0;
  exitg2 = false;
  while ((!exitg2) && (numsegments <= nx - 1)) {
    if (x->data[numsegments]) {
      idx++;
      ii->data[idx - 1] = numsegments + 1;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        numsegments++;
      }
    } else {
      numsegments++;
    }
  }

  if (x->size[0] == 1) {
    if (idx == 0) {
      ii->size[0] = 0;
    }
  } else if (1 > idx) {
    ii->size[0] = 0;
  } else {
    i64 = ii->size[0];
    ii->size[0] = idx;
    emxEnsureCapacity_int32_T(ii, i64);
  }

  emxFree_boolean_T(&x);
  nx = ii->data[0];
  a = noise->data[(ii->data[0] + noise->size[0]) - 1];
  b = noise->data[ii->data[0] - 1];

  //  checkrightnum=(length(c0)-window*keysegmentnum);
  //  checkrightnum=checkrightnum(1);
  //  关键窗口向右搜索
  i = 1.0;
  kk = 21.0 * static_cast<double>(ii->data[0]) + 1.0;
  emxFree_int32_T(&ii);
  emxFree_real_T(&noise);
  while (kk <= data->size[0]) {
    i64 = static_cast<int>(kk) - 1;
    if ((data->data[i64] <= a - 7.0 * b) || (data->data[i64] >= a + 7.0 * b)) {
      //        kk;
      kk = ((1.0 + i) - 1.0) + 21.0 * (static_cast<double>(nx) - 1.0);
      d12 = (21.0 * static_cast<double>(nx) + i) - 1.0;
      if (kk > d12) {
        i65 = 0;
        i66 = 0;
      } else {
        i65 = static_cast<int>(kk) - 1;
        i66 = static_cast<int>(d12);
      }

      numsegments = b_data->size[0];
      idx = i66 - i65;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, numsegments);
      for (i66 = 0; i66 < idx; i66++) {
        b_data->data[i66] = data->data[i65 + i66];
      }

      data->data[i64] = combineVectorElements(b_data) / (static_cast<double>
        ((idx - 1)) + 1.0);
      i++;
      kk = 21.0 * static_cast<double>(nx) + i;
    } else {
      i++;
      kk = 21.0 * static_cast<double>(nx) + i;
    }
  }

  emxFree_real_T(&b_data);

  //  关键窗口向左搜索
  return b_std(data) / 2.4494897427831779;
}

//
// lcz:计算噪声水平
// Arguments    : const emxArray_real_T *data
//                double window
//                double *noiselevel2
//                emxArray_real_T *c1
// Return Type  : void
//
void cal_noiselevel(const emxArray_real_T *data, double window, double
                    *noiselevel2, emxArray_real_T *c1)
{
  int i19;
  int idx;
  emxArray_real_T *noise;
  double numsegments;
  int i20;
  emxArray_real_T *b_data;
  int i;
  double d1;
  double d2;
  int i21;
  int i22;
  int exitg1;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  int nx;
  boolean_T exitg2;
  emxArray_uint32_T *keysegmentnum;
  double a;
  double b;
  double b_i;
  i19 = c1->size[0];
  c1->size[0] = data->size[0];
  emxEnsureCapacity_real_T(c1, i19);
  idx = data->size[0];
  for (i19 = 0; i19 < idx; i19++) {
    c1->data[i19] = data->data[i19];
  }

  emxInit_real_T(&noise, 2);

  //  window=window;
  numsegments = std::ceil(static_cast<double>(data->size[0]) / window);
  i19 = noise->size[0] * noise->size[1];
  i20 = static_cast<int>(numsegments);
  noise->size[0] = i20;
  noise->size[1] = 2;
  emxEnsureCapacity_real_T(noise, i19);
  idx = i20 << 1;
  for (i19 = 0; i19 < idx; i19++) {
    noise->data[i19] = 0.0;
  }

  emxInit_real_T(&b_data, 1);
  for (i = 0; i < i20; i++) {
    if (1.0 + static_cast<double>(i) == numsegments) {
      d1 = 1.0 + window * ((1.0 + static_cast<double>(i)) - 1.0);
      if (d1 > data->size[0]) {
        i19 = 0;
        i21 = 0;
      } else {
        i19 = static_cast<int>(d1) - 1;
        i21 = data->size[0];
      }

      i22 = b_data->size[0];
      idx = i21 - i19;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i22);
      for (i21 = 0; i21 < idx; i21++) {
        b_data->data[i21] = data->data[i19 + i21];
      }

      noise->data[i] = b_std(b_data);
      i21 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i21);
      for (i21 = 0; i21 < idx; i21++) {
        b_data->data[i21] = data->data[i19 + i21];
      }

      noise->data[i + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    } else {
      d1 = 1.0 + window * ((1.0 + static_cast<double>(i)) - 1.0);
      d2 = window * (1.0 + static_cast<double>(i));
      if (d1 > d2) {
        i19 = 0;
        i21 = 0;
      } else {
        i19 = static_cast<int>(d1) - 1;
        i21 = static_cast<int>(d2);
      }

      i22 = b_data->size[0];
      idx = i21 - i19;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i22);
      for (i21 = 0; i21 < idx; i21++) {
        b_data->data[i21] = data->data[i19 + i21];
      }

      noise->data[i] = b_std(b_data);
      i21 = b_data->size[0];
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i21);
      for (i21 = 0; i21 < idx; i21++) {
        b_data->data[i21] = data->data[i19 + i21];
      }

      noise->data[i + noise->size[0]] = combineVectorElements(b_data) /
        (static_cast<double>((idx - 1)) + 1.0);
    }
  }

  i19 = noise->size[0];
  i20 = noise->size[0];
  if (i20 <= 2) {
    i19 = noise->size[0];
    if (i19 == 1) {
      numsegments = noise->data[0];
    } else if ((noise->data[0] > noise->data[1]) || (rtIsNaN(noise->data[0]) &&
                (!rtIsNaN(noise->data[1])))) {
      numsegments = noise->data[1];
    } else {
      numsegments = noise->data[0];
    }
  } else {
    if (!rtIsNaN(noise->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      i = 2;
      do {
        exitg1 = 0;
        i20 = noise->size[0];
        if (i <= i20) {
          if (!rtIsNaN(noise->data[i - 1])) {
            idx = i;
            exitg1 = 1;
          } else {
            i++;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }

    if (idx == 0) {
      numsegments = noise->data[0];
    } else {
      numsegments = noise->data[idx - 1];
      i20 = idx + 1;
      for (i = i20; i <= i19; i++) {
        if (numsegments > noise->data[i - 1]) {
          numsegments = noise->data[i - 1];
        }
      }
    }
  }

  emxInit_boolean_T(&x, 1);
  idx = noise->size[0];
  i19 = x->size[0];
  x->size[0] = idx;
  emxEnsureCapacity_boolean_T(x, i19);
  for (i19 = 0; i19 < idx; i19++) {
    x->data[i19] = (noise->data[i19] == numsegments);
  }

  emxInit_int32_T(&ii, 1);
  nx = x->size[0];
  idx = 0;
  i19 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(ii, i19);
  i = 0;
  exitg2 = false;
  while ((!exitg2) && (i <= nx - 1)) {
    if (x->data[i]) {
      idx++;
      ii->data[idx - 1] = i + 1;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        i++;
      }
    } else {
      i++;
    }
  }

  if (x->size[0] == 1) {
    if (idx == 0) {
      ii->size[0] = 0;
    }
  } else if (1 > idx) {
    ii->size[0] = 0;
  } else {
    i19 = ii->size[0];
    ii->size[0] = idx;
    emxEnsureCapacity_int32_T(ii, i19);
  }

  emxFree_boolean_T(&x);
  emxInit_uint32_T(&keysegmentnum, 1);
  i19 = keysegmentnum->size[0];
  keysegmentnum->size[0] = ii->size[0];
  emxEnsureCapacity_uint32_T(keysegmentnum, i19);
  idx = ii->size[0];
  for (i19 = 0; i19 < idx; i19++) {
    keysegmentnum->data[i19] = static_cast<unsigned int>(ii->data[i19]);
  }

  emxFree_int32_T(&ii);
  nx = static_cast<int>(keysegmentnum->data[0]);
  a = noise->data[(static_cast<int>(keysegmentnum->data[0]) + noise->size[0]) -
    1];
  b = noise->data[static_cast<int>(keysegmentnum->data[0]) - 1];

  //  checkrightnum=(length(c0)-window*keysegmentnum);
  //  checkrightnum=checkrightnum(1);
  //  关键窗口向右搜索
  b_i = 1.0;
  numsegments = window * static_cast<double>(keysegmentnum->data[0]) + 1.0;
  emxFree_uint32_T(&keysegmentnum);
  emxFree_real_T(&noise);
  while (numsegments <= c1->size[0]) {
    i19 = static_cast<int>(numsegments) - 1;
    if ((c1->data[i19] <= a - 7.0 * b) || (c1->data[i19] >= a + 7.0 * b)) {
      //        kk;
      d1 = ((1.0 + b_i) - 1.0) + window * (static_cast<double>(nx) - 1.0);
      d2 = (window * static_cast<double>(nx) + b_i) - 1.0;
      if (d1 > d2) {
        i20 = 0;
        i21 = 0;
      } else {
        i20 = static_cast<int>(d1) - 1;
        i21 = static_cast<int>(d2);
      }

      i22 = b_data->size[0];
      idx = i21 - i20;
      b_data->size[0] = idx;
      emxEnsureCapacity_real_T(b_data, i22);
      for (i21 = 0; i21 < idx; i21++) {
        b_data->data[i21] = c1->data[i20 + i21];
      }

      c1->data[i19] = combineVectorElements(b_data) / (static_cast<double>((idx
        - 1)) + 1.0);
      b_i++;
      numsegments = window * static_cast<double>(nx) + b_i;
    } else {
      b_i++;
      numsegments = window * static_cast<double>(nx) + b_i;
    }
  }

  emxFree_real_T(&b_data);

  //  关键窗口向左搜索
  *noiselevel2 = b_std(c1) / 2.4494897427831779;
}

//
// File trailer for cal_noiselevel.cpp
//
// [EOF]
//
