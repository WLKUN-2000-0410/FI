//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: findpeaks.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//

// Include Files
#include "rt_nonfinite.h"
#include <string.h>
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
#include "findpeaks.h"
#include "analyseMixture_emxutil.h"
#include "diff.h"
#include "colon.h"
#include "sort1.h"
#include "sortIdx.h"
#include "eml_setop.h"

// Function Declarations
static void assignFullOutputs(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *wxPk, const
  emxArray_real_T *bPk, emxArray_real_T *YpkOut, emxArray_real_T *XpkOut,
  emxArray_real_T *WpkOut, emxArray_real_T *PpkOut);
static void b_getLeftBase(const emxArray_real_T *yTemp, const emxArray_int32_T
  *iPeak, const emxArray_int32_T *iFinite, const emxArray_int32_T *iInflect,
  emxArray_int32_T *iBase, emxArray_int32_T *iSaddle);
static void c_findPeaksSeparatedByMoreThanM(const emxArray_real_T *y, const
  emxArray_real_T *x, const emxArray_int32_T *iPk, emxArray_int32_T *idx);
static void combineFullPeaks(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *bPk, const
  emxArray_int32_T *iLBw, const emxArray_int32_T *iRBw, const emxArray_real_T
  *wPk, const emxArray_int32_T *iInf, emxArray_int32_T *iPkOut, emxArray_real_T *
  bPkOut, emxArray_real_T *bxPkOut, emxArray_real_T *byPkOut, emxArray_real_T
  *wxPkOut);
static void findExtents(const double y[1650], emxArray_int32_T *iPk, const int
  iFin_data[], const int iFin_size[1], const int iInf_data[], const int
  iInf_size[1], const int iInflect_data[], const int iInflect_size[1], double
  minP, emxArray_real_T *bPk, emxArray_real_T *bxPk, emxArray_real_T *byPk,
  emxArray_real_T *wxPk);
static void findLeftIntercept(const double y[1650], int *idx, int borderIdx,
  double refHeight);
static void findRightIntercept(const double y[1650], int *idx, int borderIdx,
  double refHeight);
static void getAllPeaksCodegen(const emxArray_real_T *y, emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect);
static void getLeftBase(const double yTemp[1650], const int iPeak_data[], const
  int iPeak_size[1], const int iFinite_data[], const int iFinite_size[1], const
  int iInflect_data[], int iBase_data[], int iBase_size[1], int iSaddle_data[],
  int iSaddle_size[1]);
static void getPeakBase(const emxArray_real_T *yTemp, const emxArray_int32_T
  *iPk, const emxArray_int32_T *iFin, const emxArray_int32_T *iInflect,
  emxArray_real_T *peakBase, emxArray_int32_T *iLeftSaddle, emxArray_int32_T
  *iRightSaddle);
static void getPeakWidth(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *pbPk, emxArray_int32_T
  *iLB, emxArray_int32_T *iRB, emxArray_real_T *wxPk);
static double linterp(double xa, double xb, double ya, double yb, double yc,
                      double bc);
static void removeSmallPeaks(const emxArray_real_T *y, const emxArray_int32_T
  *iFinite, double minH, emxArray_int32_T *iPk);

// Function Definitions

//
// Arguments    : const emxArray_real_T *y
//                const emxArray_real_T *x
//                const emxArray_int32_T *iPk
//                const emxArray_real_T *wxPk
//                const emxArray_real_T *bPk
//                emxArray_real_T *YpkOut
//                emxArray_real_T *XpkOut
//                emxArray_real_T *WpkOut
//                emxArray_real_T *PpkOut
// Return Type  : void
//
static void assignFullOutputs(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *wxPk, const
  emxArray_real_T *bPk, emxArray_real_T *YpkOut, emxArray_real_T *XpkOut,
  emxArray_real_T *WpkOut, emxArray_real_T *PpkOut)
{
  int i50;
  int loop_ub;
  i50 = YpkOut->size[0];
  YpkOut->size[0] = iPk->size[0];
  emxEnsureCapacity_real_T(YpkOut, i50);
  loop_ub = iPk->size[0];
  for (i50 = 0; i50 < loop_ub; i50++) {
    YpkOut->data[i50] = y->data[iPk->data[i50] - 1];
  }

  i50 = XpkOut->size[0];
  XpkOut->size[0] = iPk->size[0];
  emxEnsureCapacity_real_T(XpkOut, i50);
  loop_ub = iPk->size[0];
  for (i50 = 0; i50 < loop_ub; i50++) {
    XpkOut->data[i50] = x->data[iPk->data[i50] - 1];
  }

  c_diff(wxPk, WpkOut);
  i50 = PpkOut->size[0];
  PpkOut->size[0] = YpkOut->size[0];
  emxEnsureCapacity_real_T(PpkOut, i50);
  loop_ub = YpkOut->size[0];
  for (i50 = 0; i50 < loop_ub; i50++) {
    PpkOut->data[i50] = YpkOut->data[i50] - bPk->data[i50];
  }
}

//
// Arguments    : const emxArray_real_T *yTemp
//                const emxArray_int32_T *iPeak
//                const emxArray_int32_T *iFinite
//                const emxArray_int32_T *iInflect
//                emxArray_int32_T *iBase
//                emxArray_int32_T *iSaddle
// Return Type  : void
//
static void b_getLeftBase(const emxArray_real_T *yTemp, const emxArray_int32_T
  *iPeak, const emxArray_int32_T *iFinite, const emxArray_int32_T *iInflect,
  emxArray_int32_T *iBase, emxArray_int32_T *iSaddle)
{
  unsigned int unnamed_idx_0;
  int n;
  int i;
  emxArray_real_T *peak;
  emxArray_real_T *valley;
  emxArray_int32_T *iValley;
  int j;
  int k;
  double v;
  int iv;
  double p;
  int isv;
  unnamed_idx_0 = static_cast<unsigned int>(iPeak->size[0]);
  n = iBase->size[0];
  iBase->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_int32_T(iBase, n);
  i = static_cast<int>(unnamed_idx_0);
  for (n = 0; n < i; n++) {
    iBase->data[n] = 0;
  }

  unnamed_idx_0 = static_cast<unsigned int>(iPeak->size[0]);
  n = iSaddle->size[0];
  iSaddle->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_int32_T(iSaddle, n);
  i = static_cast<int>(unnamed_idx_0);
  for (n = 0; n < i; n++) {
    iSaddle->data[n] = 0;
  }

  emxInit_real_T(&peak, 1);
  unnamed_idx_0 = static_cast<unsigned int>(iFinite->size[0]);
  n = peak->size[0];
  peak->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(peak, n);
  i = static_cast<int>(unnamed_idx_0);
  for (n = 0; n < i; n++) {
    peak->data[n] = 0.0;
  }

  emxInit_real_T(&valley, 1);
  unnamed_idx_0 = static_cast<unsigned int>(iFinite->size[0]);
  n = valley->size[0];
  valley->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(valley, n);
  i = static_cast<int>(unnamed_idx_0);
  for (n = 0; n < i; n++) {
    valley->data[n] = 0.0;
  }

  emxInit_int32_T(&iValley, 1);
  unnamed_idx_0 = static_cast<unsigned int>(iFinite->size[0]);
  n = iValley->size[0];
  iValley->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_int32_T(iValley, n);
  i = static_cast<int>(unnamed_idx_0);
  for (n = 0; n < i; n++) {
    iValley->data[n] = 0;
  }

  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak->size[0]) {
    while (iInflect->data[i] != iFinite->data[j]) {
      v = yTemp->data[iInflect->data[i] - 1];
      iv = iInflect->data[i];
      if (rtIsNaN(yTemp->data[iInflect->data[i] - 1])) {
        n = -1;
      } else {
        while ((n + 1 > 0) && (valley->data[n] > v)) {
          n--;
        }
      }

      i++;
    }

    p = yTemp->data[iInflect->data[i] - 1];
    while ((n + 1 > 0) && (peak->data[n] < p)) {
      if (valley->data[n] < v) {
        v = valley->data[n];
        iv = iValley->data[n];
      }

      n--;
    }

    isv = iv;
    while ((n + 1 > 0) && (peak->data[n] <= p)) {
      if (valley->data[n] < v) {
        v = valley->data[n];
        iv = iValley->data[n];
      }

      n--;
    }

    n++;
    peak->data[n] = yTemp->data[iInflect->data[i] - 1];
    valley->data[n] = v;
    iValley->data[n] = iv;
    if (iInflect->data[i] == iPeak->data[k]) {
      iBase->data[k] = iv;
      iSaddle->data[k] = isv;
      k++;
    }

    i++;
    j++;
  }

  emxFree_int32_T(&iValley);
  emxFree_real_T(&valley);
  emxFree_real_T(&peak);
}

