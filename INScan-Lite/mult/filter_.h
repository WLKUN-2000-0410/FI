//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: filter.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef FILTER_H
#define FILTER_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void filter(const double b_data[], const int b_size[1], const
                   emxArray_real_T *x, emxArray_real_T *y);

#endif

//
// File trailer for filter.h
//
// [EOF]
//
