//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: zl_lsqnonneg1.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <string.h>
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
#include "mldivide.h"
#include "abs.h"
#include "any1.h"
#include "norm.h"

// Function Definitions

//
// 设置允许的误差
// Arguments    : const emxArray_real_T *C
//                const double d[1650]
//                emxArray_real_T *x
// Return Type  : void
//
void zl_lsqnonneg1(const emxArray_real_T *C, const double d[1650],
                   emxArray_real_T *x)
{
  int inner;
  int n;
  emxArray_real_T *wz;
  double tol;
  int i;
  emxArray_boolean_T *P;
  emxArray_boolean_T *Z;
  double resid[1650];
  int k;
  int aoffset;
  emxArray_real_T *a_tmp;
  emxArray_real_T *w;
  double iter;
  double itmax;
  emxArray_real_T *z;
  emxArray_int32_T *r16;
  emxArray_int32_T *r17;
  emxArray_int32_T *r18;
  emxArray_int32_T *r19;
  emxArray_int32_T *r20;
  emxArray_boolean_T *b_w;
  emxArray_real_T *b_C;
  boolean_T exitg2;
  boolean_T exitg3;
  double ex;
  int exitg1;
  inner = C->size[1];
  if (1650 > inner) {
    inner = 1650;
  }

  if (C->size[1] == 0) {
    n = 0;
  } else {
    n = inner;
  }

  emxInit_real_T(&wz, 1);
  tol = 2.2204460492503131E-15 * b_norm(C) * static_cast<double>(n);

  //  1.049795313703106e-09  1*1 double
  //   tol = 0.5*10*eps*norm(C,1)*length(C);  % 1.049795313703106e-09  1*1 double 
  n = C->size[1];

  //   n = 40
  //  初始化n个0和Infs的向量(以后使用)
  //  40*1 全0
  i = wz->size[0];
  wz->size[0] = C->size[1];
  emxEnsureCapacity_real_T(wz, i);
  inner = C->size[1];
  for (i = 0; i < inner; i++) {
    wz->data[i] = 0.0;
  }

  emxInit_boolean_T(&P, 1);

  //  40*1 全0
  //  将一组非活动列初始化为空
  i = P->size[0];
  P->size[0] = C->size[1];
  emxEnsureCapacity_boolean_T(P, i);
  inner = C->size[1];
  for (i = 0; i < inner; i++) {
    P->data[i] = false;
  }

  emxInit_boolean_T(&Z, 1);

  //  40*1 logical
  //  将一组活动列初始化为1，初始点为零
  i = Z->size[0];
  Z->size[0] = C->size[1];
  emxEnsureCapacity_boolean_T(Z, i);
  inner = C->size[1];
  for (i = 0; i < inner; i++) {
    Z->data[i] = true;
  }

  //  Z 40*1 全1
  i = x->size[0];
  x->size[0] = C->size[1];
  emxEnsureCapacity_real_T(x, i);
  inner = C->size[1];
  for (i = 0; i < inner; i++) {
    x->data[i] = 0.0;
  }

  //  x 40*1 全0
  //   resid = zeros(size(C,1),1);
  if (C->size[1] == 1) {
    inner = C->size[1];
    for (i = 0; i < 1650; i++) {
      resid[i] = 0.0;
      for (aoffset = 0; aoffset < inner; aoffset++) {
        resid[i] += C->data[i + 1650 * aoffset] * 0.0;
      }
    }
  } else {
    inner = C->size[1];
    memset(&resid[0], 0, 1650U * sizeof(double));
    for (k = 0; k < inner; k++) {
      aoffset = k * 1650;
      for (i = 0; i < 1650; i++) {
        resid[i] += 0.0 * C->data[aoffset + i];
      }
    }
  }

  for (i = 0; i < 1650; i++) {
    resid[i] = d[i] - resid[i];
  }

  emxInit_real_T(&a_tmp, 2);

  //  初始化残差为 d
  i = a_tmp->size[0] * a_tmp->size[1];
  a_tmp->size[0] = C->size[1];
  a_tmp->size[1] = 1650;
  emxEnsureCapacity_real_T(a_tmp, i);
  for (i = 0; i < 1650; i++) {
    inner = C->size[1];
    for (aoffset = 0; aoffset < inner; aoffset++) {
      a_tmp->data[aoffset + a_tmp->size[0] * i] = C->data[i + 1650 * aoffset];
    }
  }

  emxInit_real_T(&w, 1);
  inner = a_tmp->size[0];
  i = w->size[0];
  w->size[0] = a_tmp->size[0];
  emxEnsureCapacity_real_T(w, i);
  for (i = 0; i < inner; i++) {
    w->data[i] = 0.0;
  }

  for (k = 0; k < 1650; k++) {
    aoffset = k * inner;
    for (i = 0; i < inner; i++) {
      w->data[i] += resid[k] * a_tmp->data[aoffset + i];
    }
  }

  //
  //  建立迭代准则
  iter = 0.0;
  itmax = 3.0 * static_cast<double>(C->size[1]);

  //  最大迭代次数为120（3*n）
  //  退出循环的标识
  emxInit_real_T(&z, 1);
  emxInit_int32_T(&r16, 1);
  emxInit_int32_T(&r17, 1);
  emxInit_int32_T(&r18, 1);
  emxInit_int32_T(&r19, 1);
  emxInit_int32_T(&r20, 1);
  emxInit_boolean_T(&b_w, 1);
  emxInit_real_T(&b_C, 2);
  exitg2 = false;
  while ((!exitg2) && b_any(Z)) {
    k = Z->size[0] - 1;
    aoffset = 0;
    for (i = 0; i <= k; i++) {
      if (Z->data[i]) {
        aoffset++;
      }
    }

    i = r16->size[0];
    r16->size[0] = aoffset;
    emxEnsureCapacity_int32_T(r16, i);
    inner = 0;
    for (i = 0; i <= k; i++) {
      if (Z->data[i]) {
        r16->data[inner] = i + 1;
        inner++;
      }
    }

    i = b_w->size[0];
    b_w->size[0] = r16->size[0];
    emxEnsureCapacity_boolean_T(b_w, i);
    inner = r16->size[0];
    for (i = 0; i < inner; i++) {
      b_w->data[i] = (w->data[r16->data[i] - 1] > tol);
    }

    if (b_any(b_w)) {
      //  Z中有1,并且w(Z)中存在大于最大允许误差的值
      //  外迭代器次数自增1
      i = z->size[0];
      z->size[0] = n;
      emxEnsureCapacity_real_T(z, i);
      for (i = 0; i < n; i++) {
        z->data[i] = 0.0;
      }

      //  复位中间解z
      k = P->size[0];
      for (i = 0; i < k; i++) {
        if (P->data[i]) {
          wz->data[i] = rtMinusInf;
        }
      }

      k = Z->size[0];
      for (i = 0; i < k; i++) {
        if (Z->data[i]) {
          wz->data[i] = w->data[i];
        }
      }

      //  Find variable with largest Lagrange multiplier
      inner = wz->size[0];
      if (wz->size[0] <= 2) {
        if (wz->size[0] == 1) {
          aoffset = 1;
        } else if ((wz->data[0] < wz->data[1]) || (rtIsNaN(wz->data[0]) &&
                    (!rtIsNaN(wz->data[1])))) {
          aoffset = 2;
        } else {
          aoffset = 1;
        }
      } else {
        if (!rtIsNaN(wz->data[0])) {
          aoffset = 1;
        } else {
          aoffset = 0;
          k = 2;
          exitg3 = false;
          while ((!exitg3) && (k <= wz->size[0])) {
            if (!rtIsNaN(wz->data[k - 1])) {
              aoffset = k;
              exitg3 = true;
            } else {
              k++;
            }
          }
        }

        if (aoffset == 0) {
          aoffset = 1;
        } else {
          ex = wz->data[aoffset - 1];
          i = aoffset + 1;
          for (k = i; k <= inner; k++) {
            if (ex < wz->data[k - 1]) {
              ex = wz->data[k - 1];
              aoffset = k;
            }
          }
        }
      }

      //  Move variable t from zero set to positive set
      P->data[aoffset - 1] = true;
      Z->data[aoffset - 1] = false;

      //  Compute intermediate solution using only variables in positive set
      k = P->size[0] - 1;
      aoffset = 0;
      for (i = 0; i <= k; i++) {
        if (P->data[i]) {
          aoffset++;
        }
      }

      i = r17->size[0];
      r17->size[0] = aoffset;
      emxEnsureCapacity_int32_T(r17, i);
      inner = 0;
      for (i = 0; i <= k; i++) {
        if (P->data[i]) {
          r17->data[inner] = i + 1;
          inner++;
        }
      }

      i = b_C->size[0] * b_C->size[1];
      b_C->size[0] = 1650;
      b_C->size[1] = r17->size[0];
      emxEnsureCapacity_real_T(b_C, i);
      inner = r17->size[0];
      for (i = 0; i < inner; i++) {
        for (aoffset = 0; aoffset < 1650; aoffset++) {
          b_C->data[aoffset + 1650 * i] = C->data[aoffset + 1650 * (r17->data[i]
            - 1)];
        }
      }

      mldivide(b_C, d, w);
      k = P->size[0];
      inner = 0;
      for (i = 0; i < k; i++) {
        if (P->data[i]) {
          z->data[i] = w->data[inner];
          inner++;
        }
      }

      do {
        exitg1 = 0;
        k = P->size[0] - 1;
        aoffset = 0;
        for (i = 0; i <= k; i++) {
          if (P->data[i]) {
            aoffset++;
          }
        }

        i = r18->size[0];
        r18->size[0] = aoffset;
        emxEnsureCapacity_int32_T(r18, i);
        inner = 0;
        for (i = 0; i <= k; i++) {
          if (P->data[i]) {
            r18->data[inner] = i + 1;
            inner++;
          }
        }

        i = b_w->size[0];
        b_w->size[0] = r18->size[0];
        emxEnsureCapacity_boolean_T(b_w, i);
        inner = r18->size[0];
        for (i = 0; i < inner; i++) {
          b_w->data[i] = (z->data[r18->data[i] - 1] <= 0.0);
        }

        if (b_any(b_w)) {
          //         outeriter
          iter++;
          if (iter > itmax) {
            i = x->size[0];
            x->size[0] = z->size[0];
            emxEnsureCapacity_real_T(x, i);
            inner = z->size[0];
            for (i = 0; i < inner; i++) {
              x->data[i] = z->data[i];
            }

            exitg1 = 1;
          } else {
            //  Find indices where intermediate solution z is approximately negative 
            i = b_w->size[0];
            b_w->size[0] = z->size[0];
            emxEnsureCapacity_boolean_T(b_w, i);
            inner = z->size[0];
            for (i = 0; i < inner; i++) {
              b_w->data[i] = (z->data[i] <= 0.0);
            }

            //  Choose new x subject to keeping new x nonnegative
            k = b_w->size[0] - 1;
            aoffset = 0;
            for (i = 0; i <= k; i++) {
              if (b_w->data[i] && P->data[i]) {
                aoffset++;
              }
            }

            i = r19->size[0];
            r19->size[0] = aoffset;
            emxEnsureCapacity_int32_T(r19, i);
            inner = 0;
            for (i = 0; i <= k; i++) {
              if (b_w->data[i] && P->data[i]) {
                r19->data[inner] = i + 1;
                inner++;
              }
            }

            i = w->size[0];
            w->size[0] = r19->size[0];
            emxEnsureCapacity_real_T(w, i);
            inner = r19->size[0];
            for (i = 0; i < inner; i++) {
              w->data[i] = x->data[r19->data[i] - 1] / (x->data[r19->data[i] - 1]
                - z->data[r19->data[i] - 1]);
            }

            inner = w->size[0];
            if (w->size[0] <= 2) {
              if (w->size[0] == 1) {
                ex = w->data[0];
              } else if ((w->data[0] > w->data[1]) || (rtIsNaN(w->data[0]) &&
                          (!rtIsNaN(w->data[1])))) {
                ex = w->data[1];
              } else {
                ex = w->data[0];
              }
            } else {
              if (!rtIsNaN(w->data[0])) {
                aoffset = 1;
              } else {
                aoffset = 0;
                k = 2;
                exitg3 = false;
                while ((!exitg3) && (k <= w->size[0])) {
                  if (!rtIsNaN(w->data[k - 1])) {
                    aoffset = k;
                    exitg3 = true;
                  } else {
                    k++;
                  }
                }
              }

              if (aoffset == 0) {
                ex = w->data[0];
              } else {
                ex = w->data[aoffset - 1];
                i = aoffset + 1;
                for (k = i; k <= inner; k++) {
                  if (ex > w->data[k - 1]) {
                    ex = w->data[k - 1];
                  }
                }
              }
            }

            i = x->size[0];
            emxEnsureCapacity_real_T(x, i);
            inner = x->size[0];
            for (i = 0; i < inner; i++) {
              x->data[i] += ex * (z->data[i] - x->data[i]);
            }

            //  Reset Z and P given intermediate values of x
            b_abs(x, w);
            i = Z->size[0];
            Z->size[0] = w->size[0];
            emxEnsureCapacity_boolean_T(Z, i);
            inner = w->size[0];
            for (i = 0; i < inner; i++) {
              Z->data[i] = (((w->data[i] < tol) && P->data[i]) || Z->data[i]);
            }

            i = P->size[0];
            P->size[0] = Z->size[0];
            emxEnsureCapacity_boolean_T(P, i);
            inner = Z->size[0];
            for (i = 0; i < inner; i++) {
              P->data[i] = !Z->data[i];
            }

            i = z->size[0];
            z->size[0] = n;
            emxEnsureCapacity_real_T(z, i);
            for (i = 0; i < n; i++) {
              z->data[i] = 0.0;
            }

            //  Reset z
            k = P->size[0] - 1;
            aoffset = 0;
            for (i = 0; i <= k; i++) {
              if (P->data[i]) {
                aoffset++;
              }
            }

            i = r20->size[0];
            r20->size[0] = aoffset;
            emxEnsureCapacity_int32_T(r20, i);
            inner = 0;
            for (i = 0; i <= k; i++) {
              if (P->data[i]) {
                r20->data[inner] = i + 1;
                inner++;
              }
            }

            i = b_C->size[0] * b_C->size[1];
            b_C->size[0] = 1650;
            b_C->size[1] = r20->size[0];
            emxEnsureCapacity_real_T(b_C, i);
            inner = r20->size[0];
            for (i = 0; i < inner; i++) {
              for (aoffset = 0; aoffset < 1650; aoffset++) {
                b_C->data[aoffset + 1650 * i] = C->data[aoffset + 1650 *
                  (r20->data[i] - 1)];
              }
            }

            mldivide(b_C, d, w);
            k = P->size[0];
            inner = 0;
            for (i = 0; i < k; i++) {
              if (P->data[i]) {
                z->data[i] = w->data[inner];
                inner++;
              }
            }

            //  Re-solve for z
          }
        } else {
          i = x->size[0];
          x->size[0] = z->size[0];
          emxEnsureCapacity_real_T(x, i);
          inner = z->size[0];
          for (i = 0; i < inner; i++) {
            x->data[i] = z->data[i];
          }

          if ((C->size[1] == 1) || (z->size[0] == 1)) {
            inner = C->size[1];
            for (i = 0; i < 1650; i++) {
              resid[i] = 0.0;
              for (aoffset = 0; aoffset < inner; aoffset++) {
                resid[i] += C->data[i + 1650 * aoffset] * z->data[aoffset];
              }
            }
          } else {
            inner = C->size[1];
            memset(&resid[0], 0, 1650U * sizeof(double));
            for (k = 0; k < inner; k++) {
              aoffset = k * 1650;
              for (i = 0; i < 1650; i++) {
                resid[i] += z->data[k] * C->data[aoffset + i];
              }
            }
          }

          for (i = 0; i < 1650; i++) {
            resid[i] = d[i] - resid[i];
          }

          inner = a_tmp->size[0];
          i = w->size[0];
          w->size[0] = a_tmp->size[0];
          emxEnsureCapacity_real_T(w, i);
          for (i = 0; i < inner; i++) {
            w->data[i] = 0.0;
          }

          for (k = 0; k < 1650; k++) {
            aoffset = k * inner;
            for (i = 0; i < inner; i++) {
              w->data[i] += resid[k] * a_tmp->data[aoffset + i];
            }
          }

          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    } else {
      exitg2 = true;
    }
  }

  emxFree_real_T(&b_C);
  emxFree_boolean_T(&b_w);
  emxFree_real_T(&a_tmp);
  emxFree_int32_T(&r20);
  emxFree_int32_T(&r19);
  emxFree_int32_T(&r18);
  emxFree_int32_T(&r17);
  emxFree_int32_T(&r16);
  emxFree_real_T(&z);
  emxFree_real_T(&w);
  emxFree_boolean_T(&Z);
  emxFree_boolean_T(&P);
  emxFree_real_T(&wz);
}

//
// File trailer for zl_lsqnonneg1.cpp
//
// [EOF]
//