//
// Arguments    : const emxArray_real_T *y
//                const emxArray_real_T *x
//                const emxArray_int32_T *iPk
//                emxArray_int32_T *idx
// Return Type  : void
//
static void c_findPeaksSeparatedByMoreThanM(const emxArray_real_T *y, const
  emxArray_real_T *x, const emxArray_int32_T *iPk, emxArray_int32_T *idx)
{
  emxArray_real_T *locs;
  emxArray_int32_T *r12;
  int i49;
  int loop_ub;
  emxArray_real_T *locs_temp;
  emxArray_int32_T *d_sortIdx;
  emxArray_boolean_T *idelete;
  unsigned int unnamed_idx_0;
  emxArray_boolean_T *r13;
  int i;
  double b_locs;
  double c_locs;
  int end;
  emxArray_int32_T *r14;
  if (iPk->size[0] == 0) {
    emxInit_int32_T(&r12, 2);
    eml_signed_integer_colon(0, r12);
    i49 = idx->size[0];
    idx->size[0] = r12->size[1];
    emxEnsureCapacity_int32_T(idx, i49);
    loop_ub = r12->size[1];
    for (i49 = 0; i49 < loop_ub; i49++) {
      idx->data[i49] = r12->data[i49];
    }

    emxFree_int32_T(&r12);
  } else {
    emxInit_real_T(&locs, 1);
    i49 = locs->size[0];
    locs->size[0] = iPk->size[0];
    emxEnsureCapacity_real_T(locs, i49);
    loop_ub = iPk->size[0];
    for (i49 = 0; i49 < loop_ub; i49++) {
      locs->data[i49] = x->data[iPk->data[i49] - 1];
    }

    emxInit_real_T(&locs_temp, 1);
    i49 = locs_temp->size[0];
    locs_temp->size[0] = iPk->size[0];
    emxEnsureCapacity_real_T(locs_temp, i49);
    loop_ub = iPk->size[0];
    for (i49 = 0; i49 < loop_ub; i49++) {
      locs_temp->data[i49] = y->data[iPk->data[i49] - 1];
    }

    emxInit_int32_T(&d_sortIdx, 1);
    sortIdx(locs_temp, d_sortIdx);
    i49 = locs_temp->size[0];
    locs_temp->size[0] = d_sortIdx->size[0];
    emxEnsureCapacity_real_T(locs_temp, i49);
    loop_ub = d_sortIdx->size[0];
    for (i49 = 0; i49 < loop_ub; i49++) {
      locs_temp->data[i49] = locs->data[d_sortIdx->data[i49] - 1];
    }

    emxInit_boolean_T(&idelete, 1);
    unnamed_idx_0 = static_cast<unsigned int>(d_sortIdx->size[0]);
    i49 = idelete->size[0];
    idelete->size[0] = static_cast<int>(unnamed_idx_0);
    emxEnsureCapacity_boolean_T(idelete, i49);
    loop_ub = static_cast<int>(unnamed_idx_0);
    for (i49 = 0; i49 < loop_ub; i49++) {
      idelete->data[i49] = false;
    }

    i49 = d_sortIdx->size[0];
    emxInit_boolean_T(&r13, 1);
    for (i = 0; i < i49; i++) {
      if (!idelete->data[i]) {
        b_locs = locs->data[d_sortIdx->data[i] - 1] - 5.0;
        c_locs = locs->data[d_sortIdx->data[i] - 1] + 5.0;
        end = r13->size[0];
        r13->size[0] = locs_temp->size[0];
        emxEnsureCapacity_boolean_T(r13, end);
        loop_ub = locs_temp->size[0];
        for (end = 0; end < loop_ub; end++) {
          r13->data[end] = ((locs_temp->data[end] >= b_locs) && (locs_temp->
            data[end] <= c_locs));
        }

        end = idelete->size[0];
        emxEnsureCapacity_boolean_T(idelete, end);
        loop_ub = idelete->size[0];
        for (end = 0; end < loop_ub; end++) {
          idelete->data[end] = (idelete->data[end] || r13->data[end]);
        }

        idelete->data[i] = false;
      }
    }

    emxFree_boolean_T(&r13);
    emxFree_real_T(&locs_temp);
    emxFree_real_T(&locs);
    end = idelete->size[0] - 1;
    loop_ub = 0;
    for (i = 0; i <= end; i++) {
      if (!idelete->data[i]) {
        loop_ub++;
      }
    }

    emxInit_int32_T(&r14, 1);
    i49 = r14->size[0];
    r14->size[0] = loop_ub;
    emxEnsureCapacity_int32_T(r14, i49);
    loop_ub = 0;
    for (i = 0; i <= end; i++) {
      if (!idelete->data[i]) {
        r14->data[loop_ub] = i + 1;
        loop_ub++;
      }
    }

    emxFree_boolean_T(&idelete);
    i49 = idx->size[0];
    idx->size[0] = r14->size[0];
    emxEnsureCapacity_int32_T(idx, i49);
    loop_ub = r14->size[0];
    for (i49 = 0; i49 < loop_ub; i49++) {
      idx->data[i49] = d_sortIdx->data[r14->data[i49] - 1];
    }

    emxFree_int32_T(&r14);
    emxFree_int32_T(&d_sortIdx);
    b_sort(idx);
  }
}

//
// Arguments    : const emxArray_real_T *y
//                const emxArray_real_T *x
//                const emxArray_int32_T *iPk
//                const emxArray_real_T *bPk
//                const emxArray_int32_T *iLBw
//                const emxArray_int32_T *iRBw
//                const emxArray_real_T *wPk
//                const emxArray_int32_T *iInf
//                emxArray_int32_T *iPkOut
//                emxArray_real_T *bPkOut
//                emxArray_real_T *bxPkOut
//                emxArray_real_T *byPkOut
//                emxArray_real_T *wxPkOut
// Return Type  : void
//
static void combineFullPeaks(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *bPk, const
  emxArray_int32_T *iLBw, const emxArray_int32_T *iRBw, const emxArray_real_T
  *wPk, const emxArray_int32_T *iInf, emxArray_int32_T *iPkOut, emxArray_real_T *
  bPkOut, emxArray_real_T *bxPkOut, emxArray_real_T *byPkOut, emxArray_real_T
  *wxPkOut)
{
  emxArray_int32_T *iInfL;
  emxArray_int32_T *iInfR;
  emxArray_int32_T *iFinite;
  emxArray_int32_T *ia;
  int nx;
  int loop_ub;
  emxArray_int32_T *iInfinite;
  unsigned int ia_idx_0;
  int varargin_2;
  boolean_T p;
  emxInit_int32_T(&iInfL, 1);
  emxInit_int32_T(&iInfR, 1);
  emxInit_int32_T(&iFinite, 1);
  emxInit_int32_T(&ia, 1);
  do_vectors(iPk, iInf, iPkOut, ia, iInfR);
  b_do_vectors(iPkOut, iPk, iInfL, ia, iInfR);
  nx = iFinite->size[0];
  iFinite->size[0] = ia->size[0];
  emxEnsureCapacity_int32_T(iFinite, nx);
  loop_ub = ia->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    iFinite->data[nx] = ia->data[nx];
  }

  emxInit_int32_T(&iInfinite, 1);
  b_do_vectors(iPkOut, iInf, iInfL, ia, iInfR);
  nx = iInfinite->size[0];
  iInfinite->size[0] = ia->size[0];
  emxEnsureCapacity_int32_T(iInfinite, nx);
  loop_ub = ia->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    iInfinite->data[nx] = ia->data[nx];
  }

  ia_idx_0 = static_cast<unsigned int>(iPkOut->size[0]);
  nx = bPkOut->size[0];
  bPkOut->size[0] = static_cast<int>(ia_idx_0);
  emxEnsureCapacity_real_T(bPkOut, nx);
  loop_ub = static_cast<int>(ia_idx_0);
  for (nx = 0; nx < loop_ub; nx++) {
    bPkOut->data[nx] = 0.0;
  }

  loop_ub = bPk->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bPkOut->data[iFinite->data[nx] - 1] = bPk->data[nx];
  }

  loop_ub = ia->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bPkOut->data[ia->data[nx] - 1] = 0.0;
  }

  nx = ia->size[0];
  ia->size[0] = iInf->size[0];
  emxEnsureCapacity_int32_T(ia, nx);
  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    ia->data[nx] = iInf->data[nx] - 1;
  }

  ia_idx_0 = static_cast<unsigned int>(ia->size[0]);
  nx = iInfL->size[0];
  iInfL->size[0] = static_cast<int>(ia_idx_0);
  emxEnsureCapacity_int32_T(iInfL, nx);
  ia_idx_0 = static_cast<unsigned int>(ia->size[0]);
  nx = static_cast<int>(ia_idx_0);
  for (loop_ub = 0; loop_ub < nx; loop_ub++) {
    if (1 < ia->data[loop_ub]) {
      iInfL->data[loop_ub] = ia->data[loop_ub];
    } else {
      iInfL->data[loop_ub] = 1;
    }
  }

  varargin_2 = x->size[0];
  nx = ia->size[0];
  ia->size[0] = iInf->size[0];
  emxEnsureCapacity_int32_T(ia, nx);
  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    ia->data[nx] = iInf->data[nx] + 1;
  }

  ia_idx_0 = static_cast<unsigned int>(ia->size[0]);
  nx = iInfR->size[0];
  iInfR->size[0] = static_cast<int>(ia_idx_0);
  emxEnsureCapacity_int32_T(iInfR, nx);
  ia_idx_0 = static_cast<unsigned int>(ia->size[0]);
  nx = static_cast<int>(ia_idx_0);
  for (loop_ub = 0; loop_ub < nx; loop_ub++) {
    p = (ia->data[loop_ub] > varargin_2);
    if (p) {
      iInfR->data[loop_ub] = varargin_2;
    } else {
      iInfR->data[loop_ub] = ia->data[loop_ub];
    }
  }

  emxFree_int32_T(&ia);
  nx = bxPkOut->size[0] * bxPkOut->size[1];
  bxPkOut->size[0] = iPkOut->size[0];
  bxPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(bxPkOut, nx);
  loop_ub = iPkOut->size[0] << 1;
  for (nx = 0; nx < loop_ub; nx++) {
    bxPkOut->data[nx] = 0.0;
  }

  loop_ub = iLBw->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bxPkOut->data[iFinite->data[nx] - 1] = x->data[iLBw->data[nx] - 1];
  }

  loop_ub = iRBw->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bxPkOut->data[(iFinite->data[nx] + bxPkOut->size[0]) - 1] = x->data
      [iRBw->data[nx] - 1];
  }

  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bxPkOut->data[iInfinite->data[nx] - 1] = 0.5 * (x->data[iInf->data[nx] - 1]
      + x->data[iInfL->data[nx] - 1]);
  }

  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    bxPkOut->data[(iInfinite->data[nx] + bxPkOut->size[0]) - 1] = 0.5 * (x->
      data[iInf->data[nx] - 1] + x->data[iInfR->data[nx] - 1]);
  }

  nx = byPkOut->size[0] * byPkOut->size[1];
  byPkOut->size[0] = iPkOut->size[0];
  byPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(byPkOut, nx);
  loop_ub = iPkOut->size[0] << 1;
  for (nx = 0; nx < loop_ub; nx++) {
    byPkOut->data[nx] = 0.0;
  }

  loop_ub = iLBw->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    byPkOut->data[iFinite->data[nx] - 1] = y->data[iLBw->data[nx] - 1];
  }

  loop_ub = iRBw->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    byPkOut->data[(iFinite->data[nx] + byPkOut->size[0]) - 1] = y->data
      [iRBw->data[nx] - 1];
  }

  loop_ub = iInfL->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    byPkOut->data[iInfinite->data[nx] - 1] = y->data[iInfL->data[nx] - 1];
  }

  loop_ub = iInfR->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    byPkOut->data[(iInfinite->data[nx] + byPkOut->size[0]) - 1] = y->data
      [iInfR->data[nx] - 1];
  }

  nx = wxPkOut->size[0] * wxPkOut->size[1];
  wxPkOut->size[0] = iPkOut->size[0];
  wxPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(wxPkOut, nx);
  loop_ub = iPkOut->size[0] << 1;
  for (nx = 0; nx < loop_ub; nx++) {
    wxPkOut->data[nx] = 0.0;
  }

  loop_ub = wPk->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    wxPkOut->data[iFinite->data[nx] - 1] = wPk->data[nx];
  }

  loop_ub = wPk->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    wxPkOut->data[(iFinite->data[nx] + wxPkOut->size[0]) - 1] = wPk->data[nx +
      wPk->size[0]];
  }

  emxFree_int32_T(&iFinite);
  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    wxPkOut->data[iInfinite->data[nx] - 1] = 0.5 * (x->data[iInf->data[nx] - 1]
      + x->data[iInfL->data[nx] - 1]);
  }

  emxFree_int32_T(&iInfL);
  loop_ub = iInf->size[0];
  for (nx = 0; nx < loop_ub; nx++) {
    wxPkOut->data[(iInfinite->data[nx] + wxPkOut->size[0]) - 1] = 0.5 * (x->
      data[iInf->data[nx] - 1] + x->data[iInfR->data[nx] - 1]);
  }

  emxFree_int32_T(&iInfinite);
  emxFree_int32_T(&iInfR);
}

