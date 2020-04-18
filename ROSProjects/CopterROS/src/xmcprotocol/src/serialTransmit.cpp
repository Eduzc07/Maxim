// ROS includes
#include "ros/ros.h"
#include "xmcprotocol/serialProtocol.h"
#include "copter/Motor_Pwm.h"

// Motorcommands (MC) message
const int MC_PACKAGE_SIZE = 16;
uint8_t MC_TX[MC_PACKAGE_SIZE];
int uart0_filestream;

void xmcRCdata_cb (const copter::Motor_Pwm::ConstPtr& msg) {
    // Type casting float32 to uint16_t for transmission
    uint16_t pwm0 = (uint16_t)(msg->M1*65535);
    uint16_t pwm1 = (uint16_t)(msg->M2*65535);
    uint16_t pwm2 = (uint16_t)(msg->M3*65535);
    uint16_t pwm3 = (uint16_t)(msg->M4*65535);
    // Servos pwm:
    uint16_t pwm4 = (uint16_t)(msg->S1*65535);
    uint16_t pwm5 = (uint16_t)(msg->S2*65535);

    // "Splitting" bytes uint16_t to uint8_t for transmission II
    uint1628(pwm0, MC_TX, 2);
    uint1628(pwm1, MC_TX, 4);
    uint1628(pwm2, MC_TX, 6);
    uint1628(pwm3, MC_TX, 8);
    uint1628(pwm4, MC_TX, 10);
    uint1628(pwm5, MC_TX, 12);
    
    // Calculate checksum
    uint16_t sum = 0;
    for(int i=0;i<MC_PACKAGE_SIZE-2;i++) {
        sum += MC_TX[i];
    }
    uint1628(sum, MC_TX, MC_PACKAGE_SIZE-2);
    // Sent data on serial
    if (uart0_filestream != -1)	{
        int out = write(uart0_filestream, &MC_TX[0], MC_PACKAGE_SIZE); //
        if (out < 0) {
            ROS_ERROR("UART TX error");
        } else {
            // ROS_INFO("TX %i Bytes sent: %f, %f, %f, %f", out, msg->M1, msg->M2, msg->M3, msg->M4);
            
	    // ROS_INFO("Bytes: %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i", MC_TX[0], MC_TX[1], MC_TX[2], MC_TX[3], MC_TX[4], MC_TX[5], MC_TX[6], MC_TX[7], MC_TX[8], MC_TX[9], MC_TX[10], MC_TX[11], MC_TX[12], MC_TX[13], MC_TX[14], MC_TX[15]); 
	    // ROS_INFO("Checksum: %i", sum);
        }
    }
}

int main(int argc, char **argv)
{
    // Init serial
    uart0_filestream = initSerial();

    // Set ID for RC data
    MC_TX[0] = 85;
    MC_TX[1] = 143;
    
    // Init ROS
    ros::init(argc, argv, "serialTransmit");

    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("Motor_Pwm", 1, xmcRCdata_cb);

    ros::spin();
    return 0;
}
