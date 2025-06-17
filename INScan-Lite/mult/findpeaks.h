//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: findpeaks.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef FINDPEAKS_H
#define FINDPEAKS_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_findpeaks(const emxArray_real_T *Yin, double varargin_2, double
  varargin_6, emxArray_real_T *Ypk, emxArray_real_T *Xpk, emxArray_real_T *Wpk);
extern void c_findpeaks(const emxArray_real_T *Yin, double varargin_2,
  emxArray_real_T *Ypk, emxArray_real_T *Xpk, emxArray_real_T *Wpk);
extern void findpeaks(const double Yin[1650], double varargin_2, double
                      varargin_6, emxArray_real_T *Ypk, emxArray_real_T *Xpk,
                      emxArray_real_T *Wpk);

#endif

//
// File trailer for findpeaks.h
//
// [EOF]
//