//
// Arguments    : const double y[1650]
//                emxArray_int32_T *iPk
//                const int iFin_data[]
//                const int iFin_size[1]
//                const int iInf_data[]
//                const int iInf_size[1]
//                const int iInflect_data[]
//                const int iInflect_size[1]
//                double minP
//                emxArray_real_T *bPk
//                emxArray_real_T *bxPk
//                emxArray_real_T *byPk
//                emxArray_real_T *wxPk
// Return Type  : void
//
static void findExtents(const double y[1650], emxArray_int32_T *iPk, const int
  iFin_data[], const int iFin_size[1], const int iInf_data[], const int
  iInf_size[1], const int iInflect_data[], const int iInflect_size[1], double
  minP, emxArray_real_T *bPk, emxArray_real_T *bxPk, emxArray_real_T *byPk,
  emxArray_real_T *wxPk)
{
  double yFinite[1650];
  int m;
  int i78;
  emxArray_int32_T *ii;
  int iLeftBase_data[1650];
  int iLeftBase_size[1];
  int iInfR_data[1650];
  int iInfR_size[1];
  int md2;
  int i;
  emxArray_int32_T *x;
  int xtmp;
  emxArray_int32_T *b_x;
  int iRightBase_data[1650];
  int iRightSaddle_data[1650];
  int iRightSaddle_size[1];
  short csz_idx_0;
  emxArray_real_T *base;
  emxArray_boolean_T *c_x;
  boolean_T exitg1;
  emxArray_int32_T *iLB;
  emxArray_int32_T *iRB;
  emxArray_real_T *b_wxPk;
  emxArray_int32_T *c;
  double refHeight;
  emxArray_int32_T b_iInf_data;
  emxArray_int32_T c_iInf_data;
  emxArray_real_T *b_bPk;
  unsigned int unnamed_idx_0;
  memcpy(&yFinite[0], &y[0], 1650U * sizeof(double));
  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    yFinite[iInf_data[i78] - 1] = rtNaN;
  }

  emxInit_int32_T(&ii, 1);
  getLeftBase(yFinite, iPk->data, iPk->size, iFin_data, iFin_size, iInflect_data,
              iLeftBase_data, iLeftBase_size, iInfR_data, iInfR_size);
  i78 = ii->size[0];
  ii->size[0] = iPk->size[0];
  emxEnsureCapacity_int32_T(ii, i78);
  m = iPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    ii->data[i78] = iPk->data[i78];
  }

  m = iPk->size[0] - 1;
  md2 = iPk->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = ii->data[i];
    i78 = m - i;
    ii->data[i] = ii->data[i78];
    ii->data[i78] = xtmp;
  }

  emxInit_int32_T(&x, 1);
  i78 = x->size[0];
  x->size[0] = iFin_size[0];
  emxEnsureCapacity_int32_T(x, i78);
  m = iFin_size[0];
  for (i78 = 0; i78 < m; i78++) {
    x->data[i78] = iFin_data[i78];
  }

  m = iFin_size[0] - 1;
  md2 = iFin_size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = x->data[i];
    i78 = m - i;
    x->data[i] = x->data[i78];
    x->data[i78] = xtmp;
  }

  emxInit_int32_T(&b_x, 1);
  i78 = b_x->size[0];
  b_x->size[0] = iInflect_size[0];
  emxEnsureCapacity_int32_T(b_x, i78);
  m = iInflect_size[0];
  for (i78 = 0; i78 < m; i78++) {
    b_x->data[i78] = iInflect_data[i78];
  }

  m = iInflect_size[0] - 1;
  md2 = iInflect_size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = b_x->data[i];
    i78 = m - i;
    b_x->data[i] = b_x->data[i78];
    b_x->data[i78] = xtmp;
  }

  getLeftBase(yFinite, ii->data, ii->size, x->data, x->size, b_x->data,
              iRightBase_data, iInfR_size, iRightSaddle_data, iRightSaddle_size);
  i78 = ii->size[0];
  ii->size[0] = iInfR_size[0];
  emxEnsureCapacity_int32_T(ii, i78);
  m = iInfR_size[0];
  for (i78 = 0; i78 < m; i78++) {
    ii->data[i78] = iRightBase_data[i78];
  }

  m = iInfR_size[0] - 1;
  md2 = iInfR_size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = ii->data[i];
    i78 = m - i;
    ii->data[i] = ii->data[i78];
    ii->data[i78] = xtmp;
  }

  i78 = x->size[0];
  x->size[0] = iRightSaddle_size[0];
  emxEnsureCapacity_int32_T(x, i78);
  m = iRightSaddle_size[0];
  for (i78 = 0; i78 < m; i78++) {
    x->data[i78] = iRightSaddle_data[i78];
  }

  m = iRightSaddle_size[0] - 1;
  md2 = iRightSaddle_size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = x->data[i];
    i78 = m - i;
    x->data[i] = x->data[i78];
    x->data[i78] = xtmp;
  }

  if (iLeftBase_size[0] <= ii->size[0]) {
    csz_idx_0 = static_cast<short>(iLeftBase_size[0]);
  } else {
    csz_idx_0 = static_cast<short>(ii->size[0]);
  }

  emxInit_real_T(&base, 1);
  i78 = base->size[0];
  base->size[0] = csz_idx_0;
  emxEnsureCapacity_real_T(base, i78);
  xtmp = csz_idx_0;
  for (m = 0; m < xtmp; m++) {
    if ((yFinite[iLeftBase_data[m] - 1] > yFinite[ii->data[m] - 1]) || rtIsNaN
        (yFinite[ii->data[m] - 1])) {
      base->data[m] = yFinite[iLeftBase_data[m] - 1];
    } else {
      base->data[m] = yFinite[ii->data[m] - 1];
    }
  }

  emxInit_boolean_T(&c_x, 1);
  i78 = c_x->size[0];
  c_x->size[0] = iPk->size[0];
  emxEnsureCapacity_boolean_T(c_x, i78);
  m = iPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    c_x->data[i78] = (yFinite[iPk->data[i78] - 1] - base->data[i78] >= minP);
  }

  xtmp = c_x->size[0];
  md2 = 0;
  i78 = ii->size[0];
  ii->size[0] = c_x->size[0];
  emxEnsureCapacity_int32_T(ii, i78);
  m = 0;
  exitg1 = false;
  while ((!exitg1) && (m <= xtmp - 1)) {
    if (c_x->data[m]) {
      md2++;
      ii->data[md2 - 1] = m + 1;
      if (md2 >= xtmp) {
        exitg1 = true;
      } else {
        m++;
      }
    } else {
      m++;
    }
  }

  if (c_x->size[0] == 1) {
    if (md2 == 0) {
      ii->size[0] = 0;
    }
  } else if (1 > md2) {
    ii->size[0] = 0;
  } else {
    i78 = ii->size[0];
    ii->size[0] = md2;
    emxEnsureCapacity_int32_T(ii, i78);
  }

  emxFree_boolean_T(&c_x);
  i78 = b_x->size[0];
  b_x->size[0] = ii->size[0];
  emxEnsureCapacity_int32_T(b_x, i78);
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    b_x->data[i78] = iPk->data[ii->data[i78] - 1];
  }

  i78 = iPk->size[0];
  iPk->size[0] = b_x->size[0];
  emxEnsureCapacity_int32_T(iPk, i78);
  m = b_x->size[0];
  for (i78 = 0; i78 < m; i78++) {
    iPk->data[i78] = b_x->data[i78];
  }

  i78 = bPk->size[0];
  bPk->size[0] = ii->size[0];
  emxEnsureCapacity_real_T(bPk, i78);
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    bPk->data[i78] = base->data[ii->data[i78] - 1];
  }

  emxInit_int32_T(&iLB, 1);
  i78 = iLB->size[0];
  iLB->size[0] = ii->size[0];
  emxEnsureCapacity_int32_T(iLB, i78);
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    iLB->data[i78] = iInfR_data[ii->data[i78] - 1];
  }

  emxInit_int32_T(&iRB, 1);
  i78 = iRB->size[0];
  iRB->size[0] = ii->size[0];
  emxEnsureCapacity_int32_T(iRB, i78);
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    iRB->data[i78] = x->data[ii->data[i78] - 1];
  }

  if (iPk->size[0] == 0) {
    base->size[0] = 0;
    iLB->size[0] = 0;
    iRB->size[0] = 0;
  } else {
    i78 = base->size[0];
    base->size[0] = bPk->size[0];
    emxEnsureCapacity_real_T(base, i78);
    m = bPk->size[0];
    for (i78 = 0; i78 < m; i78++) {
      base->data[i78] = bPk->data[i78];
    }
  }

  emxInit_real_T(&b_wxPk, 2);
  i78 = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = iPk->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(b_wxPk, i78);
  m = iPk->size[0] << 1;
  for (i78 = 0; i78 < m; i78++) {
    b_wxPk->data[i78] = 0.0;
  }

  i78 = iPk->size[0];
  for (i = 0; i < i78; i++) {
    refHeight = (yFinite[iPk->data[i] - 1] + base->data[i]) / 2.0;
    m = iPk->data[i];
    findLeftIntercept(yFinite, &m, iLB->data[i], refHeight);
    if (m < iLB->data[i]) {
      b_wxPk->data[i] = 1.0 + (static_cast<double>(iLB->data[i]) - 1.0);
    } else {
      b_wxPk->data[i] = linterp(1.0 + (static_cast<double>(m) - 1.0), 1.0 +
        (static_cast<double>((m + 1)) - 1.0), yFinite[m - 1], yFinite[m],
        yFinite[iPk->data[i] - 1], base->data[i]);
    }

    m = iPk->data[i];
    findRightIntercept(yFinite, &m, iRB->data[i], refHeight);
    if (m > iRB->data[i]) {
      b_wxPk->data[i + b_wxPk->size[0]] = 1.0 + (static_cast<double>(iRB->data[i])
        - 1.0);
    } else {
      b_wxPk->data[i + b_wxPk->size[0]] = linterp(1.0 + (static_cast<double>(m)
        - 1.0), 1.0 + (static_cast<double>((m - 1)) - 1.0), yFinite[m - 1],
        yFinite[m - 2], yFinite[iPk->data[i] - 1], base->data[i]);
    }
  }

  emxInit_int32_T(&c, 1);
  b_iInf_data.data = const_cast<int *>(&iInf_data[0]);
  b_iInf_data.size = const_cast<int *>(&iInf_size[0]);
  b_iInf_data.allocatedSize = -1;
  b_iInf_data.numDimensions = 1;
  b_iInf_data.canFreeData = false;
  do_vectors(iPk, &b_iInf_data, c, ii, b_x);
  b_do_vectors(c, iPk, x, ii, b_x);
  i78 = base->size[0];
  base->size[0] = ii->size[0];
  emxEnsureCapacity_real_T(base, i78);
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    base->data[i78] = ii->data[i78];
  }

  c_iInf_data.data = const_cast<int *>(&iInf_data[0]);
  c_iInf_data.size = const_cast<int *>(&iInf_size[0]);
  c_iInf_data.allocatedSize = -1;
  c_iInf_data.numDimensions = 1;
  c_iInf_data.canFreeData = false;
  b_do_vectors(c, &c_iInf_data, x, ii, b_x);
  m = ii->size[0];
  emxFree_int32_T(&x);
  for (i78 = 0; i78 < m; i78++) {
    iRightBase_data[i78] = ii->data[i78];
  }

  emxInit_real_T(&b_bPk, 1);
  unnamed_idx_0 = static_cast<unsigned int>(c->size[0]);
  i78 = b_bPk->size[0];
  b_bPk->size[0] = static_cast<int>(unnamed_idx_0);
  emxEnsureCapacity_real_T(b_bPk, i78);
  m = static_cast<int>(unnamed_idx_0);
  for (i78 = 0; i78 < m; i78++) {
    b_bPk->data[i78] = 0.0;
  }

  m = bPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    b_bPk->data[static_cast<int>(base->data[i78]) - 1] = bPk->data[i78];
  }

  iLeftBase_size[0] = ii->size[0];
  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    iLeftBase_data[i78] = ii->data[i78];
  }

  m = iLeftBase_size[0];
  for (i78 = 0; i78 < m; i78++) {
    b_bPk->data[iLeftBase_data[i78] - 1] = 0.0;
  }

  i78 = ii->size[0];
  ii->size[0] = iInf_size[0];
  emxEnsureCapacity_int32_T(ii, i78);
  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    ii->data[i78] = iInf_data[i78] - 1;
  }

  csz_idx_0 = static_cast<short>(ii->size[0]);
  i78 = b_x->size[0];
  b_x->size[0] = csz_idx_0;
  emxEnsureCapacity_int32_T(b_x, i78);
  csz_idx_0 = static_cast<short>(ii->size[0]);
  xtmp = csz_idx_0;
  for (m = 0; m < xtmp; m++) {
    if (1 < ii->data[m]) {
      b_x->data[m] = ii->data[m];
    } else {
      b_x->data[m] = 1;
    }
  }

  iLeftBase_size[0] = iInf_size[0];
  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    iLeftBase_data[i78] = iInf_data[i78] + 1;
  }

  i78 = ii->size[0];
  ii->size[0] = static_cast<short>(iLeftBase_size[0]);
  emxEnsureCapacity_int32_T(ii, i78);
  m = static_cast<short>(iLeftBase_size[0]);
  for (i78 = 0; i78 < m; i78++) {
    ii->data[i78] = iInfR_data[i78];
  }

  xtmp = static_cast<short>(iLeftBase_size[0]);
  for (m = 0; m < xtmp; m++) {
    if (static_cast<short>(iLeftBase_data[m]) > 1650) {
      ii->data[m] = 1650;
    } else {
      ii->data[m] = static_cast<short>(iLeftBase_data[m]);
    }
  }

  i78 = bxPk->size[0] * bxPk->size[1];
  bxPk->size[0] = c->size[0];
  bxPk->size[1] = 2;
  emxEnsureCapacity_real_T(bxPk, i78);
  m = c->size[0] << 1;
  for (i78 = 0; i78 < m; i78++) {
    bxPk->data[i78] = 0.0;
  }

  m = iLB->size[0];
  for (i78 = 0; i78 < m; i78++) {
    bxPk->data[static_cast<int>(base->data[i78]) - 1] = static_cast<short>((1 +
      static_cast<short>((iLB->data[i78] - 1))));
  }

  m = iRB->size[0];
  for (i78 = 0; i78 < m; i78++) {
    bxPk->data[(static_cast<int>(base->data[i78]) + bxPk->size[0]) - 1] =
      static_cast<short>((1 + static_cast<short>((iRB->data[i78] - 1))));
  }

  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    bxPk->data[iRightBase_data[i78] - 1] = 0.5 * static_cast<double>(static_cast<
      short>((static_cast<short>((static_cast<short>((iInf_data[i78] - 1)) +
      static_cast<short>((b_x->data[i78] - 1)))) + 2)));
  }

  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    bxPk->data[(iRightBase_data[i78] + bxPk->size[0]) - 1] = 0.5 * static_cast<
      double>(static_cast<short>((static_cast<short>((static_cast<short>
      ((iInf_data[i78] - 1)) + static_cast<short>((ii->data[i78] - 1)))) + 2)));
  }

  i78 = byPk->size[0] * byPk->size[1];
  byPk->size[0] = c->size[0];
  byPk->size[1] = 2;
  emxEnsureCapacity_real_T(byPk, i78);
  m = c->size[0] << 1;
  for (i78 = 0; i78 < m; i78++) {
    byPk->data[i78] = 0.0;
  }

  m = iLB->size[0];
  for (i78 = 0; i78 < m; i78++) {
    byPk->data[static_cast<int>(base->data[i78]) - 1] = y[iLB->data[i78] - 1];
  }

  emxFree_int32_T(&iLB);
  m = iRB->size[0];
  for (i78 = 0; i78 < m; i78++) {
    byPk->data[(static_cast<int>(base->data[i78]) + byPk->size[0]) - 1] = y
      [iRB->data[i78] - 1];
  }

  emxFree_int32_T(&iRB);
  m = b_x->size[0];
  for (i78 = 0; i78 < m; i78++) {
    byPk->data[iRightBase_data[i78] - 1] = y[b_x->data[i78] - 1];
  }

  m = ii->size[0];
  for (i78 = 0; i78 < m; i78++) {
    byPk->data[(iRightBase_data[i78] + byPk->size[0]) - 1] = y[ii->data[i78] - 1];
  }

  i78 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = c->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i78);
  m = c->size[0] << 1;
  for (i78 = 0; i78 < m; i78++) {
    wxPk->data[i78] = 0.0;
  }

  m = b_wxPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    wxPk->data[static_cast<int>(base->data[i78]) - 1] = b_wxPk->data[i78];
  }

  m = b_wxPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    wxPk->data[(static_cast<int>(base->data[i78]) + wxPk->size[0]) - 1] =
      b_wxPk->data[i78 + b_wxPk->size[0]];
  }

  emxFree_real_T(&base);
  emxFree_real_T(&b_wxPk);
  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    wxPk->data[iRightBase_data[i78] - 1] = 0.5 * static_cast<double>(static_cast<
      short>((static_cast<short>((static_cast<short>((iInf_data[i78] - 1)) +
      static_cast<short>((b_x->data[i78] - 1)))) + 2)));
  }

  emxFree_int32_T(&b_x);
  m = iInf_size[0];
  for (i78 = 0; i78 < m; i78++) {
    wxPk->data[(iRightBase_data[i78] + wxPk->size[0]) - 1] = 0.5 * static_cast<
      double>(static_cast<short>((static_cast<short>((static_cast<short>
      ((iInf_data[i78] - 1)) + static_cast<short>((ii->data[i78] - 1)))) + 2)));
  }

  emxFree_int32_T(&ii);
  i78 = iPk->size[0];
  iPk->size[0] = c->size[0];
  emxEnsureCapacity_int32_T(iPk, i78);
  m = c->size[0];
  for (i78 = 0; i78 < m; i78++) {
    iPk->data[i78] = c->data[i78];
  }

  emxFree_int32_T(&c);
  i78 = bPk->size[0];
  bPk->size[0] = b_bPk->size[0];
  emxEnsureCapacity_real_T(bPk, i78);
  m = b_bPk->size[0];
  for (i78 = 0; i78 < m; i78++) {
    bPk->data[i78] = b_bPk->data[i78];
  }

  emxFree_real_T(&b_bPk);
}

