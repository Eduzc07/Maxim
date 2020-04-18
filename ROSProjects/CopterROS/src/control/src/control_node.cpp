#include "ros/ros.h"
#include <ros/console.h>
#include "copter/Force_Torque.h"
#include "copter/State.h"
#include "copter/Position.h"
#include <stddef.h>
#include <stdio.h>
#include "control.h"
#include "rtwtypes.h"

typedef struct{
  real_T x[3];   // cartesian position
  real_T a[3];   // angular positio in euler coordinates (zyx) (phi, theta, psi)
  real_T dx[3];  // cartesian velocity
  real_T da[3];  // angular velocity in euler coordinates (zyx) (phi, theta, psi)
} State;

typedef struct {
  real_T tau_x;    // desired torque x-axis
  real_T tau_y;    // desired torque y-axis
  real_T tau_z;    // desired torque z-axis
  real_T thrust;   // desired thrust
  real_T F_des[3]; // desired cartesian Force
} Output;

typedef struct { 
  double mass;                 // mass of the quadrotor
  double gravity;              // acceleration due to gravity
  std::vector<double> x_C_des; // desired quadrotor x-axis in global frame 
} Params;

static double d_pos[3];     //Desired position
static double d_vel[3];     //Desired velocity
static double d_acc[3];     //Desired aceleration

static Output output;                   // Storage for values to send
static State quadrotorState;            // Quadrotor state vector being updated from /State topic
static controlModelClass control_Obj;   // Matlab Simulink controller implementation
static Params params;

void rt_OneStep(double[3],double[3],double[3]);

void rt_OneStep(double d_pos[3],double d_vel[3],double d_acc[3]){
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(control_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here
  // Setting 3-dim vectors
  for (int i=0;i<3;i++){
    control_Obj.control_U.r_T[i]=d_pos[i]+0.0001; //Problems when it is 0
    control_Obj.control_U.dr_T[i]=d_vel[i];
    control_Obj.control_U.ddr_T[i]=d_acc[i];
    control_Obj.control_U.x_C_des[i]=params.x_C_des[i];
    control_Obj.control_U.B_w_BWT[i]=0;
  }

  // Setting the quadrocopter state from ros topic as input
  for (int i=0;i<3;i++){
    control_Obj.control_U.State[i]=quadrotorState.x[i];
    control_Obj.control_U.State[i+3]=quadrotorState.a[i];
    control_Obj.control_U.State[i+6]=quadrotorState.dx[i];
    control_Obj.control_U.State[i+9]=quadrotorState.da[i];
  }
  
  // Setting constants
  control_Obj.control_U.m=params.mass;
  control_Obj.control_U.g=params.gravity;
  
  // Step the model for base rate
  control_Obj.step();

  // Get model outputs here
  output.tau_x=control_Obj.control_Y.tau_x;
  output.tau_y=control_Obj.control_Y.tau_y;
  output.tau_z=control_Obj.control_Y.tau_z;
  output.thrust=control_Obj.control_Y.thrust;
  output.F_des[0]=control_Obj.control_Y.F_des[0];
  output.F_des[1]=control_Obj.control_Y.F_des[1];
  output.F_des[2]=control_Obj.control_Y.F_des[2];

 
  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

void stateUpdate(const copter::State::ConstPtr& msg){
  quadrotorState.x[0]=msg->X.linear.x;
  quadrotorState.x[1]=msg->X.linear.y;
  quadrotorState.x[2]=msg->X.linear.z;

  quadrotorState.a[0]=msg->X.angular.x;
  quadrotorState.a[1]=msg->X.angular.y;
  quadrotorState.a[2]=msg->X.angular.z;
  
  quadrotorState.dx[0]=msg->dX.linear.x;
  quadrotorState.dx[1]=msg->dX.linear.y;
  quadrotorState.dx[2]=msg->dX.linear.z;

  quadrotorState.da[0]=msg->dX.angular.x;
  quadrotorState.da[1]=msg->dX.angular.y;
  quadrotorState.da[2]=msg->dX.angular.z;
}

void positionUpdate(const copter::Position::ConstPtr& msg){
  //Position
  d_pos[0]=msg->pos.x;
  d_pos[1]=msg->pos.y;
  d_pos[2]=msg->pos.z;
  //Velocity
  d_vel[0]=msg->vel.x;
  d_vel[1]=msg->vel.y;
  d_vel[2]=msg->vel.z;
  //Acceleration
  d_acc[0]=msg->acc.x;
  d_acc[1]=msg->acc.y;
  d_acc[2]=msg->acc.z;
}

int main(int argc,char **argv){

  ros::init(argc, argv, "control");
  ros::NodeHandle n;
  ros::Publisher ft_publisher=n.advertise<copter::Force_Torque>("/Force_Torque",1);
  ros::Subscriber state_subscriber = n.subscribe<copter::State>("/State",1,stateUpdate);
  ros::Subscriber position_subscriber=n.subscribe<copter::Position>("/Position",1,positionUpdate); 
  
  copter::Force_Torque ft_msg;
  
  // Initialize controller	
  control_Obj.initialize();

  // update params
  n.getParam("mass", params.mass);
  n.getParam("gravity", params.gravity);
  n.getParam("x_C_des", params.x_C_des);

  ros::Rate rate(200);

  while(ros::ok()){
    // update inputs
    ros::spinOnce();
    
    // update params
    // n.getParam("mass", params.mass);
    // n.getParam("gravity", params.gravity);
    // n.getParam("x_C_des", params.x_C_des);

    // perform one integration step
   rt_OneStep(d_pos,d_vel,d_acc);
    
    // publish calculated desired Force and Torque values
    ft_msg.tau_x=output.tau_x;
    ft_msg.tau_y=output.tau_y;
    ft_msg.tau_z=output.tau_z;
    ft_msg.thrust=output.thrust;
    ft_msg.F_des.x=output.F_des[0];
    ft_msg.F_des.y=output.F_des[1];
    ft_msg.F_des.z=output.F_des[2];

    ft_publisher.publish(ft_msg);
    rate.sleep();
  }
}
