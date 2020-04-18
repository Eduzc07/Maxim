#include "ros/ros.h"
#include <ros/console.h>
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/MagneticField.h"
#include "copter/State.h"
#include "copter/Force_Torque.h"
#include "sensor_msgs/Range.h"
#include <tf/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "quadrotor_EKF.h"
#include "rtwtypes.h"

typedef struct {
  double x[3];   // cartesian position
  double a[3];   // angular position in euler coordinates (zyx) (phi, theta, psi)
  double dx[3];  // cartesian velocity
  double da[3];  // angular velocity in euler coordinates (zyx) (phi, theta, psi)
  double ddx[3]; // cartesian acceleration
  double bias_acc[3]; 	// accelerometer bias
  double bias_gyro[3];	// gyro bias
} State;

typedef struct { 
  double acc[3];
  double gyro[3];
} Imu_data;

typedef struct {
  double mag[3];
} Mag_data;

typedef struct {
  double data[3];
} Position;

typedef struct { 
  double mass;                 // mass of the quadrotor
  double gravity;              // acceleration due to gravity
  double Jxx;		       // inertia x axis
  double Jyy;		       // inertia y axis
  double Jzz;		       // inertia z axis
  std::vector<double> var_pos;		// position variance
  std::vector<double> var_acc;		// accelerometer variance
  std::vector<double> var_gyro;		// gyro variance
  std::vector<double> var_mag;		// magnetometer variance
  std::vector<double> process_var_pos;  // process variance position
  std::vector<double> process_var_ang;  // process variance angular
  std::vector<double> process_var_vel;  // process variance velocity
  std::vector<double> process_var_ang_vel;  // process variance angular velocity
  std::vector<double> process_var_acc;      // process variance accerleration
  std::vector<double> process_var_bias_acc; // process variance Imu acc bias
  std::vector<double> process_var_bias_gyro;// process variance Imu gyro bias

} Params;

typedef struct {
  double tau_x;
  double tau_y;
  double tau_z;
  double thrust;
} Input;

static State quadrotorState;            // Quadrotor state vector being updated from /State topic
static quadrotor_EKFModelClass ekf_Obj; // Matlab Simulink extended Kalman filter implementation for quadrotor
static Imu_data imu_data;
static Mag_data mag_data;
static Position position;
static Params params;
static Input input;

void rt_OneStep(void);

