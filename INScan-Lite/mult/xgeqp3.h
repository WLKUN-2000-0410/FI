//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgeqp3.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef XGEQP3_H
#define XGEQP3_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1],
                     emxArray_int32_T *jpvt);
extern void xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1], int
                   jpvt_data[], int jpvt_size[2]);

#endif

//
// File trailer for xgeqp3.h
//
// [EOF]
//
