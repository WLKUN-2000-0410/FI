//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: vvarstd.cpp
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
#include "vvarstd.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *v
//                int n
// Return Type  : double
//
double vvarstd(const emxArray_real_T *v, int n)
{
  double s;
  double xbar;
  int k;
  emxArray_real_T *absdiff;
  double absxk;
  double t;
  if (n == 0) {
    s = rtNaN;
  } else if (n == 1) {
    if ((!rtIsInf(v->data[0])) && (!rtIsNaN(v->data[0]))) {
      s = 0.0;
    } else {
      s = rtNaN;
    }
  } else {
    if (v->size[0] == 0) {
      xbar = 0.0;
    } else {
      xbar = v->data[0];
      for (k = 2; k <= n; k++) {
        xbar += v->data[k - 1];
      }
    }

    emxInit_real_T(&absdiff, 1);
    xbar /= static_cast<double>(n);
    k = absdiff->size[0];
    absdiff->size[0] = v->size[0];
    emxEnsureCapacity_real_T(absdiff, k);
    for (k = 0; k < n; k++) {
      absdiff->data[k] = std::abs(v->data[k] - xbar);
    }

    s = 0.0;
    if (n == 1) {
      s = absdiff->data[0];
    } else {
      xbar = 3.3121686421112381E-170;
      for (k = 0; k < n; k++) {
        absxk = absdiff->data[k];
        if (absxk > xbar) {
          t = xbar / absxk;
          s = 1.0 + s * t * t;
          xbar = absxk;
        } else {
          t = absxk / xbar;
          s += t * t;
        }
      }

      s = xbar * std::sqrt(s);
    }

    emxFree_real_T(&absdiff);
    s /= std::sqrt((double)(n - 1));
  }

  return s;
}

//
// File trailer for vvarstd.cpp
//
// [EOF]
//
