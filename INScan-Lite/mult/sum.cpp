//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sum.cpp
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
#include "sum.h"
#include "combineVectorElements.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
double b_sum(const emxArray_real_T *x)
{
  double y;
  int vlen;
  int k;
  vlen = x->size[1];
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= vlen; k++) {
      y += x->data[k - 1];
    }
  }

  return y;
}

//
// Arguments    : const emxArray_boolean_T *x
// Return Type  : double
//
double c_sum(const emxArray_boolean_T *x)
{
  double y;
  int vlen;
  int k;
  vlen = x->size[1];
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= vlen; k++) {
      y += static_cast<double>(x->data[k - 1]);
    }
  }

  return y;
}

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                double y_data[]
//                int y_size[2]
// Return Type  : void
//
void d_sum(const double x_data[], const int x_size[2], double y_data[], int
           y_size[2])
{
  int vlen;
  short sz_idx_1;
  int k;
  vlen = x_size[0];
  if ((x_size[0] == 0) || (x_size[1] == 0)) {
    sz_idx_1 = static_cast<short>(x_size[1]);
    y_size[0] = 1;
    y_size[1] = sz_idx_1;
    if (0 <= sz_idx_1 - 1) {
      memset(&y_data[0], 0, (unsigned int)(sz_idx_1 * static_cast<int>(sizeof
               (double))));
    }
  } else {
    y_size[0] = 1;
    y_size[1] = 1;
    y_data[0] = x_data[0];
    for (k = 2; k <= vlen; k++) {
      y_data[0] += x_data[k - 1];
    }
  }
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
double sum(const emxArray_real_T *x)
{
  return combineVectorElements(x);
}

//
// File trailer for sum.cpp
//
// [EOF]
//
