//
// File: sda.h
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
#ifndef RTW_HEADER_sda_h_
#define RTW_HEADER_sda_h_
#include <math.h>
#include <stddef.h>
#include <string.h>
#ifndef sda_COMMON_INCLUDES_
# define sda_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // sda_COMMON_INCLUDES_

#include "sda_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Block states (auto storage) for system '<Root>'
typedef struct {
  real_T FixPtUnitDelay1_DSTATE[3];    // '<S5>/FixPt Unit Delay1'
  real_T FixPtUnitDelay1_DSTATE_j[3];  // '<S6>/FixPt Unit Delay1'
  real_T FixPtUnitDelay1_DSTATE_o[4];  // '<S7>/FixPt Unit Delay1'
  uint8_T FixPtUnitDelay2_DSTATE;      // '<S5>/FixPt Unit Delay2'
  uint8_T FixPtUnitDelay2_DSTATE_h;    // '<S6>/FixPt Unit Delay2'
  uint8_T FixPtUnitDelay2_DSTATE_p;    // '<S7>/FixPt Unit Delay2'
} DW_sda_T;

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T m;                            // '<Root>/m'
  real_T acc[3];                       // '<Root>/acc'
  real_T gyro[3];                      // '<Root>/gyro'
  real_T ic_p[3];                      // '<Root>/ic_p'
  real_T ic_v[3];                      // '<Root>/ic_v'
  real_T ic_q[4];                      // '<Root>/ic_q'
  real_T E_g[3];                       // '<Root>/E_g'
  real_T NReset;                       // '<Root>/NReset'
} ExtU_sda_T;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T p[3];                         // '<Root>/p'
  real_T v[3];                         // '<Root>/v'
  real_T E_q_B[4];                     // '<Root>/E_q_B'
} ExtY_sda_T;

// Real-time Model Data Structure
struct tag_RTM_sda_T {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model sda
class sdaModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU_sda_T sda_U;

  // External outputs
  ExtY_sda_T sda_Y;

  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  sdaModelClass();

  // Destructor
  ~sdaModelClass();

  // Real-Time Model get method
  RT_MODEL_sda_T * getRTM();

  // private data and function members
 private:
  // Block states
  DW_sda_T sda_DW;

  // Real-Time Model
  RT_MODEL_sda_T sda_M;

  // private member function(s) for subsystem '<Root>'
  real_T sda_norm(const real_T x[3]);
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
//  '<Root>' : 'sda'
//  '<S1>'   : 'sda/SDA Update'
//  '<S2>'   : 'sda/Unit Delay Enabled External IC'
//  '<S3>'   : 'sda/Unit Delay Enabled External IC1'
//  '<S4>'   : 'sda/Unit Delay Enabled External IC2'
//  '<S5>'   : 'sda/Unit Delay Enabled External IC/Unit Delay External IC'
//  '<S6>'   : 'sda/Unit Delay Enabled External IC1/Unit Delay External IC'
//  '<S7>'   : 'sda/Unit Delay Enabled External IC2/Unit Delay External IC'

#endif                                 // RTW_HEADER_sda_h_

//
// File trailer for generated code.
//
// [EOF]
//
