#include <ros/ros.h>
#include "copter/State.h"
//#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <geometry_msgs/Point.h>
//#include <tf/transform_datatypes.h>

typedef struct{
  double x[3];   // cartesian position
  double a[3];   // angular positio in euler coordinates (zyx) (phi, theta, psi)
  double dx[3];  // cartesian velocity
  double da[3];  // angular velocity in euler coordinates (zyx) (phi, theta, psi)
} State;

static State quadrotorState;            // Quadrotor state vector being updated from /State topic


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

int main( int argc, char** argv )
{
  ros::init(argc, argv, "camera_stabilization");
  ros::NodeHandle n;
  ros::Rate r(50);
  ros::Publisher servo_pub = n.advertise<geometry_msgs::Point>("Servo_ZYX_Euler_Angles", 1);
  ros::Subscriber state_subscriber = n.subscribe<copter::State>("/State",1,stateUpdate);
  geometry_msgs::Point zyx_euler_msg;

  tf::Matrix3x3 copter_R_0; 
  tf::Matrix3x3 camera_R_copter;
  tf::Matrix3x3 camera_R_0;

  double camera_yaw_copter, camera_pitch_copter, camera_roll_copter; 
  n.getParam("camera_yaw_copter",camera_yaw_copter);
  n.getParam("camera_pitch_copter",camera_pitch_copter);
  n.getParam("camera_roll_copter",camera_roll_copter);
  
  camera_R_copter.setEulerZYX(camera_yaw_copter, camera_pitch_copter, camera_roll_copter);
  
  double camera_yaw_0, camera_pitch_0, camera_roll_0;

  while (ros::ok())
  {
    ros::spinOnce();

    copter_R_0.setEulerZYX(quadrotorState.a[2], quadrotorState.a[1], quadrotorState.a[0]);
    camera_R_0=camera_R_copter*copter_R_0;
    camera_R_0.getEulerZYX(camera_yaw_0, camera_pitch_0, camera_roll_0);
   
    zyx_euler_msg.x=camera_roll_0;
    zyx_euler_msg.y=camera_pitch_0;
    zyx_euler_msg.z=camera_yaw_0; 
    servo_pub.publish(zyx_euler_msg);

    r.sleep();
  }
}
