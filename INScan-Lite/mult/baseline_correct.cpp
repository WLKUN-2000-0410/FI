//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: baseline_correct.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include <cmath>
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
#include "circshift.h"
#include "log.h"
#include "sqrt.h"

// Function Definitions

//
// SNIP·½·¨
// Arguments    : const double b_signal[1800]
//                double baseline[1800]
// Return Type  : void
//
void baseline_correct(const double b_signal[1800], double baseline[1800])
{
  int i;
  double data[1800];
  double d0;
  int nx;
  int loop_ub;
  double dv1[1800];
  double dv2[1800];
  int r2_size_idx_0_tmp;
  short i18;
  int r2_data_tmp;
  double r2_data[1799];
  double working_spectra_data[1799];
  for (i = 0; i < 1800; i++) {
    d0 = b_signal[i];
    if (b_signal[i] < -1.0) {
      d0 = -1.0;
    }

    d0++;
    data[i] = d0;
  }

  b_sqrt(data);
  for (nx = 0; nx < 1800; nx++) {
    data[nx]++;
  }

  b_log(data);
  for (nx = 0; nx < 1800; nx++) {
    data[nx]++;
  }

  b_log(data);

  //  temp_spectra=zeros(n,1);
  for (i = 0; i < 35; i++) {
    memcpy(&dv1[0], &data[0], 1800U * sizeof(double));
    circshift(dv1, 1.0 + static_cast<double>(i));
    memcpy(&dv2[0], &data[0], 1800U * sizeof(double));
    circshift(dv2, -(1.0 + static_cast<double>(i)));
    nx = i + i;
    r2_size_idx_0_tmp = 1799 - nx;
    loop_ub = 1798 - nx;
    for (nx = 0; nx <= loop_ub; nx++) {
      r2_data_tmp = i + nx;
      r2_data[nx] = (dv1[r2_data_tmp] + dv2[r2_data_tmp]) / 2.0;
    }

    i18 = static_cast<short>((1799 - (i + i)));
    nx = i18;
    for (loop_ub = 0; loop_ub < nx; loop_ub++) {
      if ((data[i + loop_ub] < r2_data[loop_ub]) || rtIsNaN(r2_data[loop_ub])) {
        working_spectra_data[loop_ub] = data[i + loop_ub];
      } else {
        working_spectra_data[loop_ub] = r2_data[loop_ub];
      }
    }

    memcpy(&data[i], &working_spectra_data[0], (unsigned int)(r2_size_idx_0_tmp *
            static_cast<int>(sizeof(double))));

    //      baseline=(exp(exp(spectra_transformed)-1)-1).^2-1;
    //      part2=(data-baseline);
    //      d=part2(part2<0);
    //      if norm(d)<0.0000001*norm(data)
    //          break;
    //      end
  }

  for (loop_ub = 0; loop_ub < 1800; loop_ub++) {
    d0 = std::exp(std::exp(data[loop_ub]) - 1.0) - 1.0;
    data[loop_ub] = d0;
    baseline[loop_ub] = d0 * d0 - 1.0;
  }

  //  figure(1)
  //  plot(data)
  //  hold on
  //  plot(baseline)
  //  % plot(data-baseline,'linewidth',2)
  //  hold off
}

//
// File trailer for baseline_correct.cpp
//
// [EOF]
//
