//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xger.cpp
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
#include "xger.h"

// Function Definitions

//
// Arguments    : int m
//                int n
//                double alpha1
//                int ix0
//                const double y[4]
//                double A[28]
//                int ia0
// Return Type  : void
//
void b_xger(int m, int n, double alpha1, int ix0, const double y[4], double A[28],
            int ia0)
{
  int jA;
  int jy;
  int j;
  double temp;
  int ix;
  int i87;
  int i88;
  int ijA;
  if (!(alpha1 == 0.0)) {
    jA = ia0 - 1;
    jy = 0;
    for (j = 0; j < n; j++) {
      if (y[jy] != 0.0) {
        temp = y[jy] * alpha1;
        ix = ix0;
        i87 = jA + 1;
        i88 = m + jA;
        for (ijA = i87; ijA <= i88; ijA++) {
          A[ijA - 1] += A[ix - 1] * temp;
          ix++;
        }
      }

      jy++;
      jA += 7;
    }
  }
}

//
// Arguments    : int m
//                int n
//                double alpha1
//                int ix0
//                const double y[4]
//                double A_data[]
//                int ia0
//                int lda
// Return Type  : void
//
void xger(int m, int n, double alpha1, int ix0, const double y[4], double
          A_data[], int ia0, int lda)
{
  int jA;
  int jy;
  int j;
  double temp;
  int ix;
  int i71;
  int i72;
  int ijA;
  if (!(alpha1 == 0.0)) {
    jA = ia0 - 1;
    jy = 0;
    for (j = 0; j < n; j++) {
      if (y[jy] != 0.0) {
        temp = y[jy] * alpha1;
        ix = ix0;
        i71 = jA + 1;
        i72 = m + jA;
        for (ijA = i71; ijA <= i72; ijA++) {
          A_data[ijA - 1] += A_data[ix - 1] * temp;
          ix++;
        }
      }

      jy++;
      jA += lda;
    }
  }
}

//
// File trailer for xger.cpp
//
// [EOF]
//
