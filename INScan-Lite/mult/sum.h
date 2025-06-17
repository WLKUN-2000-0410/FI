//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sum.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef SUM_H
#define SUM_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern double b_sum(const emxArray_real_T *x);
extern double c_sum(const emxArray_boolean_T *x);
extern void d_sum(const double x_data[], const int x_size[2], double y_data[],
                  int y_size[2]);
extern double sum(const emxArray_real_T *x);

#endif

//
// File trailer for sum.h
//
// [EOF]
//
