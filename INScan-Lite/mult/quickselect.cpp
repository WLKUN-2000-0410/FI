//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quickselect.cpp
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
#include "quickselect.h"

// Function Declarations
static int thirdOfFive(const emxArray_real_T *v, int ia, int ib);

// Function Definitions

//
// Arguments    : const emxArray_real_T *v
//                int ia
//                int ib
// Return Type  : int
//
static int thirdOfFive(const emxArray_real_T *v, int ia, int ib)
{
  int im;
  int b_j1;
  int j2;
  int j3;
  int j4;
  int j5;
  double v4;
  double v5;
  if ((ia == ib) || (ia + 1 == ib)) {
    im = ia;
  } else if ((ia + 2 == ib) || (ia + 3 == ib)) {
    if (v->data[ia - 1] < v->data[ia]) {
      if (v->data[ia] < v->data[ia + 1]) {
        im = ia + 1;
      } else if (v->data[ia - 1] < v->data[ia + 1]) {
        im = ia + 2;
      } else {
        im = ia;
      }
    } else if (v->data[ia - 1] < v->data[ia + 1]) {
      im = ia;
    } else if (v->data[ia] < v->data[ia + 1]) {
      im = ia + 2;
    } else {
      im = ia + 1;
    }
  } else {
    if (v->data[ia - 1] < v->data[ia]) {
      if (v->data[ia] < v->data[ia + 1]) {
        b_j1 = ia;
        j2 = ia;
        j3 = ia + 2;
      } else if (v->data[ia - 1] < v->data[ia + 1]) {
        b_j1 = ia;
        j2 = ia + 1;
        j3 = ia + 1;
      } else {
        b_j1 = ia + 2;
        j2 = ia - 1;
        j3 = ia + 1;
      }
    } else if (v->data[ia - 1] < v->data[ia + 1]) {
      b_j1 = ia + 1;
      j2 = ia - 1;
      j3 = ia + 2;
    } else if (v->data[ia] < v->data[ia + 1]) {
      b_j1 = ia + 1;
      j2 = ia + 1;
      j3 = ia;
    } else {
      b_j1 = ia + 2;
      j2 = ia;
      j3 = ia;
    }

    j4 = ia;
    j5 = ia + 1;
    v4 = v->data[ia + 2];
    v5 = v->data[ia + 3];
    if (v->data[ia + 3] < v->data[ia + 2]) {
      j4 = ia + 1;
      j5 = ia;
      v5 = v->data[ia + 2];
      v4 = v->data[ia + 3];
    }

    if (v5 < v->data[b_j1 - 1]) {
      im = b_j1;
    } else if (v5 < v->data[j2]) {
      im = j5 + 3;
    } else if (v4 < v->data[j2]) {
      im = j2 + 1;
    } else if (v4 < v->data[j3 - 1]) {
      im = j4 + 3;
    } else {
      im = j3;
    }
  }

  return im;
}

//
// Arguments    : emxArray_real_T *v
//                int n
//                int vlen
//                double *vn
//                int *nfirst
//                int *nlast
// Return Type  : void
//
void quickselect(emxArray_real_T *v, int n, int vlen, double *vn, int *nfirst,
                 int *nlast)
{
  int ipiv;
  int ia;
  int ib;
  int ilast;
  int oldnv;
  boolean_T checkspeed;
  boolean_T isslow;
  boolean_T exitg1;
  double vref;
  int k;
  double vk;
  boolean_T guard1 = false;
  int c;
  int ngroupsof5;
  if (n > vlen) {
    *vn = rtNaN;
    *nfirst = 0;
    *nlast = 0;
  } else {
    ipiv = n - 1;
    ia = 0;
    ib = vlen - 1;
    *nfirst = 1;
    ilast = vlen - 1;
    oldnv = vlen;
    checkspeed = false;
    isslow = false;
    exitg1 = false;
    while ((!exitg1) && (ia + 1 < ib + 1)) {
      vref = v->data[ipiv];
      v->data[ipiv] = v->data[ib];
      v->data[ib] = vref;
      ilast = ia;
      ipiv = -1;
      for (k = ia + 1; k <= ib; k++) {
        vk = v->data[k - 1];
        if (v->data[k - 1] == vref) {
          v->data[k - 1] = v->data[ilast];
          v->data[ilast] = vk;
          ipiv++;
          ilast++;
        } else {
          if (v->data[k - 1] < vref) {
            v->data[k - 1] = v->data[ilast];
            v->data[ilast] = vk;
            ilast++;
          }
        }
      }

      v->data[ib] = v->data[ilast];
      v->data[ilast] = vref;
      guard1 = false;
      if (n <= ilast + 1) {
        *nfirst = ilast - ipiv;
        if (n >= *nfirst) {
          exitg1 = true;
        } else {
          ib = ilast - 1;
          guard1 = true;
        }
      } else {
        ia = ilast + 1;
        guard1 = true;
      }

      if (guard1) {
        c = (ib - ia) + 1;
        if (checkspeed) {
          isslow = (c > oldnv / 2);
          oldnv = c;
        }

        checkspeed = !checkspeed;
        if (isslow) {
          while (c > 1) {
            ngroupsof5 = c / 5;
            *nlast = c - ngroupsof5 * 5;
            c = ngroupsof5;
            for (k = 0; k < ngroupsof5; k++) {
              ipiv = (ia + k * 5) + 1;
              ipiv = thirdOfFive(v, ipiv, ipiv + 4) - 1;
              ilast = ia + k;
              vref = v->data[ilast];
              v->data[ilast] = v->data[ipiv];
              v->data[ipiv] = vref;
            }

            if (*nlast > 0) {
              ipiv = (ia + ngroupsof5 * 5) + 1;
              ipiv = thirdOfFive(v, ipiv, (ipiv + *nlast) - 1) - 1;
              ilast = ia + ngroupsof5;
              vref = v->data[ilast];
              v->data[ilast] = v->data[ipiv];
              v->data[ipiv] = vref;
              c = ngroupsof5 + 1;
            }
          }
        } else {
          if (c >= 3) {
            ipiv = ia + (c - 1) / 2;
            if (v->data[ia] < v->data[ipiv]) {
              if (v->data[ipiv] < v->data[ib]) {
              } else if (v->data[ia] < v->data[ib]) {
                ipiv = ib;
              } else {
                ipiv = ia;
              }
            } else if (v->data[ia] < v->data[ib]) {
              ipiv = ia;
            } else {
              if (v->data[ipiv] < v->data[ib]) {
                ipiv = ib;
              }
            }

            if (ipiv + 1 > ia + 1) {
              vref = v->data[ia];
              v->data[ia] = v->data[ipiv];
              v->data[ipiv] = vref;
            }
          }
        }

        ipiv = ia;
        *nfirst = ia + 1;
        ilast = ib;
      }
    }

    *vn = v->data[ilast];
    *nlast = ilast + 1;
  }
}

//
// File trailer for quickselect.cpp
//
// [EOF]
//
