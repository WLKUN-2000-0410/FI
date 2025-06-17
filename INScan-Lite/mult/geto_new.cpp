//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: geto_new.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
#include "rt_nonfinite.h"
#include <string.h>
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
#include "filter_.h"
#include "qr.h"
#include "power.h"
#include "mldivide.h"
#include "sum.h"
#include "diff.h"
#include "all.h"
#include "any1.h"
#include "sort1.h"
#include "mod.h"
#include "analyseMixture_rtwutil.h"

// Function Declarations
static boolean_T uniformx(const emxArray_real_T *diffx, const emxArray_real_T *x,
  const emxArray_real_T *y);

// Function Definitions

//
// ISUNIFORM True if x is of the form a:b:c
// Arguments    : const emxArray_real_T *diffx
//                const emxArray_real_T *x
//                const emxArray_real_T *y
// Return Type  : boolean_T
//
static boolean_T uniformx(const emxArray_real_T *diffx, const emxArray_real_T *x,
  const emxArray_real_T *y)
{
  boolean_T isuniform;
  emxArray_boolean_T *r7;
  int i33;
  int loop_ub;
  emxArray_real_T *b_y;
  emxArray_real_T *b_x;
  emxArray_boolean_T *c_y;
  int nx;
  double x_idx_0;
  double x_idx_1;
  double y_idx_0;
  emxInit_boolean_T(&r7, 1);
  i33 = r7->size[0];
  r7->size[0] = y->size[0];
  emxEnsureCapacity_boolean_T(r7, i33);
  loop_ub = y->size[0];
  for (i33 = 0; i33 < loop_ub; i33++) {
    r7->data[i33] = rtIsNaN(y->data[i33]);
  }

  emxInit_real_T(&b_y, 2);
  emxInit_real_T(&b_x, 2);
  emxInit_boolean_T(&c_y, 2);
  if (b_any(r7)) {
    isuniform = false;
  } else {
    i33 = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 1;
    c_y->size[1] = x->size[1];
    emxEnsureCapacity_boolean_T(c_y, i33);
    loop_ub = x->size[0] * x->size[1];
    for (i33 = 0; i33 < loop_ub; i33++) {
      c_y->data[i33] = rtIsNaN(x->data[i33]);
    }

    if (any(c_y)) {
      isuniform = false;
    } else {
      b_diff(diffx, b_x);
      nx = b_x->size[1];
      i33 = b_y->size[0] * b_y->size[1];
      b_y->size[0] = 1;
      b_y->size[1] = b_x->size[1];
      emxEnsureCapacity_real_T(b_y, i33);
      for (loop_ub = 0; loop_ub < nx; loop_ub++) {
        b_y->data[loop_ub] = std::abs(b_x->data[loop_ub]);
      }

      x_idx_0 = x->data[0];
      x_idx_1 = x->data[x->size[1] - 1];
      y_idx_0 = std::abs(x_idx_0);
      x_idx_0 = std::abs(x_idx_1);
      if ((y_idx_0 < x_idx_0) || (rtIsNaN(y_idx_0) && (!rtIsNaN(x_idx_0)))) {
        y_idx_0 = x_idx_0;
      }

      x_idx_0 = 2.2204460492503131E-16 * y_idx_0;
      i33 = c_y->size[0] * c_y->size[1];
      c_y->size[0] = 1;
      c_y->size[1] = b_y->size[1];
      emxEnsureCapacity_boolean_T(c_y, i33);
      loop_ub = b_y->size[0] * b_y->size[1];
      for (i33 = 0; i33 < loop_ub; i33++) {
        c_y->data[i33] = (b_y->data[i33] <= x_idx_0);
      }

      isuniform = b_all(c_y);
    }
  }

  emxFree_boolean_T(&c_y);
  emxFree_boolean_T(&r7);
  emxFree_real_T(&b_x);
  emxFree_real_T(&b_y);
  return isuniform;
}

