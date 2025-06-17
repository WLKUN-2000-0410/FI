//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mldivide.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef MLDIVIDE_H
#define MLDIVIDE_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_mldivide(const emxArray_real_T *A, emxArray_real_T *B);
extern void mldivide(const emxArray_real_T *A, const double B[1650],
                     emxArray_real_T *Y);

#endif

//
// File trailer for mldivide.h
//
// [EOF]
//
