//
// File: model.cpp
//
// Code generated for Simulink model 'model'.
//
// Model version                  : 1.12
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sat Sep 17 20:56:06 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "model.h"

// Private macros used by the generated code to access rtModel
#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

// private model entry point functions
extern void model_derivatives();

//
// This function updates continuous states using the ODE5 fixed-step
// solver algorithm
//
void modelModelClass::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  // Solver Matrices
  static const real_T rt_ODE5_A[6] = {
    1.0/5.0, 3.0/10.0, 4.0/5.0, 8.0/9.0, 1.0, 1.0
  };

  static const real_T rt_ODE5_B[6][6] = {
    { 1.0/5.0, 0.0, 0.0, 0.0, 0.0, 0.0 },

    { 3.0/40.0, 9.0/40.0, 0.0, 0.0, 0.0, 0.0 },

    { 44.0/45.0, -56.0/15.0, 32.0/9.0, 0.0, 0.0, 0.0 },

    { 19372.0/6561.0, -25360.0/2187.0, 64448.0/6561.0, -212.0/729.0, 0.0, 0.0 },

    { 9017.0/3168.0, -355.0/33.0, 46732.0/5247.0, 49.0/176.0, -5103.0/18656.0,
      0.0 },

    { 35.0/384.0, 0.0, 500.0/1113.0, 125.0/192.0, -2187.0/6784.0, 11.0/84.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE5_IntgData *id = (ODE5_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T *f4 = id->f[4];
  real_T *f5 = id->f[5];
  real_T hB[6];
  int_T i;
  int_T nXc = 12;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  // Save the state values at time t in y, we'll use x as ynew.
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  // Assumes that rtsiSetT and ModelOutputs are up-to-date
  // f0 = f(t,y)
  rtsiSetdX(si, f0);
  model_derivatives();

  // f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*));
  hB[0] = h * rt_ODE5_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE5_A[0]);
  rtsiSetdX(si, f1);
  this->step();
  model_derivatives();

  // f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*));
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE5_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE5_A[1]);
  rtsiSetdX(si, f2);
  this->step();
  model_derivatives();

  // f(:,4) = feval(odefile, t + hA(3), y + f*hB(:,3), args(:)(*));
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE5_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, t + h*rt_ODE5_A[2]);
  rtsiSetdX(si, f3);
  this->step();
  model_derivatives();

  // f(:,5) = feval(odefile, t + hA(4), y + f*hB(:,4), args(:)(*));
  for (i = 0; i <= 3; i++) {
    hB[i] = h * rt_ODE5_B[3][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2] +
                   f3[i]*hB[3]);
  }

  rtsiSetT(si, t + h*rt_ODE5_A[3]);
  rtsiSetdX(si, f4);
  this->step();
  model_derivatives();

  // f(:,6) = feval(odefile, t + hA(5), y + f*hB(:,5), args(:)(*));
  for (i = 0; i <= 4; i++) {
    hB[i] = h * rt_ODE5_B[4][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2] +
                   f3[i]*hB[3] + f4[i]*hB[4]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f5);
  this->step();
  model_derivatives();

  // tnew = t + hA(6);
  // ynew = y + f*hB(:,6);
  for (i = 0; i <= 5; i++) {
    hB[i] = h * rt_ODE5_B[5][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2] +
                   f3[i]*hB[3] + f4[i]*hB[4] + f5[i]*hB[5]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

// Model step function
void modelModelClass::step()
{
  real_T t2;
  real_T t3;
  real_T t4;
  real_T t5;
  real_T t6;
  real_T t7;
  real_T t8;
  real_T t9;
  real_T t11;
  real_T t12;
  real_T t13;
  real_T t14;
  real_T t15;
  real_T t16;
  real_T t17;
  real_T t18;
  real_T t19;
  real_T t20;
  real_T t21;
  real_T t22;
  if (rtmIsMajorTimeStep((&rtM))) {
    // set solver stop time
    rtsiSetSolverStopTime(&(&rtM)->solverInfo,(((&rtM)->Timing.clockTick0+1)*
      (&rtM)->Timing.stepSize0));
  }                                    // end MajorTimeStep

  // Update absolute time of base rate at minor time step
  if (rtmIsMinorTimeStep((&rtM))) {
    (&rtM)->Timing.t[0] = rtsiGetT(&(&rtM)->solverInfo);
  }

  // Outputs for Atomic SubSystem: '<Root>/QuadrotorModel '
  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   Inport: '<Root>/Jxx'
  //   Inport: '<Root>/Jyy'
  //   Inport: '<Root>/Jzz'
  //   Inport: '<Root>/g '
  //   Inport: '<Root>/m '
  //   Inport: '<Root>/tau_x'
  //   Inport: '<Root>/tau_y'
  //   Inport: '<Root>/tau_z'
  //   Inport: '<Root>/thrust'
  //   Integrator: '<S1>/Integrator'

  // MATLAB Function 'QuadrotorModel /MATLAB Function': '<S2>:1'
  // F
  //     F_V = F(X1,X2,X3,PHI,THETA,PSI,DX1,DX2,DX3,DPHI,DTHETA,DPSI,G,M,JXX,JYY,JZZ,TAU_X,TAU_Y,TAU_Z,THRUST) 
  //     This function was generated by the Symbolic Math Toolbox version 6.3.
  //     27-Jul-2016 23:53:51
  // '<S2>:1:8'
  t2 = 1.0 / rtU.m;

  // '<S2>:1:9'
  t3 = cos(rtX.Integrator_CSTATE[4]);

  // '<S2>:1:10'
  t4 = sin(rtX.Integrator_CSTATE[4]);

  // '<S2>:1:11'
  t5 = cos(rtX.Integrator_CSTATE[3]);

  // '<S2>:1:12'
  t6 = cos(rtX.Integrator_CSTATE[5]);

  // '<S2>:1:13'
  t7 = t6 * t6;

  // '<S2>:1:14'
  t8 = rtU.Jxx * rtU.Jxx;

  // '<S2>:1:15'
  t9 = sin(rtX.Integrator_CSTATE[5]);

  // '<S2>:1:16'
  // '<S2>:1:17'
  t11 = sin(rtX.Integrator_CSTATE[4] * 2.0);

  // '<S2>:1:18'
  t12 = sin(rtX.Integrator_CSTATE[3]);

  // '<S2>:1:19'
  t13 = t3 * t3;

  // '<S2>:1:20'
  t14 = rtU.Jyy * rtU.Jyy;

  // '<S2>:1:21'
  t15 = rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[9];

  // '<S2>:1:22'
  t16 = 1.0 / rtU.Jxx;

  // '<S2>:1:23'
  t17 = 1.0 / rtU.Jyy;

  // '<S2>:1:24'
  t18 = 1.0 / (t3 * t3);

  // '<S2>:1:25'
  t19 = rtU.Jzz * rtU.Jzz;

  // '<S2>:1:26'
  t20 = t9 * t9;

  // '<S2>:1:27'
  t21 = t13 * t13;

  // '<S2>:1:28'
  t22 = rtX.Integrator_CSTATE[10] * rtX.Integrator_CSTATE[10];

  // '<S2>:1:29'
  rtDW.F_v[0] = rtX.Integrator_CSTATE[6];
  rtDW.F_v[1] = rtX.Integrator_CSTATE[7];
  rtDW.F_v[2] = rtX.Integrator_CSTATE[8];
  rtDW.F_v[3] = rtX.Integrator_CSTATE[9];
  rtDW.F_v[4] = rtX.Integrator_CSTATE[10];
  rtDW.F_v[5] = rtX.Integrator_CSTATE[11];
  rtDW.F_v[6] = t2 * t4 * rtU.thrust;
  rtDW.F_v[7] = -t2 * t3 * t12 * rtU.thrust;
  rtDW.F_v[8] = (rtU.g * rtU.m - t3 * t5 * rtU.thrust) * -t2;
  rtDW.F_v[9] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((-rtU.Jxx *
    t4 * rtU.tau_z - rtU.Jxx * t3 * t6 * rtU.tau_x) - rtU.Jxx * t5 * t6 *
    rtU.tau_x) + rtU.Jxx * t3 * t9 * rtU.tau_y) + rtU.Jxx * t4 * t7 * rtU.tau_z)
    + rtU.Jxx * t5 * t9 * rtU.tau_y) - rtU.Jyy * t4 * t7 * rtU.tau_z) -
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t8 * t11 * 0.5) -
    rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t3 * t8) - rtU.Jxx *
    rtU.Jyy * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t11 * 0.5)
    + rtU.Jxx * rtU.Jyy * rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] *
    t3) + rtU.Jxx * rtU.Jzz * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE
    [10] * t11 * 0.5) + rtU.Jxx * rtU.Jzz * rtX.Integrator_CSTATE[11] *
    rtX.Integrator_CSTATE[10] * t3) + rtU.Jxx * t3 * t4 * t5 * rtU.tau_z) +
    rtU.Jxx * t3 * t5 * t6 * rtU.tau_x) + rtU.Jxx * t3 * t6 * t7 * rtU.tau_x) +
    rtU.Jxx * t5 * t6 * t7 * rtU.tau_x) - rtU.Jxx * t3 * t7 * t9 * rtU.tau_y) -
    rtU.Jxx * t5 * t7 * t9 * rtU.tau_y) + rtU.Jxx * t4 * t6 * t12 * rtU.tau_y) +
    rtU.Jxx * t5 * t6 * t13 * rtU.tau_x) + rtU.Jxx * t4 * t9 * t12 * rtU.tau_x)
    - rtU.Jxx * t5 * t9 * t13 * rtU.tau_y) - rtU.Jyy * t3 * t5 * t6 * rtU.tau_x)
    - rtU.Jyy * t3 * t6 * t7 * rtU.tau_x) - rtU.Jyy * t5 * t6 * t7 * rtU.tau_x)
    + rtU.Jyy * t3 * t7 * t9 * rtU.tau_y) + rtU.Jyy * t5 * t7 * t9 * rtU.tau_y)
    + rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t3 * t7 * t8) -
    rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t3 * t7 * t14) - t4 *
    t6 * t8 * t9 * t13 * t15) + t4 * t6 * t9 * t13 * t14 * t15) - rtU.Jxx *
    rtU.Jzz * rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t3 * t7) +
    rtU.Jyy * rtU.Jzz * rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] *
    t3 * t7) - rtU.Jxx * t3 * t4 * t5 * t7 * rtU.tau_z) - rtU.Jxx * t3 * t5 * t6
    * t7 * rtU.tau_x) + rtU.Jxx * t3 * t5 * t7 * t9 * rtU.tau_y) - rtU.Jxx * t3 *
    t4 * t6 * t12 * rtU.tau_y) - rtU.Jxx * t4 * t6 * t7 * t12 * rtU.tau_y) -
    rtU.Jxx * t5 * t6 * t7 * t13 * rtU.tau_x) - rtU.Jxx * t4 * t7 * t9 * t12 *
    rtU.tau_x) + rtU.Jxx * t5 * t7 * t9 * t13 * rtU.tau_y) - rtU.Jxx * t6 * t9 *
    t12 * t13 * rtU.tau_z) + rtU.Jyy * t3 * t4 * t5 * t7 * rtU.tau_z) + rtU.Jyy *
    t3 * t5 * t6 * t7 * rtU.tau_x) - rtU.Jyy * t3 * t5 * t7 * t9 * rtU.tau_y) +
    rtU.Jyy * t3 * t4 * t6 * t12 * rtU.tau_y) + rtU.Jyy * t4 * t6 * t7 * t12 *
                       rtU.tau_y) + rtU.Jyy * t5 * t6 * t7 * t13 * rtU.tau_x) +
                     rtU.Jyy * t4 * t7 * t9 * t12 * rtU.tau_x) - rtU.Jyy * t5 *
                    t7 * t9 * t13 * rtU.tau_y) + rtU.Jyy * t6 * t9 * t12 * t13 *
                   rtU.tau_z) - rtX.Integrator_CSTATE[9] *
                  rtX.Integrator_CSTATE[11] * t6 * t8 * t9 * t13) +
                 rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t6 * t9 *
                 t13 * t14) + rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE
                [10] * t3 * t4 * t7 * t8) - rtX.Integrator_CSTATE[9] *
               rtX.Integrator_CSTATE[10] * t3 * t4 * t7 * t14) + rtU.Jxx *
              rtU.Jzz * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] *
              t6 * t9 * t13) - rtU.Jyy * rtU.Jzz * rtX.Integrator_CSTATE[9] *
             rtX.Integrator_CSTATE[11] * t6 * t9 * t13) - rtU.Jxx * rtU.Jzz *
            rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t4 * t7)
           + rtU.Jyy * rtU.Jzz * rtX.Integrator_CSTATE[9] *
           rtX.Integrator_CSTATE[10] * t3 * t4 * t7) + rtU.Jxx * rtU.Jzz * t4 *
          t6 * t9 * t13 * t15) - rtU.Jyy * rtU.Jzz * t4 * t6 * t9 * t13 * t15) +
        rtU.Jxx * t3 * t4 * t6 * t7 * t12 * rtU.tau_y) + rtU.Jxx * t3 * t4 * t7 *
       t9 * t12 * rtU.tau_x) - rtU.Jyy * t3 * t4 * t6 * t7 * t12 * rtU.tau_y) -
     rtU.Jyy * t3 * t4 * t7 * t9 * t12 * rtU.tau_x) * (-t16 * t17 * t18);
  rtDW.F_v[10] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((rtU.Jxx * t3 *
    t6 * rtU.tau_y + rtU.Jxx * t5 * t6 * rtU.tau_y) - rtU.Jyy * t3 * t6 *
    rtU.tau_y) - rtU.Jyy * t5 * t6 * rtU.tau_y) - rtU.Jyy * t12 * t13 *
    rtU.tau_z) - rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t13 *
    t14) - t4 * t13 * t14 * t15) + rtU.Jxx * rtU.Jyy * rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[11] * t13) + rtU.Jyy * rtU.Jzz *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t13) + rtU.Jyy *
    rtU.Jzz * t4 * t13 * t15) - rtU.Jxx * t3 * t6 * t7 * rtU.tau_y) - rtU.Jxx *
    t3 * t7 * t9 * rtU.tau_x) - rtU.Jxx * t5 * t6 * t7 * rtU.tau_y) - rtU.Jxx *
    t4 * t6 * t9 * rtU.tau_z) - rtU.Jxx * t5 * t7 * t9 * rtU.tau_x) + rtU.Jxx *
    t4 * t6 * t12 * rtU.tau_x) - rtU.Jxx * t5 * t6 * t13 * rtU.tau_y) - rtU.Jxx *
    t7 * t12 * t13 * rtU.tau_z) + rtU.Jyy * t3 * t5 * t6 * rtU.tau_y) + rtU.Jyy *
    t3 * t5 * t9 * rtU.tau_x) + rtU.Jyy * t3 * t6 * t7 * rtU.tau_y) + rtU.Jyy *
    t3 * t7 * t9 * rtU.tau_x) + rtU.Jyy * t5 * t6 * t7 * rtU.tau_y) + rtU.Jyy *
    t4 * t6 * t9 * rtU.tau_z) + rtU.Jyy * t5 * t7 * t9 * rtU.tau_x) - rtU.Jyy *
    t4 * t6 * t12 * rtU.tau_x) + rtU.Jyy * t5 * t6 * t13 * rtU.tau_y) + rtU.Jyy *
    t7 * t12 * t13 * rtU.tau_z) - rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[11] * t7 * t8 * t13) + rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[11] * t7 * t13 * t14) - t4 * t7 * t8 * t13 * t15) + t4
    * t7 * t13 * t14 * t15) + rtU.Jxx * rtU.Jzz * rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[11] * t7 * t13) - rtU.Jyy * rtU.Jzz *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t7 * t13) + rtU.Jxx *
    rtU.Jzz * t4 * t7 * t13 * t15) - rtU.Jyy * rtU.Jzz * t4 * t7 * t13 * t15) +
    rtU.Jxx * t3 * t5 * t6 * t7 * rtU.tau_y) + rtU.Jxx * t3 * t5 * t7 * t9 *
    rtU.tau_x) - rtU.Jxx * t4 * t6 * t7 * t12 * rtU.tau_x) + rtU.Jxx * t5 * t6 *
    t7 * t13 * rtU.tau_y) + rtU.Jxx * t4 * t7 * t9 * t12 * rtU.tau_y) + rtU.Jxx *
    t5 * t7 * t9 * t13 * rtU.tau_x) - rtU.Jyy * t3 * t5 * t6 * t7 * rtU.tau_y) -
    rtU.Jyy * t3 * t5 * t7 * t9 * rtU.tau_x) + rtU.Jyy * t3 * t4 * t6 * t12 *
    rtU.tau_x) - rtU.Jyy * t3 * t4 * t9 * t12 * rtU.tau_y) + rtU.Jyy * t4 * t6 *
                      t7 * t12 * rtU.tau_x) - rtU.Jyy * t5 * t6 * t7 * t13 *
                     rtU.tau_y) - rtU.Jyy * t4 * t7 * t9 * t12 * rtU.tau_y) -
                   rtU.Jyy * t5 * t7 * t9 * t13 * rtU.tau_x) -
                  rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t3 *
                  t6 * t8 * t9) + rtX.Integrator_CSTATE[11] *
                 rtX.Integrator_CSTATE[10] * t3 * t6 * t9 * t14) + rtU.Jxx *
                rtU.Jzz * rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] *
                t3 * t6 * t9) - rtU.Jyy * rtU.Jzz * rtX.Integrator_CSTATE[11] *
               rtX.Integrator_CSTATE[10] * t3 * t6 * t9) + rtU.Jxx * t3 * t4 *
              t5 * t6 * t9 * rtU.tau_z) + rtU.Jxx * t3 * t4 * t6 * t7 * t12 *
             rtU.tau_x) - rtU.Jxx * t3 * t4 * t7 * t9 * t12 * rtU.tau_y) -
           rtU.Jyy * t3 * t4 * t5 * t6 * t9 * rtU.tau_z) - rtU.Jyy * t3 * t4 *
          t6 * t7 * t12 * rtU.tau_x) + rtU.Jyy * t3 * t4 * t7 * t9 * t12 *
         rtU.tau_y) - rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 *
        t4 * t6 * t8 * t9) + rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10]
       * t3 * t4 * t6 * t9 * t14) + rtU.Jxx * rtU.Jzz * rtX.Integrator_CSTATE[9]
      * rtX.Integrator_CSTATE[10] * t3 * t4 * t6 * t9) - rtU.Jyy * rtU.Jzz *
     rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t4 * t6 * t9) *
    (t16 * t17) / t3;
  rtDW.F_v[11] =
    (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
    (((((((((((((((((((((((((((((rtU.Jxx * rtU.Jzz * t7 * rtU.tau_z + -rtU.Jxx *
    rtU.Jzz * rtU.tau_z) + rtU.Jxx * rtU.Jzz * t13 * rtU.tau_z) - rtU.Jyy *
    rtU.Jzz * t7 * rtU.tau_z) + rtU.Jxx * rtU.Jzz * t3 * t5 * rtU.tau_z) +
    rtU.Jxx * rtU.Jzz * t6 * t12 * rtU.tau_y) - rtU.Jxx * rtU.Jzz * t7 * t13 *
    rtU.tau_z) + rtU.Jyy * rtU.Jzz * t9 * t12 * rtU.tau_x) + rtU.Jyy * rtU.Jzz *
    t7 * t13 * rtU.tau_z) + rtU.Jxx * rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[10] * t3 * t19) + rtU.Jxx * rtX.Integrator_CSTATE[11] *
    rtX.Integrator_CSTATE[10] * t11 * t19 * 0.5) - rtU.Jzz *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t8) - rtU.Jzz *
    rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t8 * t11 * 0.5) -
    rtU.Jxx * rtU.Jyy * rtU.Jzz * rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[10] * t3) + rtU.Jxx * rtU.Jyy * rtU.Jzz *
    rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10] * t11 * 0.5) + rtU.Jxx
    * rtU.Jyy * t3 * t5 * t13 * rtU.tau_z) + rtU.Jxx * rtU.Jyy * t6 * t12 * t13 *
    rtU.tau_y) + rtU.Jxx * rtU.Jyy * t9 * t12 * t13 * rtU.tau_x) - rtU.Jxx *
    rtU.Jzz * t3 * t4 * t6 * rtU.tau_x) - rtU.Jxx * rtU.Jzz * t4 * t5 * t6 *
    rtU.tau_x) + rtU.Jxx * rtU.Jzz * t3 * t4 * t9 * rtU.tau_y) - rtU.Jxx *
    rtU.Jzz * t3 * t5 * t7 * rtU.tau_z) + rtU.Jxx * rtU.Jzz * t4 * t5 * t9 *
    rtU.tau_y) - rtU.Jxx * rtU.Jzz * t3 * t6 * t12 * rtU.tau_y) - rtU.Jxx *
    rtU.Jzz * t3 * t5 * t13 * rtU.tau_z) - rtU.Jxx * rtU.Jzz * t6 * t7 * t12 *
    rtU.tau_y) - rtU.Jxx * rtU.Jzz * t6 * t12 * t13 * rtU.tau_y) - rtU.Jxx *
    rtU.Jzz * t9 * t12 * t13 * rtU.tau_x) + rtU.Jxx * rtU.Jzz * t9 * t12 * t20 *
    rtU.tau_x) + rtU.Jyy * rtU.Jzz * t3 * t5 * t7 * rtU.tau_z) + rtU.Jyy *
    rtU.Jzz * t3 * t6 * t12 * rtU.tau_y) + rtU.Jyy * rtU.Jzz * t6 * t7 * t12 *
    rtU.tau_y) - rtU.Jyy * rtU.Jzz * t9 * t12 * t20 * rtU.tau_x) - rtU.Jxx *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t7 * t19) +
    rtU.Jxx * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t13 *
    t14) - rtU.Jxx * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 *
    t13 * t19) - rtU.Jyy * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] *
    t3 * t8 * t13) + rtU.Jyy * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE
    [10] * t3 * t7 * t19) + rtU.Jzz * rtX.Integrator_CSTATE[9] *
    rtX.Integrator_CSTATE[10] * t3 * t7 * t8) - rtU.Jzz * rtX.Integrator_CSTATE
    [9] * rtX.Integrator_CSTATE[10] * t3 * t7 * t14) + rtU.Jzz *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t8 * t13) +
    rtU.Jxx * t6 * t9 * t13 * t15 * t19) - rtU.Jxx * t6 * t9 * t13 * t14 * t22)
    + rtU.Jxx * t6 * t9 * t14 * t15 * t21) - rtU.Jxx * t6 * t9 * t15 * t19 * t21)
    + rtU.Jyy * t6 * t8 * t9 * t13 * t22) - rtU.Jyy * t6 * t8 * t9 * t15 * t21)
    - rtU.Jyy * t6 * t9 * t13 * t15 * t19) + rtU.Jyy * t6 * t9 * t15 * t19 * t21)
    - rtU.Jzz * t6 * t8 * t9 * t13 * t15) + rtU.Jzz * t6 * t9 * t13 * t14 * t15)
    + rtU.Jzz * t6 * t8 * t9 * t15 * t21) - rtU.Jzz * t6 * t9 * t14 * t15 * t21)
    - rtU.Jxx * rtU.Jyy * t4 * t5 * t6 * t13 * rtU.tau_x) + rtU.Jxx * rtU.Jyy *
    t4 * t5 * t9 * t13 * rtU.tau_y) + rtU.Jxx * rtU.Jzz * t3 * t4 * t5 * t6 *
    rtU.tau_x) + rtU.Jxx * rtU.Jzz * t3 * t4 * t6 * t7 * rtU.tau_x) + rtU.Jxx *
    rtU.Jzz * t4 * t5 * t6 * t7 * rtU.tau_x) - rtU.Jxx * rtU.Jzz * t3 * t4 * t7 *
    t9 * rtU.tau_y) - rtU.Jxx * rtU.Jzz * t4 * t5 * t7 * t9 * rtU.tau_y) +
    rtU.Jxx * rtU.Jzz * t4 * t5 * t6 * t13 * rtU.tau_x) + rtU.Jxx * rtU.Jzz * t3
    * t6 * t7 * t12 * rtU.tau_y) + rtU.Jxx * rtU.Jzz * t3 * t5 * t7 * t13 *
    rtU.tau_z) + rtU.Jxx * rtU.Jzz * t3 * t7 * t9 * t12 * rtU.tau_x) - rtU.Jxx *
    rtU.Jzz * t4 * t5 * t9 * t13 * rtU.tau_y) + rtU.Jxx * rtU.Jzz * t3 * t6 *
    t12 * t13 * rtU.tau_y) + rtU.Jxx * rtU.Jzz * t6 * t7 * t12 * t13 * rtU.tau_y)
    + rtU.Jxx * rtU.Jzz * t7 * t9 * t12 * t13 * rtU.tau_x) - rtU.Jyy * rtU.Jzz *
    t3 * t4 * t5 * t6 * rtU.tau_x) - rtU.Jyy * rtU.Jzz * t3 * t4 * t6 * t7 *
    rtU.tau_x) - rtU.Jyy * rtU.Jzz * t4 * t5 * t6 * t7 * rtU.tau_x) + rtU.Jyy *
    rtU.Jzz * t3 * t4 * t7 * t9 * rtU.tau_y) + rtU.Jyy * rtU.Jzz * t4 * t5 * t7 *
    t9 * rtU.tau_y) - rtU.Jyy * rtU.Jzz * t3 * t6 * t7 * t12 * rtU.tau_y) -
    rtU.Jyy * rtU.Jzz * t3 * t5 * t7 * t13 * rtU.tau_z) - rtU.Jyy * rtU.Jzz * t3
    * t7 * t9 * t12 * rtU.tau_x) - rtU.Jyy * rtU.Jzz * t3 * t6 * t12 * t13 *
    rtU.tau_y) - rtU.Jyy * rtU.Jzz * t6 * t7 * t12 * t13 * rtU.tau_y) - rtU.Jyy *
    rtU.Jzz * t7 * t9 * t12 * t13 * rtU.tau_x) - rtU.Jxx *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t7 * t13 * t14 *
    2.0) + rtU.Jxx * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 *
    t7 * t13 * t19) - rtU.Jxx * rtX.Integrator_CSTATE[11] *
    rtX.Integrator_CSTATE[10] * t3 * t4 * t7 * t19) + rtU.Jyy *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t7 * t8 * t13 *
    2.0) - rtU.Jyy * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 *
    t7 * t13 * t19) + rtU.Jyy * rtX.Integrator_CSTATE[11] *
    rtX.Integrator_CSTATE[10] * t3 * t4 * t7 * t19) - rtU.Jzz *
    rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t7 * t8 * t13) +
    rtU.Jzz * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[10] * t3 * t7 *
    t13 * t14) + rtU.Jzz * rtX.Integrator_CSTATE[11] * rtX.Integrator_CSTATE[10]
    * t3 * t4 * t7 * t8) - rtU.Jzz * rtX.Integrator_CSTATE[11] *
                       rtX.Integrator_CSTATE[10] * t3 * t4 * t7 * t14) - rtU.Jxx
                      * rtU.Jzz * t3 * t4 * t5 * t6 * t7 * rtU.tau_x) + rtU.Jxx *
                     rtU.Jzz * t3 * t4 * t5 * t7 * t9 * rtU.tau_y) - rtU.Jxx *
                    rtU.Jzz * t4 * t5 * t6 * t7 * t13 * rtU.tau_x) + rtU.Jxx *
                   rtU.Jzz * t4 * t5 * t7 * t9 * t13 * rtU.tau_y) - rtU.Jxx *
                  rtU.Jzz * t3 * t6 * t7 * t12 * t13 * rtU.tau_y) - rtU.Jxx *
                 rtU.Jzz * t3 * t7 * t9 * t12 * t13 * rtU.tau_x) - rtU.Jxx *
                rtU.Jzz * t4 * t6 * t9 * t12 * t13 * rtU.tau_z) + rtU.Jyy *
               rtU.Jzz * t3 * t4 * t5 * t6 * t7 * rtU.tau_x) - rtU.Jyy * rtU.Jzz
              * t3 * t4 * t5 * t7 * t9 * rtU.tau_y) + rtU.Jyy * rtU.Jzz * t4 *
             t5 * t6 * t7 * t13 * rtU.tau_x) - rtU.Jyy * rtU.Jzz * t4 * t5 * t7 *
            t9 * t13 * rtU.tau_y) + rtU.Jyy * rtU.Jzz * t3 * t6 * t7 * t12 * t13
           * rtU.tau_y) + rtU.Jyy * rtU.Jzz * t3 * t7 * t9 * t12 * t13 *
          rtU.tau_x) + rtU.Jyy * rtU.Jzz * t4 * t6 * t9 * t12 * t13 * rtU.tau_z)
        + rtU.Jxx * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t4 *
        t6 * t9 * t13 * t19) - rtU.Jyy * rtX.Integrator_CSTATE[9] *
       rtX.Integrator_CSTATE[11] * t4 * t6 * t9 * t13 * t19) - rtU.Jzz *
      rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t4 * t6 * t8 * t9 *
      t13) + rtU.Jzz * rtX.Integrator_CSTATE[9] * rtX.Integrator_CSTATE[11] * t4
     * t6 * t9 * t13 * t14) * (t16 * t17 * t18) / rtU.Jzz;

  // Outport: '<Root>/state' incorporates:
  //   Integrator: '<S1>/Integrator'

  memcpy(&rtY.state[0], &rtX.Integrator_CSTATE[0], 12U * sizeof(real_T));

  // End of Outputs for SubSystem: '<Root>/QuadrotorModel '
  if (rtmIsMajorTimeStep((&rtM))) {
    rt_ertODEUpdateContinuousStates(&(&rtM)->solverInfo);

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.

    ++(&rtM)->Timing.clockTick0;
    (&rtM)->Timing.t[0] = rtsiGetSolverStopTime(&(&rtM)->solverInfo);

    {
      // Update absolute timer for sample time: [0.005s, 0.0s]
      // The "clockTick1" counts the number of times the code of this task has
      //  been executed. The resolution of this integer timer is 0.005, which is the step size
      //  of the task. Size of "clockTick1" ensures timer will not overflow during the
      //  application lifespan selected.

      (&rtM)->Timing.clockTick1++;
    }
  }                                    // end MajorTimeStep
}

