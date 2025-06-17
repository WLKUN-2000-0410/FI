//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eml_setop.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef EML_SETOP_H
#define EML_SETOP_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);
extern void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);

#endif

//
// File trailer for eml_setop.h
//
// [EOF]
//