//
// Arguments    : const double y[1650]
//                int *idx
//                int borderIdx
//                double refHeight
// Return Type  : void
//
static void findLeftIntercept(const double y[1650], int *idx, int borderIdx,
  double refHeight)
{
  while ((*idx >= borderIdx) && (y[*idx - 1] > refHeight)) {
    (*idx)--;
  }
}

//
// Arguments    : const double y[1650]
//                int *idx
//                int borderIdx
//                double refHeight
// Return Type  : void
//
static void findRightIntercept(const double y[1650], int *idx, int borderIdx,
  double refHeight)
{
  while ((*idx <= borderIdx) && (y[*idx - 1] > refHeight)) {
    (*idx)++;
  }
}

//
// Arguments    : const emxArray_real_T *y
//                emxArray_int32_T *iPk
//                emxArray_int32_T *iInf
//                emxArray_int32_T *iInflect
// Return Type  : void
//
static void getAllPeaksCodegen(const emxArray_real_T *y, emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect)
{
  unsigned int y_idx_0;
  int ny;
  int nPk;
  int nInf;
  int nInflect;
  char dir;
  int kfirst;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  double yk;
  boolean_T isinfyk;
  char previousdir;
  int i47;
  y_idx_0 = static_cast<unsigned int>(y->size[0]);
  ny = iPk->size[0];
  iPk->size[0] = static_cast<int>(y_idx_0);
  emxEnsureCapacity_int32_T(iPk, ny);
  y_idx_0 = static_cast<unsigned int>(y->size[0]);
  ny = iInf->size[0];
  iInf->size[0] = static_cast<int>(y_idx_0);
  emxEnsureCapacity_int32_T(iInf, ny);
  y_idx_0 = static_cast<unsigned int>(y->size[0]);
  ny = iInflect->size[0];
  iInflect->size[0] = static_cast<int>(y_idx_0);
  emxEnsureCapacity_int32_T(iInflect, ny);
  ny = y->size[0];
  nPk = 0;
  nInf = 0;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 1; k <= ny; k++) {
    yk = y->data[k - 1];
    if (rtIsNaN(y->data[k - 1])) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(y->data[k - 1]) && (y->data[k - 1] > 0.0)) {
      isinfyk = true;
      nInf++;
      iInf->data[nInf - 1] = k;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iPk->data[nPk - 1] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      }

      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }

  if ((y->size[0] > 0) && (!isinfykfirst) && ((nInflect + 1 == 0) ||
       (iInflect->data[nInflect] < y->size[0]))) {
    nInflect++;
    iInflect->data[nInflect] = y->size[0];
  }

  ny = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity_int32_T(iPk, ny);
  ny = iInf->size[0];
  if (1 > nInf) {
    iInf->size[0] = 0;
  } else {
    iInf->size[0] = nInf;
  }

  emxEnsureCapacity_int32_T(iInf, ny);
  ny = iInflect->size[0];
  if (1 > nInflect + 1) {
    i47 = -1;
  } else {
    i47 = nInflect;
  }

  iInflect->size[0] = i47 + 1;
  emxEnsureCapacity_int32_T(iInflect, ny);
}

