//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mldivide.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
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
#include "mldivide.h"
#include "analyseMixture_emxutil.h"
#include "colon.h"
#include "xgeqp3.h"
#include "xtrsm.h"
#include "xgetrf.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *B
// Return Type  : void
//
void b_mldivide(const emxArray_real_T *A, emxArray_real_T *B)
{
  emxArray_real_T *b_A;
  emxArray_int32_T *r21;
  signed char unnamed_idx_0;
  int i73;
  int n;
  int minmn;
  double tau_data[4];
  int tau_size[1];
  int jpvt_data[4];
  int jpvt_size[2];
  int rankR;
  int maxmn;
  int ldap1;
  double tol;
  int u1;
  double Y_data[4];
  int j;
  int mn;
  int mmj_tmp;
  int jj;
  int i;
  int ix;
  double s;
  emxInit_real_T(&b_A, 2);
  emxInit_int32_T(&r21, 2);
  if ((A->size[0] == 0) || (B->size[0] == 0)) {
    unnamed_idx_0 = static_cast<signed char>(A->size[1]);
    i73 = B->size[0];
    B->size[0] = unnamed_idx_0;
    emxEnsureCapacity_real_T(B, i73);
    minmn = unnamed_idx_0;
    for (i73 = 0; i73 < minmn; i73++) {
      B->data[i73] = 0.0;
    }
  } else if (A->size[0] == A->size[1]) {
    n = A->size[1];
    i73 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i73);
    minmn = A->size[0] * A->size[1];
    for (i73 = 0; i73 < minmn; i73++) {
      b_A->data[i73] = A->data[i73];
    }

    minmn = A->size[1];
    eml_signed_integer_colon(minmn, r21);
    minmn = r21->size[0] * r21->size[1];
    for (i73 = 0; i73 < minmn; i73++) {
      jpvt_data[i73] = r21->data[i73];
    }

    ldap1 = A->size[1] + 1;
    maxmn = A->size[1] - 1;
    u1 = A->size[1];
    if (maxmn < u1) {
      u1 = maxmn;
    }

    for (j = 0; j < u1; j++) {
      mmj_tmp = n - j;
      maxmn = j * (n + 1);
      jj = j * ldap1;
      mn = maxmn + 2;
      if (mmj_tmp < 1) {
        minmn = -1;
      } else {
        minmn = 0;
        if (mmj_tmp > 1) {
          ix = maxmn;
          tol = std::abs(b_A->data[maxmn]);
          for (rankR = 2; rankR <= mmj_tmp; rankR++) {
            ix++;
            s = std::abs(b_A->data[ix]);
            if (s > tol) {
              minmn = rankR - 1;
              tol = s;
            }
          }
        }
      }

      if (b_A->data[jj + minmn] != 0.0) {
        if (minmn != 0) {
          minmn += j;
          jpvt_data[j] = minmn + 1;
          ix = j;
          for (rankR = 0; rankR < n; rankR++) {
            tol = b_A->data[ix];
            b_A->data[ix] = b_A->data[minmn];
            b_A->data[minmn] = tol;
            ix += n;
            minmn += n;
          }
        }

        i73 = jj + mmj_tmp;
        for (i = mn; i <= i73; i++) {
          b_A->data[i - 1] /= b_A->data[jj];
        }
      }

      minmn = maxmn + n;
      maxmn = jj + ldap1;
      for (mn = 0; mn <= mmj_tmp - 2; mn++) {
        tol = b_A->data[minmn];
        if (b_A->data[minmn] != 0.0) {
          ix = jj + 1;
          i73 = maxmn + 1;
          rankR = mmj_tmp + maxmn;
          for (i = i73; i < rankR; i++) {
            b_A->data[i - 1] += b_A->data[ix] * -tol;
            ix++;
          }
        }

        minmn += n;
        maxmn += n;
      }
    }

    i73 = A->size[1];
    for (minmn = 0; minmn <= i73 - 2; minmn++) {
      if (jpvt_data[minmn] != minmn + 1) {
        tol = B->data[minmn];
        B->data[minmn] = B->data[jpvt_data[minmn] - 1];
        B->data[jpvt_data[minmn] - 1] = tol;
      }
    }

    for (rankR = 0; rankR < n; rankR++) {
      minmn = n * rankR;
      if (B->data[rankR] != 0.0) {
        for (i = rankR + 2; i <= n; i++) {
          B->data[i - 1] -= B->data[rankR] * b_A->data[(i + minmn) - 1];
        }
      }
    }

    for (rankR = n; rankR >= 1; rankR--) {
      minmn = n * (rankR - 1);
      if (B->data[rankR - 1] != 0.0) {
        B->data[rankR - 1] /= b_A->data[(rankR + minmn) - 1];
        for (i = 0; i <= rankR - 2; i++) {
          B->data[i] -= B->data[rankR - 1] * b_A->data[i + minmn];
        }
      }
    }
  } else {
    i73 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i73);
    minmn = A->size[0] * A->size[1];
    for (i73 = 0; i73 < minmn; i73++) {
      b_A->data[i73] = A->data[i73];
    }

    xgeqp3(b_A, tau_data, tau_size, jpvt_data, jpvt_size);
    rankR = 0;
    if (b_A->size[0] < b_A->size[1]) {
      minmn = b_A->size[0];
      maxmn = b_A->size[1];
    } else {
      minmn = b_A->size[1];
      maxmn = b_A->size[0];
    }

    if (minmn > 0) {
      tol = 2.2204460492503131E-15 * static_cast<double>(maxmn);
      if (1.4901161193847656E-8 < tol) {
        tol = 1.4901161193847656E-8;
      }

      tol *= std::abs(b_A->data[0]);
      while ((rankR < minmn) && (!(std::abs(b_A->data[rankR + b_A->size[0] *
                rankR]) <= tol))) {
        rankR++;
      }
    }

    unnamed_idx_0 = static_cast<signed char>(b_A->size[1]);
    if (0 <= unnamed_idx_0 - 1) {
      memset(&Y_data[0], 0, (unsigned int)(unnamed_idx_0 * static_cast<int>
              (sizeof(double))));
    }

    minmn = b_A->size[0];
    maxmn = b_A->size[0];
    mn = b_A->size[1];
    if (maxmn < mn) {
      mn = maxmn;
    }

    for (j = 0; j < mn; j++) {
      if (tau_data[j] != 0.0) {
        tol = B->data[j];
        i73 = j + 2;
        for (i = i73; i <= minmn; i++) {
          tol += b_A->data[(i + b_A->size[0] * j) - 1] * B->data[i - 1];
        }

        tol *= tau_data[j];
        if (tol != 0.0) {
          B->data[j] -= tol;
          i73 = j + 2;
          for (i = i73; i <= minmn; i++) {
            B->data[i - 1] -= b_A->data[(i + b_A->size[0] * j) - 1] * tol;
          }
        }
      }
    }

    for (i = 0; i < rankR; i++) {
      Y_data[jpvt_data[i] - 1] = B->data[i];
    }

    for (j = rankR; j >= 1; j--) {
      minmn = jpvt_data[j - 1] - 1;
      Y_data[minmn] /= b_A->data[(j + b_A->size[0] * (j - 1)) - 1];
      for (i = 0; i <= j - 2; i++) {
        Y_data[jpvt_data[i] - 1] -= Y_data[minmn] * b_A->data[i + b_A->size[0] *
          (j - 1)];
      }
    }

    i73 = B->size[0];
    B->size[0] = unnamed_idx_0;
    emxEnsureCapacity_real_T(B, i73);
    minmn = unnamed_idx_0;
    for (i73 = 0; i73 < minmn; i73++) {
      B->data[i73] = Y_data[i73];
    }
  }

  emxFree_int32_T(&r21);
  emxFree_real_T(&b_A);
}

