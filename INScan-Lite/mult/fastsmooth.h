//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fastsmooth.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef FASTSMOOTH_H
#define FASTSMOOTH_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_sa(const emxArray_real_T *Y, emxArray_real_T *SmoothY);
extern void c_sa(const emxArray_real_T *Y, double smoothwidth, emxArray_real_T
                 *SmoothY);
extern void fastsmooth(const emxArray_real_T *Y, double w, double type,
  emxArray_real_T *SmoothY);
extern void sa(const emxArray_real_T *Y, emxArray_real_T *SmoothY);

#endif

//
// File trailer for fastsmooth.h
//
// [EOF]
//
