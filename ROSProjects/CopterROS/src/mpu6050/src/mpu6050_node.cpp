#include "MPU6050.h" 
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

int main(int argc,char **argv){
	ros::init(argc, argv, "mpu");
	ros::NodeHandle n;
	ros::Publisher mpu_pub=n.advertise<sensor_msgs::Imu>("Imu",1);

	float gyroQ[9];
	float x_acc,y_acc,z_acc;
	float x_gyro,y_gyro,z_gyro;
	
	MPU6050 mpu;
	mpu.calibrationAccel(600);
	mpu.calibrationGyro(1000,gyroQ);
	ros::Rate rate(200);
	sensor_msgs::Imu imu_msg;

	imu_msg.header.frame_id = "base_link";
	imu_msg.orientation.x=1;        
	imu_msg.orientation.w=0;        
        imu_msg.orientation_covariance[0]=0.02;
        imu_msg.orientation_covariance[4]=0.02;
        imu_msg.orientation_covariance[8]=0.02;
        //for (int i=0;i<9;i++){
	//	imu_msg.angular_velocity_covariance[i]=gyroQ[i];
	//}
        imu_msg.angular_velocity_covariance[0]=0.02;
        imu_msg.angular_velocity_covariance[4]=0.02;
        imu_msg.angular_velocity_covariance[8]=0.02;
        imu_msg.linear_acceleration_covariance[0]=0.02;
        imu_msg.linear_acceleration_covariance[4]=0.02;
        imu_msg.linear_acceleration_covariance[8]=0.02;

	if (mpu.getInitError()<0){
		ROS_ERROR_STREAM("mpu6050:InitError "<<mpu.getInitError());
		return -1;
	}
	while(ros::ok()){
		mpu.readAccel(x_acc, y_acc, z_acc);
		mpu.readGyro(x_gyro, y_gyro, z_gyro);
 	
		imu_msg.linear_acceleration.x=x_acc;
		imu_msg.linear_acceleration.y=y_acc;
		imu_msg.linear_acceleration.z=z_acc;

		imu_msg.angular_velocity.x=x_gyro;
		imu_msg.angular_velocity.y=y_gyro;
		imu_msg.angular_velocity.z=z_gyro;

		imu_msg.header.stamp = ros::Time::now();

		mpu_pub.publish(imu_msg);
		
		rate.sleep();
	}
}
