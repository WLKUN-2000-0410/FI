//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: zl_fullspectrumcontrast.cpp
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
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// 全谱比对函数，对数据库数据进行位移校正
// Arguments    : const double fix_data_2[1650]
//                const double fix_data1[1650]
//                double offset
// Return Type  : double
//
double zl_fullspectrumcontrast(const double fix_data_2[1650], const double
  fix_data1[1650], double offset)
{
  double res;
  emxArray_real_T *libr_s;
  double c;
  int ixlast;
  int k;
  double y;
  double scale;
  double b_y;
  double absxk;
  double t;
  emxInit_real_T(&libr_s, 1);
  offsetCorrect(fix_data_2, offset, libr_s);
  c = 0.0;
  ixlast = libr_s->size[0];
  for (k = 0; k < ixlast; k++) {
    c += libr_s->data[k] * fix_data1[k];
  }

  y = 0.0;
  if (libr_s->size[0] == 1) {
    y = std::abs(libr_s->data[0]);
  } else {
    scale = 3.3121686421112381E-170;
    ixlast = libr_s->size[0];
    for (k = 0; k < ixlast; k++) {
      absxk = std::abs(libr_s->data[k]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * std::sqrt(y);
  }

  emxFree_real_T(&libr_s);
  b_y = 0.0;
  scale = 3.3121686421112381E-170;
  for (k = 0; k < 1650; k++) {
    absxk = std::abs(fix_data1[k]);
    if (absxk > scale) {
      t = scale / absxk;
      b_y = 1.0 + b_y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      b_y += t * t;
    }
  }

  b_y = scale * std::sqrt(b_y);
  res = c / (y * b_y);
  if (res == 1.0) {
    res = 0.9999;
  }

  return res;
}

//
// File trailer for zl_fullspectrumcontrast.cpp
//
// [EOF]
//
