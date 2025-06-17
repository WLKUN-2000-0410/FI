//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgetrf.cpp
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
#include "xgetrf.h"
#include "xswap.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *A
//                int ipiv[1650]
//                int *info
// Return Type  : void
//
void xgetrf(emxArray_real_T *A, int ipiv[1650], int *info)
{
  int i89;
  int j;
  int b;
  int jj;
  int jp1j;
  int n;
  int jy;
  int ix;
  double smax;
  int jA;
  double s;
  int ijA;
  for (i89 = 0; i89 < 1650; i89++) {
    ipiv[i89] = 1 + i89;
  }

  *info = 0;
  for (j = 0; j < 1649; j++) {
    b = j * 1651;
    jj = j * 1651;
    jp1j = b + 2;
    n = 1650 - j;
    jy = 1;
    ix = b;
    smax = std::abs(A->data[b]);
    for (jA = 2; jA <= n; jA++) {
      ix++;
      s = std::abs(A->data[ix]);
      if (s > smax) {
        jy = jA;
        smax = s;
      }
    }

    if (A->data[(jj + jy) - 1] != 0.0) {
      if (jy - 1 != 0) {
        ipiv[j] = j + jy;
        xswap(A, j + 1, j + jy);
      }

      i89 = jj - j;
      for (jy = jp1j; jy <= i89 + 1650; jy++) {
        A->data[jy - 1] /= A->data[jj];
      }
    } else {
      *info = j + 1;
    }

    n = 1648 - j;
    jy = b + 1650;
    jA = jj + 1651;
    for (b = 0; b <= n; b++) {
      smax = A->data[jy];
      if (A->data[jy] != 0.0) {
        ix = jj + 1;
        i89 = jA + 1;
        jp1j = (jA - j) + 1649;
        for (ijA = i89; ijA <= jp1j; ijA++) {
          A->data[ijA - 1] += A->data[ix] * -smax;
          ix++;
        }
      }

      jy += 1650;
      jA += 1650;
    }
  }

  if ((*info == 0) && (!(A->data[2722499] != 0.0))) {
    *info = 1650;
  }
}

//
// File trailer for xgetrf.cpp
//
// [EOF]
//
