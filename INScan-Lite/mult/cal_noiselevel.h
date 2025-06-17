//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: cal_noiselevel.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Dec-2020 16:15:00
//
#ifndef CAL_NOISELEVEL_H
#define CAL_NOISELEVEL_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "omp.h"
#include "analyseMixture_types.h"

// Function Declarations
extern double b_cal_noiselevel(emxArray_real_T *data);
extern double c_cal_noiselevel(emxArray_real_T *data);
extern void cal_noiselevel(const emxArray_real_T *data, double window, double
  *noiselevel2, emxArray_real_T *c1);

#endif

//
// File trailer for cal_noiselevel.h
//
// [EOF]
//