//
// Arguments    : const double yTemp[1650]
//                const int iPeak_data[]
//                const int iPeak_size[1]
//                const int iFinite_data[]
//                const int iFinite_size[1]
//                const int iInflect_data[]
//                int iBase_data[]
//                int iBase_size[1]
//                int iSaddle_data[]
//                int iSaddle_size[1]
// Return Type  : void
//
static void getLeftBase(const double yTemp[1650], const int iPeak_data[], const
  int iPeak_size[1], const int iFinite_data[], const int iFinite_size[1], const
  int iInflect_data[], int iBase_data[], int iBase_size[1], int iSaddle_data[],
  int iSaddle_size[1])
{
  int n;
  double peak_data[1650];
  double valley_data[1650];
  int iValley_data[1650];
  int i;
  int j;
  int k;
  double v;
  int iv;
  double p_tmp;
  int isv;
  iBase_size[0] = static_cast<short>(iPeak_size[0]);
  n = static_cast<short>(iPeak_size[0]);
  if (0 <= n - 1) {
    memset(&iBase_data[0], 0, (unsigned int)(n * static_cast<int>(sizeof(int))));
  }

  iSaddle_size[0] = static_cast<short>(iPeak_size[0]);
  n = static_cast<short>(iPeak_size[0]);
  if (0 <= n - 1) {
    memset(&iSaddle_data[0], 0, (unsigned int)(n * static_cast<int>(sizeof(int))));
  }

  n = static_cast<short>(iFinite_size[0]);
  if (0 <= n - 1) {
    memset(&peak_data[0], 0, (unsigned int)(n * static_cast<int>(sizeof(double))));
  }

  n = static_cast<short>(iFinite_size[0]);
  if (0 <= n - 1) {
    memset(&valley_data[0], 0, (unsigned int)(n * static_cast<int>(sizeof(double))));
  }

  n = static_cast<short>(iFinite_size[0]);
  if (0 <= n - 1) {
    memset(&iValley_data[0], 0, (unsigned int)(n * static_cast<int>(sizeof(int))));
  }

  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak_size[0]) {
    while (iInflect_data[i] != iFinite_data[j]) {
      v = yTemp[iInflect_data[i] - 1];
      iv = iInflect_data[i];
      if (rtIsNaN(yTemp[iInflect_data[i] - 1])) {
        n = -1;
      } else {
        while ((n + 1 > 0) && (valley_data[n] > v)) {
          n--;
        }
      }

      i++;
    }

    p_tmp = yTemp[iInflect_data[i] - 1];
    while ((n + 1 > 0) && (peak_data[n] < p_tmp)) {
      if (valley_data[n] < v) {
        v = valley_data[n];
        iv = iValley_data[n];
      }

      n--;
    }

    isv = iv;
    while ((n + 1 > 0) && (peak_data[n] <= p_tmp)) {
      if (valley_data[n] < v) {
        v = valley_data[n];
        iv = iValley_data[n];
      }

      n--;
    }

    n++;
    peak_data[n] = p_tmp;
    valley_data[n] = v;
    iValley_data[n] = iv;
    if (iInflect_data[i] == iPeak_data[k]) {
      iBase_data[k] = iv;
      iSaddle_data[k] = isv;
      k++;
    }

    i++;
    j++;
  }
}

//
// Arguments    : const emxArray_real_T *yTemp
//                const emxArray_int32_T *iPk
//                const emxArray_int32_T *iFin
//                const emxArray_int32_T *iInflect
//                emxArray_real_T *peakBase
//                emxArray_int32_T *iLeftSaddle
//                emxArray_int32_T *iRightSaddle
// Return Type  : void
//
static void getPeakBase(const emxArray_real_T *yTemp, const emxArray_int32_T
  *iPk, const emxArray_int32_T *iFin, const emxArray_int32_T *iInflect,
  emxArray_real_T *peakBase, emxArray_int32_T *iLeftSaddle, emxArray_int32_T
  *iRightSaddle)
{
  emxArray_int32_T *iLeftBase;
  emxArray_int32_T *x;
  int i48;
  int m;
  int md2;
  int i;
  emxArray_int32_T *b_x;
  int xtmp;
  emxArray_int32_T *c_x;
  emxArray_int32_T *iRightBase;
  emxInit_int32_T(&iLeftBase, 1);
  emxInit_int32_T(&x, 1);
  b_getLeftBase(yTemp, iPk, iFin, iInflect, iLeftBase, iLeftSaddle);
  i48 = x->size[0];
  x->size[0] = iPk->size[0];
  emxEnsureCapacity_int32_T(x, i48);
  m = iPk->size[0];
  for (i48 = 0; i48 < m; i48++) {
    x->data[i48] = iPk->data[i48];
  }

  m = iPk->size[0] - 1;
  md2 = iPk->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = x->data[i];
    i48 = m - i;
    x->data[i] = x->data[i48];
    x->data[i48] = xtmp;
  }

  emxInit_int32_T(&b_x, 1);
  i48 = b_x->size[0];
  b_x->size[0] = iFin->size[0];
  emxEnsureCapacity_int32_T(b_x, i48);
  m = iFin->size[0];
  for (i48 = 0; i48 < m; i48++) {
    b_x->data[i48] = iFin->data[i48];
  }

  m = iFin->size[0] - 1;
  md2 = iFin->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = b_x->data[i];
    i48 = m - i;
    b_x->data[i] = b_x->data[i48];
    b_x->data[i48] = xtmp;
  }

  emxInit_int32_T(&c_x, 1);
  i48 = c_x->size[0];
  c_x->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(c_x, i48);
  m = iInflect->size[0];
  for (i48 = 0; i48 < m; i48++) {
    c_x->data[i48] = iInflect->data[i48];
  }

  m = iInflect->size[0] - 1;
  md2 = iInflect->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = c_x->data[i];
    i48 = m - i;
    c_x->data[i] = c_x->data[i48];
    c_x->data[i48] = xtmp;
  }

  emxInit_int32_T(&iRightBase, 1);
  b_getLeftBase(yTemp, x, b_x, c_x, iRightBase, iRightSaddle);
  m = iRightBase->size[0] - 1;
  md2 = iRightBase->size[0] >> 1;
  emxFree_int32_T(&c_x);
  emxFree_int32_T(&b_x);
  emxFree_int32_T(&x);
  for (i = 0; i < md2; i++) {
    xtmp = iRightBase->data[i];
    i48 = m - i;
    iRightBase->data[i] = iRightBase->data[i48];
    iRightBase->data[i48] = xtmp;
  }

  m = iRightSaddle->size[0] - 1;
  md2 = iRightSaddle->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = iRightSaddle->data[i];
    i48 = m - i;
    iRightSaddle->data[i] = iRightSaddle->data[i48];
    iRightSaddle->data[i48] = xtmp;
  }

  if (iLeftBase->size[0] <= iRightBase->size[0]) {
    md2 = iLeftBase->size[0];
  } else {
    md2 = iRightBase->size[0];
  }

  i48 = peakBase->size[0];
  peakBase->size[0] = md2;
  emxEnsureCapacity_real_T(peakBase, i48);
  for (m = 0; m < md2; m++) {
    if ((yTemp->data[iLeftBase->data[m] - 1] > yTemp->data[iRightBase->data[m] -
         1]) || rtIsNaN(yTemp->data[iRightBase->data[m] - 1])) {
      peakBase->data[m] = yTemp->data[iLeftBase->data[m] - 1];
    } else {
      peakBase->data[m] = yTemp->data[iRightBase->data[m] - 1];
    }
  }

  emxFree_int32_T(&iLeftBase);
  emxFree_int32_T(&iRightBase);
}

//
// Arguments    : const emxArray_real_T *y
//                const emxArray_real_T *x
//                const emxArray_int32_T *iPk
//                const emxArray_real_T *pbPk
//                emxArray_int32_T *iLB
//                emxArray_int32_T *iRB
//                emxArray_real_T *wxPk
// Return Type  : void
//
static void getPeakWidth(const emxArray_real_T *y, const emxArray_real_T *x,
  const emxArray_int32_T *iPk, const emxArray_real_T *pbPk, emxArray_int32_T
  *iLB, emxArray_int32_T *iRB, emxArray_real_T *wxPk)
{
  emxArray_real_T *base;
  int i84;
  int iLeft;
  int i;
  double refHeight;
  double xc;
  emxInit_real_T(&base, 1);
  if (iPk->size[0] == 0) {
    base->size[0] = 0;
    iLB->size[0] = 0;
    iRB->size[0] = 0;
  } else {
    i84 = base->size[0];
    base->size[0] = pbPk->size[0];
    emxEnsureCapacity_real_T(base, i84);
    iLeft = pbPk->size[0];
    for (i84 = 0; i84 < iLeft; i84++) {
      base->data[i84] = pbPk->data[i84];
    }
  }

  i84 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = iPk->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i84);
  iLeft = iPk->size[0] << 1;
  for (i84 = 0; i84 < iLeft; i84++) {
    wxPk->data[i84] = 0.0;
  }

  i84 = iPk->size[0];
  for (i = 0; i < i84; i++) {
    refHeight = (y->data[iPk->data[i] - 1] + base->data[i]) / 2.0;
    iLeft = iPk->data[i];
    while ((iLeft >= iLB->data[i]) && (y->data[iLeft - 1] > refHeight)) {
      iLeft--;
    }

    if (iLeft < iLB->data[i]) {
      wxPk->data[i] = x->data[iLB->data[i] - 1];
    } else {
      xc = x->data[iLeft - 1] + (x->data[iLeft] - x->data[iLeft - 1]) * (0.5 *
        (y->data[iPk->data[i] - 1] + base->data[i]) - y->data[iLeft - 1]) /
        (y->data[iLeft] - y->data[iLeft - 1]);
      if (rtIsNaN(xc)) {
        if (rtIsInf(base->data[i])) {
          xc = 0.5 * (x->data[iLeft - 1] + x->data[iLeft]);
        } else {
          xc = x->data[iLeft];
        }
      }

      wxPk->data[i] = xc;
    }

    iLeft = iPk->data[i] - 1;
    while ((iLeft + 1 <= iRB->data[i]) && (y->data[iLeft] > refHeight)) {
      iLeft++;
    }

    if (iLeft + 1 > iRB->data[i]) {
      wxPk->data[i + wxPk->size[0]] = x->data[iRB->data[i] - 1];
    } else {
      xc = x->data[iLeft] + (x->data[iLeft - 1] - x->data[iLeft]) * (0.5 *
        (y->data[iPk->data[i] - 1] + base->data[i]) - y->data[iLeft]) / (y->
        data[iLeft - 1] - y->data[iLeft]);
      if (rtIsNaN(xc)) {
        if (rtIsInf(base->data[i])) {
          xc = 0.5 * (x->data[iLeft] + x->data[iLeft - 1]);
        } else {
          xc = x->data[iLeft - 1];
        }
      }

      wxPk->data[i + wxPk->size[0]] = xc;
    }
  }

  emxFree_real_T(&base);
}

