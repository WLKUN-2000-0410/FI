//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sort1.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef SORT1_H
#define SORT1_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_sort(emxArray_int32_T *x);
extern void c_sort(emxArray_real_T *x, emxArray_int32_T *idx);
extern void sort(emxArray_real_T *x, emxArray_int32_T *idx);

#endif

//
// File trailer for sort1.h
//
// [EOF]
//