void rt_OneStep(void){
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(ekf_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // z_k=[x y z acc_x acc_y acc_z gyro_x gyro_y gyro_z]
  
  for (int i=0;i<3;i++){
    ekf_Obj.quadrotor_EKF_U.z_k[i]=position.data[i];  	// position measurement vision system
    ekf_Obj.quadrotor_EKF_U.z_k[i+3]=imu_data.acc[i];   // acceleration from IMU
    ekf_Obj.quadrotor_EKF_U.z_k[i+6]=imu_data.gyro[i];  // angular velocity from IMU
    ekf_Obj.quadrotor_EKF_U.z_k[i+9]=mag_data.mag[i];   // magnetic field measured from mag sensor
  }

  ekf_Obj.quadrotor_EKF_U.u_k[0]=input.tau_x;
  ekf_Obj.quadrotor_EKF_U.u_k[1]=input.tau_y;
  ekf_Obj.quadrotor_EKF_U.u_k[2]=input.tau_z;
  ekf_Obj.quadrotor_EKF_U.u_k[3]=input.thrust;

  // Step the model for base rate
  ekf_Obj.step();

  // Get model outputs here
  for (int i=0;i<3;i++){
    quadrotorState.x[i]=ekf_Obj.quadrotor_EKF_Y.x[i];
    quadrotorState.a[i]=ekf_Obj.quadrotor_EKF_Y.a[i];
    quadrotorState.dx[i]=ekf_Obj.quadrotor_EKF_Y.dx[i];
    quadrotorState.da[i]=ekf_Obj.quadrotor_EKF_Y.da[i];
    quadrotorState.ddx[i]=ekf_Obj.quadrotor_EKF_Y.ddx[i];
    quadrotorState.bias_acc[i]=ekf_Obj.quadrotor_EKF_Y.bias_acc[i];
    quadrotorState.bias_gyro[i]=ekf_Obj.quadrotor_EKF_Y.bias_gyro[i];
  }

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

void inputUpdate(const copter::Force_Torque::ConstPtr& msg){
  input.tau_x=msg->tau_x;
  input.tau_y=msg->tau_y;
  input.tau_z=msg->tau_z;
  input.thrust=msg->thrust;
}

void magUpdate(const sensor_msgs::MagneticField::ConstPtr& msg){
  mag_data.mag[0]=msg->magnetic_field.x;
  mag_data.mag[1]=msg->magnetic_field.y;
  mag_data.mag[2]=msg->magnetic_field.z;
}

void teraUpdate(const sensor_msgs::Range::ConstPtr& msg){
  position.data[2]=msg->range;
}

void poseUpdate(const geometry_msgs::Twist::ConstPtr& msg){
  position.data[0]=msg->linear.x;
  position.data[1]=msg->linear.y;
  position.data[2]=msg->linear.z;
  
  // TODO add yaw update
}

int main(int argc,char **argv){

  ros::init(argc, argv, "state_estimation");
  ros::NodeHandle n;
  ros::Subscriber imu_subscriber=n.subscribe<sensor_msgs::Imu>("/Imu",1,stateUpdate);
  ros::Publisher state_publisher = n.advertise<copter::State>("/State",1); 
  ros::Subscriber f_t_subscriber=n.subscribe<copter::Force_Torque>("/Force_Torque",1,inputUpdate);
  ros::Subscriber mag_subscriber=n.subscribe<sensor_msgs::MagneticField>("/Magnetic_Field",1,magUpdate);
  ros::Subscriber tera_subscriber=n.subscribe<sensor_msgs::Range>("terarangerone",1,teraUpdate);
  ros::Subscriber vision_subscriber=n.subscribe<geometry_msgs::Twist>("Pose_Vision",1,poseUpdate); 
  copter::State state_msg;

  // initialize extended Kalman filter model
  ekf_Obj.initialize();
  
  // update params
  n.getParam("mass", params.mass);
  n.getParam("gravity", params.gravity);
  n.getParam("Jxx", params.Jxx);
  n.getParam("Jyy", params.Jyy);
  n.getParam("Jzz", params.Jzz);
  n.getParam("var_pos", params.var_pos);
  n.getParam("var_acc", params.var_acc);
  n.getParam("var_gyro", params.var_gyro);
  n.getParam("var_mag", params.var_mag);
  n.getParam("process_var_pos",params.process_var_pos);
  n.getParam("process_var_ang",params.process_var_ang);
  n.getParam("process_var_vel",params.process_var_vel);
  n.getParam("process_var_ang_vel",params.process_var_ang_vel);
  n.getParam("process_var_acc",params.process_var_acc);
  n.getParam("process_var_bias_acc",params.process_var_bias_acc);
  n.getParam("process_var_bias_gyro",params.process_var_bias_gyro);

  // initialize input
  input.tau_x=0;
  input.tau_y=0;
  input.tau_z=0;
  input.thrust=params.mass*params.gravity;
  
  // initialize position estimate
  position.data[0]=0;
  position.data[1]=0;
  position.data[2]=0;

  // set parameters
  ekf_Obj.quadrotor_EKF_U.params[0]=params.gravity;
  ekf_Obj.quadrotor_EKF_U.params[1]=params.mass;
  ekf_Obj.quadrotor_EKF_U.params[2]=params.Jxx;
  ekf_Obj.quadrotor_EKF_U.params[3]=params.Jyy;
  ekf_Obj.quadrotor_EKF_U.params[4]=params.Jzz;
  
  // initialize matrices 12x12
  double diag_R_k[12];
  for (int i=0; i<3; i++){
    diag_R_k[i]=params.var_pos[i];
    diag_R_k[i+3]=params.var_acc[i];
    diag_R_k[i+6]=params.var_gyro[i];
    diag_R_k[i+9]=params.var_mag[i];
  } 
  for (int i=0;i<12;i++){
    for (int j=0;j<12;j++){
      if(i==j){
        ekf_Obj.quadrotor_EKF_U.R_k[i*12+j]=diag_R_k[i];
      } else{
        ekf_Obj.quadrotor_EKF_U.R_k[i*12+j]=0;
      }
    }
  }
  
  // initialize matrices 21x21
  double diag_Q_k[21];
  for (int i=0; i<3; i++){
    diag_Q_k[i]=params.process_var_pos[i];
    diag_Q_k[i+3]=params.process_var_ang[i];
    diag_Q_k[i+6]=params.process_var_vel[i];
    diag_Q_k[i+9]=params.process_var_ang_vel[i];
    diag_Q_k[i+12]=params.process_var_acc[i];
    diag_Q_k[i+15]=params.process_var_bias_acc[i];
    diag_Q_k[i+18]=params.process_var_bias_gyro[i];
  }
  for (int i=0;i<21;i++){
    for (int j=0;j<21;j++){
      if(i==j){
        ekf_Obj.quadrotor_EKF_U.P_0[i*21+j]=1;
        ekf_Obj.quadrotor_EKF_U.Q_k[i*21+j]=diag_Q_k[i];
      } else{
        ekf_Obj.quadrotor_EKF_U.P_0[i*21+j]=0;
        ekf_Obj.quadrotor_EKF_U.Q_k[i*21+j]=0;
      }
    }
  }
  // initialize vectors of dimension 21
  for (int i=0;i<21;i++){
    ekf_Obj.quadrotor_EKF_U.x_0[i]=0;
  }
  
  // calculate mean of reference magnetic field
  double B_earth_ref[3]={0,0,0};
  double nr_meas=600;
  for (int i=0; i<nr_meas; i++){
    ros::spinOnce();
    for(int j=0; j<3; j++){
      B_earth_ref[j]+=mag_data.mag[j];
    }
    usleep(5000);
  }
  for (int i=0; i<3;i++){
    ekf_Obj.quadrotor_EKF_U.B_earth[i]=B_earth_ref[i]/nr_meas;
  }
  ROS_INFO_STREAM("Magnetic field reference: x:"<<B_earth_ref[0]/nr_meas<<" y:"<<B_earth_ref[1]/nr_meas<<" z:"<<B_earth_ref[2]/nr_meas);
  ekf_Obj.quadrotor_EKF_U.NReset=1;

  ros::Rate rate(100);

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

    state_msg.dX.angular.x=quadrotorState.da[0];
    state_msg.dX.angular.y=quadrotorState.da[1];
    state_msg.dX.angular.z=quadrotorState.da[2];

    state_publisher.publish(state_msg);
    rate.sleep();
  }
}
