// ROS includes
#include "ros/ros.h"
#include "serialProtocol.h"
#include "xmcprotocol/IMUstates.h"
#include "xmcprotocol/IMUstatesRaw.h"

void xmcIMUstates_cb (const xmcprotocol::IMUstates::ConstPtr& imu_msg) {
    ROS_INFO("IMU States");
    ROS_INFO("yaw: %f -- pitch: %f -- roll: %f\n", imu_msg->yaw, imu_msg->pitch, imu_msg->roll);
}

void xmcIMUraw_cb (const xmcprotocol::IMUstatesRaw::ConstPtr& imuraw_msg) {
    ROS_INFO("IMU States raw");
    ROS_INFO("acc -- x: %f, y: %f, z: %f",imuraw_msg->accx,imuraw_msg->accy,imuraw_msg->accz);
    ROS_INFO("gyr -- x: %f, y: %f, z: %f",imuraw_msg->gyrx,imuraw_msg->gyry,imuraw_msg->gyrz);
    ROS_INFO("mag -- x: %f, y: %f, z: %f\n",imuraw_msg->magx,imuraw_msg->magy,imuraw_msg->magz);
}

int main(int argc, char **argv)
{
    // Init ROS
    ros::init(argc, argv, "exampleList");
    ros::NodeHandle nh;
    ros::Subscriber imu_sub = nh.subscribe("xmcIMUstates", 1, xmcIMUstates_cb);
    ros::Subscriber imuraw_sub = nh.subscribe("xmcIMUraw", 1, xmcIMUraw_cb);
    
    ros::spin();
    return 0;
}
