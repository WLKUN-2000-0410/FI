//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: qr.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef QR_H
#define QR_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern void b_qr(double A[28], double R[16]);
extern void qr(const double A_data[], const int A_size[2], double Q_data[], int
               Q_size[2], double R_data[], int R_size[2]);

#endif

//
// File trailer for qr.h
//
// [EOF]
//
