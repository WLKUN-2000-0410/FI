//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: norm.cpp
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
#include "norm.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
double b_norm(const emxArray_real_T *x)
{
  double y;
  int j;
  boolean_T exitg1;
  double s;
  int i;
  if (x->size[1] == 0) {
    y = 0.0;
  } else if (x->size[1] == 1) {
    y = 0.0;
    for (j = 0; j < 1650; j++) {
      y += std::abs(x->data[j]);
    }
  } else {
    y = 0.0;
    j = 0;
    exitg1 = false;
    while ((!exitg1) && (j <= x->size[1] - 1)) {
      s = 0.0;
      for (i = 0; i < 1650; i++) {
        s += std::abs(x->data[i + 1650 * j]);
      }

      if (rtIsNaN(s)) {
        y = rtNaN;
        exitg1 = true;
      } else {
        if (s > y) {
          y = s;
        }

        j++;
      }
    }
  }

  return y;
}

//
// File trailer for norm.cpp
//
// [EOF]
//
