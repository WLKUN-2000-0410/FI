//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eml_setop.cpp
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
#include "eml_setop.h"
#include "analyseMixture_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_int32_T *a
//                const emxArray_int32_T *b
//                emxArray_int32_T *c
//                emxArray_int32_T *ia
//                emxArray_int32_T *ib
// Return Type  : void
//
void b_do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
                  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T
                  *ib)
{
  int iafirst;
  int ncmax;
  int nc;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  int ak;
  int b_iblast;
  int bk;
  iafirst = a->size[0];
  ncmax = b->size[0];
  if (iafirst < ncmax) {
    ncmax = iafirst;
  }

  iafirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity_int32_T(c, iafirst);
  iafirst = ia->size[0];
  ia->size[0] = ncmax;
  emxEnsureCapacity_int32_T(ia, iafirst);
  iafirst = ib->size[0];
  ib->size[0] = ncmax;
  emxEnsureCapacity_int32_T(ib, iafirst);
  nc = 0;
  iafirst = 0;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= a->size[0]) && (iblast <= b->size[0])) {
    b_ialast = ialast;
    ak = a->data[ialast - 1];
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast;
    b_iblast = iblast;
    bk = b->data[iblast - 1];
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c->data[nc - 1] = ak;
      ia->data[nc - 1] = iafirst + 1;
      ib->data[nc - 1] = ibfirst + 1;
      ialast = b_ialast + 1;
      iafirst = b_ialast;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      ialast = b_ialast + 1;
      iafirst = b_ialast;
    } else {
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }

  if (ncmax > 0) {
    if (1 > nc) {
      ia->size[0] = 0;
      ib->size[0] = 0;
      c->size[0] = 0;
    } else {
      iafirst = ia->size[0];
      ia->size[0] = nc;
      emxEnsureCapacity_int32_T(ia, iafirst);
      iafirst = ib->size[0];
      ib->size[0] = nc;
      emxEnsureCapacity_int32_T(ib, iafirst);
      iafirst = c->size[0];
      c->size[0] = nc;
      emxEnsureCapacity_int32_T(c, iafirst);
    }
  }
}

//
// Arguments    : const emxArray_int32_T *a
//                const emxArray_int32_T *b
//                emxArray_int32_T *c
//                emxArray_int32_T *ia
//                emxArray_int32_T *ib
// Return Type  : void
//
void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
                emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib)
{
  int na;
  int nb;
  int ncmax;
  int iafirst;
  int nc;
  int nia;
  int nib;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  int ak;
  int b_iblast;
  int bk;
  na = a->size[0];
  nb = b->size[0];
  ncmax = a->size[0] + b->size[0];
  iafirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity_int32_T(c, iafirst);
  iafirst = ia->size[0];
  ia->size[0] = a->size[0];
  emxEnsureCapacity_int32_T(ia, iafirst);
  iafirst = ib->size[0];
  ib->size[0] = b->size[0];
  emxEnsureCapacity_int32_T(ib, iafirst);
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= na) && (iblast <= nb)) {
    b_ialast = ialast;
    ak = a->data[ialast - 1];
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast;
    b_iblast = iblast;
    bk = b->data[iblast - 1];
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c->data[nc] = ak;
      nia++;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      nc++;
      nia++;
      c->data[nc] = ak;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
    } else {
      nc++;
      nib++;
      c->data[nc] = bk;
      ib->data[nib - 1] = ibfirst + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }

  while (ialast <= na) {
    b_ialast = ialast;
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == a->data[ialast - 1]))
    {
      b_ialast++;
    }

    nc++;
    nia++;
    c->data[nc] = a->data[ialast - 1];
    ia->data[nia] = ialast;
    ialast = b_ialast + 1;
  }

  while (iblast <= nb) {
    b_iblast = iblast;
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == b->data[iblast - 1]))
    {
      b_iblast++;
    }

    nc++;
    nib++;
    c->data[nc] = b->data[iblast - 1];
    ib->data[nib - 1] = iblast;
    iblast = b_iblast + 1;
  }

  if (a->size[0] > 0) {
    if (1 > nia + 1) {
      ia->size[0] = 0;
    } else {
      iafirst = ia->size[0];
      ia->size[0] = nia + 1;
      emxEnsureCapacity_int32_T(ia, iafirst);
    }
  }

  if (b->size[0] > 0) {
    if (1 > nib) {
      ib->size[0] = 0;
    } else {
      iafirst = ib->size[0];
      ib->size[0] = nib;
      emxEnsureCapacity_int32_T(ib, iafirst);
    }
  }

  if (ncmax > 0) {
    if (1 > nc + 1) {
      c->size[0] = 0;
    } else {
      iafirst = c->size[0];
      c->size[0] = nc + 1;
      emxEnsureCapacity_int32_T(c, iafirst);
    }
  }
}

//
// File trailer for eml_setop.cpp
//
// [EOF]
//
