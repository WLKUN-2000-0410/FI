//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemv.cpp
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
#include "xgemv.h"

// Function Definitions

//
// Arguments    : int m
//                int n
//                const double A[28]
//                int ia0
//                const double x[28]
//                int ix0
//                double y[4]
// Return Type  : void
//
void b_xgemv(int m, int n, const double A[28], int ia0, const double x[28], int
             ix0, double y[4])
{
  int iy;
  int i85;
  int iac;
  int ix;
  double c;
  int i86;
  int ia;
  if (n != 0) {
    if (0 <= n - 1) {
      memset(&y[0], 0, (unsigned int)(n * static_cast<int>(sizeof(double))));
    }

    iy = 0;
    i85 = ia0 + 7 * (n - 1);
    for (iac = ia0; iac <= i85; iac += 7) {
      ix = ix0;
      c = 0.0;
      i86 = (iac + m) - 1;
      for (ia = iac; ia <= i86; ia++) {
        c += A[ia - 1] * x[ix - 1];
        ix++;
      }

      y[iy] += c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const double A_data[]
//                int ia0
//                int lda
//                const double x_data[]
//                int ix0
//                double y[4]
// Return Type  : void
//
void xgemv(int m, int n, const double A_data[], int ia0, int lda, const double
           x_data[], int ix0, double y[4])
{
  int iy;
  int i69;
  int iac;
  int ix;
  double c;
  int i70;
  int ia;
  if (n != 0) {
    if (0 <= n - 1) {
      memset(&y[0], 0, (unsigned int)(n * static_cast<int>(sizeof(double))));
    }

    iy = 0;
    i69 = ia0 + lda * (n - 1);
    for (iac = ia0; lda < 0 ? iac >= i69 : iac <= i69; iac += lda) {
      ix = ix0;
      c = 0.0;
      i70 = (iac + m) - 1;
      for (ia = iac; ia <= i70; ia++) {
        c += A_data[ia - 1] * x_data[ix - 1];
        ix++;
      }

      y[iy] += c;
      iy++;
    }
  }
}

//
// File trailer for xgemv.cpp
//
// [EOF]
//
