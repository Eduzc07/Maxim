#include "ros/ros.h"
#include "copter/Force_Torque.h"
#include "copter/State.h"

#include <stddef.h>
#include <stdio.h>
#include "model.h"
#include "rtwtypes.h"
#include <time.h>

static modelModelClass rtObj;
static double state[12];

typedef struct {
  real_T tau_x;
  real_T tau_y;
  real_T tau_z;
  real_T thrust;
} Input;

typedef struct { 
  double mass;                    // mass of the quadrotor
  double gravity;                 // acceleration due to gravity
  double Jxx;                     // inertia of the quadrotor x-axis 
  double Jyy;                     // inertia of the quadrotor y-axis
  double Jzz;                     // inertia of the quadrotor z-axis
  std::vector<double> state_init; // desired quadrotor x-axis in global frame 
} Params;

static Input input;
static Params params;

void rt_OneStep(void);
void rt_OneStep(void){
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here
  rtObj.rtU.g=params.gravity;
  rtObj.rtU.m=params.mass;   
  rtObj.rtU.Jxx=params.Jxx;
  rtObj.rtU.Jyy=params.Jyy;
  rtObj.rtU.Jzz=params.Jzz;
  rtObj.rtU.tau_x=input.tau_x;
  rtObj.rtU.tau_y=input.tau_y;
  rtObj.rtU.tau_z=input.tau_z;
  rtObj.rtU.thrust=input.thrust;

  // Step the model for base rate
  rtObj.step();

  // Get model outputs here
  for (int i=0;i<12;i++){
    state[i]=rtObj.rtY.state[i];
  }
  // printf("State: %4.2f , %4.2f, %4.2f\n",state[0],state[1],state[2]);

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}
void inputUpdate(const copter::Force_Torque::ConstPtr& msg){
  input.tau_x=msg->tau_x;
  input.tau_y=msg->tau_y;
  input.tau_z=msg->tau_z;
  input.thrust=msg->thrust;
}

int main(int argc,char **argv){

  ros::init(argc, argv, "model");
  ros::NodeHandle n;
  
  ros::Publisher state_publisher=n.advertise<copter::State>("/State",1);
  ros::Subscriber force_torque_subscriber=n.subscribe<copter::Force_Torque>("/Force_Torque",1,inputUpdate);

  copter::State state_msg;
  
  // Initialize model	
  rtObj.initialize();

  // update params
  n.getParam("mass", params.mass);
  n.getParam("gravity", params.gravity);

  n.getParam("Jxx", params.Jxx);
  n.getParam("Jyy", params.Jyy);
  n.getParam("Jzz", params.Jzz);
  n.getParam("state_init", params.state_init);
  
  ros::Rate rate(200);
	

  while(ros::ok()){
    // update inputs
    ros::spinOnce();

    // perform one integration step
    rt_OneStep();
    
    // publish new state via msg
    state_msg.X.linear.x=state[0];
    state_msg.X.linear.y=state[1];
    state_msg.X.linear.z=state[2];
    
    state_msg.X.angular.x=state[3];
    state_msg.X.angular.y=state[4];
    state_msg.X.angular.z=state[5];
   
    state_msg.dX.linear.x=state[6];
    state_msg.dX.linear.y=state[7];
    state_msg.dX.linear.z=state[8];
    
    state_msg.dX.angular.x=state[9];
    state_msg.dX.angular.y=state[10];
    state_msg.dX.angular.z=state[11];
    
    state_publisher.publish(state_msg);
    rate.sleep();
  }
}
