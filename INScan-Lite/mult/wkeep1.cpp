//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: wkeep1.cpp
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
#include "wkeep1.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void wkeep1(const emxArray_real_T *x, emxArray_real_T *y)
{
  int first;
  int d;
  int i25;
  first = 1;
  d = x->size[0];
  if (1650 < x->size[0]) {
    d = (x->size[0] - 1650) >> 1;
    first = 1 + d;
    d = x->size[0] - d;
    if (((x->size[0] - 1650) & 1) != 0) {
      d--;
    }
  }

  if (first > d) {
    first = 1;
    d = 0;
  }

  i25 = y->size[0];
  d -= first;
  y->size[0] = d + 1;
  emxEnsureCapacity_real_T(y, i25);
  for (i25 = 0; i25 <= d; i25++) {
    y->data[i25] = x->data[(first + i25) - 1];
  }
}

//
// File trailer for wkeep1.cpp
//
// [EOF]
//
