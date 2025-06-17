//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: cal_snr.cpp
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
#include "sum.h"
#include "power.h"

// Function Definitions

//
// lcz:计算信噪比，为了做什么分析呢？
//  使用新的方法计算信噪比，先去基线，然后在进行计算
// Arguments    : const emxArray_real_T *data
// Return Type  : double
//
double cal_snr(const emxArray_real_T *data)
{
  double snr;
  emxArray_real_T *quji1;
  emxArray_real_T *c00;
  boolean_T guard1 = false;
  int n;
  int idx;
  double noiseenergy;
  int k;
  boolean_T exitg1;
  emxInit_real_T(&quji1, 1);
  autoestibl(data, quji1);

  //  subplot(212)
  //  plot(data)
  //
  //  % temp = PureFeatureExtract(data);
  //  hold on
  //  bg_line = data - data_quji;
  //  plot(bg_line,'r-');
  //  plot(data_quji);
  //  hold off
  emxInit_real_T(&c00, 1);
  guard1 = false;
  if (sum(data) == 0.0) {
    snr = 0.0;
  } else {
    n = data->size[0];
    if (data->size[0] <= 2) {
      if (data->size[0] == 1) {
        noiseenergy = data->data[0];
      } else if ((data->data[0] < data->data[1]) || (rtIsNaN(data->data[0]) && (
                   !rtIsNaN(data->data[1])))) {
        noiseenergy = data->data[1];
      } else {
        noiseenergy = data->data[0];
      }
    } else {
      if (!rtIsNaN(data->data[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= data->size[0])) {
          if (!rtIsNaN(data->data[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        noiseenergy = data->data[0];
      } else {
        noiseenergy = data->data[idx - 1];
        idx++;
        for (k = idx; k <= n; k++) {
          if (noiseenergy < data->data[k - 1]) {
            noiseenergy = data->data[k - 1];
          }
        }
      }
    }

    if (noiseenergy == 65535.0) {
      snr = 0.0;
    } else {
      n = data->size[0];
      if (data->size[0] <= 2) {
        if (data->size[0] == 1) {
          noiseenergy = data->data[0];
        } else if ((data->data[0] < data->data[1]) || (rtIsNaN(data->data[0]) &&
                    (!rtIsNaN(data->data[1])))) {
          noiseenergy = data->data[1];
        } else {
          noiseenergy = data->data[0];
        }
      } else {
        if (!rtIsNaN(data->data[0])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= data->size[0])) {
            if (!rtIsNaN(data->data[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          noiseenergy = data->data[0];
        } else {
          noiseenergy = data->data[idx - 1];
          idx++;
          for (k = idx; k <= n; k++) {
            if (noiseenergy < data->data[k - 1]) {
              noiseenergy = data->data[k - 1];
            }
          }
        }
      }

      if (noiseenergy < 2000.0) {
        snr = 0.0;
      } else {
        deri2(quji1, c00);

        //  二阶微分
        noiseenergy = c_cal_noiselevel(c00);

        //  计算噪声标准差
        //  总能量
        noiseenergy = (static_cast<double>(quji1->size[0]) - 1.0) * (noiseenergy
          * noiseenergy);

        //  噪声能量
        //  信号能量
        //  计算输出信噪比
        power(quji1, c00);
        snr = 10.0 * std::log10((sum(c00) - noiseenergy) / noiseenergy);
        if (snr < 0.0) {
          noiseenergy = std::abs(snr);
          if (noiseenergy < 6.0) {
            snr = noiseenergy;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }
  }

  if (guard1 && rtIsNaN(snr)) {
    snr = 0.0;
  }

  emxFree_real_T(&c00);
  emxFree_real_T(&quji1);
  return snr;
}

//
// File trailer for cal_snr.cpp
//
// [EOF]
//