//
// Arguments    : double xa
//                double xb
//                double ya
//                double yb
//                double yc
//                double bc
// Return Type  : double
//
static double linterp(double xa, double xb, double ya, double yb, double yc,
                      double bc)
{
  double xc;
  xc = xa + (xb - xa) * (0.5 * (yc + bc) - ya) / (yb - ya);
  if (rtIsNaN(xc)) {
    if (rtIsInf(bc)) {
      xc = 0.5 * (xa + xb);
    } else {
      xc = xb;
    }
  }

  return xc;
}

//
// Arguments    : const emxArray_real_T *y
//                const emxArray_int32_T *iFinite
//                double minH
//                emxArray_int32_T *iPk
// Return Type  : void
//
static void removeSmallPeaks(const emxArray_real_T *y, const emxArray_int32_T
  *iFinite, double minH, emxArray_int32_T *iPk)
{
  int n;
  int nPk;
  int k;
  int j;
  double base;
  n = iPk->size[0];
  iPk->size[0] = iFinite->size[0];
  emxEnsureCapacity_int32_T(iPk, n);
  nPk = 0;
  n = iFinite->size[0];
  for (k = 0; k < n; k++) {
    j = iFinite->data[k];
    if (y->data[j - 1] > minH) {
      if ((y->data[j - 2] > y->data[j]) || rtIsNaN(y->data[j])) {
        base = y->data[j - 2];
      } else {
        base = y->data[j];
      }

      if (y->data[j - 1] - base >= 0.0) {
        nPk++;
        iPk->data[nPk - 1] = iFinite->data[k];
      }
    }
  }

  n = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity_int32_T(iPk, n);
}

//
// Arguments    : const emxArray_real_T *Yin
//                double varargin_2
//                double varargin_6
//                emxArray_real_T *Ypk
//                emxArray_real_T *Xpk
//                emxArray_real_T *Wpk
// Return Type  : void
//
void b_findpeaks(const emxArray_real_T *Yin, double varargin_2, double
                 varargin_6, emxArray_real_T *Ypk, emxArray_real_T *Xpk,
                 emxArray_real_T *Wpk)
{
  emxArray_real_T *y;
  int i46;
  int ii;
  int nx;
  emxArray_real_T *x;
  emxArray_int32_T *iInfinite;
  emxArray_int32_T *iInflect;
  emxArray_int32_T *iPk;
  emxArray_real_T *yFinite;
  emxArray_int32_T *b_ii;
  emxArray_real_T *bPk;
  emxArray_int32_T *iLB;
  emxArray_int32_T *iRB;
  emxArray_boolean_T *b_x;
  int idx;
  boolean_T exitg1;
  emxArray_real_T *b_idx;
  emxArray_real_T *b_bPk;
  emxArray_real_T *wxPk;
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *b_wxPk;
  emxInit_real_T(&y, 2);
  if (Yin->size[0] < 1) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    i46 = Yin->size[0];
    ii = y->size[0] * y->size[1];
    y->size[0] = 1;
    nx = static_cast<int>((static_cast<double>(i46) - 1.0));
    y->size[1] = nx + 1;
    emxEnsureCapacity_real_T(y, ii);
    for (i46 = 0; i46 <= nx; i46++) {
      y->data[i46] = 1.0 + static_cast<double>(i46);
    }
  }

  emxInit_real_T(&x, 1);
  i46 = x->size[0];
  x->size[0] = y->size[1];
  emxEnsureCapacity_real_T(x, i46);
  nx = y->size[1];
  for (i46 = 0; i46 < nx; i46++) {
    x->data[i46] = y->data[i46];
  }

  emxFree_real_T(&y);
  emxInit_int32_T(&iInfinite, 1);
  emxInit_int32_T(&iInflect, 1);
  emxInit_int32_T(&iPk, 1);
  emxInit_real_T(&yFinite, 1);
  emxInit_int32_T(&b_ii, 1);
  getAllPeaksCodegen(Yin, b_ii, iInfinite, iInflect);
  removeSmallPeaks(Yin, b_ii, varargin_2, iPk);
  i46 = yFinite->size[0];
  yFinite->size[0] = Yin->size[0];
  emxEnsureCapacity_real_T(yFinite, i46);
  nx = Yin->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    yFinite->data[i46] = Yin->data[i46];
  }

  nx = iInfinite->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    yFinite->data[iInfinite->data[i46] - 1] = rtNaN;
  }

  emxInit_real_T(&bPk, 1);
  emxInit_int32_T(&iLB, 1);
  emxInit_int32_T(&iRB, 1);
  emxInit_boolean_T(&b_x, 1);
  getPeakBase(yFinite, iPk, b_ii, iInflect, bPk, iLB, iRB);
  i46 = b_x->size[0];
  b_x->size[0] = iPk->size[0];
  emxEnsureCapacity_boolean_T(b_x, i46);
  nx = iPk->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    b_x->data[i46] = (yFinite->data[iPk->data[i46] - 1] - bPk->data[i46] >=
                      varargin_6);
  }

  nx = b_x->size[0];
  idx = 0;
  i46 = b_ii->size[0];
  b_ii->size[0] = b_x->size[0];
  emxEnsureCapacity_int32_T(b_ii, i46);
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (b_x->data[ii]) {
      idx++;
      b_ii->data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      b_ii->size[0] = 0;
    }
  } else if (1 > idx) {
    b_ii->size[0] = 0;
  } else {
    i46 = b_ii->size[0];
    b_ii->size[0] = idx;
    emxEnsureCapacity_int32_T(b_ii, i46);
  }

  emxFree_boolean_T(&b_x);
  emxInit_real_T(&b_idx, 1);
  i46 = b_idx->size[0];
  b_idx->size[0] = b_ii->size[0];
  emxEnsureCapacity_real_T(b_idx, i46);
  nx = b_ii->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    b_idx->data[i46] = b_ii->data[i46];
  }

  i46 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i46);
  nx = b_idx->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iInflect->data[i46] = iPk->data[static_cast<int>(b_idx->data[i46]) - 1];
  }

  i46 = iPk->size[0];
  iPk->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iPk, i46);
  nx = iInflect->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iPk->data[i46] = iInflect->data[i46];
  }

  emxInit_real_T(&b_bPk, 1);
  i46 = b_bPk->size[0];
  b_bPk->size[0] = b_idx->size[0];
  emxEnsureCapacity_real_T(b_bPk, i46);
  nx = b_idx->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    b_bPk->data[i46] = bPk->data[static_cast<int>(b_idx->data[i46]) - 1];
  }

  i46 = bPk->size[0];
  bPk->size[0] = b_bPk->size[0];
  emxEnsureCapacity_real_T(bPk, i46);
  nx = b_bPk->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    bPk->data[i46] = b_bPk->data[i46];
  }

  emxFree_real_T(&b_bPk);
  i46 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i46);
  nx = b_idx->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iInflect->data[i46] = iLB->data[static_cast<int>(b_idx->data[i46]) - 1];
  }

  i46 = iLB->size[0];
  iLB->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iLB, i46);
  nx = iInflect->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iLB->data[i46] = iInflect->data[i46];
  }

  i46 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i46);
  nx = b_idx->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iInflect->data[i46] = iRB->data[static_cast<int>(b_idx->data[i46]) - 1];
  }

  i46 = iRB->size[0];
  iRB->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iRB, i46);
  nx = iInflect->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iRB->data[i46] = iInflect->data[i46];
  }

  emxInit_real_T(&wxPk, 2);
  getPeakWidth(yFinite, x, iPk, bPk, iLB, iRB, wxPk);
  i46 = b_ii->size[0];
  b_ii->size[0] = iPk->size[0];
  emxEnsureCapacity_int32_T(b_ii, i46);
  nx = iPk->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    b_ii->data[i46] = iPk->data[i46];
  }

  emxInit_real_T(&bxPk, 2);
  emxInit_real_T(&byPk, 2);
  emxInit_real_T(&b_wxPk, 2);
  combineFullPeaks(Yin, x, b_ii, bPk, iLB, iRB, wxPk, iInfinite, iPk, b_idx,
                   bxPk, byPk, b_wxPk);
  c_findPeaksSeparatedByMoreThanM(Yin, x, iPk, b_ii);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  emxFree_int32_T(&iRB);
  emxFree_int32_T(&iLB);
  emxFree_int32_T(&iInfinite);
  if (b_ii->size[0] > Yin->size[0]) {
    i46 = b_ii->size[0];
    b_ii->size[0] = Yin->size[0];
    emxEnsureCapacity_int32_T(b_ii, i46);
  }

  i46 = bPk->size[0];
  bPk->size[0] = b_ii->size[0];
  emxEnsureCapacity_real_T(bPk, i46);
  nx = b_ii->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    bPk->data[i46] = b_idx->data[b_ii->data[i46] - 1];
  }

  i46 = b_idx->size[0];
  b_idx->size[0] = bPk->size[0];
  emxEnsureCapacity_real_T(b_idx, i46);
  nx = bPk->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    b_idx->data[i46] = bPk->data[i46];
  }

  emxFree_real_T(&bPk);
  i46 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = b_ii->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i46);
  nx = b_ii->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    wxPk->data[i46] = b_wxPk->data[b_ii->data[i46] - 1];
  }

  nx = b_ii->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    wxPk->data[i46 + wxPk->size[0]] = b_wxPk->data[(b_ii->data[i46] +
      b_wxPk->size[0]) - 1];
  }

  i46 = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = wxPk->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(b_wxPk, i46);
  nx = wxPk->size[0] * wxPk->size[1];
  for (i46 = 0; i46 < nx; i46++) {
    b_wxPk->data[i46] = wxPk->data[i46];
  }

  emxFree_real_T(&wxPk);
  i46 = iInflect->size[0];
  iInflect->size[0] = b_ii->size[0];
  emxEnsureCapacity_int32_T(iInflect, i46);
  nx = b_ii->size[0];
  for (i46 = 0; i46 < nx; i46++) {
    iInflect->data[i46] = iPk->data[b_ii->data[i46] - 1];
  }

  emxFree_int32_T(&b_ii);
  emxFree_int32_T(&iPk);
  assignFullOutputs(Yin, x, iInflect, b_wxPk, b_idx, Ypk, Xpk, Wpk, yFinite);
  emxFree_real_T(&b_idx);
  emxFree_real_T(&b_wxPk);
  emxFree_real_T(&yFinite);
  emxFree_int32_T(&iInflect);
  emxFree_real_T(&x);
}

