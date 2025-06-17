//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: interp1.cpp
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
#include "interp1.h"
#include "analyseMixture_emxutil.h"

// Function Declarations
static void interp1Linear(const emxArray_real_T *y, const double xi[1800],
  double yi[1800], const emxArray_real_T *varargin_1);

// Function Definitions

//
// Arguments    : const emxArray_real_T *y
//                const double xi[1800]
//                double yi[1800]
//                const emxArray_real_T *varargin_1
// Return Type  : void
//
static void interp1Linear(const emxArray_real_T *y, const double xi[1800],
  double yi[1800], const emxArray_real_T *varargin_1)
{
  double minx;
  double maxx;
  int k;
  int high_i;
  int low_i;
  int low_ip1;
  double r;
  int mid_i;
  minx = varargin_1->data[0];
  maxx = varargin_1->data[varargin_1->size[0] - 1];

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(high_i,low_i,low_ip1,r,mid_i)

  for (k = 0; k < 1800; k++) {
    if (rtIsNaN(xi[k])) {
      yi[k] = rtNaN;
    } else {
      if ((!(xi[k] > maxx)) && (!(xi[k] < minx))) {
        high_i = varargin_1->size[0];
        low_i = 1;
        low_ip1 = 2;
        while (high_i > low_ip1) {
          mid_i = (low_i >> 1) + (high_i >> 1);
          if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
            mid_i++;
          }

          if (xi[k] >= varargin_1->data[mid_i - 1]) {
            low_i = mid_i;
            low_ip1 = mid_i + 1;
          } else {
            high_i = mid_i;
          }
        }

        r = (xi[k] - varargin_1->data[low_i - 1]) / (varargin_1->data[low_i] -
          varargin_1->data[low_i - 1]);
        if (r == 0.0) {
          yi[k] = y->data[low_i - 1];
        } else if (r == 1.0) {
          yi[k] = y->data[low_i];
        } else if (y->data[low_i - 1] == y->data[low_i]) {
          yi[k] = y->data[low_i - 1];
        } else {
          yi[k] = (1.0 - r) * y->data[low_i - 1] + r * y->data[low_i];
        }
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *varargin_1
//                const emxArray_real_T *varargin_2
//                double Vq[1800]
// Return Type  : void
//
void interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
             *varargin_2, double Vq[1800])
{
  emxArray_real_T *y;
  int i52;
  int n;
  emxArray_real_T *x;
  int nx;
  int k;
  int exitg1;
  double xtmp;
  int nd2;
  double dv3[1800];
  emxInit_real_T(&y, 1);
  i52 = y->size[0];
  y->size[0] = varargin_2->size[0];
  emxEnsureCapacity_real_T(y, i52);
  n = varargin_2->size[0];
  for (i52 = 0; i52 < n; i52++) {
    y->data[i52] = varargin_2->data[i52];
  }

  emxInit_real_T(&x, 1);
  i52 = x->size[0];
  x->size[0] = varargin_1->size[0];
  emxEnsureCapacity_real_T(x, i52);
  n = varargin_1->size[0];
  for (i52 = 0; i52 < n; i52++) {
    x->data[i52] = varargin_1->data[i52];
  }

  nx = varargin_1->size[0] - 1;
  k = 0;
  do {
    exitg1 = 0;
    if (k <= nx) {
      if (rtIsNaN(varargin_1->data[k])) {
        exitg1 = 1;
      } else {
        k++;
      }
    } else {
      if (varargin_1->data[1] < varargin_1->data[0]) {
        i52 = (nx + 1) >> 1;
        for (n = 0; n < i52; n++) {
          xtmp = x->data[n];
          nd2 = nx - n;
          x->data[n] = x->data[nd2];
          x->data[nd2] = xtmp;
        }

        if ((varargin_2->size[0] != 0) && (varargin_2->size[0] > 1)) {
          n = varargin_2->size[0] - 1;
          nd2 = varargin_2->size[0] >> 1;
          for (k = 0; k < nd2; k++) {
            xtmp = y->data[k];
            i52 = n - k;
            y->data[k] = y->data[i52];
            y->data[i52] = xtmp;
          }
        }
      }

      for (i52 = 0; i52 < 1800; i52++) {
        Vq[i52] = rtNaN;
      }

      for (i52 = 0; i52 < 1800; i52++) {
        dv3[i52] = 201.0 + static_cast<double>(i52);
      }

      interp1Linear(y, dv3, Vq, x);
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  emxFree_real_T(&x);
  emxFree_real_T(&y);
}

//
// File trailer for interp1.cpp
//
// [EOF]
//
