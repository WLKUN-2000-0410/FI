//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortIdx.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef SORTIDX_H
#define SORTIDX_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_sortIdx(emxArray_int32_T *x, emxArray_int32_T *idx);
extern void c_sortIdx(emxArray_real_T *x, emxArray_int32_T *idx);
extern void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset,
  int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real_T *xwork);
extern void sortIdx(const emxArray_real_T *x, emxArray_int32_T *idx);

#endif

//
// File trailer for sortIdx.h
//
// [EOF]
//