//
// Arguments    : const emxArray_real_T *Yin
//                double varargin_2
//                emxArray_real_T *Ypk
//                emxArray_real_T *Xpk
//                emxArray_real_T *Wpk
// Return Type  : void
//
void c_findpeaks(const emxArray_real_T *Yin, double varargin_2, emxArray_real_T *
                 Ypk, emxArray_real_T *Xpk, emxArray_real_T *Wpk)
{
  emxArray_real_T *y;
  int i51;
  int ii;
  int nx;
  emxArray_real_T *x;
  emxArray_int32_T *iInfinite;
  emxArray_int32_T *iInflect;
  emxArray_int32_T *iPk;
  emxArray_real_T *yFinite;
  emxArray_int32_T *b_ii;
  emxArray_real_T *bPk;
  emxArray_int32_T *iLB;
  emxArray_int32_T *iRB;
  emxArray_boolean_T *b_x;
  int idx;
  boolean_T exitg1;
  emxArray_real_T *b_idx;
  emxArray_real_T *b_bPk;
  emxArray_real_T *wxPk;
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *b_wxPk;
  emxInit_real_T(&y, 2);
  if (Yin->size[0] < 1) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    i51 = Yin->size[0];
    ii = y->size[0] * y->size[1];
    y->size[0] = 1;
    nx = static_cast<int>((static_cast<double>(i51) - 1.0));
    y->size[1] = nx + 1;
    emxEnsureCapacity_real_T(y, ii);
    for (i51 = 0; i51 <= nx; i51++) {
      y->data[i51] = 1.0 + static_cast<double>(i51);
    }
  }

  emxInit_real_T(&x, 1);
  i51 = x->size[0];
  x->size[0] = y->size[1];
  emxEnsureCapacity_real_T(x, i51);
  nx = y->size[1];
  for (i51 = 0; i51 < nx; i51++) {
    x->data[i51] = y->data[i51];
  }

  emxFree_real_T(&y);
  emxInit_int32_T(&iInfinite, 1);
  emxInit_int32_T(&iInflect, 1);
  emxInit_int32_T(&iPk, 1);
  emxInit_real_T(&yFinite, 1);
  emxInit_int32_T(&b_ii, 1);
  getAllPeaksCodegen(Yin, b_ii, iInfinite, iInflect);
  removeSmallPeaks(Yin, b_ii, varargin_2, iPk);
  i51 = yFinite->size[0];
  yFinite->size[0] = Yin->size[0];
  emxEnsureCapacity_real_T(yFinite, i51);
  nx = Yin->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    yFinite->data[i51] = Yin->data[i51];
  }

  nx = iInfinite->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    yFinite->data[iInfinite->data[i51] - 1] = rtNaN;
  }

  emxInit_real_T(&bPk, 1);
  emxInit_int32_T(&iLB, 1);
  emxInit_int32_T(&iRB, 1);
  emxInit_boolean_T(&b_x, 1);
  getPeakBase(yFinite, iPk, b_ii, iInflect, bPk, iLB, iRB);
  i51 = b_x->size[0];
  b_x->size[0] = iPk->size[0];
  emxEnsureCapacity_boolean_T(b_x, i51);
  nx = iPk->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    b_x->data[i51] = (yFinite->data[iPk->data[i51] - 1] - bPk->data[i51] >=
                      250.0);
  }

  nx = b_x->size[0];
  idx = 0;
  i51 = b_ii->size[0];
  b_ii->size[0] = b_x->size[0];
  emxEnsureCapacity_int32_T(b_ii, i51);
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (b_x->data[ii]) {
      idx++;
      b_ii->data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (b_x->size[0] == 1) {
    if (idx == 0) {
      b_ii->size[0] = 0;
    }
  } else if (1 > idx) {
    b_ii->size[0] = 0;
  } else {
    i51 = b_ii->size[0];
    b_ii->size[0] = idx;
    emxEnsureCapacity_int32_T(b_ii, i51);
  }

  emxFree_boolean_T(&b_x);
  emxInit_real_T(&b_idx, 1);
  i51 = b_idx->size[0];
  b_idx->size[0] = b_ii->size[0];
  emxEnsureCapacity_real_T(b_idx, i51);
  nx = b_ii->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    b_idx->data[i51] = b_ii->data[i51];
  }

  i51 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i51);
  nx = b_idx->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iInflect->data[i51] = iPk->data[static_cast<int>(b_idx->data[i51]) - 1];
  }

  i51 = iPk->size[0];
  iPk->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iPk, i51);
  nx = iInflect->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iPk->data[i51] = iInflect->data[i51];
  }

  emxInit_real_T(&b_bPk, 1);
  i51 = b_bPk->size[0];
  b_bPk->size[0] = b_idx->size[0];
  emxEnsureCapacity_real_T(b_bPk, i51);
  nx = b_idx->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    b_bPk->data[i51] = bPk->data[static_cast<int>(b_idx->data[i51]) - 1];
  }

  i51 = bPk->size[0];
  bPk->size[0] = b_bPk->size[0];
  emxEnsureCapacity_real_T(bPk, i51);
  nx = b_bPk->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    bPk->data[i51] = b_bPk->data[i51];
  }

  emxFree_real_T(&b_bPk);
  i51 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i51);
  nx = b_idx->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iInflect->data[i51] = iLB->data[static_cast<int>(b_idx->data[i51]) - 1];
  }

  i51 = iLB->size[0];
  iLB->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iLB, i51);
  nx = iInflect->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iLB->data[i51] = iInflect->data[i51];
  }

  i51 = iInflect->size[0];
  iInflect->size[0] = b_idx->size[0];
  emxEnsureCapacity_int32_T(iInflect, i51);
  nx = b_idx->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iInflect->data[i51] = iRB->data[static_cast<int>(b_idx->data[i51]) - 1];
  }

  i51 = iRB->size[0];
  iRB->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(iRB, i51);
  nx = iInflect->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iRB->data[i51] = iInflect->data[i51];
  }

  emxInit_real_T(&wxPk, 2);
  getPeakWidth(yFinite, x, iPk, bPk, iLB, iRB, wxPk);
  i51 = b_ii->size[0];
  b_ii->size[0] = iPk->size[0];
  emxEnsureCapacity_int32_T(b_ii, i51);
  nx = iPk->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    b_ii->data[i51] = iPk->data[i51];
  }

  emxInit_real_T(&bxPk, 2);
  emxInit_real_T(&byPk, 2);
  emxInit_real_T(&b_wxPk, 2);
  combineFullPeaks(Yin, x, b_ii, bPk, iLB, iRB, wxPk, iInfinite, iPk, b_idx,
                   bxPk, byPk, b_wxPk);
  c_findPeaksSeparatedByMoreThanM(Yin, x, iPk, b_ii);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  emxFree_int32_T(&iRB);
  emxFree_int32_T(&iLB);
  emxFree_int32_T(&iInfinite);
  if (b_ii->size[0] > Yin->size[0]) {
    i51 = b_ii->size[0];
    b_ii->size[0] = Yin->size[0];
    emxEnsureCapacity_int32_T(b_ii, i51);
  }

  i51 = bPk->size[0];
  bPk->size[0] = b_ii->size[0];
  emxEnsureCapacity_real_T(bPk, i51);
  nx = b_ii->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    bPk->data[i51] = b_idx->data[b_ii->data[i51] - 1];
  }

  i51 = b_idx->size[0];
  b_idx->size[0] = bPk->size[0];
  emxEnsureCapacity_real_T(b_idx, i51);
  nx = bPk->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    b_idx->data[i51] = bPk->data[i51];
  }

  emxFree_real_T(&bPk);
  i51 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = b_ii->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i51);
  nx = b_ii->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    wxPk->data[i51] = b_wxPk->data[b_ii->data[i51] - 1];
  }

  nx = b_ii->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    wxPk->data[i51 + wxPk->size[0]] = b_wxPk->data[(b_ii->data[i51] +
      b_wxPk->size[0]) - 1];
  }

  i51 = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = wxPk->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(b_wxPk, i51);
  nx = wxPk->size[0] * wxPk->size[1];
  for (i51 = 0; i51 < nx; i51++) {
    b_wxPk->data[i51] = wxPk->data[i51];
  }

  emxFree_real_T(&wxPk);
  i51 = iInflect->size[0];
  iInflect->size[0] = b_ii->size[0];
  emxEnsureCapacity_int32_T(iInflect, i51);
  nx = b_ii->size[0];
  for (i51 = 0; i51 < nx; i51++) {
    iInflect->data[i51] = iPk->data[b_ii->data[i51] - 1];
  }

  emxFree_int32_T(&b_ii);
  emxFree_int32_T(&iPk);
  assignFullOutputs(Yin, x, iInflect, b_wxPk, b_idx, Ypk, Xpk, Wpk, yFinite);
  emxFree_real_T(&b_idx);
  emxFree_real_T(&b_wxPk);
  emxFree_real_T(&yFinite);
  emxFree_int32_T(&iInflect);
  emxFree_real_T(&x);
}

