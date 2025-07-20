//
// File: control.h
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
#ifndef RTW_HEADER_control_h_
#define RTW_HEADER_control_h_
#include <math.h>
#include <stddef.h>
#include <string.h>
#ifndef control_COMMON_INCLUDES_
# define control_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // control_COMMON_INCLUDES_

#include "control_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T r_T[3];                       // '<Root>/r_T'
  real_T dr_T[3];                      // '<Root>/dr_T'
  real_T ddr_T[3];                     // '<Root>/ddr_T'
  real_T x_C_des[3];                   // '<Root>/x_C_des'
  real_T B_w_BWT[3];                   // '<Root>/B_w_BWT'
  real_T m;                            // '<Root>/m '
  real_T g;                            // '<Root>/g'
  real_T State[12];                    // '<Root>/State'
} ExtU_control_T;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T tau_x;                        // '<Root>/tau_x'
  real_T tau_y;                        // '<Root>/tau_y'
  real_T tau_z;                        // '<Root>/tau_z'
  real_T thrust;                       // '<Root>/thrust'
  real_T F_des[3];                     // '<Root>/F_des'
} ExtY_control_T;

// Real-time Model Data Structure
struct tag_RTM_control_T {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model control
class controlModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU_control_T control_U;

  // External outputs
  ExtY_control_T control_Y;

  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  controlModelClass();

  // Destructor
  ~controlModelClass();

  // Real-Time Model get method
  RT_MODEL_control_T * getRTM();

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL_control_T control_M;

  // private member function(s) for subsystem '<Root>'
  real_T control_norm(const real_T x[3]);
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('ctl_quadrotor/Subsystem/control')    - opens subsystem ctl_quadrotor/Subsystem/control
//  hilite_system('ctl_quadrotor/Subsystem/control/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'ctl_quadrotor/Subsystem'
//  '<S1>'   : 'ctl_quadrotor/Subsystem/control'
//  '<S2>'   : 'ctl_quadrotor/Subsystem/control/PD-Cartesian'
//  '<S3>'   : 'ctl_quadrotor/Subsystem/control/body axis desired'
//  '<S4>'   : 'ctl_quadrotor/Subsystem/control/calculation of outputs'
//  '<S5>'   : 'ctl_quadrotor/Subsystem/control/euler2bodyFrame'
//  '<S6>'   : 'ctl_quadrotor/Subsystem/control/rotation Error	'

#endif                                 // RTW_HEADER_control_h_

//
// File trailer for generated code.
//
// [EOF]
//
