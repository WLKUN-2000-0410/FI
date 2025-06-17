//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: resolution_factor.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef RESOLUTION_FACTOR_H
#define RESOLUTION_FACTOR_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_resolution_factor(const emxArray_real_T *a, double d_data[], int
  d_size[2]);
extern void resolution_factor(const emxArray_real_T *a, double d_data[], int
  d_size[2]);

#endif

//
// File trailer for resolution_factor.h
//
// [EOF]
//
