//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: filter.cpp
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
#include "filter_.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const double b_data[]
//                const int b_size[1]
//                const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void filter(const double b_data[], const int b_size[1], const emxArray_real_T *x,
            emxArray_real_T *y)
{
  int nb;
  unsigned int x_idx_0;
  int i34;
  int nx;
  int nx_m_nb;
  int k;
  int j;
  nb = b_size[0] - 1;
  x_idx_0 = static_cast<unsigned int>(x->size[0]);
  i34 = y->size[0];
  y->size[0] = static_cast<int>(x_idx_0);
  emxEnsureCapacity_real_T(y, i34);
  nx = x->size[0];
  nx_m_nb = y->size[0];
  i34 = y->size[0];
  y->size[0] = nx_m_nb;
  emxEnsureCapacity_real_T(y, i34);
  for (i34 = 0; i34 < nx_m_nb; i34++) {
    y->data[i34] = 0.0;
  }

  if (x->size[0] >= (b_size[0] << 1)) {
    for (k = 0; k <= nb; k++) {
      nx_m_nb = k + 1;
      for (j = nx_m_nb; j <= nx; j++) {
        y->data[j - 1] += b_data[k] * x->data[(j - k) - 1];
      }
    }
  } else {
    if (x->size[0] > b_size[0]) {
      nx_m_nb = x->size[0] - b_size[0];
    } else {
      nx_m_nb = 0;
    }

    for (k = 0; k < nx_m_nb; k++) {
      for (j = 0; j <= nb; j++) {
        i34 = k + j;
        y->data[i34] += x->data[k] * b_data[j];
      }
    }

    nb = x->size[0] - nx_m_nb;
    i34 = nx_m_nb + 1;
    for (k = i34; k <= nx; k++) {
      for (j = 0; j < nb; j++) {
        nx_m_nb = (k + j) - 1;
        y->data[nx_m_nb] += x->data[k - 1] * b_data[j];
      }

      nb--;
    }
  }
}

//
// File trailer for filter.cpp
//
// [EOF]
//
