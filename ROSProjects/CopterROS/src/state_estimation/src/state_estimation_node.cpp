#include "ros/ros.h"
#include <ros/console.h>
#include "sensor_msgs/Imu.h"
#include "copter/State.h"
#include <tf/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <stddef.h>
#include <stdio.h>
#include "sda.h"
#include "rtwtypes.h"

typedef struct {
  double x[3];   // cartesian position
  double a[3];   // angular position in euler coordinates (zyx) (phi, theta, psi)
  double dx[3];  // cartesian velocity
  double da[3];  // angular velocity in euler coordinates (zyx) (phi, theta, psi)
} State;

typedef struct { 
  double acc[3];
  double gyro[3];
} Imu_data;

typedef struct { 
  double mass;                 // mass of the quadrotor
  double gravity;              // acceleration due to gravity
} Params;

static State quadrotorState;            // Quadrotor state vector being updated from /State topic
static sdaModelClass sda_Obj;           // Matlab Simulink Strap Down Algorithm implementation
static Imu_data imu_data;
static Params params;

void rt_OneStep(void);

void rt_OneStep(void){
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(sda_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here
  // Setting 3-dim vectors
  for (int i=0;i<3;i++){
    sda_Obj.sda_U.acc[i]=imu_data.acc[i];
    sda_Obj.sda_U.gyro[i]=imu_data.gyro[i];
  }
  
  // Setting constants
  sda_Obj.sda_U.m=params.mass;
  sda_Obj.sda_U.E_g[0]=0;
  sda_Obj.sda_U.E_g[1]=0;
  sda_Obj.sda_U.E_g[2]=-params.gravity;
  
  // Step the model for base rate
  sda_Obj.step();

  // Get model outputs here
  for (int i=0;i<3;i++){
    quadrotorState.x[i]=sda_Obj.sda_Y.p[i];
    quadrotorState.dx[i]=sda_Obj.sda_Y.v[i];
  }

  // Get orientation quaternion
  double q[4];
  q[0]=sda_Obj.sda_Y.E_q_B[0];
  q[1]=sda_Obj.sda_Y.E_q_B[1];
  q[2]=sda_Obj.sda_Y.E_q_B[2];
  q[3]=sda_Obj.sda_Y.E_q_B[3];
  
  // calculate zyx euler angles from quaternion
  tf::Quaternion quat = tf::Quaternion(q[0], q[1], q[2], q[3]);
  double roll, pitch, yaw;
  tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
  ROS_INFO_STREAM("w: "<<q[0]<<" x:"<<q[1]<<" y:"<<q[2]<<" z:"<<q[3]);
  ROS_INFO_STREAM("r: "<<roll<<" p:"<<pitch<<" y:"<<yaw);
  
  quadrotorState.a[0]=roll;
  quadrotorState.a[1]=pitch;
  quadrotorState.a[2]=yaw;

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

void stateUpdate(const sensor_msgs::Imu::ConstPtr& msg){
  imu_data.acc[0]=msg->linear_acceleration.x;
  imu_data.acc[1]=msg->linear_acceleration.y;
  imu_data.acc[2]=msg->linear_acceleration.z;
  
  imu_data.gyro[0]=msg->angular_velocity.x;
  imu_data.gyro[1]=msg->angular_velocity.y;
  imu_data.gyro[2]=msg->angular_velocity.z;
}

int main(int argc,char **argv){

  ros::init(argc, argv, "state_estimation");
  ros::NodeHandle n;
  ros::Subscriber imu_subscriber=n.subscribe<sensor_msgs::Imu>("/Imu",1,stateUpdate);
  ros::Publisher state_publisher = n.advertise<copter::State>("/State",1); 

  copter::State state_msg;

  // Initialize controller	
  sda_Obj.initialize();
  for (int i=0;i<3;i++){
    sda_Obj.sda_U.E_g[i]=0;
    sda_Obj.sda_U.ic_p[i]=0;
    sda_Obj.sda_U.ic_v[i]=0;
    sda_Obj.sda_U.ic_q[i]=0;
  }
  sda_Obj.sda_U.ic_q[3]=1;
  sda_Obj.sda_U.NReset=1;

  // update params
  n.getParam("mass", params.mass);
  n.getParam("gravity", params.gravity);

  ros::Rate rate(200);

  while(ros::ok()){
    // update inputs
    ros::spinOnce();
    
    // perform one integration step
    rt_OneStep();
    
    // publish estimated state vector
    state_msg.X.linear.x=quadrotorState.x[0];
    state_msg.X.linear.y=quadrotorState.x[1];
    state_msg.X.linear.z=quadrotorState.x[2];
    
    state_msg.X.angular.x=quadrotorState.a[0];
    state_msg.X.angular.y=quadrotorState.a[1];
    state_msg.X.angular.z=quadrotorState.a[2]; 

    state_msg.dX.linear.x=quadrotorState.dx[0];
    state_msg.dX.linear.y=quadrotorState.dx[1];
    state_msg.dX.linear.z=quadrotorState.dx[2];

    state_msg.dX.angular.x=imu_data.gyro[0];
    state_msg.dX.angular.y=imu_data.gyro[1];
    state_msg.dX.angular.z=imu_data.gyro[2];

    state_publisher.publish(state_msg);
    rate.sleep();
  }
}