// Derivatives for root system: '<Root>'
void modelModelClass::model_derivatives()
{
  XDot *_rtXdot;
  _rtXdot = ((XDot *) (&rtM)->ModelData.derivs);

  // Derivatives for Atomic SubSystem: '<Root>/QuadrotorModel '
  // Derivatives for Integrator: '<S1>/Integrator'
  memcpy(&_rtXdot->Integrator_CSTATE[0], &rtDW.F_v[0], 12U * sizeof(real_T));

  // End of Derivatives for SubSystem: '<Root>/QuadrotorModel '
}

// Model initialize function
void modelModelClass::initialize()
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&rtM)->solverInfo, &(&rtM)->Timing.simTimeStep);
    rtsiSetTPtr(&(&rtM)->solverInfo, &rtmGetTPtr((&rtM)));
    rtsiSetStepSizePtr(&(&rtM)->solverInfo, &(&rtM)->Timing.stepSize0);
    rtsiSetdXPtr(&(&rtM)->solverInfo, &(&rtM)->ModelData.derivs);
    rtsiSetContStatesPtr(&(&rtM)->solverInfo, (real_T **) &(&rtM)
                         ->ModelData.contStates);
    rtsiSetNumContStatesPtr(&(&rtM)->solverInfo, &(&rtM)->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->ModelData.periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->ModelData.periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&rtM)->solverInfo, (&rtmGetErrorStatus((&rtM))));
    rtsiSetRTModelPtr(&(&rtM)->solverInfo, (&rtM));
  }

  rtsiSetSimTimeStep(&(&rtM)->solverInfo, MAJOR_TIME_STEP);
  (&rtM)->ModelData.intgData.y = (&rtM)->ModelData.odeY;
  (&rtM)->ModelData.intgData.f[0] = (&rtM)->ModelData.odeF[0];
  (&rtM)->ModelData.intgData.f[1] = (&rtM)->ModelData.odeF[1];
  (&rtM)->ModelData.intgData.f[2] = (&rtM)->ModelData.odeF[2];
  (&rtM)->ModelData.intgData.f[3] = (&rtM)->ModelData.odeF[3];
  (&rtM)->ModelData.intgData.f[4] = (&rtM)->ModelData.odeF[4];
  (&rtM)->ModelData.intgData.f[5] = (&rtM)->ModelData.odeF[5];
  (&rtM)->ModelData.contStates = ((X *) &rtX);
  rtsiSetSolverData(&(&rtM)->solverInfo, (void *)&(&rtM)->ModelData.intgData);
  rtsiSetSolverName(&(&rtM)->solverInfo,"ode5");
  rtmSetTPtr((&rtM), &(&rtM)->Timing.tArray[0]);
  (&rtM)->Timing.stepSize0 = 0.005;

  // InitializeConditions for Atomic SubSystem: '<Root>/QuadrotorModel '
  // InitializeConditions for Integrator: '<S1>/Integrator'
  memset(&rtX.Integrator_CSTATE[0], 0, 12U * sizeof(real_T));

  // End of InitializeConditions for SubSystem: '<Root>/QuadrotorModel '
}

// Constructor
modelModelClass::modelModelClass()
{
}

// Destructor
modelModelClass::~modelModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL * modelModelClass::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
