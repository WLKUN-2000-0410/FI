//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: selectitem_m.cpp
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
#include "analyseMixture_emxutil.h"
#include "sortrows.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *reslist
//                double d_data[]
//                int d_size[2]
//                double *flag_m
// Return Type  : void
//
void selectitem_m(const emxArray_real_T *reslist, double d_data[], int d_size[2],
                  double *flag_m)
{
  emxArray_real_T *sav;
  int i57;
  int loop_ub;
  emxArray_boolean_T *x;
  int k;
  int idx;
  boolean_T exitg1;
  int ii_data[1];
  unsigned int dl_data[1];
  emxInit_real_T(&sav, 2);
  *flag_m = 0.0;

  //   lcz：筛选候选物
  //  排序并筛选
  i57 = sav->size[0] * sav->size[1];
  sav->size[0] = reslist->size[0];
  sav->size[1] = 3;
  emxEnsureCapacity_real_T(sav, i57);
  loop_ub = reslist->size[0] * reslist->size[1];
  for (i57 = 0; i57 < loop_ub; i57++) {
    sav->data[i57] = reslist->data[i57];
  }

  emxInit_boolean_T(&x, 1);
  sortrows(sav);

  //      cc=sav(:,2);   % 将所有相似度提取出来
  //      a=find(cc>=C1,1,'last'); % a为候选物的个数，cc>C1表示相似度大于均值与标准差之和 
  loop_ub = sav->size[0];
  i57 = x->size[0];
  x->size[0] = loop_ub;
  emxEnsureCapacity_boolean_T(x, i57);
  for (i57 = 0; i57 < loop_ub; i57++) {
    x->data[i57] = (sav->data[i57 + sav->size[0]] > 0.5);
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

  for (i57 = 0; i57 < k; i57++) {
    dl_data[0] = static_cast<unsigned int>(ii_data[0]);
  }

  if (k == 0) {
    d_size[0] = 0;
    d_size[1] = 0;
    *flag_m = 1.0;
  } else if (static_cast<int>(dl_data[0]) > 40) {
    d_size[0] = 40;
    d_size[1] = 3;
    for (i57 = 0; i57 < 3; i57++) {
      for (idx = 0; idx < 40; idx++) {
        d_data[idx + 40 * i57] = sav->data[idx + sav->size[0] * i57];
      }
    }

    //  d为a个候选物对应的reslist
  } else {
    loop_ub = static_cast<int>(dl_data[0]);
    d_size[0] = static_cast<int>(dl_data[0]);
    d_size[1] = 3;
    for (i57 = 0; i57 < 3; i57++) {
      for (idx = 0; idx < loop_ub; idx++) {
        d_data[idx + loop_ub * i57] = sav->data[idx + sav->size[0] * i57];
      }
    }
  }

  emxFree_real_T(&sav);

  //      rel=d;
  //      d=sav(1:dl,:);   % d为个候选物对应的reslist
  //      re1=d;
}

//
// File trailer for selectitem_m.cpp
//
// [EOF]
//
