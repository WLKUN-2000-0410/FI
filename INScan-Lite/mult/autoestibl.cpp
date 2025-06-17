//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: autoestibl.cpp
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
#include "vmedian.h"
#include "abs.h"

// Function Definitions

//
// lcz:自动去基线函数，其中包括计算噪声标准差、计算卡方值等，但最后使用的是另外一个函数baseline_correct?
//  去基线函数
//  A small-window moving average-based fully automated baseline estimation
// Arguments    : const emxArray_real_T *b_signal
//                emxArray_real_T *data_quji
// Return Type  : void
//
void autoestibl(const emxArray_real_T *b_signal, emxArray_real_T *data_quji)
{
  emxArray_real_T *c00;
  emxArray_real_T *signal_1;
  emxArray_real_T *x;
  double biaozhuncha;
  double kafang1;
  double b;
  boolean_T exitg1;
  emxArray_real_T *background;
  int i2;
  int idx;
  int flag;
  emxArray_real_T *signal_afterfilter;
  emxArray_real_T *r1;
  int n;
  int i;
  emxArray_int32_T *r2;
  boolean_T exitg2;
  emxInit_real_T(&c00, 1);
  emxInit_real_T(&signal_1, 1);
  emxInit_real_T(&x, 1);

  //  计算噪声标准差
  //  signal=totest(:);
  deri2(b_signal, c00);
  biaozhuncha = b_cal_noiselevel(c00);

  //  计算卡方检验值
  //   lcz???ì??????
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
  sa(b_signal, x);
  sa(x, c00);
  sa(c00, signal_1);

  //  calkafang=@(x,y)(sum(((x-y).^2)./(tao_noise^2)));
  kafang1 = calkafang(b_signal, signal_1, biaozhuncha);
  b = 0.0;
  exitg1 = false;
  while ((!exitg1) && (kafang1 < static_cast<double>(b_signal->size[0]) / 1.1))
  {
    //   lcz???ì??????
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
    b_sa(b_signal, x);
    b_sa(x, c00);
    kafang1 = calkafang(b_signal, c00, biaozhuncha);
    i2 = signal_1->size[0];
    signal_1->size[0] = c00->size[0];
    emxEnsureCapacity_real_T(signal_1, i2);
    idx = c00->size[0];
    for (i2 = 0; i2 < idx; i2++) {
      signal_1->data[i2] = c00->data[i2];
    }

    b++;
    if (b == 200.0) {
      exitg1 = true;
    }
  }

  emxInit_real_T(&background, 1);

  //  figure(1)
  //  plot(signal);
  //  hold on
  //  plot(signal_1);
  //  hold off
  //  legend('原始光谱','平滑光谱');
  //  figure(2)
  //  plot(signal-signal_1);
  b = 1.0;
  i2 = background->size[0];
  background->size[0] = signal_1->size[0];
  emxEnsureCapacity_real_T(background, i2);
  idx = signal_1->size[0];
  for (i2 = 0; i2 < idx; i2++) {
    background->data[i2] = 1.0E+9;
  }

  i2 = c00->size[0];
  c00->size[0] = signal_1->size[0];
  emxEnsureCapacity_real_T(c00, i2);
  idx = signal_1->size[0];
  for (i2 = 0; i2 < idx; i2++) {
    c00->data[i2] = signal_1->data[i2];
  }

  flag = 1;

  //
  emxInit_real_T(&signal_afterfilter, 1);
  emxInit_real_T(&r1, 1);
  exitg1 = false;
  while ((!exitg1) && (flag != 0)) {
    //      filterb=ones(2*a+1,1)/(2*a+1);
    //      signal_afterfilter=filter(filterb,1,signal_2);
    biaozhuncha = 2.0 * b + 1.0;

    //   lcz???ì??????
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
    c_sa(c00, biaozhuncha, x);
    c_sa(x, biaozhuncha, signal_afterfilter);
    i2 = r1->size[0];
    r1->size[0] = signal_1->size[0];
    emxEnsureCapacity_real_T(r1, i2);
    idx = signal_1->size[0];
    for (i2 = 0; i2 < idx; i2++) {
      r1->data[i2] = signal_1->data[i2] - signal_afterfilter->data[i2];
    }

    //      figure(61)
    //      hold on
    //      plot(r1);
    //      hold off
    b_abs(r1, c00);
    if (c00->size[0] == 0) {
      biaozhuncha = rtNaN;
    } else {
      i2 = x->size[0];
      x->size[0] = c00->size[0];
      emxEnsureCapacity_real_T(x, i2);
      idx = c00->size[0];
      for (i2 = 0; i2 < idx; i2++) {
        x->data[i2] = c00->data[i2];
      }

      biaozhuncha = vmedian(x, c00->size[0]);
    }

    biaozhuncha /= 0.6745;
    i2 = signal_1->size[0];
    for (i = 0; i < i2; i++) {
      if (r1->data[i] < 1.5 * biaozhuncha) {
        background->data[i] = signal_1->data[i];
      } else {
        background->data[i] = signal_afterfilter->data[i];
      }
    }

    //      plot(background);
    //  迭代替换
    i2 = c00->size[0];
    c00->size[0] = background->size[0];
    emxEnsureCapacity_real_T(c00, i2);
    idx = background->size[0];
    for (i2 = 0; i2 < idx; i2++) {
      c00->data[i2] = background->data[i2];
    }

    //      ori_signal=signal_afterfilter;
    //  跳出条件
    n = r1->size[0];
    if (r1->size[0] <= 2) {
      if (r1->size[0] == 1) {
        kafang1 = r1->data[0];
      } else if ((r1->data[0] > r1->data[1]) || (rtIsNaN(r1->data[0]) &&
                  (!rtIsNaN(r1->data[1])))) {
        kafang1 = r1->data[1];
      } else {
        kafang1 = r1->data[0];
      }
    } else {
      if (!rtIsNaN(r1->data[0])) {
        idx = 1;
      } else {
        idx = 0;
        i = 2;
        exitg2 = false;
        while ((!exitg2) && (i <= r1->size[0])) {
          if (!rtIsNaN(r1->data[i - 1])) {
            idx = i;
            exitg2 = true;
          } else {
            i++;
          }
        }
      }

      if (idx == 0) {
        kafang1 = r1->data[0];
      } else {
        kafang1 = r1->data[idx - 1];
        i2 = idx + 1;
        for (i = i2; i <= n; i++) {
          if (kafang1 > r1->data[i - 1]) {
            kafang1 = r1->data[i - 1];
          }
        }
      }
    }

    if (kafang1 > -biaozhuncha * 1.4) {
      flag = 0;
    }

    b++;
    if (b == 100.0) {
      exitg1 = true;
    }
  }

  emxFree_real_T(&x);
  emxFree_real_T(&r1);
  emxFree_real_T(&signal_afterfilter);
  emxFree_real_T(&signal_1);
  emxFree_real_T(&c00);
  i2 = data_quji->size[0];
  data_quji->size[0] = b_signal->size[0];
  emxEnsureCapacity_real_T(data_quji, i2);
  idx = b_signal->size[0];
  for (i2 = 0; i2 < idx; i2++) {
    data_quji->data[i2] = b_signal->data[i2] - background->data[i2];
  }

  n = b_signal->size[0] - 1;
  idx = 0;
  for (i = 0; i <= n; i++) {
    if (b_signal->data[i] - background->data[i] < 0.0) {
      idx++;
    }
  }

  emxInit_int32_T(&r2, 1);
  i2 = r2->size[0];
  r2->size[0] = idx;
  emxEnsureCapacity_int32_T(r2, i2);
  idx = 0;
  for (i = 0; i <= n; i++) {
    if (b_signal->data[i] - background->data[i] < 0.0) {
      r2->data[idx] = i + 1;
      idx++;
    }
  }

  emxFree_real_T(&background);
  idx = r2->size[0];
  for (i2 = 0; i2 < idx; i2++) {
    data_quji->data[r2->data[i2] - 1] = 0.0;
  }

  emxFree_int32_T(&r2);

  //  figure(3)
  //  plot(signal);
  //  hold on
  //  plot(signal-background);
  //  plot(background,'linewidth',1.5)
  //  hold off
  //  legend('原始光谱','去背景后','背景基线');
  //  jixian=background;
}

//
// File trailer for autoestibl.cpp
//
// [EOF]
//
