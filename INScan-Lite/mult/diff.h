//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: diff.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef DIFF_H
#define DIFF_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_diff(const emxArray_real_T *x, emxArray_real_T *y);
extern void c_diff(const emxArray_real_T *x, emxArray_real_T *y);
extern void diff(const emxArray_real_T *x, emxArray_real_T *y);

#endif

//
// File trailer for diff.h
//
// [EOF]
//
