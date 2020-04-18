#include "ros/ros.h"
#include "copter/Force_Torque.h"
#include "copter/Motor_Pwm.h"
#include "copter/Motor_Voltage.h"
#include "copter/Status.h"
#include "thrust2pwm/thrust2pwm.h"

typedef struct {
  double tau_x;    // desired torque x-axis
  double tau_y;    // desired torque y-axis
  double tau_z;    // desired torque z-axis
  double thrust;   // desired thrust
  double F_des[3]; // desired cartesian Force
} Force_Torque;

static Force_Torque ft;

static float motor_voltage;	// variable for motor voltage measured by xmc to calculate desired pwm
static float motor_enable;	// variable to deaktivate motor output

void force_torque_update(const copter::Force_Torque::ConstPtr& msg){
  ft.tau_x=msg->tau_x;
  ft.tau_y=msg->tau_y;
  ft.tau_z=msg->tau_z;
  ft.thrust=msg->thrust;
}

void voltage_update(const copter::Motor_Voltage::ConstPtr& msg){
  motor_voltage=msg->cell123;
}

//void status_update(const copter::Status::ConstPtr& msg){
//  motor_enable=msg->motor_enable;
//}

int main(int argc, char **argv){
  ros::init(argc, argv, "force_torque2pwm");
  ros::NodeHandle n;
  ros::Publisher pwm_publisher=n.advertise<copter::Motor_Pwm>("/Motor_Pwm",1);
  ros::Subscriber ft_subscriber=n.subscribe<copter::Force_Torque>("/Force_Torque", 1, force_torque_update);
  ros::Subscriber voltage_subscriber=n.subscribe<copter::Motor_Voltage>("/Motor_Voltage",1,voltage_update);
  // ros::Subscriber status_subscriber=n.subscribe<copter::Status>("/Status",1, status_update);
  double L=1.0f;
  double K_m=0.1f;
  n.getParam("l_arm", L);
  n.getParam("K_m", K_m);

  // ROS_INFO_STREAM("l_arm: " << L << " K_m: " <<K_m);
  copter::Motor_Pwm pwm_msg;
  ros::Rate rate(200);
  
  // initialize desired force torque vector and voltage
  ft.tau_x=0.0f;
  ft.tau_y=0.0f;
  ft.tau_z=0.0f;
  ft.thrust=0.0f;
  motor_voltage=12.5f;

  while(ros::ok()){

    ros::spinOnce();

    // calculate desired motor thrust for desired torques and thrust
    double F_M1=-1.0f/(2.0f*L)*ft.tau_y-1.0f/(4.0f*K_m)*ft.tau_z+1.0f/4.0f*ft.thrust;
    double F_M2=-1.0f/(2.0f*L)*ft.tau_x+1.0f/(4.0f*K_m)*ft.tau_z+1.0f/4.0f*ft.thrust;
    double F_M3=1.0f/(2.0f*L)*ft.tau_y-1.0f/(4.0f*K_m)*ft.tau_z+1.0f/4.0f*ft.thrust;
    double F_M4=1.0f/(2.0f*L)*ft.tau_x+1.0f/(4.0f*K_m)*ft.tau_z+1.0f/4.0f*ft.thrust;    
    
    // ROS_INFO_STREAM("F_M1: "<<F_M1<<" F_M2: "<<F_M2<<" F_M3: "<<F_M3<<" F_M4: "<<F_M4<<" motor voltage: "<< motor_voltage<<"V"<<" sum of thrust: "<<ft.thrust<<" Torques: x:"<<ft.tau_x<<" y:"<<ft.tau_y<<" z:"<<ft.tau_z);

    // calculate desired pwm value from motor thrust curve
    double pwm_M1=thrust2pwm(motor_voltage,F_M1);
    double pwm_M2=thrust2pwm(motor_voltage,F_M2);
    double pwm_M3=thrust2pwm(motor_voltage,F_M3);
    double pwm_M4=thrust2pwm(motor_voltage,F_M4);
    
    // ROS_INFO_STREAM("M1: "<<pwm_M1<<" M2: "<<pwm_M2<<" M3: "<<pwm_M3<< " M4: "<<pwm_M4);

    if(std::isnan(pwm_M1) or std::isinf(pwm_M1) or std::isnan(pwm_M2) or std::isinf(pwm_M2) or std::isnan(pwm_M3) or std::isinf(pwm_M3) or std::isnan(pwm_M4) or std::isinf(pwm_M4)){
      pwm_M1=0.0f;
      pwm_M2=0.0f;
      pwm_M3=0.0f;
      pwm_M4=0.0f;
    }
    
    if(pwm_M1<0.0f){ pwm_M1=0.0f;}
    if(pwm_M2<0.0f){ pwm_M2=0.0f;}
    if(pwm_M3<0.0f){ pwm_M3=0.0f;}
    if(pwm_M4<0.0f){ pwm_M4=0.0f;}
 
    if(pwm_M1>1.0f){ pwm_M1=1.0f;}
    if(pwm_M2>1.0f){ pwm_M2=1.0f;}
    if(pwm_M3>1.0f){ pwm_M3=1.0f;}
    if(pwm_M4>1.0f){ pwm_M4=1.0f;}
    
    
    n.getParam("motor_enable", motor_enable);
    if(motor_enable!=1){ motor_enable=0; }

    pwm_msg.M1=pwm_M1*motor_enable;
    pwm_msg.M2=pwm_M2*motor_enable;
    pwm_msg.M3=pwm_M3*motor_enable;
    pwm_msg.M4=pwm_M4*motor_enable;

    pwm_publisher.publish(pwm_msg);
    rate.sleep();
  }
}

