//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: offsetCorrect.cpp
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
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const double sp[1650]
//                double c
//                emxArray_real_T *correctedsp
// Return Type  : void
//
void offsetCorrect(const double sp[1650], double c, emxArray_real_T *correctedsp)
{
  int i37;
  int loop_ub;
  int i38;
  int b_loop_ub;
  int i39;
  if (c < 0.0) {
    if (1.0 > 1650.0 + c) {
      loop_ub = 0;
    } else {
      loop_ub = static_cast<int>((1650.0 + c));
    }

    i37 = correctedsp->size[0];
    b_loop_ub = static_cast<int>(-c);
    correctedsp->size[0] = b_loop_ub + loop_ub;
    emxEnsureCapacity_real_T(correctedsp, i37);
    for (i37 = 0; i37 < b_loop_ub; i37++) {
      correctedsp->data[i37] = 0.0;
    }

    for (i37 = 0; i37 < loop_ub; i37++) {
      correctedsp->data[i37 + b_loop_ub] = sp[i37];
    }
  } else if (c > 0.0) {
    if (c + 1.0 > 1650.0) {
      i37 = 0;
      i38 = 0;
    } else {
      i37 = static_cast<int>((c + 1.0)) - 1;
      i38 = 1650;
    }

    i39 = correctedsp->size[0];
    loop_ub = i38 - i37;
    b_loop_ub = static_cast<int>(c);
    correctedsp->size[0] = loop_ub + b_loop_ub;
    emxEnsureCapacity_real_T(correctedsp, i39);
    for (i39 = 0; i39 < loop_ub; i39++) {
      correctedsp->data[i39] = sp[i37 + i39];
    }

    for (i39 = 0; i39 < b_loop_ub; i39++) {
      correctedsp->data[(i39 + i38) - i37] = 0.0;
    }
  } else {
    i37 = correctedsp->size[0];
    correctedsp->size[0] = 1650;
    emxEnsureCapacity_real_T(correctedsp, i37);
    for (i37 = 0; i37 < 1650; i37++) {
      correctedsp->data[i37] = sp[i37];
    }
  }
}

//
// File trailer for offsetCorrect.cpp
//
// [EOF]
//
