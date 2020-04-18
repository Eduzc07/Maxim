//
// File: quadrotor_EKF.h
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
#ifndef RTW_HEADER_quadrotor_EKF_h_
#define RTW_HEADER_quadrotor_EKF_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef quadrotor_EKF_COMMON_INCLUDES_
# define quadrotor_EKF_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "dspsvd_rt.h"
#endif                                 // quadrotor_EKF_COMMON_INCLUDES_

#include "quadrotor_EKF_types.h"
#include "rt_zcfcn.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Block states (auto storage) for system '<Root>'
typedef struct {
  real_T x[21];                        // '<Root>/Resettable Delay1'
  real_T ResettableDelay_DSTATE[441];  // '<Root>/Resettable Delay'
  uint8_T icLoad;                      // '<Root>/Resettable Delay1'
  uint8_T icLoad_i;                    // '<Root>/Resettable Delay'
} DW_quadrotor_EKF_T;

// Zero-crossing (trigger) state
typedef struct {
  ZCSigState ResettableDelay1_Reset_ZCE;// '<Root>/Resettable Delay1'
  ZCSigState ResettableDelay_Reset_ZCE;// '<Root>/Resettable Delay'
} PrevZCX_quadrotor_EKF_T;

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T params[5];                    // '<Root>/params'
  real_T u_k[4];                       // '<Root>/u_k'
  real_T Q_k[441];                     // '<Root>/Q_k'
  real_T z_k[12];                      // '<Root>/z_k'
  real_T B_earth[3];                   // '<Root>/B_earth'
  real_T R_k[144];                     // '<Root>/R_k'
  real_T x_0[21];                      // '<Root>/x_0'
  real_T NReset;                       // '<Root>/NReset'
  real_T P_0[441];                     // '<Root>/P_0'
} ExtU_quadrotor_EKF_T;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T x[3];                         // '<Root>/x'
  real_T a[3];                         // '<Root>/a'
  real_T dx[3];                        // '<Root>/dx'
  real_T da[3];                        // '<Root>/da'
  real_T ddx[3];                       // '<Root>/ddx'
  real_T bias_acc[3];                  // '<Root>/bias_acc'
  real_T bias_gyro[3];                 // '<Root>/bias_gyro'
} ExtY_quadrotor_EKF_T;

// Real-time Model Data Structure
struct tag_RTM_quadrotor_EKF_T {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model quadrotor_EKF
class quadrotor_EKFModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU_quadrotor_EKF_T quadrotor_EKF_U;

  // External outputs
  ExtY_quadrotor_EKF_T quadrotor_EKF_Y;

  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  quadrotor_EKFModelClass();

  // Destructor
  ~quadrotor_EKFModelClass();

  // Real-Time Model get method
  RT_MODEL_quadrotor_EKF_T * getRTM();

  // private data and function members
 private:
  // Block states
  DW_quadrotor_EKF_T quadrotor_EKF_DW;
  PrevZCX_quadrotor_EKF_T quadrotor_EKF_PrevZCX;// Triggered events

  // Real-Time Model
  RT_MODEL_quadrotor_EKF_T quadrotor_EKF_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'quadrotor_EKF'
//  '<S1>'   : 'quadrotor_EKF/innovation//measurement residual'
//  '<S2>'   : 'quadrotor_EKF/innovation//residual covariance'
//  '<S3>'   : 'quadrotor_EKF/near-optimal Kalman gain'
//  '<S4>'   : 'quadrotor_EKF/predicted covariance estimate'
//  '<S5>'   : 'quadrotor_EKF/predicted state estimate'
//  '<S6>'   : 'quadrotor_EKF/update covariance estimate'
//  '<S7>'   : 'quadrotor_EKF/update state estimate'
//  '<S8>'   : 'quadrotor_EKF/innovation//measurement residual/nonlinear output model'
//  '<S9>'   : 'quadrotor_EKF/innovation//measurement residual/nonlinear output model/nonlinear output model'
//  '<S10>'  : 'quadrotor_EKF/innovation//residual covariance/linearized output model'
//  '<S11>'  : 'quadrotor_EKF/innovation//residual covariance/linearized output model/linearized output model matrix'
//  '<S12>'  : 'quadrotor_EKF/predicted covariance estimate/linearized state transition matrix'
//  '<S13>'  : 'quadrotor_EKF/predicted covariance estimate/linearized state transition matrix/F_k-1=dF//dx|x=x_k-1'
//  '<S14>'  : 'quadrotor_EKF/predicted state estimate/nonlinear model'

#endif                                 // RTW_HEADER_quadrotor_EKF_h_

//
// File trailer for generated code.
//
// [EOF]
//
