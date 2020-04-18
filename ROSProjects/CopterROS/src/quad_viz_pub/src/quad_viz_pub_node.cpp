#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "copter/State.h"
#include "copter/Position.h"
#include <tf/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>

typedef struct{
  double x[3];   // cartesian position
  double a[3];   // angular positio in euler coordinates (zyx) (phi, theta, psi)
  double dx[3];  // cartesian velocity
  double da[3];  // angular velocity in euler coordinates (zyx) (phi, theta, psi)
} State;

static State quadrotorState;            // Quadrotor state vector being updated from /State topic
static double d_pos[3];     //Desired position

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
  d_pos[0]=msg->pos.x;
  d_pos[1]=msg->pos.y;
  d_pos[2]=msg->pos.z;
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "quad_viz_pub");
  ros::NodeHandle n;
  ros::Rate r(25);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber state_subscriber = n.subscribe<copter::State>("/State",1,stateUpdate);
  ros::Subscriber position_subscriber=n.subscribe<copter::Position>("/Position",1,positionUpdate);

  geometry_msgs::Point p;

    //-----------------------------------------------------------------
    visualization_msgs::Marker line_strip;
    line_strip.header.frame_id= "/world_frame";
    line_strip.header.stamp = ros::Time::now();
    
    line_strip.action = visualization_msgs::Marker::ADD;
    line_strip.pose.orientation.w = 1.0;

    line_strip.ns = "Lines";
    line_strip.id = 1;

    line_strip.type = visualization_msgs::Marker::LINE_STRIP;

 // POINTS markers use x and y scale for width/height respectively
    line_strip.scale.x = 0.03;


    // Points are red
    line_strip.color.r = 0.8f;
    line_strip.color.a = 0.6;

    line_strip.lifetime = ros::Duration();

    //-------------------------------------------------------------------
    //Desirable
    //-----------------------------------------------------------------
    visualization_msgs::Marker line_des;
    line_des.header.frame_id= "/world_frame";
    line_des.header.stamp = ros::Time::now();
    
    line_des.action = visualization_msgs::Marker::ADD;
    line_des.pose.orientation.w = 1.0;

    line_des.ns = "Trajectory";
    line_des.id = 2;

    line_des.type = visualization_msgs::Marker::LINE_STRIP;
    //line_des.type = visualization_msgs::Marker::SPHERE;


 // POINTS markers use x and y scale for width/height respectively
     line_des.scale.x = 0.02;
   
       // Set the scale of the marker -- 1x1x1 here means 1m on a side
       //line_des.scale.x = 0.2;
       //line_des.scale.y = 0.2;
       //line_des.scale.z = 0.2;

    // Points are blue
    line_des.color.b = 0.8f;
    line_des.color.a = 0.6;

    line_des.lifetime = ros::Duration();


  while (ros::ok())
  {
    ros::spinOnce();
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/world_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "quad_ns";
    marker.id = 0;

    // Set the marker type.  Here mesh resource for an stl-File
    marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    marker.mesh_resource = "package://quad_viz_pub/meshes/quadrotor.stl";

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = quadrotorState.x[0]-0.04;
    marker.pose.position.y = quadrotorState.x[1]+0.06;
    marker.pose.position.z = quadrotorState.x[2]+0.09;

    //Trayectory
    p.x = d_pos[0];
    p.y = d_pos[1];
    p.z = d_pos[2];
    line_des.points.push_back(p);

    p.x = quadrotorState.x[0];
    p.y = quadrotorState.x[1];
    p.z = quadrotorState.x[2];
    line_strip.points.push_back(p);

    tf::Quaternion q;
    q=tf::createQuaternionFromRPY(quadrotorState.a[0],quadrotorState.a[1],quadrotorState.a[2]);
    marker.pose.orientation.x = q.getX();
    marker.pose.orientation.y = q.getY();
    marker.pose.orientation.z = q.getZ();
    marker.pose.orientation.w = q.getW();

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0e-2;
    marker.scale.y = 1.0e-2;
    marker.scale.z = 1.0e-2;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.8f;
    marker.color.b = 0.0f;
    marker.color.a = 0.6;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
    marker_pub.publish(line_strip);
    marker_pub.publish(line_des);

    r.sleep();
  }
}
