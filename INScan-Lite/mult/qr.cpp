//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: qr.cpp
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
#include "qr.h"
#include "xger.h"
#include "xgemv.h"
#include "xnrm2.h"
#include "analyseMixture_rtwutil.h"

// Function Definitions

//
// Arguments    : double A[28]
//                double R[16]
// Return Type  : void
//
void b_qr(double A[28], double R[16])
{
  double work[4];
  double atmp;
  double tau[4];
  double xnorm;
  double beta1;
  int knt;
  int lastv;
  int i;
  boolean_T exitg2;
  int ia;
  int exitg1;
  work[0] = 0.0;
  work[1] = 0.0;
  work[2] = 0.0;
  work[3] = 0.0;
  atmp = A[0];
  tau[0] = 0.0;
  xnorm = c_xnrm2(6, A, 2);
  if (xnorm != 0.0) {
    beta1 = rt_hypotd_snf(A[0], xnorm);
    if (A[0] >= 0.0) {
      beta1 = -beta1;
    }

    if (std::abs(beta1) < 1.0020841800044864E-292) {
      knt = -1;
      do {
        knt++;
        for (i = 2; i < 8; i++) {
          A[i - 1] *= 9.9792015476736E+291;
        }

        beta1 *= 9.9792015476736E+291;
        atmp *= 9.9792015476736E+291;
      } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

      beta1 = rt_hypotd_snf(atmp, c_xnrm2(6, A, 2));
      if (atmp >= 0.0) {
        beta1 = -beta1;
      }

      tau[0] = (beta1 - atmp) / beta1;
      xnorm = 1.0 / (atmp - beta1);
      for (i = 2; i < 8; i++) {
        A[i - 1] *= xnorm;
      }

      for (i = 0; i <= knt; i++) {
        beta1 *= 1.0020841800044864E-292;
      }

      atmp = beta1;
    } else {
      tau[0] = (beta1 - A[0]) / beta1;
      xnorm = 1.0 / (A[0] - beta1);
      for (i = 2; i < 8; i++) {
        A[i - 1] *= xnorm;
      }

      atmp = beta1;
    }
  }

  A[0] = atmp;
  atmp = A[0];
  A[0] = 1.0;
  if (tau[0] != 0.0) {
    lastv = 7;
    i = 6;
    while ((lastv > 0) && (A[i] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 3;
    exitg2 = false;
    while ((!exitg2) && (knt > 0)) {
      i = 8 + (knt - 1) * 7;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (A[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          knt--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 8, A, 1, work);
    b_xger(lastv, knt, -tau[0], 1, work, A, 8);
  }

  A[0] = atmp;
  atmp = A[8];
  tau[1] = 0.0;
  xnorm = c_xnrm2(5, A, 10);
  if (xnorm != 0.0) {
    beta1 = rt_hypotd_snf(A[8], xnorm);
    if (A[8] >= 0.0) {
      beta1 = -beta1;
    }

    if (std::abs(beta1) < 1.0020841800044864E-292) {
      knt = -1;
      do {
        knt++;
        for (i = 10; i < 15; i++) {
          A[i - 1] *= 9.9792015476736E+291;
        }

        beta1 *= 9.9792015476736E+291;
        atmp *= 9.9792015476736E+291;
      } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

      beta1 = rt_hypotd_snf(atmp, c_xnrm2(5, A, 10));
      if (atmp >= 0.0) {
        beta1 = -beta1;
      }

      tau[1] = (beta1 - atmp) / beta1;
      xnorm = 1.0 / (atmp - beta1);
      for (i = 10; i < 15; i++) {
        A[i - 1] *= xnorm;
      }

      for (i = 0; i <= knt; i++) {
        beta1 *= 1.0020841800044864E-292;
      }

      atmp = beta1;
    } else {
      tau[1] = (beta1 - A[8]) / beta1;
      xnorm = 1.0 / (A[8] - beta1);
      for (i = 10; i < 15; i++) {
        A[i - 1] *= xnorm;
      }

      atmp = beta1;
    }
  }

  A[8] = atmp;
  atmp = A[8];
  A[8] = 1.0;
  if (tau[1] != 0.0) {
    lastv = 6;
    i = 13;
    while ((lastv > 0) && (A[i] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 2;
    exitg2 = false;
    while ((!exitg2) && (knt > 0)) {
      i = 16 + (knt - 1) * 7;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (A[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          knt--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 16, A, 9, work);
    b_xger(lastv, knt, -tau[1], 9, work, A, 16);
  }

  A[8] = atmp;
  atmp = A[16];
  tau[2] = 0.0;
  xnorm = c_xnrm2(4, A, 18);
  if (xnorm != 0.0) {
    beta1 = rt_hypotd_snf(A[16], xnorm);
    if (A[16] >= 0.0) {
      beta1 = -beta1;
    }

    if (std::abs(beta1) < 1.0020841800044864E-292) {
      knt = -1;
      do {
        knt++;
        for (i = 18; i < 22; i++) {
          A[i - 1] *= 9.9792015476736E+291;
        }

        beta1 *= 9.9792015476736E+291;
        atmp *= 9.9792015476736E+291;
      } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

      beta1 = rt_hypotd_snf(atmp, c_xnrm2(4, A, 18));
      if (atmp >= 0.0) {
        beta1 = -beta1;
      }

      tau[2] = (beta1 - atmp) / beta1;
      xnorm = 1.0 / (atmp - beta1);
      for (i = 18; i < 22; i++) {
        A[i - 1] *= xnorm;
      }

      for (i = 0; i <= knt; i++) {
        beta1 *= 1.0020841800044864E-292;
      }

      atmp = beta1;
    } else {
      tau[2] = (beta1 - A[16]) / beta1;
      xnorm = 1.0 / (A[16] - beta1);
      for (i = 18; i < 22; i++) {
        A[i - 1] *= xnorm;
      }

      atmp = beta1;
    }
  }

  A[16] = atmp;
  atmp = A[16];
  A[16] = 1.0;
  if (tau[2] != 0.0) {
    lastv = 5;
    i = 20;
    while ((lastv > 0) && (A[i] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 1;
    ia = 24;
    do {
      exitg1 = 0;
      if (ia <= lastv + 23) {
        if (A[ia - 1] != 0.0) {
          exitg1 = 1;
        } else {
          ia++;
        }
      } else {
        knt = 0;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 24, A, 17, work);
    b_xger(lastv, knt, -tau[2], 17, work, A, 24);
  }

  A[16] = atmp;
  atmp = A[24];
  tau[3] = 0.0;
  xnorm = c_xnrm2(3, A, 26);
  if (xnorm != 0.0) {
    beta1 = rt_hypotd_snf(A[24], xnorm);
    if (A[24] >= 0.0) {
      beta1 = -beta1;
    }

    if (std::abs(beta1) < 1.0020841800044864E-292) {
      knt = -1;
      do {
        knt++;
        for (i = 26; i < 29; i++) {
          A[i - 1] *= 9.9792015476736E+291;
        }

        beta1 *= 9.9792015476736E+291;
        atmp *= 9.9792015476736E+291;
      } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

      beta1 = rt_hypotd_snf(atmp, c_xnrm2(3, A, 26));
      if (atmp >= 0.0) {
        beta1 = -beta1;
      }

      tau[3] = (beta1 - atmp) / beta1;
      xnorm = 1.0 / (atmp - beta1);
      for (i = 26; i < 29; i++) {
        A[i - 1] *= xnorm;
      }

      for (i = 0; i <= knt; i++) {
        beta1 *= 1.0020841800044864E-292;
      }

      atmp = beta1;
    } else {
      tau[3] = (beta1 - A[24]) / beta1;
      xnorm = 1.0 / (A[24] - beta1);
      for (i = 26; i < 29; i++) {
        A[i - 1] *= xnorm;
      }

      atmp = beta1;
    }
  }

  A[24] = atmp;
  for (i = 0; i < 1; i++) {
    R[0] = A[0];
  }

  memset(&R[1], 0, (unsigned int)(3 * static_cast<int>(sizeof(double))));
  work[0] = 0.0;
  for (i = 0; i < 2; i++) {
    R[i + 4] = A[i + 7];
  }

  memset(&R[6], 0, (unsigned int)(2 * static_cast<int>(sizeof(double))));
  work[1] = 0.0;
  for (i = 0; i < 3; i++) {
    R[i + 8] = A[i + 14];
  }

  memset(&R[11], 0, sizeof(double));
  work[2] = 0.0;
  for (i = 0; i < 4; i++) {
    R[i + 12] = A[i + 21];
  }

  work[3] = 0.0;
  for (i = 26; i < 29; i++) {
    A[i - 1] *= -tau[3];
  }

  A[24] = 1.0 - tau[3];
  for (i = 0; i < 3; i++) {
    A[23 - i] = 0.0;
  }

  A[16] = 1.0;
  if (tau[2] != 0.0) {
    lastv = 5;
    i = 22;
    while ((lastv > 0) && (A[i - 2] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 1;
    ia = 24;
    do {
      exitg1 = 0;
      if (ia <= lastv + 23) {
        if (A[ia - 1] != 0.0) {
          exitg1 = 1;
        } else {
          ia++;
        }
      } else {
        knt = 0;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 24, A, 17, work);
    b_xger(lastv, knt, -tau[2], 17, work, A, 24);
  }

  for (i = 18; i < 22; i++) {
    A[i - 1] *= -tau[2];
  }

  A[16] = 1.0 - tau[2];
  for (i = 0; i < 2; i++) {
    A[15 - i] = 0.0;
  }

  A[8] = 1.0;
  if (tau[1] != 0.0) {
    lastv = 6;
    i = 15;
    while ((lastv > 0) && (A[i - 2] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 2;
    exitg2 = false;
    while ((!exitg2) && (knt > 0)) {
      i = 16 + (knt - 1) * 7;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (A[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          knt--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 16, A, 9, work);
    b_xger(lastv, knt, -tau[1], 9, work, A, 16);
  }

  for (i = 10; i < 15; i++) {
    A[i - 1] *= -tau[1];
  }

  A[8] = 1.0 - tau[1];
  for (i = 0; i < 1; i++) {
    A[7] = 0.0;
  }

  A[0] = 1.0;
  if (tau[0] != 0.0) {
    lastv = 7;
    i = 8;
    while ((lastv > 0) && (A[i - 2] == 0.0)) {
      lastv--;
      i--;
    }

    knt = 3;
    exitg2 = false;
    while ((!exitg2) && (knt > 0)) {
      i = 8 + (knt - 1) * 7;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (A[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          knt--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    knt = 0;
  }

  if (lastv > 0) {
    b_xgemv(lastv, knt, A, 8, A, 1, work);
    b_xger(lastv, knt, -tau[0], 1, work, A, 8);
  }

  for (i = 2; i < 8; i++) {
    A[i - 1] *= -tau[0];
  }

  A[0] = 1.0 - tau[0];
}

//
// Arguments    : const double A_data[]
//                const int A_size[2]
//                double Q_data[]
//                int Q_size[2]
//                double R_data[]
//                int R_size[2]
// Return Type  : void
//
void qr(const double A_data[], const int A_size[2], double Q_data[], int Q_size
        [2], double R_data[], int R_size[2])
{
  int A_size_idx_0;
  int knt;
  double b_A_data[36];
  int m;
  int mn;
  double work[4];
  int i;
  int b_tmp;
  int i_i;
  int lastc;
  double tau_data[4];
  double atmp;
  int b_i;
  double xnorm;
  double beta1;
  int lastv;
  boolean_T exitg2;
  int coltop;
  int exitg1;
  A_size_idx_0 = A_size[0];
  knt = A_size[0] * A_size[1];
  if (0 <= knt - 1) {
    memcpy(&b_A_data[0], &A_data[0], (unsigned int)(knt * static_cast<int>
            (sizeof(double))));
  }

  m = A_size[0];
  mn = A_size[0];
  if (mn >= 4) {
    mn = 4;
  }

  if (A_size[0] != 0) {
    work[0] = 0.0;
    work[1] = 0.0;
    work[2] = 0.0;
    work[3] = 0.0;
    for (i = 0; i < mn; i++) {
      i_i = i + i * m;
      b_tmp = (m - i) - 1;
      if (i + 1 < m) {
        atmp = b_A_data[i_i];
        b_i = i_i + 2;
        tau_data[i] = 0.0;
        if (1 + b_tmp > 0) {
          xnorm = xnrm2(b_tmp, b_A_data, i_i + 2);
          if (xnorm != 0.0) {
            beta1 = rt_hypotd_snf(b_A_data[i_i], xnorm);
            if (b_A_data[i_i] >= 0.0) {
              beta1 = -beta1;
            }

            if (std::abs(beta1) < 1.0020841800044864E-292) {
              knt = -1;
              lastc = i_i + b_tmp;
              do {
                knt++;
                for (coltop = b_i; coltop <= lastc + 1; coltop++) {
                  b_A_data[coltop - 1] *= 9.9792015476736E+291;
                }

                beta1 *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(std::abs(beta1) >= 1.0020841800044864E-292));

              beta1 = rt_hypotd_snf(atmp, xnrm2(b_tmp, b_A_data, i_i + 2));
              if (atmp >= 0.0) {
                beta1 = -beta1;
              }

              tau_data[i] = (beta1 - atmp) / beta1;
              xnorm = 1.0 / (atmp - beta1);
              for (coltop = b_i; coltop <= lastc + 1; coltop++) {
                b_A_data[coltop - 1] *= xnorm;
              }

              for (coltop = 0; coltop <= knt; coltop++) {
                beta1 *= 1.0020841800044864E-292;
              }

              atmp = beta1;
            } else {
              tau_data[i] = (beta1 - b_A_data[i_i]) / beta1;
              xnorm = 1.0 / (b_A_data[i_i] - beta1);
              lastc = i_i + b_tmp;
              for (coltop = b_i; coltop <= lastc + 1; coltop++) {
                b_A_data[coltop - 1] *= xnorm;
              }

              atmp = beta1;
            }
          }
        }

        b_A_data[i_i] = atmp;
      } else {
        tau_data[i] = 0.0;
      }

      if (i + 1 < 4) {
        atmp = b_A_data[i_i];
        b_A_data[i_i] = 1.0;
        knt = (i + (i + 1) * m) + 1;
        if (tau_data[i] != 0.0) {
          lastv = 1 + b_tmp;
          b_i = i_i + b_tmp;
          while ((lastv > 0) && (b_A_data[b_i] == 0.0)) {
            lastv--;
            b_i--;
          }

          lastc = 3 - i;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            coltop = knt + (lastc - 1) * m;
            b_i = coltop;
            do {
              exitg1 = 0;
              if (b_i <= (coltop + lastv) - 1) {
                if (b_A_data[b_i - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  b_i++;
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
          lastc = 0;
        }

        if (lastv > 0) {
          xgemv(lastv, lastc, b_A_data, knt, m, b_A_data, i_i + 1, work);
          xger(lastv, lastc, -tau_data[i], i_i + 1, work, b_A_data, knt, m);
        }

        b_A_data[i_i] = atmp;
      }
    }
  }

  m = A_size[0];
  if (A_size[0] > 4) {
    R_size[0] = 4;
    R_size[1] = 4;
    for (i = 0; i < 1; i++) {
      R_data[0] = b_A_data[0];
    }

    for (i = 2; i < 5; i++) {
      R_data[i - 1] = 0.0;
    }

    work[0] = 0.0;
    for (i = 0; i < 2; i++) {
      R_data[i + 4] = b_A_data[i + A_size_idx_0];
    }

    for (i = 3; i < 5; i++) {
      R_data[i + 3] = 0.0;
    }

    work[1] = 0.0;
    for (i = 0; i < 3; i++) {
      R_data[i + 8] = b_A_data[i + (A_size_idx_0 << 1)];
    }

    for (i = 4; i < 5; i++) {
      R_data[11] = 0.0;
    }

    work[2] = 0.0;
    for (i = 0; i < 4; i++) {
      R_data[i + 12] = b_A_data[i + A_size_idx_0 * 3];
    }

    work[3] = 0.0;
    mn = 3 * A_size[0] + 4;
    b_i = mn + 1;
    lastc = (mn + A_size[0]) - 3;
    for (coltop = b_i; coltop < lastc; coltop++) {
      b_A_data[coltop - 1] *= -tau_data[3];
    }

    b_A_data[mn - 1] = 1.0 - tau_data[3];
    for (b_tmp = 0; b_tmp < 3; b_tmp++) {
      b_A_data[(mn - b_tmp) - 2] = 0.0;
    }

    mn = (A_size[0] << 1) + 3;
    b_A_data[mn - 1] = 1.0;
    b_tmp = mn + A_size[0];
    if (tau_data[2] != 0.0) {
      lastv = A_size[0] - 2;
      i = b_tmp - 2;
      while ((lastv > 0) && (b_A_data[i - 2] == 0.0)) {
        lastv--;
        i--;
      }

      lastc = 1;
      b_i = b_tmp;
      do {
        exitg1 = 0;
        if (b_i <= (b_tmp + lastv) - 1) {
          if (b_A_data[b_i - 1] != 0.0) {
            exitg1 = 1;
          } else {
            b_i++;
          }
        } else {
          lastc = 0;
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    } else {
      lastv = 0;
      lastc = 0;
    }

    if (lastv > 0) {
      xgemv(lastv, lastc, b_A_data, b_tmp, A_size[0], b_A_data, mn, work);
      xger(lastv, lastc, -tau_data[2], mn, work, b_A_data, b_tmp, A_size[0]);
    }

    b_i = mn + 1;
    lastc = (mn + A_size[0]) - 2;
    for (coltop = b_i; coltop < lastc; coltop++) {
      b_A_data[coltop - 1] *= -tau_data[2];
    }

    b_A_data[mn - 1] = 1.0 - tau_data[2];
    for (b_tmp = 0; b_tmp < 2; b_tmp++) {
      b_A_data[(mn - b_tmp) - 2] = 0.0;
    }

    mn = A_size[0] + 2;
    b_A_data[mn - 1] = 1.0;
    b_tmp = mn + A_size[0];
    if (tau_data[1] != 0.0) {
      lastv = A_size[0] - 1;
      i = b_tmp - 1;
      while ((lastv > 0) && (b_A_data[i - 2] == 0.0)) {
        lastv--;
        i--;
      }

      lastc = 2;
      exitg2 = false;
      while ((!exitg2) && (lastc > 0)) {
        coltop = b_tmp + (lastc - 1) * m;
        b_i = coltop;
        do {
          exitg1 = 0;
          if (b_i <= (coltop + lastv) - 1) {
            if (b_A_data[b_i - 1] != 0.0) {
              exitg1 = 1;
            } else {
              b_i++;
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
      lastc = 0;
    }

    if (lastv > 0) {
      xgemv(lastv, lastc, b_A_data, b_tmp, A_size[0], b_A_data, mn, work);
      xger(lastv, lastc, -tau_data[1], mn, work, b_A_data, b_tmp, A_size[0]);
    }

    b_i = mn + 1;
    lastc = (mn + A_size[0]) - 1;
    for (coltop = b_i; coltop < lastc; coltop++) {
      b_A_data[coltop - 1] *= -tau_data[1];
    }

    b_A_data[mn - 1] = 1.0 - tau_data[1];
    for (b_tmp = 0; b_tmp < 1; b_tmp++) {
      b_A_data[mn - 2] = 0.0;
    }

    b_A_data[0] = 1.0;
    knt = 1 + A_size[0];
    if (tau_data[0] != 0.0) {
      lastv = A_size[0];
      i = 1 + A_size[0];
      while ((lastv > 0) && (b_A_data[i - 2] == 0.0)) {
        lastv--;
        i--;
      }

      lastc = 3;
      exitg2 = false;
      while ((!exitg2) && (lastc > 0)) {
        coltop = knt + (lastc - 1) * m;
        b_i = coltop;
        do {
          exitg1 = 0;
          if (b_i <= (coltop + lastv) - 1) {
            if (b_A_data[b_i - 1] != 0.0) {
              exitg1 = 1;
            } else {
              b_i++;
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
      lastc = 0;
    }

    if (lastv > 0) {
      xgemv(lastv, lastc, b_A_data, knt, A_size[0], b_A_data, 1, work);
      xger(lastv, lastc, -tau_data[0], 1, work, b_A_data, knt, A_size[0]);
    }

    lastc = 1 + A_size[0];
    for (coltop = 2; coltop < lastc; coltop++) {
      b_A_data[coltop - 1] *= -tau_data[0];
    }

    b_A_data[0] = 1.0 - tau_data[0];
    Q_size[0] = A_size[0];
    Q_size[1] = 4;
    knt = A_size[0] << 2;
    if (0 <= knt - 1) {
      memcpy(&Q_data[0], &b_A_data[0], (unsigned int)(knt * static_cast<int>
              (sizeof(double))));
    }
  } else {
    R_size[0] = A_size[0];
    R_size[1] = 4;
    for (b_tmp = 0; b_tmp < m; b_tmp++) {
      for (i = 0; i <= b_tmp; i++) {
        R_data[i + R_size[0] * b_tmp] = b_A_data[i + A_size_idx_0 * b_tmp];
      }

      for (i = b_tmp + 2; i <= m; i++) {
        R_data[(i + R_size[0] * b_tmp) - 1] = 0.0;
      }
    }

    lastc = A_size[0] + 1;
    for (b_tmp = lastc; b_tmp < 5; b_tmp++) {
      for (i = 0; i < m; i++) {
        R_data[i + R_size[0] * (b_tmp - 1)] = b_A_data[i + A_size_idx_0 * (b_tmp
          - 1)];
      }
    }

    if (A_size[0] >= 1) {
      lastc = A_size[0] - 1;
      for (b_tmp = m; b_tmp <= lastc; b_tmp++) {
        b_i = b_tmp * m;
        knt = m - 1;
        if (0 <= knt) {
          memset(&b_A_data[b_i], 0, (unsigned int)((knt + 1) * static_cast<int>
                  (sizeof(double))));
        }

        b_A_data[b_i + b_tmp] = 1.0;
      }

      knt = A_size[0] - 1;
      work[0] = 0.0;
      work[1] = 0.0;
      work[2] = 0.0;
      work[3] = 0.0;
      for (i = A_size[0]; i >= 1; i--) {
        mn = i + (i - 1) * m;
        if (i < m) {
          b_A_data[mn - 1] = 1.0;
          b_tmp = mn + m;
          if (tau_data[knt] != 0.0) {
            lastc = m - i;
            lastv = lastc + 1;
            b_i = b_tmp - i;
            while ((lastv > 0) && (b_A_data[b_i - 1] == 0.0)) {
              lastv--;
              b_i--;
            }

            exitg2 = false;
            while ((!exitg2) && (lastc > 0)) {
              coltop = b_tmp + (lastc - 1) * m;
              b_i = coltop;
              do {
                exitg1 = 0;
                if (b_i <= (coltop + lastv) - 1) {
                  if (b_A_data[b_i - 1] != 0.0) {
                    exitg1 = 1;
                  } else {
                    b_i++;
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
            lastc = 0;
          }

          if (lastv > 0) {
            xgemv(lastv, lastc, b_A_data, b_tmp, m, b_A_data, mn, work);
            xger(lastv, lastc, -tau_data[knt], mn, work, b_A_data, b_tmp, m);
          }

          b_i = mn + 1;
          lastc = (mn + m) - i;
          for (coltop = b_i; coltop <= lastc; coltop++) {
            b_A_data[coltop - 1] *= -tau_data[knt];
          }
        }

        b_A_data[mn - 1] = 1.0 - tau_data[knt];
        for (b_tmp = 0; b_tmp <= i - 2; b_tmp++) {
          b_A_data[(mn - b_tmp) - 2] = 0.0;
        }

        knt--;
      }
    }

    Q_size[0] = static_cast<signed char>(A_size[0]);
    Q_size[1] = static_cast<signed char>(A_size[0]);
    for (b_tmp = 0; b_tmp < m; b_tmp++) {
      for (i = 0; i < m; i++) {
        Q_data[i + static_cast<signed char>(m) * b_tmp] = b_A_data[i +
          A_size_idx_0 * b_tmp];
      }
    }
  }
}

//
// File trailer for qr.cpp
//
// [EOF]
//
