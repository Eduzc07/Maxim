//
// File: quadrotor_EKF_private.h
//
// Code generated for Simulink model 'quadrotor_EKF'.
//
// Model version                  : 1.30
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Mon Oct 10 01:28:14 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_quadrotor_EKF_private_h_
#define RTW_HEADER_quadrotor_EKF_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"

extern real_T rt_powd_snf(real_T u0, real_T u1);
extern void MWDSPCG_Pseudoinverse_D(const real_T pS[], const real_T pU[], const
  real_T pV[], real_T pX[], int32_T M, int32_T N);

#endif                                 // RTW_HEADER_quadrotor_EKF_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
