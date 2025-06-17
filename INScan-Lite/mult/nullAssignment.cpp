//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: nullAssignment.cpp
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
#include "nullAssignment.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *x
//                const emxArray_boolean_T *idx
// Return Type  : void
//
void nullAssignment(emxArray_real_T *x, const emxArray_boolean_T *idx)
{
  int nxin;
  int k0;
  int i77;
  int k;
  int nxout;
  nxin = x->size[1];
  k0 = 0;
  i77 = idx->size[0];
  for (k = 0; k < i77; k++) {
    k0 += idx->data[k];
  }

  nxout = x->size[1] - k0;
  k0 = -1;
  for (k = 0; k < nxin; k++) {
    if ((k + 1 > idx->size[0]) || (!idx->data[k])) {
      k0++;
      x->data[k0] = x->data[k];
    }
  }

  if (1 > nxout) {
    x->size[1] = 0;
  } else {
    i77 = x->size[0] * x->size[1];
    x->size[1] = nxout;
    emxEnsureCapacity_real_T(x, i77);
  }
}

//
// File trailer for nullAssignment.cpp
//
// [EOF]
//
