//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: power.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef POWER_H
#define POWER_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_power(const double a_data[], const int a_size[1], double b, double
                    y_data[], int y_size[1]);
extern void c_power(const emxArray_real_T *a, double b, emxArray_real_T *y);
extern void power(const emxArray_real_T *a, emxArray_real_T *y);

#endif

//
// File trailer for power.h
//
// [EOF]
//