//
// Arguments    : const emxArray_real_T *A
//                const double B[1650]
//                emxArray_real_T *Y
// Return Type  : void
//
void mldivide(const emxArray_real_T *A, const double B[1650], emxArray_real_T *Y)
{
  emxArray_real_T *b_A;
  emxArray_real_T *c_A;
  int i59;
  int minmn;
  emxArray_int32_T *jpvt;
  int ipiv[1650];
  double tau_data[1650];
  int tau_size[1];
  double b_B[1650];
  int rankR;
  int maxmn;
  double tol;
  int i;
  if (A->size[1] == 0) {
    Y->size[0] = 0;
  } else if (1650 == A->size[1]) {
    emxInit_real_T(&c_A, 2);
    i59 = c_A->size[0] * c_A->size[1];
    c_A->size[0] = 1650;
    c_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(c_A, i59);
    minmn = A->size[0] * A->size[1];
    for (i59 = 0; i59 < minmn; i59++) {
      c_A->data[i59] = A->data[i59];
    }

    xgetrf(c_A, ipiv, &minmn);
    memcpy(&b_B[0], &B[0], 1650U * sizeof(double));
    for (minmn = 0; minmn < 1649; minmn++) {
      if (ipiv[minmn] != minmn + 1) {
        tol = b_B[minmn];
        b_B[minmn] = b_B[ipiv[minmn] - 1];
        b_B[ipiv[minmn] - 1] = tol;
      }
    }

    xtrsm(c_A, b_B);
    b_xtrsm(c_A, b_B);
    i59 = Y->size[0];
    Y->size[0] = 1650;
    emxEnsureCapacity_real_T(Y, i59);
    emxFree_real_T(&c_A);
    for (i59 = 0; i59 < 1650; i59++) {
      Y->data[i59] = b_B[i59];
    }
  } else {
    emxInit_real_T(&b_A, 2);
    i59 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = 1650;
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i59);
    minmn = A->size[0] * A->size[1];
    for (i59 = 0; i59 < minmn; i59++) {
      b_A->data[i59] = A->data[i59];
    }

    emxInit_int32_T(&jpvt, 2);
    b_xgeqp3(b_A, tau_data, tau_size, jpvt);
    rankR = 0;
    if (1650 < b_A->size[1]) {
      minmn = 1650;
      maxmn = b_A->size[1];
    } else {
      minmn = b_A->size[1];
      maxmn = 1650;
    }

    if (minmn > 0) {
      tol = 2.2204460492503131E-15 * static_cast<double>(maxmn);
      if (1.4901161193847656E-8 < tol) {
        tol = 1.4901161193847656E-8;
      }

      tol *= std::abs(b_A->data[0]);
      while ((rankR < minmn) && (!(std::abs(b_A->data[rankR + 1650 * rankR]) <=
               tol))) {
        rankR++;
      }
    }

    memcpy(&b_B[0], &B[0], 1650U * sizeof(double));
    minmn = b_A->size[1];
    i59 = Y->size[0];
    Y->size[0] = minmn;
    emxEnsureCapacity_real_T(Y, i59);
    for (i59 = 0; i59 < minmn; i59++) {
      Y->data[i59] = 0.0;
    }

    minmn = b_A->size[1];
    if (1650 < minmn) {
      minmn = 1650;
    }

    for (maxmn = 0; maxmn < minmn; maxmn++) {
      if (tau_data[maxmn] != 0.0) {
        tol = b_B[maxmn];
        i59 = maxmn + 2;
        for (i = i59; i < 1651; i++) {
          tol += b_A->data[(i + 1650 * maxmn) - 1] * b_B[i - 1];
        }

        tol *= tau_data[maxmn];
        if (tol != 0.0) {
          b_B[maxmn] -= tol;
          i59 = maxmn + 2;
          for (i = i59; i < 1651; i++) {
            b_B[i - 1] -= b_A->data[(i + 1650 * maxmn) - 1] * tol;
          }
        }
      }
    }

    for (i = 0; i < rankR; i++) {
      Y->data[jpvt->data[i] - 1] = b_B[i];
    }

    for (maxmn = rankR; maxmn >= 1; maxmn--) {
      i59 = 1650 * (maxmn - 1);
      Y->data[jpvt->data[maxmn - 1] - 1] /= b_A->data[(maxmn + i59) - 1];
      for (i = 0; i <= maxmn - 2; i++) {
        Y->data[jpvt->data[i] - 1] -= Y->data[jpvt->data[maxmn - 1] - 1] *
          b_A->data[i + i59];
      }
    }

    emxFree_int32_T(&jpvt);
    emxFree_real_T(&b_A);
  }
}

//
// File trailer for mldivide.cpp
//
// [EOF]
//
