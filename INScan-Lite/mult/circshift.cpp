//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: circshift.cpp
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
#include "circshift.h"

// Function Definitions

//
// Arguments    : double a[1800]
//                double p
// Return Type  : void
//
void circshift(double a[1800], double p)
{
  int ns;
  boolean_T shiftright;
  double unusedU0[900];
  int k;
  int i63;
  if (p < 0.0) {
    ns = static_cast<int>(-p) - 1;
    shiftright = false;
  } else {
    ns = static_cast<int>(p) - 1;
    shiftright = true;
  }

  memset(&unusedU0[0], 0, 900U * sizeof(double));
  if (shiftright) {
    for (k = 0; k <= ns; k++) {
      unusedU0[k] = a[(k - ns) + 1799];
    }

    for (k = 1800; k >= ns + 2; k--) {
      a[k - 1] = a[(k - ns) - 2];
    }

    if (0 <= ns) {
      memcpy(&a[0], &unusedU0[0], (unsigned int)((ns + 1) * static_cast<int>
              (sizeof(double))));
    }
  } else {
    if (0 <= ns) {
      memcpy(&unusedU0[0], &a[0], (unsigned int)((ns + 1) * static_cast<int>
              (sizeof(double))));
    }

    i63 = 1798 - ns;
    for (k = 0; k <= i63; k++) {
      a[k] = a[(k + ns) + 1];
    }

    for (k = 0; k <= ns; k++) {
      a[(k - ns) + 1799] = unusedU0[k];
    }
  }
}

//
// File trailer for circshift.cpp
//
// [EOF]
//
