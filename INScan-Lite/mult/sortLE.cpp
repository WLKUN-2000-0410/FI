//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortLE.cpp
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
#include "sortLE.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *v
//                int idx1
//                int idx2
// Return Type  : boolean_T
//
boolean_T sortLE(const emxArray_real_T *v, int idx1, int idx2)
{
  boolean_T p;
  double v1;
  double v2;
  p = true;
  v1 = v->data[(idx1 + v->size[0]) - 1];
  v2 = v->data[(idx2 + v->size[0]) - 1];
  if ((v1 == v2) || (rtIsNaN(v1) && rtIsNaN(v2)) || (v1 >= v2) || rtIsNaN(v1)) {
  } else {
    p = false;
  }

  return p;
}

//
// File trailer for sortLE.cpp
//
// [EOF]
//