//
// Arguments    : const double Yin[1650]
//                double varargin_2
//                double varargin_6
//                emxArray_real_T *Ypk
//                emxArray_real_T *Xpk
//                emxArray_real_T *Wpk
// Return Type  : void
//
void findpeaks(const double Yin[1650], double varargin_2, double varargin_6,
               emxArray_real_T *Ypk, emxArray_real_T *Xpk, emxArray_real_T *Wpk)
{
  int nPk;
  int nInf;
  int nInflect;
  char dir;
  int kfirst;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  int iInflect_data[1650];
  double yk;
  int loop_ub;
  boolean_T isinfyk;
  int i41;
  int iInfinite_data[1650];
  char previousdir;
  int iFinite_size[1];
  int iFinite_data[1650];
  int b_iFinite_data[1650];
  int iInfinite_size[1];
  int iInflect_size[1];
  emxArray_int32_T *iPk;
  double b_Yin;
  short iPk_data[1650];
  emxArray_real_T *bPk;
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *wxPk;
  emxArray_int32_T *idx;
  emxArray_int32_T *d_sortIdx;
  emxArray_int16_T *locs;
  emxArray_int32_T *r9;
  emxArray_int16_T *locs_temp;
  emxArray_boolean_T *idelete;
  unsigned int unnamed_idx_0;
  emxArray_boolean_T *r10;
  emxArray_int32_T *r11;
  nPk = -1;
  nInf = -1;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k < 1650; k++) {
    yk = Yin[k];
    if (rtIsNaN(Yin[k])) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(Yin[k]) && (Yin[k] > 0.0)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf] = k + 1;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            b_iFinite_data[nPk] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      }

      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }

  if ((!isinfykfirst) && ((nInflect + 1 == 0) || (iInflect_data[nInflect] < 1650)))
  {
    nInflect++;
    iInflect_data[nInflect] = 1650;
  }

  if (1 > nPk + 1) {
    loop_ub = -1;
  } else {
    loop_ub = nPk;
  }

  for (i41 = 0; i41 <= loop_ub; i41++) {
    iFinite_data[i41] = b_iFinite_data[i41];
  }

  iFinite_size[0] = loop_ub + 1;
  kfirst = loop_ub + 1;
  if (0 <= kfirst - 1) {
    memcpy(&b_iFinite_data[0], &iFinite_data[0], (unsigned int)(kfirst *
            static_cast<int>(sizeof(int))));
  }

  if (1 > nInf + 1) {
    kfirst = -1;
  } else {
    kfirst = nInf;
  }

  for (i41 = 0; i41 <= kfirst; i41++) {
    iFinite_data[i41] = iInfinite_data[i41];
  }

  iInfinite_size[0] = kfirst + 1;
  kfirst++;
  if (0 <= kfirst - 1) {
    memcpy(&iInfinite_data[0], &iFinite_data[0], (unsigned int)(kfirst *
            static_cast<int>(sizeof(int))));
  }

  if (1 > nInflect + 1) {
    kfirst = -1;
  } else {
    kfirst = nInflect;
  }

  for (i41 = 0; i41 <= kfirst; i41++) {
    iFinite_data[i41] = iInflect_data[i41];
  }

  iInflect_size[0] = kfirst + 1;
  kfirst++;
  if (0 <= kfirst - 1) {
    memcpy(&iInflect_data[0], &iFinite_data[0], (unsigned int)(kfirst *
            static_cast<int>(sizeof(int))));
  }

  nPk = 0;
  kfirst = loop_ub + 1;
  for (k = 0; k < kfirst; k++) {
    ykfirst = Yin[b_iFinite_data[k] - 1];
    if (ykfirst > varargin_2) {
      if ((Yin[b_iFinite_data[k] - 2] > Yin[b_iFinite_data[k]]) || rtIsNaN
          (Yin[b_iFinite_data[k]])) {
        b_Yin = Yin[b_iFinite_data[k] - 2];
      } else {
        b_Yin = Yin[b_iFinite_data[k]];
      }

      if (ykfirst - b_Yin >= 0.0) {
        nPk++;
        iPk_data[nPk - 1] = static_cast<short>(b_iFinite_data[k]);
      }
    }
  }

  if (1 > nPk) {
    loop_ub = 0;
  } else {
    loop_ub = nPk;
  }

  emxInit_int32_T(&iPk, 1);
  i41 = iPk->size[0];
  iPk->size[0] = loop_ub;
  emxEnsureCapacity_int32_T(iPk, i41);
  for (i41 = 0; i41 < loop_ub; i41++) {
    iPk->data[i41] = iPk_data[i41];
  }

  emxInit_real_T(&bPk, 1);
  emxInit_real_T(&bxPk, 2);
  emxInit_real_T(&byPk, 2);
  emxInit_real_T(&wxPk, 2);
  findExtents(Yin, iPk, b_iFinite_data, iFinite_size, iInfinite_data,
              iInfinite_size, iInflect_data, iInflect_size, varargin_6, bPk,
              bxPk, byPk, wxPk);
  emxFree_real_T(&byPk);
  emxInit_int32_T(&idx, 1);
  emxInit_int32_T(&d_sortIdx, 1);
  if (iPk->size[0] == 0) {
    emxInit_int32_T(&r9, 2);
    eml_signed_integer_colon(0, r9);
    i41 = idx->size[0];
    idx->size[0] = r9->size[1];
    emxEnsureCapacity_int32_T(idx, i41);
    loop_ub = r9->size[1];
    for (i41 = 0; i41 < loop_ub; i41++) {
      idx->data[i41] = r9->data[i41];
    }

    emxFree_int32_T(&r9);
  } else {
    emxInit_int16_T(&locs, 1);
    i41 = locs->size[0];
    locs->size[0] = iPk->size[0];
    emxEnsureCapacity_int16_T(locs, i41);
    loop_ub = iPk->size[0];
    for (i41 = 0; i41 < loop_ub; i41++) {
      locs->data[i41] = static_cast<short>((1 + static_cast<short>((iPk->
        data[i41] - 1))));
    }

    i41 = bPk->size[0];
    bPk->size[0] = iPk->size[0];
    emxEnsureCapacity_real_T(bPk, i41);
    loop_ub = iPk->size[0];
    for (i41 = 0; i41 < loop_ub; i41++) {
      bPk->data[i41] = Yin[iPk->data[i41] - 1];
    }

    emxInit_int16_T(&locs_temp, 1);
    sortIdx(bPk, d_sortIdx);
    i41 = locs_temp->size[0];
    locs_temp->size[0] = d_sortIdx->size[0];
    emxEnsureCapacity_int16_T(locs_temp, i41);
    loop_ub = d_sortIdx->size[0];
    for (i41 = 0; i41 < loop_ub; i41++) {
      locs_temp->data[i41] = locs->data[d_sortIdx->data[i41] - 1];
    }

    emxInit_boolean_T(&idelete, 1);
    unnamed_idx_0 = static_cast<unsigned int>(d_sortIdx->size[0]);
    i41 = idelete->size[0];
    idelete->size[0] = static_cast<int>(unnamed_idx_0);
    emxEnsureCapacity_boolean_T(idelete, i41);
    loop_ub = static_cast<int>(unnamed_idx_0);
    for (i41 = 0; i41 < loop_ub; i41++) {
      idelete->data[i41] = false;
    }

    i41 = d_sortIdx->size[0];
    emxInit_boolean_T(&r10, 1);
    for (nInf = 0; nInf < i41; nInf++) {
      if (!idelete->data[nInf]) {
        kfirst = locs->data[d_sortIdx->data[nInf] - 1];
        k = locs->data[d_sortIdx->data[nInf] - 1];
        nPk = r10->size[0];
        r10->size[0] = locs_temp->size[0];
        emxEnsureCapacity_boolean_T(r10, nPk);
        loop_ub = locs_temp->size[0];
        for (nPk = 0; nPk < loop_ub; nPk++) {
          r10->data[nPk] = ((locs_temp->data[nPk] >= kfirst - 12) &&
                            (locs_temp->data[nPk] <= k + 12));
        }

        nPk = idelete->size[0];
        emxEnsureCapacity_boolean_T(idelete, nPk);
        loop_ub = idelete->size[0];
        for (nPk = 0; nPk < loop_ub; nPk++) {
          idelete->data[nPk] = (idelete->data[nPk] || r10->data[nPk]);
        }

        idelete->data[nInf] = false;
      }
    }

    emxFree_boolean_T(&r10);
    emxFree_int16_T(&locs_temp);
    emxFree_int16_T(&locs);
    k = idelete->size[0] - 1;
    kfirst = 0;
    for (nInf = 0; nInf <= k; nInf++) {
      if (!idelete->data[nInf]) {
        kfirst++;
      }
    }

    emxInit_int32_T(&r11, 1);
    i41 = r11->size[0];
    r11->size[0] = kfirst;
    emxEnsureCapacity_int32_T(r11, i41);
    kfirst = 0;
    for (nInf = 0; nInf <= k; nInf++) {
      if (!idelete->data[nInf]) {
        r11->data[kfirst] = nInf + 1;
        kfirst++;
      }
    }

    emxFree_boolean_T(&idelete);
    i41 = idx->size[0];
    idx->size[0] = r11->size[0];
    emxEnsureCapacity_int32_T(idx, i41);
    loop_ub = r11->size[0];
    for (i41 = 0; i41 < loop_ub; i41++) {
      idx->data[i41] = d_sortIdx->data[r11->data[i41] - 1];
    }

    emxFree_int32_T(&r11);
    b_sort(idx);
  }

  emxFree_real_T(&bPk);
  if (idx->size[0] > 1650) {
    i41 = d_sortIdx->size[0];
    d_sortIdx->size[0] = 1650;
    emxEnsureCapacity_int32_T(d_sortIdx, i41);
    for (i41 = 0; i41 < 1650; i41++) {
      d_sortIdx->data[i41] = idx->data[i41];
    }

    i41 = idx->size[0];
    idx->size[0] = d_sortIdx->size[0];
    emxEnsureCapacity_int32_T(idx, i41);
    loop_ub = d_sortIdx->size[0];
    for (i41 = 0; i41 < loop_ub; i41++) {
      idx->data[i41] = d_sortIdx->data[i41];
    }
  }

  i41 = d_sortIdx->size[0];
  d_sortIdx->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(d_sortIdx, i41);
  loop_ub = idx->size[0];
  for (i41 = 0; i41 < loop_ub; i41++) {
    d_sortIdx->data[i41] = iPk->data[idx->data[i41] - 1];
  }

  emxFree_int32_T(&iPk);
  i41 = bxPk->size[0] * bxPk->size[1];
  bxPk->size[0] = idx->size[0];
  bxPk->size[1] = 2;
  emxEnsureCapacity_real_T(bxPk, i41);
  loop_ub = idx->size[0];
  for (i41 = 0; i41 < loop_ub; i41++) {
    bxPk->data[i41] = wxPk->data[idx->data[i41] - 1];
  }

  loop_ub = idx->size[0];
  for (i41 = 0; i41 < loop_ub; i41++) {
    bxPk->data[i41 + bxPk->size[0]] = wxPk->data[(idx->data[i41] + wxPk->size[0])
      - 1];
  }

  emxFree_int32_T(&idx);
  i41 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = bxPk->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i41);
  loop_ub = bxPk->size[0] * bxPk->size[1];
  for (i41 = 0; i41 < loop_ub; i41++) {
    wxPk->data[i41] = bxPk->data[i41];
  }

  emxFree_real_T(&bxPk);
  i41 = Ypk->size[0];
  Ypk->size[0] = d_sortIdx->size[0];
  emxEnsureCapacity_real_T(Ypk, i41);
  loop_ub = d_sortIdx->size[0];
  for (i41 = 0; i41 < loop_ub; i41++) {
    Ypk->data[i41] = Yin[d_sortIdx->data[i41] - 1];
  }

  i41 = Xpk->size[0];
  Xpk->size[0] = d_sortIdx->size[0];
  emxEnsureCapacity_real_T(Xpk, i41);
  loop_ub = d_sortIdx->size[0];
  for (i41 = 0; i41 < loop_ub; i41++) {
    Xpk->data[i41] = static_cast<short>((1 + static_cast<short>((d_sortIdx->
      data[i41] - 1))));
  }

  emxFree_int32_T(&d_sortIdx);
  c_diff(wxPk, Wpk);
  emxFree_real_T(&wxPk);
}

//
// File trailer for findpeaks.cpp
//
// [EOF]
//
