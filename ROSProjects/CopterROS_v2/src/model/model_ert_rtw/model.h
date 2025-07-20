//
// File: model.h
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
#ifndef RTW_HEADER_model_h_
#define RTW_HEADER_model_h_
#include <math.h>
#include <string.h>
#ifndef model_COMMON_INCLUDES_
# define model_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 // model_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T F_v[12];                      // '<S1>/MATLAB Function'
} DW;

// Continuous states (auto storage)
typedef struct {
  real_T Integrator_CSTATE[12];        // '<S1>/Integrator'
} X;

// State derivatives (auto storage)
typedef struct {
  real_T Integrator_CSTATE[12];        // '<S1>/Integrator'
} XDot;

// State disabled
typedef struct {
  boolean_T Integrator_CSTATE[12];     // '<S1>/Integrator'
} XDis;

#ifndef ODE5_INTG
#define ODE5_INTG

// ODE5 Integration Data
typedef struct {
  real_T *y;                           // output
  real_T *f[6];                        // derivatives
} ODE5_IntgData;

#endif

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T g;                            // '<Root>/g '
  real_T m;                            // '<Root>/m '
  real_T Jxx;                          // '<Root>/Jxx'
  real_T Jyy;                          // '<Root>/Jyy'
  real_T Jzz;                          // '<Root>/Jzz'
  real_T tau_x;                        // '<Root>/tau_x'
  real_T tau_y;                        // '<Root>/tau_y'
  real_T tau_z;                        // '<Root>/tau_z'
  real_T thrust;                       // '<Root>/thrust'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T state[12];                    // '<Root>/state'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  //
  //  ModelData:
  //  The following substructure contains information regarding
  //  the data used in the model.

  struct {
    X *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T blkStateChange;
    real_T odeY[12];
    real_T odeF[6][12];
    ODE5_IntgData intgData;
  } ModelData;

  //
  //  Sizes:
  //  The following substructure contains sizes information
  //  for many of the model attributes such as inputs, outputs,
  //  dwork, sample times, etc.

  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model model
class modelModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  modelModelClass();

  // Destructor
  ~modelModelClass();

  // Real-Time Model get method
  RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;
  X rtX;                               // Block continuous states

  // Real-Time Model
  RT_MODEL rtM;

  // Continuous states update member function
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  // Derivatives member function
  void model_derivatives();
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Scope' : Unused code path elimination
//  Block '<S1>/Scope1' : Unused code path elimination


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
//  '<Root>' : 'model'
//  '<S1>'   : 'model/QuadrotorModel '
//  '<S2>'   : 'model/QuadrotorModel /MATLAB Function'

#endif                                 // RTW_HEADER_model_h_

//
// File trailer for generated code.
//
// [EOF]
//
