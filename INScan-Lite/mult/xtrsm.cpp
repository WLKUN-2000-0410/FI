//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xtrsm.cpp
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
#include "xtrsm.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *A
//                double B[1650]
// Return Type  : void
//
void b_xtrsm(const emxArray_real_T *A, double B[1650])
{
  int k;
  int kAcol;
  int i;
  for (k = 1649; k >= 0; k--) {
    kAcol = 1650 * k;
    if (B[k] != 0.0) {
      B[k] /= A->data[k + kAcol];
      for (i = 0; i < k; i++) {
        B[i] -= B[k] * A->data[i + kAcol];
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *A
//                double B[1650]
// Return Type  : void
//
void xtrsm(const emxArray_real_T *A, double B[1650])
{
  int k;
  int kAcol;
  int i90;
  int i;
  for (k = 0; k < 1650; k++) {
    kAcol = 1650 * k;
    if (B[k] != 0.0) {
      i90 = k + 2;
      for (i = i90; i < 1651; i++) {
        B[i - 1] -= B[k] * A->data[(i + kAcol) - 1];
      }
    }
  }
}

//
// File trailer for xtrsm.cpp
//
// [EOF]
//
