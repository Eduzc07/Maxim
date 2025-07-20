//
// File: control.cpp
//
// Code generated for Simulink model 'control'.
//
// Model version                  : 1.123
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Oct  9 01:24:07 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "control.h"
#include "control_private.h"

// Function for MATLAB Function: '<S1>/body axis desired'
real_T controlModelClass::control_norm(const real_T x[3])
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;
  scale = 2.2250738585072014E-308;
  absxk = fabs(x[0]);
  if (absxk > 2.2250738585072014E-308) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 2.2250738585072014E-308;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrt(y);
}

// Model step function
void controlModelClass::step()
{
  static const int8_T b_a[9] = { -3, 0, 0, 0, -3, 0, 0, 0, -3 };

  static const int8_T c_a[9] = { 8, 0, 0, 0, 8, 0, 0, 0, 8 };

  static const int8_T b[3] = { 0, 0, 1 };

  static const int8_T b_0[3] = { 0, 0, 1 };

  static const int8_T c[3] = { 1, 0, 0 };

  static const real_T a[9] = { -0.12, -0.0, -0.0, -0.0, -0.12, -0.0, -0.0, -0.0,
    -0.12 };

  static const real_T b_a_0[9] = { 0.12, 0.0, 0.0, 0.0, 0.12, 0.0, 0.0, 0.0,
    0.12 };

  real_T rtb_F_des[3];
  real_T rtb_z_B_des[3];
  real_T rtb_W_R_B[9];
  real_T rtb_W_R_B_des[9];
  real_T rtb_z_B_des_0[3];
  int32_T i;
  real_T c_0[9];
  real_T c_1[9];
  real_T tmp[9];
  int32_T i_0;
  real_T rtb_W_R_B_0[9];
  real_T a_0[3];
  real_T rtb_y_B_des_idx_0;
  real_T rtb_y_B_des_idx_1;
  real_T rtb_y_B_des_idx_2;
  real_T rtb_x_B_des_idx_0;
  real_T rtb_x_B_des_idx_1;
  real_T rtb_x_B_des_idx_2;
  real_T tmp_0;

  // Outputs for Atomic SubSystem: '<Root>/control'
  // MATLAB Function: '<S1>/PD-Cartesian' incorporates:
  //   Inport: '<Root>/State'
  //   Inport: '<Root>/ddr_T'
  //   Inport: '<Root>/dr_T'
  //   Inport: '<Root>/g'
  //   Inport: '<Root>/m '
  //   Inport: '<Root>/r_T'

  // MATLAB Function 'control/PD-Cartesian': '<S2>:1'
  //  standard value 1
  //  stadaard value 2.5
  // '<S2>:1:4'
  // '<S2>:1:5'
  // '<S2>:1:6'
  rtb_y_B_des_idx_2 = control_U.m * control_U.g;
  rtb_y_B_des_idx_0 = control_U.State[0] - control_U.r_T[0];
  rtb_y_B_des_idx_1 = control_U.State[6] - control_U.dr_T[0];
  rtb_x_B_des_idx_0 = control_U.State[1] - control_U.r_T[1];
  rtb_x_B_des_idx_1 = control_U.State[7] - control_U.dr_T[1];
  rtb_x_B_des_idx_2 = control_U.State[2] - control_U.r_T[2];
  tmp_0 = control_U.State[8] - control_U.dr_T[2];

  // MATLAB Function 'control/euler2bodyFrame': '<S5>:1'
  // '<S5>:1:3'
  // '<S5>:1:4'
  // '<S5>:1:5'
  //  Euler ZYX angles
  // '<S5>:1:8'
  // '<S5>:1:9'
  // '<S5>:1:10'
  // '<S5>:1:12'
  for (i = 0; i < 3; i++) {
    rtb_F_des[i] = ((((real_T)b_a[i + 6] * rtb_x_B_des_idx_2 + ((real_T)b_a[i +
      3] * rtb_x_B_des_idx_0 + (real_T)b_a[i] * rtb_y_B_des_idx_0)) - ((real_T)
      c_a[i + 6] * tmp_0 + ((real_T)c_a[i + 3] * rtb_x_B_des_idx_1 + (real_T)
      c_a[i] * rtb_y_B_des_idx_1))) + rtb_y_B_des_idx_2 * (real_T)b[i]) +
      control_U.m * control_U.ddr_T[i];

    // MATLAB Function: '<S1>/euler2bodyFrame' incorporates:
    //   Inport: '<Root>/ddr_T'

    c_0[3 * i] = c[i];
  }

  // End of MATLAB Function: '<S1>/PD-Cartesian'

  // MATLAB Function: '<S1>/euler2bodyFrame' incorporates:
  //   Inport: '<Root>/State'

  c_0[1] = 0.0;
  c_0[4] = cos(control_U.State[3]);
  c_0[7] = -sin(control_U.State[3]);
  c_0[2] = 0.0;
  c_0[5] = sin(control_U.State[3]);
  c_0[8] = cos(control_U.State[3]);
  rtb_W_R_B_0[0] = cos(control_U.State[4]);
  rtb_W_R_B_0[3] = 0.0;
  rtb_W_R_B_0[6] = sin(control_U.State[4]);
  rtb_W_R_B_0[1] = 0.0;
  rtb_W_R_B_0[4] = 1.0;
  rtb_W_R_B_0[7] = 0.0;
  rtb_W_R_B_0[2] = -sin(control_U.State[4]);
  rtb_W_R_B_0[5] = 0.0;
  rtb_W_R_B_0[8] = cos(control_U.State[4]);
  tmp[0] = cos(control_U.State[5]);
  tmp[3] = -sin(control_U.State[5]);
  tmp[6] = 0.0;
  tmp[1] = sin(control_U.State[5]);
  tmp[4] = cos(control_U.State[5]);
  tmp[7] = 0.0;
  for (i = 0; i < 3; i++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      c_1[i + 3 * i_0] = 0.0;
      c_1[i + 3 * i_0] += rtb_W_R_B_0[3 * i_0] * c_0[i];
      c_1[i + 3 * i_0] += rtb_W_R_B_0[3 * i_0 + 1] * c_0[i + 3];
      c_1[i + 3 * i_0] += rtb_W_R_B_0[3 * i_0 + 2] * c_0[i + 6];
    }

    tmp[2 + 3 * i] = b_0[i];
  }

  // MATLAB Function: '<S1>/body axis desired'
  //  Body axis
  // '<S5>:1:15'
  // '<S5>:1:16'
  // '<S5>:1:17'
  // MATLAB Function 'control/body axis desired': '<S3>:1'
  // '<S3>:1:2'
  rtb_y_B_des_idx_2 = control_norm(rtb_F_des);
  for (i_0 = 0; i_0 < 3; i_0++) {
    // MATLAB Function: '<S1>/euler2bodyFrame'
    for (i = 0; i < 3; i++) {
      rtb_W_R_B[i_0 + 3 * i] = 0.0;
      rtb_W_R_B[i_0 + 3 * i] += tmp[3 * i_0] * c_1[i];
      rtb_W_R_B[i_0 + 3 * i] += tmp[3 * i_0 + 1] * c_1[i + 3];
      rtb_W_R_B[i_0 + 3 * i] += tmp[3 * i_0 + 2] * c_1[i + 6];
    }

    // MATLAB Function: '<S1>/body axis desired'
    rtb_z_B_des[i_0] = rtb_F_des[i_0] / rtb_y_B_des_idx_2;
  }

  // MATLAB Function: '<S1>/body axis desired' incorporates:
  //   Inport: '<Root>/x_C_des'
  //   MATLAB Function: '<S1>/euler2bodyFrame'

  // '<S3>:1:3'
  rtb_z_B_des_0[0] = rtb_z_B_des[1] * control_U.x_C_des[2] - rtb_z_B_des[2] *
    control_U.x_C_des[1];
  rtb_z_B_des_0[1] = rtb_z_B_des[2] * control_U.x_C_des[0] - rtb_z_B_des[0] *
    control_U.x_C_des[2];
  rtb_z_B_des_0[2] = rtb_z_B_des[0] * control_U.x_C_des[1] - rtb_z_B_des[1] *
    control_U.x_C_des[0];
  rtb_y_B_des_idx_2 = control_norm(rtb_z_B_des_0);
  rtb_y_B_des_idx_0 = (rtb_z_B_des[1] * control_U.x_C_des[2] - rtb_z_B_des[2] *
                       control_U.x_C_des[1]) / rtb_y_B_des_idx_2;
  rtb_y_B_des_idx_1 = (rtb_z_B_des[2] * control_U.x_C_des[0] - rtb_z_B_des[0] *
                       control_U.x_C_des[2]) / rtb_y_B_des_idx_2;
  rtb_y_B_des_idx_2 = (rtb_z_B_des[0] * control_U.x_C_des[1] - rtb_z_B_des[1] *
                       control_U.x_C_des[0]) / rtb_y_B_des_idx_2;

  // '<S3>:1:4'
  rtb_x_B_des_idx_0 = rtb_y_B_des_idx_1 * rtb_z_B_des[2] - rtb_y_B_des_idx_2 *
    rtb_z_B_des[1];
  rtb_x_B_des_idx_1 = rtb_y_B_des_idx_2 * rtb_z_B_des[0] - rtb_y_B_des_idx_0 *
    rtb_z_B_des[2];
  rtb_x_B_des_idx_2 = rtb_y_B_des_idx_0 * rtb_z_B_des[1] - rtb_y_B_des_idx_1 *
    rtb_z_B_des[0];

  //   check if -x_B_des and -y_B_des is better for current quadrotor
  //   orientation
  if (((rtb_y_B_des_idx_0 * rtb_W_R_B[3] + rtb_y_B_des_idx_1 * rtb_W_R_B[4]) +
       rtb_y_B_des_idx_2 * rtb_W_R_B[5]) + ((rtb_x_B_des_idx_0 * rtb_W_R_B[0] +
        rtb_x_B_des_idx_1 * rtb_W_R_B[1]) + rtb_x_B_des_idx_2 * rtb_W_R_B[2]) >
      0.0) {
    // '<S3>:1:8'
    // '<S3>:1:9'
    rtb_W_R_B_des[0] = rtb_x_B_des_idx_0;
    rtb_W_R_B_des[1] = rtb_y_B_des_idx_0;
    rtb_W_R_B_des[2] = rtb_z_B_des[0];
    rtb_W_R_B_des[3] = rtb_x_B_des_idx_1;
    rtb_W_R_B_des[4] = rtb_y_B_des_idx_1;
    rtb_W_R_B_des[5] = rtb_z_B_des[1];
    rtb_W_R_B_des[6] = rtb_x_B_des_idx_2;
    rtb_W_R_B_des[7] = rtb_y_B_des_idx_2;
    rtb_W_R_B_des[8] = rtb_z_B_des[2];
  } else {
    // '<S3>:1:11'
    rtb_W_R_B_des[0] = -rtb_x_B_des_idx_0;
    rtb_W_R_B_des[1] = -rtb_y_B_des_idx_0;
    rtb_W_R_B_des[2] = rtb_z_B_des[0];
    rtb_W_R_B_des[3] = -rtb_x_B_des_idx_1;
    rtb_W_R_B_des[4] = -rtb_y_B_des_idx_1;
    rtb_W_R_B_des[5] = rtb_z_B_des[1];
    rtb_W_R_B_des[6] = -rtb_x_B_des_idx_2;
    rtb_W_R_B_des[7] = -rtb_y_B_des_idx_2;
    rtb_W_R_B_des[8] = rtb_z_B_des[2];
  }

  // MATLAB Function: '<S1>/rotation Error	' incorporates:
  //   Inport: '<Root>/B_w_BWT'
  //   Inport: '<Root>/State'
  //   MATLAB Function: '<S1>/calculation of outputs'

  // MATLAB Function 'control/rotation Error	': '<S6>:1'
  // '<S6>:1:2'
  for (i = 0; i < 3; i++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      c_0[i + 3 * i_0] = 0.0;
      rtb_W_R_B_0[i + 3 * i_0] = 0.0;
      c_0[i + 3 * i_0] += rtb_W_R_B_des[3 * i] * rtb_W_R_B[3 * i_0];
      rtb_W_R_B_0[i + 3 * i_0] += rtb_W_R_B[3 * i] * rtb_W_R_B_des[3 * i_0];
      c_0[i + 3 * i_0] += rtb_W_R_B_des[3 * i + 1] * rtb_W_R_B[3 * i_0 + 1];
      rtb_W_R_B_0[i + 3 * i_0] += rtb_W_R_B[3 * i + 1] * rtb_W_R_B_des[3 * i_0 +
        1];
      c_0[i + 3 * i_0] += rtb_W_R_B_des[3 * i + 2] * rtb_W_R_B[3 * i_0 + 2];
      rtb_W_R_B_0[i + 3 * i_0] += rtb_W_R_B[3 * i + 2] * rtb_W_R_B_des[3 * i_0 +
        2];
    }
  }

  for (i = 0; i < 3; i++) {
    rtb_W_R_B_des[3 * i] = c_0[3 * i] - rtb_W_R_B_0[3 * i];
    rtb_W_R_B_des[1 + 3 * i] = c_0[3 * i + 1] - rtb_W_R_B_0[3 * i + 1];
    rtb_W_R_B_des[2 + 3 * i] = c_0[3 * i + 2] - rtb_W_R_B_0[3 * i + 2];
  }

  // '<S6>:1:4'
  // '<S6>:1:5'
  // '<S6>:1:6'
  // '<S6>:1:8'
  // '<S6>:1:9'
  // MATLAB Function 'control/calculation of outputs': '<S4>:1'
  //  120
  // '<S4>:1:4'
  for (i = 0; i < 3; i++) {
    rtb_z_B_des_0[i] = control_U.State[9 + i] - control_U.B_w_BWT[i];
    a_0[i] = a[i + 6] * (0.5 * rtb_W_R_B_des[3]) + (a[i + 3] * (0.5 *
      -rtb_W_R_B_des[6]) + 0.5 * rtb_W_R_B_des[7] * a[i]);
  }

  // End of MATLAB Function: '<S1>/rotation Error	'

  // MATLAB Function: '<S1>/calculation of outputs'
  for (i = 0; i < 3; i++) {
    rtb_z_B_des[i] = a_0[i] - (b_a_0[i + 6] * rtb_z_B_des_0[2] + (b_a_0[i + 3] *
      rtb_z_B_des_0[1] + b_a_0[i] * rtb_z_B_des_0[0]));
  }

  // Outport: '<Root>/tau_x' incorporates:
  //   MATLAB Function: '<S1>/calculation of outputs'

  // '<S4>:1:5'
  // '<S4>:1:6'
  // '<S4>:1:7'
  control_Y.tau_x = rtb_z_B_des[0];

  // Outport: '<Root>/tau_y' incorporates:
  //   MATLAB Function: '<S1>/calculation of outputs'

  control_Y.tau_y = rtb_z_B_des[1];

  // Outport: '<Root>/tau_z' incorporates:
  //   MATLAB Function: '<S1>/calculation of outputs'

  control_Y.tau_z = rtb_z_B_des[2];

  // End of Outputs for SubSystem: '<Root>/control'

  // Outport: '<Root>/F_des'
  control_Y.F_des[0] = rtb_F_des[0];
  control_Y.F_des[1] = rtb_F_des[1];
  control_Y.F_des[2] = rtb_F_des[2];

  // Outputs for Atomic SubSystem: '<Root>/control'
  // Outport: '<Root>/thrust' incorporates:
  //   DotProduct: '<S1>/Dot Product'
  //   MATLAB Function: '<S1>/euler2bodyFrame'

  control_Y.thrust = (rtb_F_des[0] * rtb_W_R_B[6] + rtb_F_des[1] * rtb_W_R_B[7])
    + rtb_F_des[2] * rtb_W_R_B[8];

  // End of Outputs for SubSystem: '<Root>/control'
}

// Model initialize function
void controlModelClass::initialize()
{
  // Registration code

  // initialize error status
  rtmSetErrorStatus((&control_M), (NULL));

  // external inputs
  (void) memset((void *)&control_U, 0,
                sizeof(ExtU_control_T));

  // external outputs
  (void) memset((void *)&control_Y, 0,
                sizeof(ExtY_control_T));
}

// Model terminate function
void controlModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
controlModelClass::controlModelClass()
{
}

// Destructor
controlModelClass::~controlModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_control_T * controlModelClass::getRTM()
{
  return (&control_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
