//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: selectitem_p.cpp
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
#include "analyseMixture_emxutil.h"
#include "sortrows.h"

// Function Definitions

//
// lcz：筛选候选物
//  排序并筛选
//      sav=sortrows(reslist,2,'descend');
//      re1=sav(1:40,1);
// Arguments    : const emxArray_real_T *reslist
//                double rel_data[]
//                int rel_size[2]
//                double *flag_p
// Return Type  : void
//
void selectitem_p(const emxArray_real_T *reslist, double rel_data[], int
                  rel_size[2], double *flag_p)
{
  emxArray_real_T *sav;
  int i58;
  int loop_ub;
  emxArray_boolean_T *x;
  int k;
  int idx;
  boolean_T exitg1;
  int ii_data[1];
  unsigned int dl_data[1];
  double d_data[120];
  emxInit_real_T(&sav, 2);
  *flag_p = 0.0;
  i58 = sav->size[0] * sav->size[1];
  sav->size[0] = reslist->size[0];
  sav->size[1] = 3;
  emxEnsureCapacity_real_T(sav, i58);
  loop_ub = reslist->size[0] * reslist->size[1];
  for (i58 = 0; i58 < loop_ub; i58++) {
    sav->data[i58] = reslist->data[i58];
  }

  emxInit_boolean_T(&x, 1);
  sortrows(sav);
  loop_ub = sav->size[0];
  i58 = x->size[0];
  x->size[0] = loop_ub;
  emxEnsureCapacity_boolean_T(x, i58);
  for (i58 = 0; i58 < loop_ub; i58++) {
    x->data[i58] = (sav->data[i58 + sav->size[0]] > 0.5);
  }

  k = (1 <= x->size[0]);
  loop_ub = x->size[0];
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (loop_ub > 0)) {
    if (x->data[loop_ub - 1]) {
      idx = 1;
      ii_data[0] = loop_ub;
      exitg1 = true;
    } else {
      loop_ub--;
    }
  }

  emxFree_boolean_T(&x);
  if (k == 1) {
    if (idx == 0) {
      k = 0;
    }
  } else {
    k = (1 <= idx);
  }

  for (i58 = 0; i58 < k; i58++) {
    dl_data[0] = static_cast<unsigned int>(ii_data[0]);
  }

  if (k == 0) {
    rel_size[0] = 0;
    rel_size[1] = 0;
    *flag_p = 1.0;
  } else {
    if (static_cast<int>(dl_data[0]) > 40) {
      idx = 40;
      for (i58 = 0; i58 < 3; i58++) {
        for (k = 0; k < 40; k++) {
          d_data[k + 40 * i58] = sav->data[k + sav->size[0] * i58];
        }
      }

      //  d为a个候选物对应的reslist
    } else {
      loop_ub = static_cast<int>(dl_data[0]);
      idx = static_cast<int>(dl_data[0]);
      for (i58 = 0; i58 < 3; i58++) {
        for (k = 0; k < loop_ub; k++) {
          d_data[k + loop_ub * i58] = sav->data[k + sav->size[0] * i58];
        }
      }
    }

    rel_size[0] = idx;
    rel_size[1] = 3;
    loop_ub = idx * 3;
    if (0 <= loop_ub - 1) {
      memcpy(&rel_data[0], &d_data[0], (unsigned int)(loop_ub * static_cast<int>
              (sizeof(double))));
    }
  }

  emxFree_real_T(&sav);

  //      rel=sav(1:dl,:);   % d为个候选物对应的reslist
}

//
// File trailer for selectitem_p.cpp
//
// [EOF]
//
