//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: conv2.cpp
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
#include "conv2.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const double a[1650]
//                const emxArray_real_T *b
//                emxArray_real_T *c
// Return Type  : void
//
void conv2(const double a[1650], const emxArray_real_T *b, emxArray_real_T *c)
{
  int mb;
  int mc;
  int i24;
  int ic;
  int ia;
  int i;
  double bij;
  if (b->size[0] > 1650) {
    mb = b->size[0];
    mc = b->size[0] + 1648;
    i24 = c->size[0];
    c->size[0] = mc + 1;
    emxEnsureCapacity_real_T(c, i24);
    for (i24 = 0; i24 <= mc; i24++) {
      c->data[i24] = 0.0;
    }

    for (mc = 0; mc < 1650; mc++) {
      ic = 1649 - mc;
      ia = 0;
      for (i = 0; i < mb; i++) {
        c->data[ic] += a[1649 - mc] * b->data[ia];
        ic++;
        ia++;
      }
    }
  } else {
    mb = b->size[0] - 1;
    if (b->size[0] == 0) {
      mc = 1649;
    } else {
      mc = b->size[0] + 1648;
    }

    i24 = c->size[0];
    c->size[0] = mc + 1;
    emxEnsureCapacity_real_T(c, i24);
    for (i24 = 0; i24 <= mc; i24++) {
      c->data[i24] = 0.0;
    }

    if (b->size[0] != 0) {
      i24 = b->size[0] - 1;
      for (mc = 0; mc <= i24; mc++) {
        ic = mb - mc;
        bij = b->data[ic];
        ia = 0;
        for (i = 0; i < 1650; i++) {
          c->data[ic] += bij * a[ia];
          ic++;
          ia++;
        }
      }
    }
  }
}

//
// File trailer for conv2.cpp
//
// [EOF]
//
