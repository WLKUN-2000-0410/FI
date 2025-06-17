//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xger.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef XGER_H
#define XGER_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_xger(int m, int n, double alpha1, int ix0, const double y[4],
                   double A[28], int ia0);
extern void xger(int m, int n, double alpha1, int ix0, const double y[4], double
                 A_data[], int ia0, int lda);

#endif

//
// File trailer for xger.h
//
// [EOF]
//
