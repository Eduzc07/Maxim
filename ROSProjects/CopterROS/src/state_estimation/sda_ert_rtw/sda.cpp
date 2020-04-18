//
// File: sda.cpp
//
// Code generated for Simulink model 'sda'.
//
// Model version                  : 1.38
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sat Oct  1 14:19:10 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "sda.h"
#include "sda_private.h"

// Function for MATLAB Function: '<Root>/SDA Update'
real_T sdaModelClass::sda_norm(const real_T x[3])
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
void sdaModelClass::step()
{
  real_T dsigma[3];
  real_T E_R_B[9];
  real_T rtb_Product[3];
  real_T rtb_Init_f[3];
  real_T dsigma_0[3];
  real_T dsigma_1[3];
  int32_T i;
  real_T tmp[3];
  real_T rtb_Init_c_idx_0;
  real_T rtb_Init_c_idx_1;
  real_T rtb_Init_c_idx_2;
  real_T rtb_Init_c_idx_3;
  real_T rtb_E_q_B1_idx_0;
  real_T rtb_E_q_B1_idx_1;
  real_T rtb_E_q_B1_idx_2;
  real_T rtb_E_q_B1_idx_3;
  real_T q_dsigma_idx_0;
  real_T q_dsigma_idx_2;
  real_T q_dsigma_idx_3;
  real_T rtb_Init_idx_0;
  real_T rtb_Init_idx_1;
  real_T rtb_Init_idx_2;

  // Product: '<Root>/Product' incorporates:
  //   Inport: '<Root>/acc'
  //   Inport: '<Root>/m'

  rtb_Product[0] = sda_U.acc[0] * sda_U.m;

  // Switch: '<S5>/Init' incorporates:
  //   Inport: '<Root>/ic_p'
  //   UnitDelay: '<S5>/FixPt Unit Delay1'
  //   UnitDelay: '<S5>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE != 0) {
    rtb_Init_idx_0 = sda_U.ic_p[0];
  } else {
    rtb_Init_idx_0 = sda_DW.FixPtUnitDelay1_DSTATE[0];
  }

  // Switch: '<S6>/Init' incorporates:
  //   Inport: '<Root>/ic_v'
  //   UnitDelay: '<S6>/FixPt Unit Delay1'
  //   UnitDelay: '<S6>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE_h != 0) {
    rtb_Init_f[0] = sda_U.ic_v[0];
  } else {
    rtb_Init_f[0] = sda_DW.FixPtUnitDelay1_DSTATE_j[0];
  }

  // Product: '<Root>/Product' incorporates:
  //   Inport: '<Root>/acc'
  //   Inport: '<Root>/m'

  rtb_Product[1] = sda_U.acc[1] * sda_U.m;

  // Switch: '<S5>/Init' incorporates:
  //   Inport: '<Root>/ic_p'
  //   UnitDelay: '<S5>/FixPt Unit Delay1'
  //   UnitDelay: '<S5>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE != 0) {
    rtb_Init_idx_1 = sda_U.ic_p[1];
  } else {
    rtb_Init_idx_1 = sda_DW.FixPtUnitDelay1_DSTATE[1];
  }

  // Switch: '<S6>/Init' incorporates:
  //   Inport: '<Root>/ic_v'
  //   UnitDelay: '<S6>/FixPt Unit Delay1'
  //   UnitDelay: '<S6>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE_h != 0) {
    rtb_Init_f[1] = sda_U.ic_v[1];
  } else {
    rtb_Init_f[1] = sda_DW.FixPtUnitDelay1_DSTATE_j[1];
  }

  // Product: '<Root>/Product' incorporates:
  //   Inport: '<Root>/acc'
  //   Inport: '<Root>/m'

  rtb_Product[2] = sda_U.acc[2] * sda_U.m;

  // Switch: '<S5>/Init' incorporates:
  //   Inport: '<Root>/ic_p'
  //   UnitDelay: '<S5>/FixPt Unit Delay1'
  //   UnitDelay: '<S5>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE != 0) {
    rtb_Init_idx_2 = sda_U.ic_p[2];
  } else {
    rtb_Init_idx_2 = sda_DW.FixPtUnitDelay1_DSTATE[2];
  }

  // Switch: '<S6>/Init' incorporates:
  //   Inport: '<Root>/ic_v'
  //   UnitDelay: '<S6>/FixPt Unit Delay1'
  //   UnitDelay: '<S6>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE_h != 0) {
    rtb_Init_f[2] = sda_U.ic_v[2];
  } else {
    rtb_Init_f[2] = sda_DW.FixPtUnitDelay1_DSTATE_j[2];
  }

  // Switch: '<S7>/Init' incorporates:
  //   Inport: '<Root>/ic_q'
  //   UnitDelay: '<S7>/FixPt Unit Delay1'
  //   UnitDelay: '<S7>/FixPt Unit Delay2'

  if (sda_DW.FixPtUnitDelay2_DSTATE_p != 0) {
    rtb_Init_c_idx_0 = sda_U.ic_q[0];
    rtb_Init_c_idx_1 = sda_U.ic_q[1];
    rtb_Init_c_idx_2 = sda_U.ic_q[2];
    rtb_Init_c_idx_3 = sda_U.ic_q[3];
  } else {
    rtb_Init_c_idx_0 = sda_DW.FixPtUnitDelay1_DSTATE_o[0];
    rtb_Init_c_idx_1 = sda_DW.FixPtUnitDelay1_DSTATE_o[1];
    rtb_Init_c_idx_2 = sda_DW.FixPtUnitDelay1_DSTATE_o[2];
    rtb_Init_c_idx_3 = sda_DW.FixPtUnitDelay1_DSTATE_o[3];
  }

  // End of Switch: '<S7>/Init'

  // MATLAB Function: '<Root>/SDA Update' incorporates:
  //   Constant: '<Root>/dt'
  //   Inport: '<Root>/E_g'
  //   Inport: '<Root>/gyro'

  // MATLAB Function 'SDA Update': '<S1>:1'
  // '<S1>:1:4'
  dsigma[0] = sda_U.gyro[0] * 0.005;
  dsigma[1] = sda_U.gyro[1] * 0.005;
  dsigma[2] = sda_U.gyro[2] * 0.005;
  if (sda_norm(dsigma) != 0.0) {
    // '<S1>:1:5'
    // '<S1>:1:6'
    dsigma_1[0] = dsigma[0] / 2.0;
    dsigma_1[1] = dsigma[1] / 2.0;
    dsigma_1[2] = dsigma[2] / 2.0;
    rtb_E_q_B1_idx_0 = sda_norm(dsigma_1);
    rtb_E_q_B1_idx_1 = sin(sda_norm(dsigma) / 2.0);
    dsigma_0[0] = dsigma[0] / 2.0;
    rtb_E_q_B1_idx_3 = dsigma[0] / rtb_E_q_B1_idx_0 * rtb_E_q_B1_idx_1;
    dsigma_0[1] = dsigma[1] / 2.0;
    q_dsigma_idx_2 = dsigma[1] / rtb_E_q_B1_idx_0 * rtb_E_q_B1_idx_1;
    dsigma_0[2] = dsigma[2] / 2.0;
    q_dsigma_idx_3 = dsigma[2] / rtb_E_q_B1_idx_0 * rtb_E_q_B1_idx_1;
    q_dsigma_idx_0 = cos(sda_norm(dsigma_0));

    // '<S1>:1:7'
    rtb_E_q_B1_idx_0 = ((rtb_Init_c_idx_0 * q_dsigma_idx_0 - rtb_Init_c_idx_1 *
                         rtb_E_q_B1_idx_3) - rtb_Init_c_idx_2 * q_dsigma_idx_2)
      - rtb_Init_c_idx_3 * q_dsigma_idx_3;
    rtb_E_q_B1_idx_1 = (rtb_Init_c_idx_0 * rtb_E_q_B1_idx_3 + q_dsigma_idx_0 *
                        rtb_Init_c_idx_1) + (rtb_Init_c_idx_2 * q_dsigma_idx_3 -
      rtb_Init_c_idx_3 * q_dsigma_idx_2);
    rtb_E_q_B1_idx_2 = (rtb_Init_c_idx_0 * q_dsigma_idx_2 + q_dsigma_idx_0 *
                        rtb_Init_c_idx_2) + (rtb_Init_c_idx_3 * rtb_E_q_B1_idx_3
      - rtb_Init_c_idx_1 * q_dsigma_idx_3);
    rtb_E_q_B1_idx_3 = (rtb_Init_c_idx_0 * q_dsigma_idx_3 + q_dsigma_idx_0 *
                        rtb_Init_c_idx_3) + (rtb_Init_c_idx_1 * q_dsigma_idx_2 -
      rtb_Init_c_idx_2 * rtb_E_q_B1_idx_3);
  } else {
    // '<S1>:1:9'
    rtb_E_q_B1_idx_0 = rtb_Init_c_idx_0;
    rtb_E_q_B1_idx_1 = rtb_Init_c_idx_1;
    rtb_E_q_B1_idx_2 = rtb_Init_c_idx_2;
    rtb_E_q_B1_idx_3 = rtb_Init_c_idx_3;
  }

  // '<S1>:1:11'
  E_R_B[0] = 1.0 - (rtb_Init_c_idx_2 * rtb_Init_c_idx_2 + rtb_Init_c_idx_3 *
                    rtb_Init_c_idx_3) * 2.0;
  E_R_B[3] = (rtb_Init_c_idx_1 * rtb_Init_c_idx_2 - rtb_Init_c_idx_0 *
              rtb_Init_c_idx_3) * 2.0;
  E_R_B[6] = (rtb_Init_c_idx_0 * rtb_Init_c_idx_2 + rtb_Init_c_idx_1 *
              rtb_Init_c_idx_3) * 2.0;
  E_R_B[1] = (rtb_Init_c_idx_1 * rtb_Init_c_idx_2 + rtb_Init_c_idx_0 *
              rtb_Init_c_idx_3) * 2.0;
  E_R_B[4] = 1.0 - (rtb_Init_c_idx_1 * rtb_Init_c_idx_1 + rtb_Init_c_idx_3 *
                    rtb_Init_c_idx_3) * 2.0;
  E_R_B[7] = (rtb_Init_c_idx_2 * rtb_Init_c_idx_3 - rtb_Init_c_idx_0 *
              rtb_Init_c_idx_1) * 2.0;
  E_R_B[2] = (rtb_Init_c_idx_1 * rtb_Init_c_idx_3 - rtb_Init_c_idx_0 *
              rtb_Init_c_idx_2) * 2.0;
  E_R_B[5] = (rtb_Init_c_idx_0 * rtb_Init_c_idx_1 + rtb_Init_c_idx_2 *
              rtb_Init_c_idx_3) * 2.0;
  E_R_B[8] = 1.0 - (rtb_Init_c_idx_1 * rtb_Init_c_idx_1 + rtb_Init_c_idx_2 *
                    rtb_Init_c_idx_2) * 2.0;

  // '<S1>:1:14'
  for (i = 0; i < 3; i++) {
    dsigma[i] = ((E_R_B[3 * i + 1] * sda_U.E_g[1] + E_R_B[3 * i] * sda_U.E_g[0])
                 + E_R_B[3 * i + 2] * sda_U.E_g[2]) + rtb_Product[i];
  }

  //  TODO: CHECK weird Sigma cross a
  //  quat2axang(E_q_B');
  // '<S1>:1:17'
  tmp[0] = 0.0 * dsigma[2] - 0.0 * dsigma[1];
  tmp[1] = 0.0 * dsigma[0] - 0.0 * dsigma[2];
  tmp[2] = 0.0 * dsigma[1] - 0.0 * dsigma[0];
  for (i = 0; i < 3; i++) {
    dsigma[i] = ((((E_R_B[i + 3] * rtb_Product[1] + E_R_B[i] * rtb_Product[0]) +
                   E_R_B[i + 6] * rtb_Product[2]) + sda_U.E_g[i]) + 0.0 * tmp[i])
      * 0.005 + rtb_Init_f[i];
  }

  // '<S1>:1:18'
  q_dsigma_idx_2 = rtb_Init_idx_0 + dsigma[0];

  // Outport: '<Root>/p'
  sda_Y.p[0] = q_dsigma_idx_2;

  // Outport: '<Root>/v'
  sda_Y.v[0] = dsigma[0];

  // MATLAB Function: '<Root>/SDA Update'
  rtb_Product[0] = q_dsigma_idx_2;
  q_dsigma_idx_2 = rtb_Init_idx_1 + dsigma[1];

  // Outport: '<Root>/p'
  sda_Y.p[1] = q_dsigma_idx_2;

  // Outport: '<Root>/v'
  sda_Y.v[1] = dsigma[1];

  // MATLAB Function: '<Root>/SDA Update'
  rtb_Product[1] = q_dsigma_idx_2;
  q_dsigma_idx_2 = rtb_Init_idx_2 + dsigma[2];

  // Outport: '<Root>/p'
  sda_Y.p[2] = q_dsigma_idx_2;

  // Outport: '<Root>/v'
  sda_Y.v[2] = dsigma[2];

  // Outport: '<Root>/E_q_B'
  sda_Y.E_q_B[0] = rtb_E_q_B1_idx_0;
  sda_Y.E_q_B[1] = rtb_E_q_B1_idx_1;
  sda_Y.E_q_B[2] = rtb_E_q_B1_idx_2;
  sda_Y.E_q_B[3] = rtb_E_q_B1_idx_3;

  // Update for UnitDelay: '<S5>/FixPt Unit Delay2' incorporates:
  //   Constant: '<S5>/FixPt Constant'

  sda_DW.FixPtUnitDelay2_DSTATE = 0U;

  // Update for UnitDelay: '<S6>/FixPt Unit Delay2' incorporates:
  //   Constant: '<S6>/FixPt Constant'

  sda_DW.FixPtUnitDelay2_DSTATE_h = 0U;

  // Switch: '<S2>/Enable' incorporates:
  //   Inport: '<Root>/NReset'
  //   Switch: '<S3>/Enable'
  //   Switch: '<S4>/Enable'

  if (sda_U.NReset != 0.0) {
    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[0] = rtb_Product[0];

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[0] = dsigma[0];

    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[1] = rtb_Product[1];

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[1] = dsigma[1];

    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[2] = q_dsigma_idx_2;

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[2] = dsigma[2];

    // Update for UnitDelay: '<S7>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_o[0] = rtb_E_q_B1_idx_0;
    sda_DW.FixPtUnitDelay1_DSTATE_o[1] = rtb_E_q_B1_idx_1;
    sda_DW.FixPtUnitDelay1_DSTATE_o[2] = rtb_E_q_B1_idx_2;
    sda_DW.FixPtUnitDelay1_DSTATE_o[3] = rtb_E_q_B1_idx_3;
  } else {
    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[0] = rtb_Init_idx_0;

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[0] = rtb_Init_f[0];

    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[1] = rtb_Init_idx_1;

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[1] = rtb_Init_f[1];

    // Update for UnitDelay: '<S5>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE[2] = rtb_Init_idx_2;

    // Update for UnitDelay: '<S6>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_j[2] = rtb_Init_f[2];

    // Update for UnitDelay: '<S7>/FixPt Unit Delay1'
    sda_DW.FixPtUnitDelay1_DSTATE_o[0] = rtb_Init_c_idx_0;
    sda_DW.FixPtUnitDelay1_DSTATE_o[1] = rtb_Init_c_idx_1;
    sda_DW.FixPtUnitDelay1_DSTATE_o[2] = rtb_Init_c_idx_2;
    sda_DW.FixPtUnitDelay1_DSTATE_o[3] = rtb_Init_c_idx_3;
  }

  // End of Switch: '<S2>/Enable'

  // Update for UnitDelay: '<S7>/FixPt Unit Delay2' incorporates:
  //   Constant: '<S7>/FixPt Constant'

  sda_DW.FixPtUnitDelay2_DSTATE_p = 0U;
}

// Model initialize function
void sdaModelClass::initialize()
{
  // Registration code

  // initialize error status
  rtmSetErrorStatus((&sda_M), (NULL));

  // states (dwork)
  (void) memset((void *)&sda_DW, 0,
                sizeof(DW_sda_T));

  // external inputs
  (void) memset((void *)&sda_U, 0,
                sizeof(ExtU_sda_T));

  // external outputs
  (void) memset((void *)&sda_Y, 0,
                sizeof(ExtY_sda_T));

  // InitializeConditions for UnitDelay: '<S5>/FixPt Unit Delay2'
  sda_DW.FixPtUnitDelay2_DSTATE = 1U;

  // InitializeConditions for UnitDelay: '<S6>/FixPt Unit Delay2'
  sda_DW.FixPtUnitDelay2_DSTATE_h = 1U;

  // InitializeConditions for UnitDelay: '<S7>/FixPt Unit Delay2'
  sda_DW.FixPtUnitDelay2_DSTATE_p = 1U;
}

// Model terminate function
void sdaModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
sdaModelClass::sdaModelClass()
{
}

// Destructor
sdaModelClass::~sdaModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_sda_T * sdaModelClass::getRTM()
{
  return (&sda_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