//
// lcz：新的小波去噪
//  nk=smooth(ns,7,'sgolay',3);
//  nk=fastsmooth(ns,7,3);
// Arguments    : const double ns[1800]
//                double nk[1800]
// Return Type  : void
//
void b_geto_new(const double ns[1800], double nk[1800])
{
  int i;
  boolean_T nomissing;
  boolean_T notnan[1800];
  int R;
  boolean_T exitg1;
  emxArray_real_T *y;
  emxArray_real_T *r15;
  emxArray_real_T *v;
  boolean_T guard1 = false;
  boolean_T b[1800];
  int i53;
  double b_v[28];
  int L;
  double unusedExpr[16];
  double ncols;
  double ymid[1800];
  double z1[7];
  int HF;
  double c_v[3];
  double d_v[3];
  int i54;
  double ybegin_tmp[28];
  double d5;
  double d6;
  int trueCount;
  short tmp_data[1805];
  double tidx_data[1805];
  int q_size[2];
  double q_data[1805];
  emxArray_real_T b_q_data;
  int y_size[2];
  emxArray_boolean_T y_data;
  boolean_T b_y_data[1804];
  int v_size[2];
  static double v_data[7220];
  emxArray_real_T c_q_data;
  signed char sizes_idx_1;
  short input_sizes_idx_0;
  emxArray_real_T b_v_data;
  double d_data[1806];

  //  savitziki-golay smooth
  //  (x,y) are given data. f is the frame length to be taken, should
  //  be an odd number. k is the degree of polynomial filter. It should
  //  be less than f.
  //  function c=fastsmooth(Y,w,order)
  //  % 'sgolay'方法去噪，Y为输入的信号值，w为匀滑宽度，order为使用的阶次
  //  %
  //   y = Y(:);
  //   x = (1:length(y))';
  //   span=w;
  //   t = length(y);
  //  %  idx = 1:t;
  //  %  sortx = any(diff(isnan(x))<0);   % if NaNs not all at end
  //   c = NaN(size(y),'like',y);
  //   ok = ~isnan(x);
  //   c(ok) = sgolay(x(ok),y(ok),span,order);
  //  end
  //  Reference: Orfanidis, S.J., Introduction to Signal Processing,
  //  Prentice-Hall, Englewood Cliffs, NJ, 1996.
  //  will subtract 1 if frame is even.
  for (i = 0; i < 1800; i++) {
    notnan[i] = !rtIsNaN(ns[i]);
  }

  nomissing = true;
  R = 0;
  exitg1 = false;
  while ((!exitg1) && (R < 1800)) {
    if (!notnan[R]) {
      nomissing = false;
      exitg1 = true;
    } else {
      R++;
    }
  }

  //  half frame length
  //  note that x is sorted so max(abs(x)) must be abs(x(1)) or abs(x(end));
  //  already calculated diffx for monotonic case, so use it again. Only
  //  recalculate if we sort x.
  emxInit_real_T(&y, 2);
  emxInit_real_T(&r15, 1);
  emxInit_real_T(&v, 2);
  guard1 = false;
  if (nomissing) {
    // ISUNIFORM True if x is of the form a:b:c
    for (i = 0; i < 1800; i++) {
      b[i] = rtIsNaN(ns[i]);
    }

    nomissing = false;
    R = 0;
    exitg1 = false;
    while ((!exitg1) && (R < 1800)) {
      if (b[R]) {
        nomissing = true;
        exitg1 = true;
      } else {
        R++;
      }
    }

    if (!nomissing) {
      for (i53 = 0; i53 < 28; i53++) {
        b_v[i53] = 1.0;
      }

      for (i = 0; i < 3; i++) {
        for (R = 0; R < 7; R++) {
          ncols = rt_powd_snf(-3.0 + ((static_cast<double>(R) + 1.0) - 1.0), 1.0
                              + static_cast<double>(i));
          z1[R] = ncols;
          b_v[R + 7 * (i + 1)] = ncols;
        }
      }

      b_qr(b_v, unusedExpr);
      for (i53 = 0; i53 < 7; i53++) {
        z1[i53] = ((b_v[i53] * b_v[3] + b_v[i53 + 7] * b_v[10]) + b_v[i53 + 14] *
                   b_v[17]) + b_v[i53 + 21] * b_v[24];
      }

      memset(&ymid[0], 0, 1800U * sizeof(double));
      for (R = 0; R < 7; R++) {
        HF = R + 1;
        for (L = HF; L < 1801; L++) {
          ymid[L - 1] += z1[R] * ns[(L - R) - 1];
        }

        HF = R << 2;
        ybegin_tmp[HF] = b_v[R];
        ybegin_tmp[1 + HF] = b_v[R + 7];
        ybegin_tmp[2 + HF] = b_v[R + 14];
        ybegin_tmp[3 + HF] = b_v[R + 21];
      }

      for (i53 = 0; i53 < 3; i53++) {
        c_v[i53] = 0.0;
        d_v[i53] = 0.0;
        for (i54 = 0; i54 < 7; i54++) {
          HF = i54 << 2;
          ncols = ybegin_tmp[1 + HF];
          d5 = b_v[i53] * ybegin_tmp[HF] + b_v[i53 + 7] * ncols;
          d6 = b_v[i53 + 4] * ybegin_tmp[HF] + b_v[i53 + 11] * ncols;
          ncols = ybegin_tmp[2 + HF];
          d5 += b_v[i53 + 14] * ncols;
          d6 += b_v[i53 + 18] * ncols;
          ncols = ybegin_tmp[3 + HF];
          d5 += b_v[i53 + 21] * ncols;
          d6 += b_v[i53 + 25] * ncols;
          c_v[i53] += d5 * ns[i54];
          d_v[i53] += d6 * ns[1793 + i54];
        }

        nk[i53] = c_v[i53];
      }

      memcpy(&nk[3], &ymid[6], 1794U * sizeof(double));
      nk[1797] = d_v[0];
      nk[1798] = d_v[1];
      nk[1799] = d_v[2];
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    //  non-uniformly distributed data
    memcpy(&nk[0], &ns[0], 1800U * sizeof(double));

    //  Turn off warnings when called from command line (already off if called from 
    //  cftool).
    //  ws = warning('off', 'all');
    //  [lastwarnmsg,lastwarnid]=lastwarn;
    for (i = 0; i < 1800; i++) {
      if ((1 + i > 1) && (1 + i == i)) {
        nk[i] = nk[i - 1];
      } else {
        L = 1 + i;
        R = 1 + i;

        //  find leftmost and rightmost values
        while ((R < 1800) && (R == i)) {
          R++;
        }

        while ((L > 1) && (L - 1 == 1 + i)) {
          L--;
        }

        ncols = (7.0 - (static_cast<double>((R - L)) + 1.0)) / 2.0;
        if (0.0 > ncols) {
          ncols = 0.0;
        }

        HF = static_cast<int>(std::ceil(ncols));

        //  need this many more on each side
        L -= HF;
        if (1 > L) {
          L = 1;
        }

        if (1794 < L) {
          L = 1794;
        }

        //  find leftmost point needed
        while ((L > 1) && (L == L - 1)) {
          L--;
        }

        HF += R;
        R = L + 6;
        if (HF > R) {
          R = HF;
        }

        if (1800 < R) {
          R = 1800;
        }

        //  find rightmost point needed
        while ((R < 1800) && (R == 1 + R)) {
          R++;
        }

        if (R < L) {
          y->size[0] = 1;
          y->size[1] = 0;
        } else {
          i53 = y->size[0] * y->size[1];
          y->size[0] = 1;
          R -= L;
          y->size[1] = R + 1;
          emxEnsureCapacity_real_T(y, i53);
          for (i53 = 0; i53 <= R; i53++) {
            y->data[i53] = L + i53;
          }
        }

        HF = y->size[1];
        HF--;
        trueCount = 0;
        for (R = 0; R <= HF; R++) {
          if (notnan[static_cast<int>(y->data[R]) - 1]) {
            trueCount++;
          }
        }

        L = 0;
        for (R = 0; R <= HF; R++) {
          if (notnan[static_cast<int>(y->data[R]) - 1]) {
            tmp_data[L] = static_cast<short>((R + 1));
            L++;
          }
        }

        for (i53 = 0; i53 < trueCount; i53++) {
          tidx_data[i53] = y->data[tmp_data[i53] - 1];
        }

        if (trueCount == 0) {
          nk[i] = rtNaN;
        } else {
          q_size[0] = 1;
          q_size[1] = trueCount;
          for (i53 = 0; i53 < trueCount; i53++) {
            q_data[i53] = (static_cast<short>((1 + static_cast<short>
              ((static_cast<int>(tidx_data[i53]) - 1)))) - i) - 1;
          }

          //  center to improve conditioning
          b_q_data.data = &q_data[0];
          b_q_data.size = &q_size[0];
          b_q_data.allocatedSize = 1805;
          b_q_data.numDimensions = 2;
          b_q_data.canFreeData = false;
          b_diff(&b_q_data, y);
          y_size[0] = 1;
          y_size[1] = y->size[1];
          R = y->size[0] * y->size[1];
          for (i53 = 0; i53 < R; i53++) {
            b_y_data[i53] = (y->data[i53] > 0.0);
          }

          y_data.data = &b_y_data[0];
          y_data.size = &y_size[0];
          y_data.allocatedSize = 1804;
          y_data.numDimensions = 2;
          y_data.canFreeData = false;
          ncols = 1.0 + c_sum(&y_data);
          if ((4.0 < ncols) || rtIsNaN(ncols)) {
            ncols = 4.0;
          }

          v_size[0] = q_size[1];
          v_size[1] = static_cast<int>(ncols);
          R = q_size[1] * static_cast<int>(ncols);
          for (i53 = 0; i53 < R; i53++) {
            v_data[i53] = 1.0;
          }

          i53 = static_cast<int>((ncols - 1.0));
          for (L = 0; L < i53; L++) {
            R = v_size[0];
            c_q_data.data = &q_data[0];
            c_q_data.size = &q_size[0];
            c_q_data.allocatedSize = 1805;
            c_q_data.numDimensions = 2;
            c_q_data.canFreeData = false;
            c_power(&c_q_data, 1.0 + static_cast<double>(L), y);
            for (i54 = 0; i54 < R; i54++) {
              v_data[i54 + v_size[0] * (L + 1)] = y->data[i54];
            }
          }

          if (v_size[0] == v_size[1]) {
            //  Square v may give infs in the \ solution, so force least squares 
            sizes_idx_1 = static_cast<signed char>(v_size[1]);
            if ((v_size[0] != 0) && (v_size[1] != 0)) {
              input_sizes_idx_0 = static_cast<short>(v_size[0]);
            } else {
              input_sizes_idx_0 = 0;
            }

            i53 = r15->size[0];
            r15->size[0] = trueCount + 1;
            emxEnsureCapacity_real_T(r15, i53);
            for (i53 = 0; i53 < trueCount; i53++) {
              r15->data[i53] = ns[static_cast<int>(tidx_data[i53]) - 1];
            }

            r15->data[trueCount] = 0.0;
            HF = input_sizes_idx_0;
            i53 = v->size[0] * v->size[1];
            v->size[0] = input_sizes_idx_0 + 1;
            v->size[1] = sizes_idx_1;
            emxEnsureCapacity_real_T(v, i53);
            R = sizes_idx_1;
            for (i53 = 0; i53 < R; i53++) {
              for (i54 = 0; i54 < HF; i54++) {
                v->data[i54 + v->size[0] * i53] = v_data[i54 + input_sizes_idx_0
                  * i53];
              }
            }

            R = sizes_idx_1;
            for (i53 = 0; i53 < R; i53++) {
              for (i54 = 0; i54 < 1; i54++) {
                v->data[input_sizes_idx_0 + v->size[0] * i53] = 0.0;
              }
            }

            b_mldivide(v, r15);
            R = r15->size[0];
            for (i53 = 0; i53 < R; i53++) {
              d_data[i53] = r15->data[i53];
            }
          } else {
            i53 = r15->size[0];
            r15->size[0] = trueCount;
            emxEnsureCapacity_real_T(r15, i53);
            for (i53 = 0; i53 < trueCount; i53++) {
              r15->data[i53] = ns[static_cast<int>(tidx_data[i53]) - 1];
            }

            b_v_data.data = &v_data[0];
            b_v_data.size = &v_size[0];
            b_v_data.allocatedSize = 7220;
            b_v_data.numDimensions = 2;
            b_v_data.canFreeData = false;
            b_mldivide(&b_v_data, r15);
            R = r15->size[0];
            for (i53 = 0; i53 < R; i53++) {
              d_data[i53] = r15->data[i53];
            }
          }

          nk[i] = d_data[0];
        }
      }
    }
  }

  emxFree_real_T(&v);
  emxFree_real_T(&r15);
  emxFree_real_T(&y);

  //  nk=wden(ns,'heursure','s','sln',5,'sym8');
}

//
// lcz：新的小波去噪
//  nk=smooth(ns,7,'sgolay',3);
//  nk=fastsmooth(ns,7,3);
// Arguments    : const emxArray_real_T *ns
//                emxArray_real_T *nk
// Return Type  : void
//
void geto_new(const emxArray_real_T *ns, emxArray_real_T *nk)
{
  emxArray_real_T *x;
  int i27;
  int i28;
  int nm1d2;
  emxArray_real_T *y;
  int varargin_2;
  int n;
  double f;
  emxArray_real_T *diffx;
  emxArray_boolean_T *notnan;
  boolean_T nomissing;
  emxArray_real_T *idx;
  emxArray_real_T *v;
  emxArray_real_T *tidx;
  emxArray_real_T *d;
  emxArray_int32_T *r6;
  emxArray_int32_T *iidx;
  emxArray_boolean_T *b_diffx;
  emxArray_boolean_T *b_notnan;
  emxArray_real_T *b_tidx;
  emxArray_int32_T *c_tidx;
  emxArray_real_T *d_tidx;
  emxArray_real_T *b_v;
  boolean_T guard1 = false;
  double hf;
  int v_size_tmp;
  int v_size[2];
  double v_data[36];
  double ndbl;
  int i;
  double apnd;
  double cdiff;
  double u0;
  double L;
  double u1;
  double R;
  int exitg1;
  int input_sizes_idx_0;
  double d3;
  int t_size[1];
  int k;
  double t_data[9];
  double q_data[36];
  int q_size[2];
  double unusedU0_data[36];
  int unusedU0_size[2];
  double yend_data[9];
  int yend_size[1];
  int aoffset;
  int y_size_idx_0_tmp;
  int b_tmp_size_idx_0;
  int b_tmp_size_idx_1;
  double a_data[36];
  int y_size_idx_1;
  int boffset;
  double b_tmp_data[36];
  int j;
  boolean_T guard2 = false;
  int y_data_tmp;
  double y_data[81];
  int i29;
  int i30;
  int i31;
  int i32;
  signed char sizes_idx_1;
  emxInit_real_T(&x, 2);
  if (ns->size[0] < 1) {
    x->size[0] = 1;
    x->size[1] = 0;
  } else {
    i27 = ns->size[0];
    i28 = x->size[0] * x->size[1];
    x->size[0] = 1;
    nm1d2 = i27 - 1;
    x->size[1] = nm1d2 + 1;
    emxEnsureCapacity_real_T(x, i28);
    for (i27 = 0; i27 <= nm1d2; i27++) {
      x->data[i27] = 1.0 + static_cast<double>(i27);
    }
  }

  emxInit_real_T(&y, 1);
  i27 = y->size[0];
  y->size[0] = ns->size[0];
  emxEnsureCapacity_real_T(y, i27);
  nm1d2 = ns->size[0];
  for (i27 = 0; i27 < nm1d2; i27++) {
    y->data[i27] = ns->data[i27];
  }

  //  savitziki-golay smooth
  //  (x,y) are given data. f is the frame length to be taken, should
  //  be an odd number. k is the degree of polynomial filter. It should
  //  be less than f.
  //  function c=fastsmooth(Y,w,order)
  //  % 'sgolay'方法去噪，Y为输入的信号值，w为匀滑宽度，order为使用的阶次
  //  %
  //   y = Y(:);
  //   x = (1:length(y))';
  //   span=w;
  //   t = length(y);
  //  %  idx = 1:t;
  //  %  sortx = any(diff(isnan(x))<0);   % if NaNs not all at end
  //   c = NaN(size(y),'like',y);
  //   ok = ~isnan(x);
  //   c(ok) = sgolay(x(ok),y(ok),span,order);
  //  end
  //  Reference: Orfanidis, S.J., Introduction to Signal Processing,
  //  Prentice-Hall, Englewood Cliffs, NJ, 1996.
  varargin_2 = x->size[1];
  n = x->size[1];
  if (7 < x->size[1]) {
    f = 7.0;
  } else {
    f = x->size[1];
  }

  emxInit_real_T(&diffx, 2);
  emxInit_boolean_T(&notnan, 1);
  f -= b_mod(f - 1.0);

  //  will subtract 1 if frame is even.
  b_diff(x, diffx);
  i27 = notnan->size[0];
  notnan->size[0] = ns->size[0];
  emxEnsureCapacity_boolean_T(notnan, i27);
  nm1d2 = ns->size[0];
  for (i27 = 0; i27 < nm1d2; i27++) {
    notnan->data[i27] = rtIsNaN(ns->data[i27]);
  }

  i27 = notnan->size[0];
  emxEnsureCapacity_boolean_T(notnan, i27);
  nm1d2 = notnan->size[0];
  for (i27 = 0; i27 < nm1d2; i27++) {
    notnan->data[i27] = !notnan->data[i27];
  }

  nomissing = all(notnan);
  emxInit_real_T(&idx, 2);
  emxInit_real_T(&v, 2);
  emxInit_real_T(&tidx, 2);
  emxInit_real_T(&d, 1);
  emxInit_int32_T(&r6, 1);
  emxInit_int32_T(&iidx, 2);
  emxInit_boolean_T(&b_diffx, 2);
  emxInit_boolean_T(&b_notnan, 1);
  emxInit_real_T(&b_tidx, 1);
  emxInit_int32_T(&c_tidx, 1);
  emxInit_real_T(&d_tidx, 2);
  emxInit_real_T(&b_v, 2);
  guard1 = false;
  if (f <= 3.0) {
    i27 = b_diffx->size[0] * b_diffx->size[1];
    b_diffx->size[0] = 1;
    b_diffx->size[1] = diffx->size[1];
    emxEnsureCapacity_boolean_T(b_diffx, i27);
    nm1d2 = diffx->size[0] * diffx->size[1];
    for (i27 = 0; i27 < nm1d2; i27++) {
      b_diffx->data[i27] = (diffx->data[i27] > 0.0);
    }

    if (b_all(b_diffx) && nomissing) {
      i27 = nk->size[0];
      nk->size[0] = ns->size[0];
      emxEnsureCapacity_real_T(nk, i27);
      nm1d2 = ns->size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        nk->data[i27] = ns->data[i27];
      }
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    hf = (f - 1.0) / 2.0;

    //  half frame length
    if (x->size[1] < 1) {
      idx->size[0] = 1;
      idx->size[1] = 0;
    } else {
      i27 = x->size[1];
      i28 = idx->size[0] * idx->size[1];
      idx->size[0] = 1;
      nm1d2 = i27 - 1;
      idx->size[1] = nm1d2 + 1;
      emxEnsureCapacity_real_T(idx, i28);
      for (i27 = 0; i27 <= nm1d2; i27++) {
        idx->data[i27] = 1.0 + static_cast<double>(i27);
      }
    }

    i27 = b_diffx->size[0] * b_diffx->size[1];
    b_diffx->size[0] = 1;
    b_diffx->size[1] = diffx->size[1];
    emxEnsureCapacity_boolean_T(b_diffx, i27);
    nm1d2 = diffx->size[0] * diffx->size[1];
    for (i27 = 0; i27 < nm1d2; i27++) {
      b_diffx->data[i27] = (diffx->data[i27] < 0.0);
    }

    if (any(b_diffx)) {
      //  make sure x is monotonically increasing
      sort(x, iidx);
      i27 = idx->size[0] * idx->size[1];
      idx->size[0] = 1;
      idx->size[1] = iidx->size[1];
      emxEnsureCapacity_real_T(idx, i27);
      nm1d2 = iidx->size[0] * iidx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        idx->data[i27] = iidx->data[i27];
      }

      i27 = y->size[0];
      y->size[0] = idx->size[1];
      emxEnsureCapacity_real_T(y, i27);
      nm1d2 = idx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        y->data[i27] = ns->data[static_cast<int>(idx->data[i27]) - 1];
      }

      i27 = b_notnan->size[0];
      b_notnan->size[0] = idx->size[1];
      emxEnsureCapacity_boolean_T(b_notnan, i27);
      nm1d2 = idx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        b_notnan->data[i27] = notnan->data[static_cast<int>(idx->data[i27]) - 1];
      }

      i27 = notnan->size[0];
      notnan->size[0] = b_notnan->size[0];
      emxEnsureCapacity_boolean_T(notnan, i27);
      nm1d2 = b_notnan->size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        notnan->data[i27] = b_notnan->data[i27];
      }

      b_diff(x, diffx);
    }

    //  note that x is sorted so max(abs(x)) must be abs(x(1)) or abs(x(end));
    //  already calculated diffx for monotonic case, so use it again. Only
    //  recalculate if we sort x.
    if (nomissing && uniformx(diffx, x, y)) {
      v_size_tmp = static_cast<int>(f);
      v_size[0] = v_size_tmp;
      v_size[1] = 4;
      nm1d2 = v_size_tmp << 2;
      for (i27 = 0; i27 < nm1d2; i27++) {
        v_data[i27] = 1.0;
      }

      if (rtIsNaN(-hf) || rtIsNaN(hf)) {
        i27 = diffx->size[0] * diffx->size[1];
        diffx->size[0] = 1;
        diffx->size[1] = 1;
        emxEnsureCapacity_real_T(diffx, i27);
        diffx->data[0] = rtNaN;
      } else if (hf < -hf) {
        diffx->size[0] = 1;
        diffx->size[1] = 0;
      } else if ((rtIsInf(-hf) || rtIsInf(hf)) && (-hf == hf)) {
        i27 = diffx->size[0] * diffx->size[1];
        diffx->size[0] = 1;
        diffx->size[1] = 1;
        emxEnsureCapacity_real_T(diffx, i27);
        diffx->data[0] = rtNaN;
      } else if (std::floor(-hf) == -hf) {
        i27 = diffx->size[0] * diffx->size[1];
        diffx->size[0] = 1;
        nm1d2 = static_cast<int>(std::floor(hf - (-hf)));
        diffx->size[1] = nm1d2 + 1;
        emxEnsureCapacity_real_T(diffx, i27);
        for (i27 = 0; i27 <= nm1d2; i27++) {
          diffx->data[i27] = -hf + static_cast<double>(i27);
        }
      } else {
        ndbl = std::floor((hf - (-hf)) + 0.5);
        apnd = -hf + ndbl;
        cdiff = apnd - hf;
        u0 = std::abs(-hf);
        u1 = std::abs(hf);
        if ((u0 > u1) || rtIsNaN(u1)) {
          u1 = u0;
        }

        if (std::abs(cdiff) < 4.4408920985006262E-16 * u1) {
          ndbl++;
          apnd = hf;
        } else if (cdiff > 0.0) {
          apnd = -hf + (ndbl - 1.0);
        } else {
          ndbl++;
        }

        if (ndbl >= 0.0) {
          input_sizes_idx_0 = static_cast<int>(ndbl);
        } else {
          input_sizes_idx_0 = 0;
        }

        i27 = diffx->size[0] * diffx->size[1];
        diffx->size[0] = 1;
        diffx->size[1] = input_sizes_idx_0;
        emxEnsureCapacity_real_T(diffx, i27);
        if (input_sizes_idx_0 > 0) {
          diffx->data[0] = -hf;
          if (input_sizes_idx_0 > 1) {
            diffx->data[input_sizes_idx_0 - 1] = apnd;
            nm1d2 = (input_sizes_idx_0 - 1) / 2;
            for (k = 0; k <= nm1d2 - 2; k++) {
              diffx->data[1 + k] = -hf + (1.0 + static_cast<double>(k));
              diffx->data[(input_sizes_idx_0 - k) - 2] = apnd - (1.0 +
                static_cast<double>(k));
            }

            if (nm1d2 << 1 == input_sizes_idx_0 - 1) {
              diffx->data[nm1d2] = (-hf + apnd) / 2.0;
            } else {
              diffx->data[nm1d2] = -hf + static_cast<double>(nm1d2);
              diffx->data[nm1d2 + 1] = apnd - static_cast<double>(nm1d2);
            }
          }
        }
      }

      if (rtIsNaN(-hf) || rtIsNaN(hf)) {
        i27 = tidx->size[0] * tidx->size[1];
        tidx->size[0] = 1;
        tidx->size[1] = 1;
        emxEnsureCapacity_real_T(tidx, i27);
        tidx->data[0] = rtNaN;
      } else if (hf < -hf) {
        tidx->size[0] = 1;
        tidx->size[1] = 0;
      } else if ((rtIsInf(-hf) || rtIsInf(hf)) && (-hf == hf)) {
        i27 = tidx->size[0] * tidx->size[1];
        tidx->size[0] = 1;
        tidx->size[1] = 1;
        emxEnsureCapacity_real_T(tidx, i27);
        tidx->data[0] = rtNaN;
      } else if (std::floor(-hf) == -hf) {
        i27 = tidx->size[0] * tidx->size[1];
        tidx->size[0] = 1;
        tidx->size[1] = static_cast<int>(std::floor(hf - (-hf))) + 1;
        emxEnsureCapacity_real_T(tidx, i27);
        nm1d2 = static_cast<int>(std::floor(hf - (-hf)));
        for (i27 = 0; i27 <= nm1d2; i27++) {
          tidx->data[i27] = -hf + static_cast<double>(i27);
        }
      } else {
        ndbl = std::floor((hf - (-hf)) + 0.5);
        apnd = -hf + ndbl;
        cdiff = apnd - hf;
        u0 = std::abs(-hf);
        u1 = std::abs(hf);
        if ((u0 > u1) || rtIsNaN(u1)) {
          u1 = u0;
        }

        if (std::abs(cdiff) < 4.4408920985006262E-16 * u1) {
          ndbl++;
          apnd = hf;
        } else if (cdiff > 0.0) {
          apnd = -hf + (ndbl - 1.0);
        } else {
          ndbl++;
        }

        if (ndbl >= 0.0) {
          input_sizes_idx_0 = static_cast<int>(ndbl);
        } else {
          input_sizes_idx_0 = 0;
        }

        i27 = tidx->size[0] * tidx->size[1];
        tidx->size[0] = 1;
        tidx->size[1] = input_sizes_idx_0;
        emxEnsureCapacity_real_T(tidx, i27);
        if (input_sizes_idx_0 > 0) {
          tidx->data[0] = -hf;
          if (input_sizes_idx_0 > 1) {
            tidx->data[input_sizes_idx_0 - 1] = apnd;
            nm1d2 = (input_sizes_idx_0 - 1) / 2;
            for (k = 0; k <= nm1d2 - 2; k++) {
              tidx->data[1 + k] = -hf + (1.0 + static_cast<double>(k));
              tidx->data[(input_sizes_idx_0 - k) - 2] = apnd - (1.0 +
                static_cast<double>(k));
            }

            if (nm1d2 << 1 == input_sizes_idx_0 - 1) {
              tidx->data[nm1d2] = (-hf + apnd) / 2.0;
            } else {
              tidx->data[nm1d2] = -hf + static_cast<double>(nm1d2);
              tidx->data[nm1d2 + 1] = apnd - static_cast<double>(nm1d2);
            }
          }
        }
      }

      i27 = d->size[0];
      d->size[0] = diffx->size[1];
      emxEnsureCapacity_real_T(d, i27);
      nm1d2 = diffx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        d->data[i27] = diffx->data[i27];
      }

      i27 = b_tidx->size[0];
      b_tidx->size[0] = tidx->size[1];
      emxEnsureCapacity_real_T(b_tidx, i27);
      nm1d2 = tidx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        b_tidx->data[i27] = tidx->data[i27];
      }

      t_size[0] = d->size[0];
      nm1d2 = d->size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        t_data[i27] = b_tidx->data[i27];
      }

      for (i = 0; i < 3; i++) {
        b_power(t_data, t_size, 1.0 + static_cast<double>(i), yend_data,
                yend_size);
        nm1d2 = yend_size[0];
        for (i27 = 0; i27 < nm1d2; i27++) {
          v_data[i27 + v_size_tmp * (i + 1)] = yend_data[i27];
        }
      }

      qr(v_data, v_size, q_data, q_size, unusedU0_data, unusedU0_size);
      nm1d2 = q_size[1];
      i27 = d->size[0];
      d->size[0] = q_size[1];
      emxEnsureCapacity_real_T(d, i27);
      for (i27 = 0; i27 < nm1d2; i27++) {
        d->data[i27] = q_data[(static_cast<int>((hf + 1.0)) + q_size[0] * i27) -
          1];
      }

      if ((q_size[1] == 1) || (d->size[0] == 1)) {
        t_size[0] = q_size[0];
        nm1d2 = q_size[0];
        for (i27 = 0; i27 < nm1d2; i27++) {
          t_data[i27] = 0.0;
          input_sizes_idx_0 = q_size[1];
          for (i28 = 0; i28 < input_sizes_idx_0; i28++) {
            t_data[i27] += q_data[i27 + q_size[0] * i28] * d->data[i28];
          }
        }
      } else {
        input_sizes_idx_0 = q_size[0];
        nm1d2 = q_size[1];
        t_size[0] = q_size[0];
        if (0 <= q_size[0] - 1) {
          memset(&t_data[0], 0, (unsigned int)(q_size[0] * static_cast<int>
                  (sizeof(double))));
        }

        for (k = 0; k < nm1d2; k++) {
          aoffset = k * input_sizes_idx_0;
          for (i = 0; i < input_sizes_idx_0; i++) {
            t_data[i] += d->data[k] * q_data[aoffset + i];
          }
        }
      }

      filter(t_data, t_size, y, d);
      if (1.0 > hf) {
        y_size_idx_0_tmp = 0;
      } else {
        y_size_idx_0_tmp = static_cast<int>(hf);
      }

      nm1d2 = q_size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        for (i28 = 0; i28 < y_size_idx_0_tmp; i28++) {
          a_data[i28 + y_size_idx_0_tmp * i27] = q_data[i28 + q_size[0] * i27];
        }
      }

      b_tmp_size_idx_0 = q_size[1];
      b_tmp_size_idx_1 = q_size[0];
      nm1d2 = q_size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        input_sizes_idx_0 = q_size[1];
        for (i28 = 0; i28 < input_sizes_idx_0; i28++) {
          b_tmp_data[i28 + b_tmp_size_idx_0 * i27] = q_data[i27 + q_size[0] *
            i28];
        }
      }

      if (q_size[1] == 1) {
        y_size_idx_1 = q_size[0];
        for (i27 = 0; i27 < y_size_idx_0_tmp; i27++) {
          for (i28 = 0; i28 < b_tmp_size_idx_1; i28++) {
            y_data_tmp = i27 + y_size_idx_0_tmp * i28;
            y_data[y_data_tmp] = 0.0;
            for (i30 = 0; i30 < 1; i30++) {
              y_data[y_data_tmp] += q_data[i27] * b_tmp_data[b_tmp_size_idx_0 *
                i28];
            }
          }
        }
      } else {
        i27 = y_size_idx_0_tmp - 1;
        i28 = q_size[1];
        input_sizes_idx_0 = q_size[0];
        y_size_idx_1 = q_size[0];
        for (j = 0; j < input_sizes_idx_0; j++) {
          nm1d2 = j * y_size_idx_0_tmp;
          boffset = j * i28;
          if (0 <= i27) {
            memset(&y_data[nm1d2], 0, (unsigned int)((i27 + 1) * static_cast<int>
                    (sizeof(double))));
          }

          for (k = 0; k < i28; k++) {
            aoffset = k * y_size_idx_0_tmp;
            L = b_tmp_data[boffset + k];
            for (i = 0; i <= i27; i++) {
              y_data_tmp = nm1d2 + i;
              y_data[y_data_tmp] += L * a_data[aoffset + i];
            }
          }
        }
      }

      guard2 = false;
      if (y_size_idx_1 == 1) {
        guard2 = true;
      } else {
        if (1.0 > f) {
          i29 = 0;
        } else {
          i29 = static_cast<int>(f);
        }

        if (i29 == 1) {
          guard2 = true;
        } else {
          t_size[0] = y_size_idx_0_tmp;
          if (0 <= y_size_idx_0_tmp - 1) {
            memset(&t_data[0], 0, (unsigned int)(y_size_idx_0_tmp * static_cast<
                    int>(sizeof(double))));
          }

          for (k = 0; k < y_size_idx_1; k++) {
            aoffset = k * y_size_idx_0_tmp;
            for (i = 0; i < y_size_idx_0_tmp; i++) {
              t_data[i] += y->data[k] * y_data[aoffset + i];
            }
          }
        }
      }

      if (guard2) {
        t_size[0] = y_size_idx_0_tmp;
        for (i27 = 0; i27 < y_size_idx_0_tmp; i27++) {
          t_data[i27] = 0.0;
          for (i28 = 0; i28 < y_size_idx_1; i28++) {
            t_data[i27] += y_data[i27 + y_size_idx_0_tmp * i28] * y->data[i28];
          }
        }
      }

      if (hf + 2.0 > q_size[0]) {
        i27 = 0;
        i28 = -1;
      } else {
        i27 = static_cast<int>((hf + 2.0)) - 1;
        i28 = q_size[0] - 1;
      }

      L = (static_cast<double>(varargin_2) - f) + 1.0;
      if (L > n) {
        i30 = 0;
        varargin_2 = 0;
      } else {
        i30 = static_cast<int>(L) - 1;
      }

      if (q_size[1] == 1) {
        y_size_idx_0_tmp = (i28 - i27) + 1;
        y_size_idx_1 = q_size[0];
        nm1d2 = i28 - i27;
        for (i28 = 0; i28 <= nm1d2; i28++) {
          for (i31 = 0; i31 < b_tmp_size_idx_1; i31++) {
            y_data_tmp = i28 + y_size_idx_0_tmp * i31;
            y_data[y_data_tmp] = 0.0;
            for (i32 = 0; i32 < 1; i32++) {
              y_data[y_data_tmp] += q_data[i27 + i28] *
                b_tmp_data[b_tmp_size_idx_0 * i31];
            }
          }
        }
      } else {
        input_sizes_idx_0 = i28 - i27;
        i31 = q_size[1];
        n = q_size[0];
        y_size_idx_0_tmp = (i28 - i27) + 1;
        y_size_idx_1 = q_size[0];
        for (j = 0; j < n; j++) {
          nm1d2 = j * (input_sizes_idx_0 + 1);
          boffset = j * i31;
          if (0 <= input_sizes_idx_0) {
            memset(&y_data[nm1d2], 0, (unsigned int)((input_sizes_idx_0 + 1) *
                    static_cast<int>(sizeof(double))));
          }

          for (k = 0; k < i31; k++) {
            aoffset = k * (input_sizes_idx_0 + 1);
            L = b_tmp_data[boffset + k];
            for (i = 0; i <= input_sizes_idx_0; i++) {
              i32 = aoffset + i;
              y_data_tmp = nm1d2 + i;
              y_data[y_data_tmp] += L * q_data[(i27 + i32 % ((i28 - i27) + 1)) +
                q_size[0] * (i32 / ((i28 - i27) + 1))];
            }
          }
        }
      }

      if ((y_size_idx_1 == 1) || (varargin_2 - i30 == 1)) {
        yend_size[0] = y_size_idx_0_tmp;
        for (i27 = 0; i27 < y_size_idx_0_tmp; i27++) {
          yend_data[i27] = 0.0;
          for (i28 = 0; i28 < y_size_idx_1; i28++) {
            yend_data[i27] += y_data[i27 + y_size_idx_0_tmp * i28] * y->data[i30
              + i28];
          }
        }
      } else {
        yend_size[0] = y_size_idx_0_tmp;
        if (0 <= y_size_idx_0_tmp - 1) {
          memset(&yend_data[0], 0, (unsigned int)(y_size_idx_0_tmp * static_cast<
                  int>(sizeof(double))));
        }

        for (k = 0; k < y_size_idx_1; k++) {
          aoffset = k * y_size_idx_0_tmp;
          for (i = 0; i < y_size_idx_0_tmp; i++) {
            yend_data[i] += y->data[i30 + k] * y_data[aoffset + i];
          }
        }
      }

      if (f > d->size[0]) {
        i27 = -1;
        i28 = -1;
      } else {
        i27 = v_size_tmp - 2;
        i28 = d->size[0] - 1;
      }

      i30 = nk->size[0];
      nk->size[0] = ((t_size[0] + i28) - i27) + yend_size[0];
      emxEnsureCapacity_real_T(nk, i30);
      nm1d2 = t_size[0];
      for (i30 = 0; i30 < nm1d2; i30++) {
        nk->data[i30] = t_data[i30];
      }

      nm1d2 = i28 - i27;
      for (i30 = 0; i30 < nm1d2; i30++) {
        nk->data[i30 + t_size[0]] = d->data[(i27 + i30) + 1];
      }

      nm1d2 = yend_size[0];
      for (i30 = 0; i30 < nm1d2; i30++) {
        nk->data[((i30 + t_size[0]) + i28) - i27] = yend_data[i30];
      }
    } else {
      //  non-uniformly distributed data
      i27 = nk->size[0];
      nk->size[0] = y->size[0];
      emxEnsureCapacity_real_T(nk, i27);
      nm1d2 = y->size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        nk->data[i27] = y->data[i27];
      }

      //  Turn off warnings when called from command line (already off if called from 
      //  cftool).
      //  ws = warning('off', 'all');
      //  [lastwarnmsg,lastwarnid]=lastwarn;
      for (i = 0; i < varargin_2; i++) {
        if ((1 + i > 1) && (x->data[i] == x->data[i - 1])) {
          nk->data[i] = nk->data[i - 1];
        } else {
          L = 1.0 + static_cast<double>(i);
          R = 1.0 + static_cast<double>(i);

          //  find leftmost and rightmost values
          do {
            exitg1 = 0;
            i27 = static_cast<int>(R);
            if ((i27 < varargin_2) && (x->data[i27] == x->data[i])) {
              R++;
            } else {
              exitg1 = 1;
            }
          } while (exitg1 == 0);

          while ((L > 1.0) && (x->data[static_cast<int>(L) - 2] == x->data[i]))
          {
            L--;
          }

          u1 = (f - ((R - L) + 1.0)) / 2.0;
          if ((0.0 > u1) || rtIsNaN(u1)) {
            d3 = 0.0;
          } else {
            d3 = u1;
          }

          hf = std::ceil(d3);

          //  need this many more on each side
          u1 = L - hf;
          if ((1.0 > u1) || rtIsNaN(u1)) {
            u1 = 1.0;
          }

          L = (static_cast<double>(varargin_2) - f) + 1.0;
          if ((L < u1) || rtIsNaN(u1)) {
          } else {
            L = u1;
          }

          //  find leftmost point needed
          while ((L > 1.0) && (x->data[static_cast<int>(L) - 1] == x->
                               data[static_cast<int>((L - 1.0)) - 1])) {
            L--;
          }

          u0 = R + hf;
          u1 = (L + f) - 1.0;
          if ((u0 > u1) || rtIsNaN(u1)) {
            u1 = u0;
          }

          if ((varargin_2 < u1) || rtIsNaN(u1)) {
            R = varargin_2;
          } else {
            R = u1;
          }

          //  find rightmost point needed
          while ((R < varargin_2) && (x->data[static_cast<int>(R) - 1] ==
                  x->data[static_cast<int>((R + 1.0)) - 1])) {
            R++;
          }

          if (rtIsNaN(L) || rtIsNaN(R)) {
            i27 = tidx->size[0] * tidx->size[1];
            tidx->size[0] = 1;
            tidx->size[1] = 1;
            emxEnsureCapacity_real_T(tidx, i27);
            tidx->data[0] = rtNaN;
          } else if (R < L) {
            tidx->size[0] = 1;
            tidx->size[1] = 0;
          } else if (rtIsInf(L) && (L == R)) {
            i27 = tidx->size[0] * tidx->size[1];
            tidx->size[0] = 1;
            tidx->size[1] = 1;
            emxEnsureCapacity_real_T(tidx, i27);
            tidx->data[0] = rtNaN;
          } else if (std::floor(L) == L) {
            i27 = tidx->size[0] * tidx->size[1];
            tidx->size[0] = 1;
            nm1d2 = static_cast<int>(std::floor(R - L));
            tidx->size[1] = nm1d2 + 1;
            emxEnsureCapacity_real_T(tidx, i27);
            for (i27 = 0; i27 <= nm1d2; i27++) {
              tidx->data[i27] = L + static_cast<double>(i27);
            }
          } else {
            ndbl = std::floor((R - L) + 0.5);
            apnd = L + ndbl;
            cdiff = apnd - R;
            u0 = std::abs(L);
            if ((!(u0 > R)) && (!rtIsNaN(R))) {
              u0 = R;
            }

            if (std::abs(cdiff) < 4.4408920985006262E-16 * u0) {
              ndbl++;
              apnd = R;
            } else if (cdiff > 0.0) {
              apnd = L + (ndbl - 1.0);
            } else {
              ndbl++;
            }

            if (ndbl >= 0.0) {
              n = static_cast<int>(ndbl);
            } else {
              n = 0;
            }

            i27 = tidx->size[0] * tidx->size[1];
            tidx->size[0] = 1;
            tidx->size[1] = n;
            emxEnsureCapacity_real_T(tidx, i27);
            if (n > 0) {
              tidx->data[0] = L;
              if (n > 1) {
                tidx->data[n - 1] = apnd;
                nm1d2 = (n - 1) / 2;
                for (k = 0; k <= nm1d2 - 2; k++) {
                  tidx->data[1 + k] = L + (1.0 + static_cast<double>(k));
                  tidx->data[(n - k) - 2] = apnd - (1.0 + static_cast<double>(k));
                }

                if (nm1d2 << 1 == n - 1) {
                  tidx->data[nm1d2] = (L + apnd) / 2.0;
                } else {
                  tidx->data[nm1d2] = L + static_cast<double>(nm1d2);
                  tidx->data[nm1d2 + 1] = apnd - static_cast<double>(nm1d2);
                }
              }
            }
          }

          i27 = c_tidx->size[0];
          c_tidx->size[0] = tidx->size[1];
          emxEnsureCapacity_int32_T(c_tidx, i27);
          nm1d2 = tidx->size[1];
          for (i27 = 0; i27 < nm1d2; i27++) {
            c_tidx->data[i27] = static_cast<int>(tidx->data[i27]);
          }

          boffset = c_tidx->size[0] - 1;
          input_sizes_idx_0 = 0;
          for (nm1d2 = 0; nm1d2 <= boffset; nm1d2++) {
            if (notnan->data[static_cast<int>(tidx->data[nm1d2]) - 1]) {
              input_sizes_idx_0++;
            }
          }

          i27 = r6->size[0];
          r6->size[0] = input_sizes_idx_0;
          emxEnsureCapacity_int32_T(r6, i27);
          input_sizes_idx_0 = 0;
          for (nm1d2 = 0; nm1d2 <= boffset; nm1d2++) {
            if (notnan->data[static_cast<int>(tidx->data[nm1d2]) - 1]) {
              r6->data[input_sizes_idx_0] = nm1d2 + 1;
              input_sizes_idx_0++;
            }
          }

          i27 = d_tidx->size[0] * d_tidx->size[1];
          d_tidx->size[0] = 1;
          d_tidx->size[1] = r6->size[0];
          emxEnsureCapacity_real_T(d_tidx, i27);
          nm1d2 = r6->size[0];
          for (i27 = 0; i27 < nm1d2; i27++) {
            d_tidx->data[i27] = static_cast<unsigned int>(tidx->data[r6->
              data[i27] - 1]);
          }

          i27 = tidx->size[0] * tidx->size[1];
          tidx->size[0] = 1;
          tidx->size[1] = d_tidx->size[1];
          emxEnsureCapacity_real_T(tidx, i27);
          nm1d2 = d_tidx->size[0] * d_tidx->size[1];
          for (i27 = 0; i27 < nm1d2; i27++) {
            tidx->data[i27] = d_tidx->data[i27];
          }

          if (tidx->size[1] == 0) {
            nk->data[i] = rtNaN;
          } else {
            i27 = diffx->size[0] * diffx->size[1];
            diffx->size[0] = 1;
            diffx->size[1] = tidx->size[1];
            emxEnsureCapacity_real_T(diffx, i27);
            L = x->data[i];
            nm1d2 = tidx->size[0] * tidx->size[1];
            for (i27 = 0; i27 < nm1d2; i27++) {
              diffx->data[i27] = x->data[static_cast<int>(tidx->data[i27]) - 1]
                - L;
            }

            //  center to improve conditioning
            b_diff(diffx, d_tidx);
            i27 = b_diffx->size[0] * b_diffx->size[1];
            b_diffx->size[0] = 1;
            b_diffx->size[1] = d_tidx->size[1];
            emxEnsureCapacity_boolean_T(b_diffx, i27);
            nm1d2 = d_tidx->size[0] * d_tidx->size[1];
            for (i27 = 0; i27 < nm1d2; i27++) {
              b_diffx->data[i27] = (d_tidx->data[i27] > 0.0);
            }

            u1 = 1.0 + c_sum(b_diffx);
            if ((4.0 < u1) || rtIsNaN(u1)) {
              L = 4.0;
            } else {
              L = u1;
            }

            i27 = v->size[0] * v->size[1];
            v->size[0] = diffx->size[1];
            i28 = static_cast<int>(L);
            v->size[1] = i28;
            emxEnsureCapacity_real_T(v, i27);
            nm1d2 = diffx->size[1] * i28;
            for (i27 = 0; i27 < nm1d2; i27++) {
              v->data[i27] = 1.0;
            }

            i27 = static_cast<int>((L - 1.0));
            for (j = 0; j < i27; j++) {
              nm1d2 = v->size[0];
              c_power(diffx, 1.0 + static_cast<double>(j), d_tidx);
              for (i28 = 0; i28 < nm1d2; i28++) {
                v->data[i28 + v->size[0] * (j + 1)] = d_tidx->data[i28];
              }
            }

            if (v->size[0] == v->size[1]) {
              //  Square v may give infs in the \ solution, so force least squares 
              sizes_idx_1 = static_cast<signed char>(v->size[1]);
              if (v->size[1] != 0) {
                input_sizes_idx_0 = v->size[0];
              } else {
                input_sizes_idx_0 = 0;
              }

              i27 = d->size[0];
              d->size[0] = tidx->size[1] + 1;
              emxEnsureCapacity_real_T(d, i27);
              nm1d2 = tidx->size[1];
              for (i27 = 0; i27 < nm1d2; i27++) {
                d->data[i27] = y->data[static_cast<int>(tidx->data[i27]) - 1];
              }

              d->data[tidx->size[1]] = 0.0;
              i27 = b_v->size[0] * b_v->size[1];
              b_v->size[0] = input_sizes_idx_0 + 1;
              b_v->size[1] = sizes_idx_1;
              emxEnsureCapacity_real_T(b_v, i27);
              nm1d2 = sizes_idx_1;
              for (i27 = 0; i27 < nm1d2; i27++) {
                for (i28 = 0; i28 < input_sizes_idx_0; i28++) {
                  b_v->data[i28 + b_v->size[0] * i27] = v->data[i28 +
                    input_sizes_idx_0 * i27];
                }
              }

              nm1d2 = sizes_idx_1;
              for (i27 = 0; i27 < nm1d2; i27++) {
                for (i28 = 0; i28 < 1; i28++) {
                  b_v->data[input_sizes_idx_0 + b_v->size[0] * i27] = 0.0;
                }
              }

              b_mldivide(b_v, d);
            } else {
              i27 = d->size[0];
              d->size[0] = tidx->size[1];
              emxEnsureCapacity_real_T(d, i27);
              nm1d2 = tidx->size[1];
              for (i27 = 0; i27 < nm1d2; i27++) {
                d->data[i27] = y->data[static_cast<int>(tidx->data[i27]) - 1];
              }

              b_mldivide(v, d);
            }

            nk->data[i] = d->data[0];
          }
        }
      }

      i27 = iidx->size[0] * iidx->size[1];
      iidx->size[0] = 1;
      iidx->size[1] = idx->size[1];
      emxEnsureCapacity_int32_T(iidx, i27);
      nm1d2 = idx->size[0] * idx->size[1];
      for (i27 = 0; i27 < nm1d2; i27++) {
        iidx->data[i27] = static_cast<int>(idx->data[i27]);
      }

      nm1d2 = iidx->size[0] * iidx->size[1];
      i27 = d->size[0];
      d->size[0] = nm1d2;
      emxEnsureCapacity_real_T(d, i27);
      for (i27 = 0; i27 < nm1d2; i27++) {
        d->data[i27] = nk->data[i27];
      }

      nm1d2 = d->size[0];
      for (i27 = 0; i27 < nm1d2; i27++) {
        nk->data[iidx->data[i27] - 1] = d->data[i27];
      }
    }
  }

  emxFree_real_T(&b_v);
  emxFree_real_T(&d_tidx);
  emxFree_int32_T(&c_tidx);
  emxFree_real_T(&b_tidx);
  emxFree_boolean_T(&b_notnan);
  emxFree_boolean_T(&b_diffx);
  emxFree_int32_T(&iidx);
  emxFree_int32_T(&r6);
  emxFree_real_T(&d);
  emxFree_real_T(&tidx);
  emxFree_real_T(&v);
  emxFree_real_T(&idx);
  emxFree_boolean_T(&notnan);
  emxFree_real_T(&diffx);
  emxFree_real_T(&y);
  emxFree_real_T(&x);

  //  nk=wden(ns,'heursure','s','sln',5,'sym8');
}

//
// File trailer for geto_new.cpp
//
// [EOF]
//
