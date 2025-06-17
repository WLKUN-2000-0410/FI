//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: resolution_factor.cpp
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
#include "analyseMixture_emxutil.h"
#include "sum.h"
#include "interp1.h"
#include "diff.h"
#include "findpeaks.h"

// Function Definitions

//
// 添加了一部分前期处理步骤
// Arguments    : const emxArray_real_T *a
//                double d_data[]
//                int d_size[2]
// Return Type  : void
//
void b_resolution_factor(const emxArray_real_T *a, double d_data[], int d_size[2])
{
  emxArray_real_T *c;
  emxArray_real_T *locs;
  emxArray_real_T *wid;
  emxArray_real_T *varargin_1;
  emxArray_real_T *b_a;
  emxArray_boolean_T *x;
  int i44;
  int i;
  int i45;
  int idx;
  int k;
  double ex;
  int exitg1;
  boolean_T exitg2;
  double left;
  int ii_data[1];
  unsigned int zuo1_data[1];
  int maxval;
  int loop_ub;
  double tmp_data[1];
  int tmp_size[2];
  boolean_T y;
  boolean_T x_data[1];
  double ns2[1800];
  double ns1[1800];
  double b_ns2[1800];
  boolean_T bv0[1800];
  short b_tmp_data[1800];
  double d[1650];

  //   背景矫正后调整分辨率
  emxInit_real_T(&c, 2);
  emxInit_real_T(&locs, 1);
  emxInit_real_T(&wid, 1);
  emxInit_real_T(&varargin_1, 1);
  emxInit_real_T(&b_a, 1);
  if (a->data[0] > 0.0) {
    i44 = a->size[0];
    i45 = a->size[0];
    if (i45 <= 2) {
      i44 = a->size[0];
      if (i44 == 1) {
        ex = a->data[a->size[0]];
      } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                  (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
      {
        ex = a->data[1 + a->size[0]];
      } else {
        ex = a->data[a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        do {
          exitg1 = 0;
          i45 = a->size[0];
          if (k <= i45) {
            if (!rtIsNaN(a->data[(k + a->size[0]) - 1])) {
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
        ex = a->data[a->size[0]];
      } else {
        ex = a->data[(idx + a->size[0]) - 1];
        i45 = idx + 1;
        for (k = i45; k <= i44; k++) {
          if (ex < a->data[(k + a->size[0]) - 1]) {
            ex = a->data[(k + a->size[0]) - 1];
          }
        }
      }
    }

    i44 = a->size[0];
    i45 = a->size[0];
    if (i45 <= 2) {
      i44 = a->size[0];
      if (i44 == 1) {
        left = a->data[a->size[0]];
      } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                  (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
      {
        left = a->data[1 + a->size[0]];
      } else {
        left = a->data[a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        do {
          exitg1 = 0;
          i45 = a->size[0];
          if (k <= i45) {
            if (!rtIsNaN(a->data[(k + a->size[0]) - 1])) {
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
        left = a->data[a->size[0]];
      } else {
        left = a->data[(idx + a->size[0]) - 1];
        i45 = idx + 1;
        for (k = i45; k <= i44; k++) {
          if (left < a->data[(k + a->size[0]) - 1]) {
            left = a->data[(k + a->size[0]) - 1];
          }
        }
      }
    }

    i = a->size[0];
    i44 = b_a->size[0];
    b_a->size[0] = i;
    emxEnsureCapacity_real_T(b_a, i44);
    for (i44 = 0; i44 < i; i44++) {
      b_a->data[i44] = a->data[i44 + a->size[0]];
    }

    b_findpeaks(b_a, ex * 0.02, 0.005 * left, varargin_1, locs, wid);

    //  利用findpeaks函数寻峰
    if (locs->size[0] == 0) {
      c->size[0] = 0;
      c->size[1] = 0;
    } else {
      i = a->size[0];
      i44 = b_a->size[0];
      b_a->size[0] = i;
      emxEnsureCapacity_real_T(b_a, i44);
      for (i44 = 0; i44 < i; i44++) {
        b_a->data[i44] = a->data[i44 + a->size[0]];
      }

      diff(b_a, varargin_1);
      left = std::ceil(locs->data[0] - wid->data[0]);
      if (left <= 0.0) {
        if (locs->data[0] < 0.5 * wid->data[0]) {
          left = locs->data[0] - 10.0;
        } else {
          left = 1.0;
        }
      } else {
        i44 = a->size[0];
        i45 = a->size[0];
        if (i45 <= 2) {
          i44 = a->size[0];
          if (i44 == 1) {
            ex = a->data[a->size[0]];
          } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                      (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
          {
            ex = a->data[1 + a->size[0]];
          } else {
            ex = a->data[a->size[0]];
          }
        } else {
          if (!rtIsNaN(a->data[a->size[0]])) {
            idx = 1;
          } else {
            idx = 0;
            k = 2;
            do {
              exitg1 = 0;
              i45 = a->size[0];
              if (k <= i45) {
                if (!rtIsNaN(a->data[(k + a->size[0]) - 1])) {
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
            ex = a->data[a->size[0]];
          } else {
            ex = a->data[(idx + a->size[0]) - 1];
            i45 = idx + 1;
            for (k = i45; k <= i44; k++) {
              if (ex < a->data[(k + a->size[0]) - 1]) {
                ex = a->data[(k + a->size[0]) - 1];
              }
            }
          }
        }

        if (varargin_1->data[static_cast<int>(left) - 1] / ex > 0.01) {
          left = locs->data[0] - 25.0;
        }
      }

      if (left <= 0.0) {
        left = 1.0;
      }

      if (left > a->size[0]) {
        i44 = 0;
        i45 = 0;
      } else {
        i44 = static_cast<int>(left) - 1;
        i45 = a->size[0];
      }

      i = a->size[1];
      k = c->size[0] * c->size[1];
      loop_ub = i45 - i44;
      c->size[0] = loop_ub;
      c->size[1] = i;
      emxEnsureCapacity_real_T(c, k);
      for (i45 = 0; i45 < i; i45++) {
        for (k = 0; k < loop_ub; k++) {
          c->data[k + c->size[0] * i45] = a->data[(i44 + k) + a->size[0] * i45];
        }
      }
    }
  } else {
    emxInit_boolean_T(&x, 1);
    i = a->size[0];
    i44 = x->size[0];
    x->size[0] = i;
    emxEnsureCapacity_boolean_T(x, i44);
    for (i44 = 0; i44 < i; i44++) {
      x->data[i44] = (a->data[i44] < 200.0);
    }

    k = (1 <= x->size[0]);
    i = x->size[0];
    idx = 0;
    exitg2 = false;
    while ((!exitg2) && (i > 0)) {
      if (x->data[i - 1]) {
        idx = 1;
        ii_data[0] = i;
        exitg2 = true;
      } else {
        i--;
      }
    }

    emxFree_boolean_T(&x);
    if (k == 1) {
      if (idx == 0) {
        k = 0;
      }
    } else {
      k = (1 <= idx);
    }

    for (i44 = 0; i44 < k; i44++) {
      zuo1_data[0] = static_cast<unsigned int>(ii_data[0]);
    }

    i44 = varargin_1->size[0];
    varargin_1->size[0] = k;
    emxEnsureCapacity_real_T(varargin_1, i44);
    for (i44 = 0; i44 < k; i44++) {
      varargin_1->data[0] = ii_data[0];
    }

    if (varargin_1->size[0] == 1) {
      maxval = static_cast<int>(zuo1_data[0]);
    } else if (static_cast<int>(zuo1_data[0]) < static_cast<int>(zuo1_data[1]))
    {
      maxval = static_cast<int>(zuo1_data[1]);
    } else {
      maxval = static_cast<int>(zuo1_data[0]);
    }

    if (maxval + 1U > static_cast<unsigned int>(a->size[0])) {
      maxval = 0;
      i44 = -1;
    } else {
      i44 = a->size[0] - 1;
    }

    i = a->size[1];
    i45 = c->size[0] * c->size[1];
    loop_ub = i44 - maxval;
    i44 = loop_ub + 1;
    c->size[0] = i44;
    c->size[1] = i;
    emxEnsureCapacity_real_T(c, i45);
    for (i45 = 0; i45 < i; i45++) {
      for (k = 0; k <= loop_ub; k++) {
        c->data[k + c->size[0] * i45] = a->data[(maxval + k) + a->size[0] * i45];
      }
    }

    if (i44 <= 2) {
      if (i44 == 1) {
        ex = a->data[maxval + a->size[0]];
      } else if ((a->data[maxval + a->size[0]] < a->data[(maxval + a->size[0]) +
                  1]) || (rtIsNaN(a->data[maxval + a->size[0]]) && (!rtIsNaN
                   (a->data[(maxval + a->size[0]) + 1])))) {
        ex = a->data[(maxval + a->size[0]) + 1];
      } else {
        ex = a->data[maxval + a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[maxval + a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg2 = false;
        while ((!exitg2) && (k <= i44)) {
          if (!rtIsNaN(a->data[((maxval + k) + a->size[0]) - 1])) {
            idx = k;
            exitg2 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = a->data[maxval + a->size[0]];
      } else {
        ex = a->data[((maxval + idx) + a->size[0]) - 1];
        i45 = idx + 1;
        for (k = i45; k <= i44; k++) {
          if (ex < c->data[(k + c->size[0]) - 1]) {
            ex = c->data[(k + c->size[0]) - 1];
          }
        }
      }
    }

    i45 = b_a->size[0];
    b_a->size[0] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_a, i45);
    for (i45 = 0; i45 <= loop_ub; i45++) {
      b_a->data[i45] = a->data[(maxval + i45) + a->size[0]];
    }

    c_findpeaks(b_a, ex * 0.03, varargin_1, locs, wid);

    //  利用findpeaks函数寻峰
    if (locs->size[0] == 0) {
      c->size[0] = 0;
      c->size[1] = 0;
    } else {
      i45 = b_a->size[0];
      b_a->size[0] = loop_ub + 1;
      emxEnsureCapacity_real_T(b_a, i45);
      for (i45 = 0; i45 <= loop_ub; i45++) {
        b_a->data[i45] = a->data[(maxval + i45) + a->size[0]];
      }

      diff(b_a, varargin_1);
      left = std::ceil(locs->data[0] - wid->data[0]);
      if (left <= 0.0) {
        if (locs->data[0] < 0.5 * wid->data[0]) {
          left = locs->data[0] - 10.0;
        } else {
          left = 1.0;
        }
      } else {
        if (i44 <= 2) {
          if (i44 == 1) {
            ex = a->data[maxval + a->size[0]];
          } else if ((a->data[maxval + a->size[0]] < a->data[(maxval + a->size[0])
                      + 1]) || (rtIsNaN(a->data[maxval + a->size[0]]) &&
                                (!rtIsNaN(a->data[(maxval + a->size[0]) + 1]))))
          {
            ex = a->data[(maxval + a->size[0]) + 1];
          } else {
            ex = a->data[maxval + a->size[0]];
          }
        } else {
          if (!rtIsNaN(a->data[maxval + a->size[0]])) {
            idx = 1;
          } else {
            idx = 0;
            k = 2;
            exitg2 = false;
            while ((!exitg2) && (k <= i44)) {
              if (!rtIsNaN(a->data[((maxval + k) + a->size[0]) - 1])) {
                idx = k;
                exitg2 = true;
              } else {
                k++;
              }
            }
          }

          if (idx == 0) {
            ex = a->data[maxval + a->size[0]];
          } else {
            ex = a->data[((maxval + idx) + a->size[0]) - 1];
            i45 = idx + 1;
            for (k = i45; k <= i44; k++) {
              if (ex < c->data[(k + c->size[0]) - 1]) {
                ex = c->data[(k + c->size[0]) - 1];
              }
            }
          }
        }

        if (varargin_1->data[static_cast<int>(left) - 1] / ex > 0.01) {
          left = locs->data[0] - 25.0;
        }
      }

      if (left <= 0.0) {
        left = 1.0;
      }

      if (left > i44) {
        i44 = 0;
        loop_ub = -1;
      } else {
        i44 = static_cast<int>(left) - 1;
      }

      i = a->size[1] - 1;
      i45 = c->size[0] * c->size[1];
      loop_ub -= i44;
      c->size[0] = loop_ub + 1;
      c->size[1] = i + 1;
      emxEnsureCapacity_real_T(c, i45);
      for (i45 = 0; i45 <= i; i45++) {
        for (k = 0; k <= loop_ub; k++) {
          c->data[k + c->size[0] * i45] = a->data[((maxval + i44) + k) + a->
            size[0] * i45];
        }
      }
    }
  }

  emxFree_real_T(&wid);
  emxFree_real_T(&locs);
  if ((c->size[0] == 0) || (c->size[1] == 0)) {
    d_size[0] = 0;
    d_size[1] = 0;
  } else {
    i44 = c->size[0];
    i45 = c->size[0];
    if (i45 <= 2) {
      i44 = c->size[0];
      if (i44 == 1) {
        ex = c->data[c->size[0]];
      } else if ((c->data[c->size[0]] < c->data[1 + c->size[0]]) || (rtIsNaN
                  (c->data[c->size[0]]) && (!rtIsNaN(c->data[1 + c->size[0]]))))
      {
        ex = c->data[1 + c->size[0]];
      } else {
        ex = c->data[c->size[0]];
      }
    } else {
      if (!rtIsNaN(c->data[c->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        do {
          exitg1 = 0;
          i45 = c->size[0];
          if (k <= i45) {
            if (!rtIsNaN(c->data[(k + c->size[0]) - 1])) {
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
        ex = c->data[c->size[0]];
      } else {
        ex = c->data[(idx + c->size[0]) - 1];
        i45 = idx + 1;
        for (k = i45; k <= i44; k++) {
          if (ex < c->data[(k + c->size[0]) - 1]) {
            ex = c->data[(k + c->size[0]) - 1];
          }
        }
      }
    }

    if (ex == 65535.0) {
      d_size[0] = 0;
      d_size[1] = 0;
    } else {
      //  201~2000为波数
      //  插值成标准长度
      i = c->size[0];
      i44 = varargin_1->size[0];
      varargin_1->size[0] = i;
      emxEnsureCapacity_real_T(varargin_1, i44);
      for (i44 = 0; i44 < i; i44++) {
        varargin_1->data[i44] = c->data[i44];
      }

      i = c->size[0];
      i44 = b_a->size[0];
      b_a->size[0] = i;
      emxEnsureCapacity_real_T(b_a, i44);
      for (i44 = 0; i44 < i; i44++) {
        b_a->data[i44] = c->data[i44 + c->size[0]];
      }

      interp1(varargin_1, b_a, ns2);
      baseline_correct(ns2, ns1);

      //  减背景算法-ns1为背景线
      i = c->size[0];
      i44 = varargin_1->size[0];
      varargin_1->size[0] = i;
      emxEnsureCapacity_real_T(varargin_1, i44);
      for (i44 = 0; i44 < i; i44++) {
        varargin_1->data[i44] = c->data[i44];
      }

      i = c->size[0];
      i44 = b_a->size[0];
      b_a->size[0] = i;
      emxEnsureCapacity_real_T(b_a, i44);
      for (i44 = 0; i44 < i; i44++) {
        b_a->data[i44] = c->data[i44 + c->size[0]];
      }

      interp1(varargin_1, b_a, ns2);
      for (i44 = 0; i44 < 1800; i44++) {
        b_ns2[i44] = ns2[i44] - ns1[i44];
      }

      b_geto_new(b_ns2, ns2);

      //  降噪算法
      idx = 0;
      for (i = 0; i < 1800; i++) {
        y = rtIsNaN(ns2[i]);
        bv0[i] = y;
        if (y) {
          idx++;
        }
      }

      k = 0;
      for (i = 0; i < 1800; i++) {
        if (bv0[i]) {
          b_tmp_data[k] = static_cast<short>((i + 1));
          k++;
        }
      }

      for (i44 = 0; i44 < idx; i44++) {
        ns2[b_tmp_data[i44] - 1] = 0.0;
      }

      for (i = 0; i < 1800; i++) {
        if (ns2[i] < 0.0) {
          ns2[i] = 0.0;
        }
      }

      if (!rtIsNaN(ns2[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg2 = false;
        while ((!exitg2) && (k < 1801)) {
          if (!rtIsNaN(ns2[k - 1])) {
            idx = k;
            exitg2 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = ns2[0];
      } else {
        ex = ns2[idx - 1];
        i44 = idx + 1;
        for (k = i44; k < 1801; k++) {
          left = ns2[k - 1];
          if (ex < left) {
            ex = left;
          }
        }
      }

      for (i = 0; i < 1800; i++) {
        ns1[i] = ns2[i] / ex;
      }

      for (i = 0; i < 1650; i++) {
        d[i] = ns2[i + 150] / ex;
      }

      if (!rtIsNaN(d[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg2 = false;
        while ((!exitg2) && (k < 1651)) {
          if (!rtIsNaN(d[k - 1])) {
            idx = k;
            exitg2 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = ns1[150];
      } else {
        ex = ns1[idx + 149];
        i44 = idx + 1;
        for (k = i44; k < 1651; k++) {
          left = ns1[k + 149];
          if (ex < left) {
            ex = left;
          }
        }
      }

      d_size[0] = 1650;
      d_size[1] = 1;
      for (i44 = 0; i44 < 1650; i44++) {
        d_data[i44] = ns1[150 + i44] / ex;
      }
    }

    d_sum(d_data, d_size, tmp_data, tmp_size);
    i = tmp_size[0] * tmp_size[1];
    for (i44 = 0; i44 < i; i44++) {
      x_data[i44] = (tmp_data[i44] == 0.0);
    }

    y = (tmp_size[1] != 0);
    if (y) {
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k <= tmp_size[1] - 1)) {
        if (!x_data[0]) {
          y = false;
          exitg2 = true;
        } else {
          k = 1;
        }
      }
    }

    if (y) {
      d_size[0] = 0;
      d_size[1] = 0;
    }
  }

  emxFree_real_T(&b_a);
  emxFree_real_T(&varargin_1);
  emxFree_real_T(&c);
}

//
// 添加了一部分前期处理步骤
// Arguments    : const emxArray_real_T *a
//                double d_data[]
//                int d_size[2]
// Return Type  : void
//
void resolution_factor(const emxArray_real_T *a, double d_data[], int d_size[2])
{
  emxArray_real_T *locs;
  emxArray_real_T *wid;
  emxArray_real_T *c;
  emxArray_real_T *varargin_1;
  emxArray_real_T *b_a;
  emxArray_boolean_T *x;
  int i55;
  int loop_ub;
  int i56;
  int idx;
  double ex;
  int exitg1;
  int i;
  boolean_T exitg2;
  double left;
  int ii_data[1];
  unsigned int zuo1_data[1];
  int maxval;
  int n_tmp_tmp;
  double tmp_data[1];
  int tmp_size[2];
  boolean_T y;
  boolean_T x_data[1];
  double ns2[1800];
  double ns1[1800];
  double b_ns2[1800];
  boolean_T bv1[1800];
  short b_tmp_data[1800];
  double d[1650];

  //   背景矫正后调整分辨率
  emxInit_real_T(&locs, 1);
  emxInit_real_T(&wid, 1);
  emxInit_real_T(&c, 2);
  emxInit_real_T(&varargin_1, 1);
  emxInit_real_T(&b_a, 1);
  if (a->data[0] > 0.0) {
    i55 = a->size[0];
    i56 = a->size[0];
    if (i56 <= 2) {
      i55 = a->size[0];
      if (i55 == 1) {
        ex = a->data[a->size[0]];
      } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                  (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
      {
        ex = a->data[1 + a->size[0]];
      } else {
        ex = a->data[a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        do {
          exitg1 = 0;
          i56 = a->size[0];
          if (loop_ub <= i56) {
            if (!rtIsNaN(a->data[(loop_ub + a->size[0]) - 1])) {
              idx = loop_ub;
              exitg1 = 1;
            } else {
              loop_ub++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }

      if (idx == 0) {
        ex = a->data[a->size[0]];
      } else {
        ex = a->data[(idx + a->size[0]) - 1];
        i56 = idx + 1;
        for (loop_ub = i56; loop_ub <= i55; loop_ub++) {
          if (ex < a->data[(loop_ub + a->size[0]) - 1]) {
            ex = a->data[(loop_ub + a->size[0]) - 1];
          }
        }
      }
    }

    i55 = a->size[0];
    i56 = a->size[0];
    if (i56 <= 2) {
      i55 = a->size[0];
      if (i55 == 1) {
        left = a->data[a->size[0]];
      } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                  (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
      {
        left = a->data[1 + a->size[0]];
      } else {
        left = a->data[a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        do {
          exitg1 = 0;
          i56 = a->size[0];
          if (loop_ub <= i56) {
            if (!rtIsNaN(a->data[(loop_ub + a->size[0]) - 1])) {
              idx = loop_ub;
              exitg1 = 1;
            } else {
              loop_ub++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }

      if (idx == 0) {
        left = a->data[a->size[0]];
      } else {
        left = a->data[(idx + a->size[0]) - 1];
        i56 = idx + 1;
        for (loop_ub = i56; loop_ub <= i55; loop_ub++) {
          if (left < a->data[(loop_ub + a->size[0]) - 1]) {
            left = a->data[(loop_ub + a->size[0]) - 1];
          }
        }
      }
    }

    loop_ub = a->size[0];
    i55 = b_a->size[0];
    b_a->size[0] = loop_ub;
    emxEnsureCapacity_real_T(b_a, i55);
    for (i55 = 0; i55 < loop_ub; i55++) {
      b_a->data[i55] = a->data[i55 + a->size[0]];
    }

    b_findpeaks(b_a, ex * 0.02, 0.005 * left, varargin_1, locs, wid);

    //  利用findpeaks函数寻峰
    if (locs->size[0] == 0) {
      c->size[0] = 0;
      c->size[1] = 0;
    } else {
      loop_ub = a->size[0];
      i55 = b_a->size[0];
      b_a->size[0] = loop_ub;
      emxEnsureCapacity_real_T(b_a, i55);
      for (i55 = 0; i55 < loop_ub; i55++) {
        b_a->data[i55] = a->data[i55 + a->size[0]];
      }

      diff(b_a, varargin_1);
      left = std::ceil(locs->data[0] - wid->data[0]);
      if (left <= 0.0) {
        if (locs->data[0] < 0.5 * wid->data[0]) {
          left = locs->data[0] - 10.0;
        } else {
          left = 1.0;
        }
      } else {
        i55 = a->size[0];
        i56 = a->size[0];
        if (i56 <= 2) {
          i55 = a->size[0];
          if (i55 == 1) {
            ex = a->data[a->size[0]];
          } else if ((a->data[a->size[0]] < a->data[1 + a->size[0]]) || (rtIsNaN
                      (a->data[a->size[0]]) && (!rtIsNaN(a->data[1 + a->size[0]]))))
          {
            ex = a->data[1 + a->size[0]];
          } else {
            ex = a->data[a->size[0]];
          }
        } else {
          if (!rtIsNaN(a->data[a->size[0]])) {
            idx = 1;
          } else {
            idx = 0;
            loop_ub = 2;
            do {
              exitg1 = 0;
              i56 = a->size[0];
              if (loop_ub <= i56) {
                if (!rtIsNaN(a->data[(loop_ub + a->size[0]) - 1])) {
                  idx = loop_ub;
                  exitg1 = 1;
                } else {
                  loop_ub++;
                }
              } else {
                exitg1 = 1;
              }
            } while (exitg1 == 0);
          }

          if (idx == 0) {
            ex = a->data[a->size[0]];
          } else {
            ex = a->data[(idx + a->size[0]) - 1];
            i56 = idx + 1;
            for (loop_ub = i56; loop_ub <= i55; loop_ub++) {
              if (ex < a->data[(loop_ub + a->size[0]) - 1]) {
                ex = a->data[(loop_ub + a->size[0]) - 1];
              }
            }
          }
        }

        if (varargin_1->data[static_cast<int>(left) - 1] / ex > 0.01) {
          left = locs->data[0] - 25.0;
        }
      }

      if (left <= 0.0) {
        left = 1.0;
      }

      if (left > a->size[0]) {
        i55 = 0;
        i56 = 0;
      } else {
        i55 = static_cast<int>(left) - 1;
        i56 = a->size[0];
      }

      i = c->size[0] * c->size[1];
      loop_ub = i56 - i55;
      c->size[0] = loop_ub;
      c->size[1] = 2;
      emxEnsureCapacity_real_T(c, i);
      for (i56 = 0; i56 < loop_ub; i56++) {
        c->data[i56] = a->data[i55 + i56];
      }

      for (i56 = 0; i56 < loop_ub; i56++) {
        c->data[i56 + c->size[0]] = a->data[(i55 + i56) + a->size[0]];
      }
    }
  } else {
    emxInit_boolean_T(&x, 1);
    loop_ub = a->size[0];
    i55 = x->size[0];
    x->size[0] = loop_ub;
    emxEnsureCapacity_boolean_T(x, i55);
    for (i55 = 0; i55 < loop_ub; i55++) {
      x->data[i55] = (a->data[i55] < 200.0);
    }

    loop_ub = (1 <= x->size[0]);
    i = x->size[0];
    idx = 0;
    exitg2 = false;
    while ((!exitg2) && (i > 0)) {
      if (x->data[i - 1]) {
        idx = 1;
        ii_data[0] = i;
        exitg2 = true;
      } else {
        i--;
      }
    }

    emxFree_boolean_T(&x);
    if (loop_ub == 1) {
      if (idx == 0) {
        loop_ub = 0;
      }
    } else {
      loop_ub = (1 <= idx);
    }

    for (i55 = 0; i55 < loop_ub; i55++) {
      zuo1_data[0] = static_cast<unsigned int>(ii_data[0]);
    }

    i55 = varargin_1->size[0];
    varargin_1->size[0] = loop_ub;
    emxEnsureCapacity_real_T(varargin_1, i55);
    for (i55 = 0; i55 < loop_ub; i55++) {
      varargin_1->data[0] = ii_data[0];
    }

    if (varargin_1->size[0] == 1) {
      maxval = static_cast<int>(zuo1_data[0]);
    } else if (static_cast<int>(zuo1_data[0]) < static_cast<int>(zuo1_data[1]))
    {
      maxval = static_cast<int>(zuo1_data[1]);
    } else {
      maxval = static_cast<int>(zuo1_data[0]);
    }

    if (maxval + 1U > static_cast<unsigned int>(a->size[0])) {
      maxval = 0;
      i55 = -1;
    } else {
      i55 = a->size[0] - 1;
    }

    n_tmp_tmp = i55 - maxval;
    i = n_tmp_tmp + 1;
    if (i <= 2) {
      if (i == 1) {
        ex = a->data[maxval + a->size[0]];
      } else if ((a->data[maxval + a->size[0]] < a->data[(maxval + a->size[0]) +
                  1]) || (rtIsNaN(a->data[maxval + a->size[0]]) && (!rtIsNaN
                   (a->data[(maxval + a->size[0]) + 1])))) {
        ex = a->data[(maxval + a->size[0]) + 1];
      } else {
        ex = a->data[maxval + a->size[0]];
      }
    } else {
      if (!rtIsNaN(a->data[maxval + a->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        exitg2 = false;
        while ((!exitg2) && (loop_ub <= i)) {
          if (!rtIsNaN(a->data[((maxval + loop_ub) + a->size[0]) - 1])) {
            idx = loop_ub;
            exitg2 = true;
          } else {
            loop_ub++;
          }
        }
      }

      if (idx == 0) {
        ex = a->data[maxval + a->size[0]];
      } else {
        ex = a->data[((maxval + idx) + a->size[0]) - 1];
        i55 = idx + 1;
        for (loop_ub = i55; loop_ub <= i; loop_ub++) {
          if (ex < a->data[((maxval + loop_ub) + a->size[0]) - 1]) {
            ex = a->data[((maxval + loop_ub) + a->size[0]) - 1];
          }
        }
      }
    }

    i55 = b_a->size[0];
    b_a->size[0] = n_tmp_tmp + 1;
    emxEnsureCapacity_real_T(b_a, i55);
    for (i55 = 0; i55 <= n_tmp_tmp; i55++) {
      b_a->data[i55] = a->data[(maxval + i55) + a->size[0]];
    }

    c_findpeaks(b_a, ex * 0.03, varargin_1, locs, wid);

    //  利用findpeaks函数寻峰
    if (locs->size[0] == 0) {
      c->size[0] = 0;
      c->size[1] = 0;
    } else {
      i55 = b_a->size[0];
      b_a->size[0] = n_tmp_tmp + 1;
      emxEnsureCapacity_real_T(b_a, i55);
      for (i55 = 0; i55 <= n_tmp_tmp; i55++) {
        b_a->data[i55] = a->data[(maxval + i55) + a->size[0]];
      }

      diff(b_a, varargin_1);
      left = std::ceil(locs->data[0] - wid->data[0]);
      if (left <= 0.0) {
        if (locs->data[0] < 0.5 * wid->data[0]) {
          left = locs->data[0] - 10.0;
        } else {
          left = 1.0;
        }
      } else {
        if (i <= 2) {
          if (i == 1) {
            ex = a->data[maxval + a->size[0]];
          } else if ((a->data[maxval + a->size[0]] < a->data[(maxval + a->size[0])
                      + 1]) || (rtIsNaN(a->data[maxval + a->size[0]]) &&
                                (!rtIsNaN(a->data[(maxval + a->size[0]) + 1]))))
          {
            ex = a->data[(maxval + a->size[0]) + 1];
          } else {
            ex = a->data[maxval + a->size[0]];
          }
        } else {
          if (!rtIsNaN(a->data[maxval + a->size[0]])) {
            idx = 1;
          } else {
            idx = 0;
            loop_ub = 2;
            exitg2 = false;
            while ((!exitg2) && (loop_ub <= i)) {
              if (!rtIsNaN(a->data[((maxval + loop_ub) + a->size[0]) - 1])) {
                idx = loop_ub;
                exitg2 = true;
              } else {
                loop_ub++;
              }
            }
          }

          if (idx == 0) {
            ex = a->data[maxval + a->size[0]];
          } else {
            ex = a->data[((maxval + idx) + a->size[0]) - 1];
            i55 = idx + 1;
            for (loop_ub = i55; loop_ub <= i; loop_ub++) {
              if (ex < a->data[((maxval + loop_ub) + a->size[0]) - 1]) {
                ex = a->data[((maxval + loop_ub) + a->size[0]) - 1];
              }
            }
          }
        }

        if (varargin_1->data[static_cast<int>(left) - 1] / ex > 0.01) {
          left = locs->data[0] - 25.0;
        }
      }

      if (left <= 0.0) {
        left = 1.0;
      }

      if (left > i) {
        i55 = 0;
        n_tmp_tmp = -1;
      } else {
        i55 = static_cast<int>(left) - 1;
      }

      i56 = c->size[0] * c->size[1];
      loop_ub = n_tmp_tmp - i55;
      c->size[0] = loop_ub + 1;
      c->size[1] = 2;
      emxEnsureCapacity_real_T(c, i56);
      for (i56 = 0; i56 <= loop_ub; i56++) {
        c->data[i56] = a->data[(maxval + i55) + i56];
      }

      for (i56 = 0; i56 <= loop_ub; i56++) {
        c->data[i56 + c->size[0]] = a->data[((maxval + i55) + i56) + a->size[0]];
      }
    }
  }

  emxFree_real_T(&wid);
  emxFree_real_T(&locs);
  if ((c->size[0] == 0) || (c->size[1] == 0)) {
    d_size[0] = 0;
    d_size[1] = 0;
  } else {
    i55 = c->size[0];
    i56 = c->size[0];
    if (i56 <= 2) {
      i55 = c->size[0];
      if (i55 == 1) {
        ex = c->data[c->size[0]];
      } else if ((c->data[c->size[0]] < c->data[1 + c->size[0]]) || (rtIsNaN
                  (c->data[c->size[0]]) && (!rtIsNaN(c->data[1 + c->size[0]]))))
      {
        ex = c->data[1 + c->size[0]];
      } else {
        ex = c->data[c->size[0]];
      }
    } else {
      if (!rtIsNaN(c->data[c->size[0]])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        do {
          exitg1 = 0;
          i56 = c->size[0];
          if (loop_ub <= i56) {
            if (!rtIsNaN(c->data[(loop_ub + c->size[0]) - 1])) {
              idx = loop_ub;
              exitg1 = 1;
            } else {
              loop_ub++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }

      if (idx == 0) {
        ex = c->data[c->size[0]];
      } else {
        ex = c->data[(idx + c->size[0]) - 1];
        i56 = idx + 1;
        for (loop_ub = i56; loop_ub <= i55; loop_ub++) {
          if (ex < c->data[(loop_ub + c->size[0]) - 1]) {
            ex = c->data[(loop_ub + c->size[0]) - 1];
          }
        }
      }
    }

    if (ex == 65535.0) {
      d_size[0] = 0;
      d_size[1] = 0;
    } else {
      //  201~2000为波数
      //  插值成标准长度
      loop_ub = c->size[0];
      i55 = varargin_1->size[0];
      varargin_1->size[0] = loop_ub;
      emxEnsureCapacity_real_T(varargin_1, i55);
      for (i55 = 0; i55 < loop_ub; i55++) {
        varargin_1->data[i55] = c->data[i55];
      }

      loop_ub = c->size[0];
      i55 = b_a->size[0];
      b_a->size[0] = loop_ub;
      emxEnsureCapacity_real_T(b_a, i55);
      for (i55 = 0; i55 < loop_ub; i55++) {
        b_a->data[i55] = c->data[i55 + c->size[0]];
      }

      interp1(varargin_1, b_a, ns2);
      baseline_correct(ns2, ns1);

      //  减背景算法-ns1为背景线
      loop_ub = c->size[0];
      i55 = varargin_1->size[0];
      varargin_1->size[0] = loop_ub;
      emxEnsureCapacity_real_T(varargin_1, i55);
      for (i55 = 0; i55 < loop_ub; i55++) {
        varargin_1->data[i55] = c->data[i55];
      }

      loop_ub = c->size[0];
      i55 = b_a->size[0];
      b_a->size[0] = loop_ub;
      emxEnsureCapacity_real_T(b_a, i55);
      for (i55 = 0; i55 < loop_ub; i55++) {
        b_a->data[i55] = c->data[i55 + c->size[0]];
      }

      interp1(varargin_1, b_a, ns2);
      for (i55 = 0; i55 < 1800; i55++) {
        b_ns2[i55] = ns2[i55] - ns1[i55];
      }

      b_geto_new(b_ns2, ns2);

      //  降噪算法
      n_tmp_tmp = 0;
      for (i = 0; i < 1800; i++) {
        y = rtIsNaN(ns2[i]);
        bv1[i] = y;
        if (y) {
          n_tmp_tmp++;
        }
      }

      loop_ub = 0;
      for (i = 0; i < 1800; i++) {
        if (bv1[i]) {
          b_tmp_data[loop_ub] = static_cast<short>((i + 1));
          loop_ub++;
        }
      }

      for (i55 = 0; i55 < n_tmp_tmp; i55++) {
        ns2[b_tmp_data[i55] - 1] = 0.0;
      }

      for (i = 0; i < 1800; i++) {
        if (ns2[i] < 0.0) {
          ns2[i] = 0.0;
        }
      }

      if (!rtIsNaN(ns2[0])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        exitg2 = false;
        while ((!exitg2) && (loop_ub < 1801)) {
          if (!rtIsNaN(ns2[loop_ub - 1])) {
            idx = loop_ub;
            exitg2 = true;
          } else {
            loop_ub++;
          }
        }
      }

      if (idx == 0) {
        ex = ns2[0];
      } else {
        ex = ns2[idx - 1];
        i55 = idx + 1;
        for (loop_ub = i55; loop_ub < 1801; loop_ub++) {
          left = ns2[loop_ub - 1];
          if (ex < left) {
            ex = left;
          }
        }
      }

      for (i = 0; i < 1800; i++) {
        ns1[i] = ns2[i] / ex;
      }

      for (i = 0; i < 1650; i++) {
        d[i] = ns2[i + 150] / ex;
      }

      if (!rtIsNaN(d[0])) {
        idx = 1;
      } else {
        idx = 0;
        loop_ub = 2;
        exitg2 = false;
        while ((!exitg2) && (loop_ub < 1651)) {
          if (!rtIsNaN(d[loop_ub - 1])) {
            idx = loop_ub;
            exitg2 = true;
          } else {
            loop_ub++;
          }
        }
      }

      if (idx == 0) {
        ex = ns1[150];
      } else {
        ex = ns1[idx + 149];
        i55 = idx + 1;
        for (loop_ub = i55; loop_ub < 1651; loop_ub++) {
          left = ns1[loop_ub + 149];
          if (ex < left) {
            ex = left;
          }
        }
      }

      d_size[0] = 1650;
      d_size[1] = 1;
      for (i55 = 0; i55 < 1650; i55++) {
        d_data[i55] = ns1[150 + i55] / ex;
      }
    }

    d_sum(d_data, d_size, tmp_data, tmp_size);
    loop_ub = tmp_size[0] * tmp_size[1];
    for (i55 = 0; i55 < loop_ub; i55++) {
      x_data[i55] = (tmp_data[i55] == 0.0);
    }

    y = (tmp_size[1] != 0);
    if (y) {
      loop_ub = 0;
      exitg2 = false;
      while ((!exitg2) && (loop_ub <= tmp_size[1] - 1)) {
        if (!x_data[0]) {
          y = false;
          exitg2 = true;
        } else {
          loop_ub = 1;
        }
      }
    }

    if (y) {
      d_size[0] = 0;
      d_size[1] = 0;
    }
  }

  emxFree_real_T(&b_a);
  emxFree_real_T(&varargin_1);
  emxFree_real_T(&c);
}

//
// File trailer for resolution_factor.cpp
//
// [EOF]
//
