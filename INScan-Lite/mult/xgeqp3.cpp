//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgeqp3.cpp
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
#include "xgeqp3.h"
#include "xnrm2.h"
#include "xscal.h"
#include "qr.h"
#include "ixamax.h"
#include "analyseMixture_emxutil.h"
#include "colon.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *A
//                double tau_data[]
//                int tau_size[1]
//                emxArray_int32_T *jpvt
// Return Type  : void
//
void b_xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1],
              emxArray_int32_T *jpvt)
{
  int n;
  int mn;
  emxArray_real_T *work;
  int kend;
  int i91;
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  int k;
  int nmi;
  int i;
  double temp;
  double scale;
  int b_i;
  int ip1;
  int pvt;
  int iy;
  int i_i;
  double absxk;
  double t;
  int ix;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  int i92;
  n = A->size[1];
  mn = A->size[1];
  if (1650 < mn) {
    mn = 1650;
  }

  tau_size[0] = mn;
  eml_signed_integer_colon(A->size[1], jpvt);
  if (A->size[1] != 0) {
    emxInit_real_T(&work, 1);
    kend = A->size[1];
    i91 = work->size[0];
    work->size[0] = kend;
    emxEnsureCapacity_real_T(work, i91);
    for (i91 = 0; i91 < kend; i91++) {
      work->data[i91] = 0.0;
    }

    emxInit_real_T(&vn1, 1);
    emxInit_real_T(&vn2, 1);
    kend = A->size[1];
    i91 = vn1->size[0];
    vn1->size[0] = kend;
    emxEnsureCapacity_real_T(vn1, i91);
    i91 = vn2->size[0];
    vn2->size[0] = vn1->size[0];
    emxEnsureCapacity_real_T(vn2, i91);
    k = 1;
    for (nmi = 0; nmi < n; nmi++) {
      temp = 0.0;
      scale = 3.3121686421112381E-170;
      kend = k + 1649;
      for (pvt = k; pvt <= kend; pvt++) {
        absxk = std::abs(A->data[pvt - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          temp = 1.0 + temp * t * t;
          scale = absxk;
        } else {
          t = absxk / scale;
          temp += t * t;
        }
      }

      vn1->data[nmi] = scale * std::sqrt(temp);
      vn2->data[nmi] = vn1->data[nmi];
      k += 1650;
    }

    for (i = 0; i < mn; i++) {
      b_i = 1 + i;
      ip1 = b_i + 1;
      iy = (b_i - 1) * 1650;
      i_i = (b_i + iy) - 1;
      nmi = n - b_i;
      pvt = (b_i + ixamax(1 + nmi, vn1, b_i)) - 2;
      if (pvt + 1 != b_i) {
        ix = 1650 * pvt;
        for (k = 0; k < 1650; k++) {
          temp = A->data[ix];
          A->data[ix] = A->data[iy];
          A->data[iy] = temp;
          ix++;
          iy++;
        }

        kend = jpvt->data[pvt];
        jpvt->data[pvt] = jpvt->data[b_i - 1];
        jpvt->data[b_i - 1] = kend;
        vn1->data[pvt] = vn1->data[b_i - 1];
        vn2->data[pvt] = vn2->data[b_i - 1];
      }

      if (b_i < 1650) {
        absxk = A->data[i_i];
        tau_data[b_i - 1] = 0.0;
        temp = b_xnrm2(1650 - b_i, A, i_i + 2);
        if (temp != 0.0) {
          scale = rt_hypotd_snf(A->data[i_i], temp);
          if (A->data[i_i] >= 0.0) {
            scale = -scale;
          }

          if (std::abs(scale) < 1.0020841800044864E-292) {
            kend = -1;
            do {
              kend++;
              xscal(1650 - b_i, 9.9792015476736E+291, A, i_i + 2);
              scale *= 9.9792015476736E+291;
              absxk *= 9.9792015476736E+291;
            } while (!(std::abs(scale) >= 1.0020841800044864E-292));

            scale = rt_hypotd_snf(absxk, b_xnrm2(1650 - b_i, A, i_i + 2));
            if (absxk >= 0.0) {
              scale = -scale;
            }

            tau_data[b_i - 1] = (scale - absxk) / scale;
            xscal(1650 - b_i, 1.0 / (absxk - scale), A, i_i + 2);
            for (k = 0; k <= kend; k++) {
              scale *= 1.0020841800044864E-292;
            }

            absxk = scale;
          } else {
            tau_data[b_i - 1] = (scale - A->data[i_i]) / scale;
            temp = 1.0 / (A->data[i_i] - scale);
            xscal(1650 - b_i, temp, A, i_i + 2);
            absxk = scale;
          }
        }

        A->data[i_i] = absxk;
      } else {
        tau_data[1649] = 0.0;
      }

      if (b_i < n) {
        absxk = A->data[i_i];
        A->data[i_i] = 1.0;
        k = b_i + b_i * 1650;
        if (tau_data[b_i - 1] != 0.0) {
          lastv = 1651 - b_i;
          kend = (i_i - b_i) + 1650;
          while ((lastv > 0) && (A->data[kend] == 0.0)) {
            lastv--;
            kend--;
          }

          lastc = nmi - 1;
          exitg2 = false;
          while ((!exitg2) && (lastc + 1 > 0)) {
            kend = k + lastc * 1650;
            pvt = kend;
            do {
              exitg1 = 0;
              if (pvt <= (kend + lastv) - 1) {
                if (A->data[pvt - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  pvt++;
                }
              } else {
                lastc--;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = 0;
          lastc = -1;
        }

        if (lastv > 0) {
          if (lastc + 1 != 0) {
            for (iy = 0; iy <= lastc; iy++) {
              work->data[iy] = 0.0;
            }

            iy = 0;
            i91 = k + 1650 * lastc;
            for (kend = k; kend <= i91; kend += 1650) {
              ix = i_i;
              temp = 0.0;
              i92 = (kend + lastv) - 1;
              for (pvt = kend; pvt <= i92; pvt++) {
                temp += A->data[pvt - 1] * A->data[ix];
                ix++;
              }

              work->data[iy] += temp;
              iy++;
            }
          }

          if (!(-tau_data[b_i - 1] == 0.0)) {
            kend = k - 1;
            pvt = 0;
            for (nmi = 0; nmi <= lastc; nmi++) {
              if (work->data[pvt] != 0.0) {
                temp = work->data[pvt] * -tau_data[b_i - 1];
                ix = i_i;
                i91 = kend + 1;
                i92 = lastv + kend;
                for (k = i91; k <= i92; k++) {
                  A->data[k - 1] += A->data[ix] * temp;
                  ix++;
                }
              }

              pvt++;
              kend += 1650;
            }
          }
        }

        A->data[i_i] = absxk;
      }

      for (nmi = ip1; nmi <= n; nmi++) {
        if (vn1->data[nmi - 1] != 0.0) {
          kend = b_i + 1650 * (nmi - 1);
          temp = std::abs(A->data[kend - 1]) / vn1->data[nmi - 1];
          temp = 1.0 - temp * temp;
          if (temp < 0.0) {
            temp = 0.0;
          }

          scale = vn1->data[nmi - 1] / vn2->data[nmi - 1];
          scale = temp * (scale * scale);
          if (scale <= 1.4901161193847656E-8) {
            if (b_i < 1650) {
              vn1->data[nmi - 1] = b_xnrm2(1650 - b_i, A, kend + 1);
              vn2->data[nmi - 1] = vn1->data[nmi - 1];
            } else {
              vn1->data[nmi - 1] = 0.0;
              vn2->data[nmi - 1] = 0.0;
            }
          } else {
            vn1->data[nmi - 1] *= std::sqrt(temp);
          }
        }
      }
    }

    emxFree_real_T(&vn2);
    emxFree_real_T(&vn1);
    emxFree_real_T(&work);
  }
}

//
// Arguments    : emxArray_real_T *A
//                double tau_data[]
//                int tau_size[1]
//                int jpvt_data[]
//                int jpvt_size[2]
// Return Type  : void
//
void xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1], int
            jpvt_data[], int jpvt_size[2])
{
  int m;
  int n;
  int itemp;
  int mn;
  emxArray_int32_T *r22;
  int i74;
  signed char unnamed_idx_0;
  double work_data[4];
  int vn1_size[1];
  int k;
  int nmi;
  int i;
  double vn1_data[4];
  double vn2_data[4];
  int ip1;
  int iy;
  int i_i;
  int mmi;
  emxArray_real_T b_vn1_data;
  int pvt;
  int ix;
  double atmp;
  double temp;
  double beta1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  int i75;
  m = A->size[0];
  n = A->size[1];
  itemp = A->size[0];
  mn = A->size[1];
  if (itemp < mn) {
    mn = itemp;
  }

  emxInit_int32_T(&r22, 2);
  tau_size[0] = mn;
  eml_signed_integer_colon(A->size[1], r22);
  jpvt_size[0] = 1;
  jpvt_size[1] = r22->size[1];
  itemp = r22->size[0] * r22->size[1];
  for (i74 = 0; i74 < itemp; i74++) {
    jpvt_data[i74] = r22->data[i74];
  }

  emxFree_int32_T(&r22);
  if (A->size[0] != 0) {
    unnamed_idx_0 = static_cast<signed char>(A->size[1]);
    if (0 <= unnamed_idx_0 - 1) {
      memset(&work_data[0], 0, (unsigned int)(unnamed_idx_0 * static_cast<int>
              (sizeof(double))));
    }

    vn1_size[0] = unnamed_idx_0;
    k = 1;
    for (nmi = 0; nmi < n; nmi++) {
      vn1_data[nmi] = b_xnrm2(m, A, k);
      vn2_data[nmi] = vn1_data[nmi];
      k += m;
    }

    for (i = 0; i < mn; i++) {
      ip1 = i + 2;
      iy = i * m;
      i_i = i + iy;
      nmi = n - i;
      mmi = (m - i) - 1;
      b_vn1_data.data = &vn1_data[0];
      b_vn1_data.size = &vn1_size[0];
      b_vn1_data.allocatedSize = 4;
      b_vn1_data.numDimensions = 1;
      b_vn1_data.canFreeData = false;
      pvt = (i + ixamax(nmi, &b_vn1_data, i + 1)) - 1;
      if (pvt + 1 != i + 1) {
        ix = m * pvt;
        for (k = 0; k < m; k++) {
          temp = A->data[ix];
          A->data[ix] = A->data[iy];
          A->data[iy] = temp;
          ix++;
          iy++;
        }

        itemp = jpvt_data[pvt];
        jpvt_data[pvt] = jpvt_data[i];
        jpvt_data[i] = itemp;
        vn1_data[pvt] = vn1_data[i];
        vn2_data[pvt] = vn2_data[i];
      }

      if (i + 1 < m) {
        atmp = A->data[i_i];
        tau_data[i] = 0.0;
        if (1 + mmi > 0) {
          temp = b_xnrm2(mmi, A, i_i + 2);
          if (temp != 0.0) {
            beta1 = rt_hypotd_snf(A->data[i_i], temp);
            if (A->data[i_i] >= 0.0) {
              beta1 = -beta1;
            }

            if (std::abs(beta1) < 1.0020841800044864E-292) {
              itemp = -1;
              do {
                itemp++;
                xscal(mmi, 9.9792015476736E+291, A, i_i + 2);
                beta1 *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

              beta1 = rt_hypotd_snf(atmp, b_xnrm2(mmi, A, i_i + 2));
              if (atmp >= 0.0) {
                beta1 = -beta1;
              }

              tau_data[i] = (beta1 - atmp) / beta1;
              xscal(mmi, 1.0 / (atmp - beta1), A, i_i + 2);
              for (k = 0; k <= itemp; k++) {
                beta1 *= 1.0020841800044864E-292;
              }

              atmp = beta1;
            } else {
              tau_data[i] = (beta1 - A->data[i_i]) / beta1;
              temp = 1.0 / (A->data[i_i] - beta1);
              xscal(mmi, temp, A, i_i + 2);
              atmp = beta1;
            }
          }
        }

        A->data[i_i] = atmp;
      } else {
        tau_data[i] = 0.0;
      }

      if (i + 1 < n) {
        atmp = A->data[i_i];
        A->data[i_i] = 1.0;
        k = (i + (i + 1) * m) + 1;
        if (tau_data[i] != 0.0) {
          lastv = mmi + 1;
          itemp = i_i + mmi;
          while ((lastv > 0) && (A->data[itemp] == 0.0)) {
            lastv--;
            itemp--;
          }

          lastc = nmi - 2;
          exitg2 = false;
          while ((!exitg2) && (lastc + 1 > 0)) {
            itemp = k + lastc * m;
            pvt = itemp;
            do {
              exitg1 = 0;
              if (pvt <= (itemp + lastv) - 1) {
                if (A->data[pvt - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  pvt++;
                }
              } else {
                lastc--;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = 0;
          lastc = -1;
        }

        if (lastv > 0) {
          if (lastc + 1 != 0) {
            if (0 <= lastc) {
              memset(&work_data[0], 0, (unsigned int)((lastc + 1) * static_cast<
                      int>(sizeof(double))));
            }

            iy = 0;
            i74 = k + m * lastc;
            for (itemp = k; m < 0 ? itemp >= i74 : itemp <= i74; itemp += m) {
              ix = i_i;
              temp = 0.0;
              i75 = (itemp + lastv) - 1;
              for (pvt = itemp; pvt <= i75; pvt++) {
                temp += A->data[pvt - 1] * A->data[ix];
                ix++;
              }

              work_data[iy] += temp;
              iy++;
            }
          }

          if (!(-tau_data[i] == 0.0)) {
            itemp = k - 1;
            pvt = 0;
            for (nmi = 0; nmi <= lastc; nmi++) {
              if (work_data[pvt] != 0.0) {
                temp = work_data[pvt] * -tau_data[i];
                ix = i_i;
                i74 = itemp + 1;
                i75 = lastv + itemp;
                for (k = i74; k <= i75; k++) {
                  A->data[k - 1] += A->data[ix] * temp;
                  ix++;
                }
              }

              pvt++;
              itemp += m;
            }
          }
        }

        A->data[i_i] = atmp;
      }

      for (nmi = ip1; nmi <= n; nmi++) {
        if (vn1_data[nmi - 1] != 0.0) {
          temp = std::abs(A->data[i + A->size[0] * (nmi - 1)]) / vn1_data[nmi -
            1];
          temp = 1.0 - temp * temp;
          if (temp < 0.0) {
            temp = 0.0;
          }

          beta1 = vn1_data[nmi - 1] / vn2_data[nmi - 1];
          beta1 = temp * (beta1 * beta1);
          if (beta1 <= 1.4901161193847656E-8) {
            if (i + 1 < m) {
              vn1_data[nmi - 1] = b_xnrm2(mmi, A, (i + m * (nmi - 1)) + 2);
              vn2_data[nmi - 1] = vn1_data[nmi - 1];
            } else {
              vn1_data[nmi - 1] = 0.0;
              vn2_data[nmi - 1] = 0.0;
            }
          } else {
            vn1_data[nmi - 1] *= std::sqrt(temp);
          }
        }
      }
    }
  }
}

//
// File trailer for xgeqp3.cpp
//
// [EOF]
//
