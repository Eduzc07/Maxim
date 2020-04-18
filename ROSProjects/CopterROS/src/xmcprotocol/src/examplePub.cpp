// ROS includes
#include "ros/ros.h"
#include "xmcprotocol/serialProtocol.h"
#include "xmcprotocol/Motorcommand.h"

int main(int argc, char **argv)
{
    // Init ROS
    ros::init(argc, argv, "examplePub");
    ros::NodeHandle nh;
    ros::Publisher mc_pub = nh.advertise<xmcprotocol::Motorcommand>("xmcMotorcommands", 1);
    ros::Rate loop_rate(200);
    
    int i = 0;
    int dir = 1;
    
    // activate ESCs
    float pwms[4] = {0,0,0,0};
    xmcprotocol::Motorcommand mc_msg;
    
    while (ros::ok())
    {
        if (dir) {
            for (int j=0; j<4; j++) {
                pwms[j] = i*3;
            }
            i++;
            if (i>=33) {
                dir=0;
            }
        }else {
            for (int j=0; j<4; j++) {
                pwms[j] = i*3;
            }
            i--;
            if (i<=3) {
                dir=1;
            }
        }
        
        mc_msg.pwm0 = (uint16_t)(pwms[0]/100*65536);
        mc_msg.pwm1 = (uint16_t)(pwms[1]/100*65536);
        mc_msg.pwm2 = (uint16_t)(pwms[2]/100*65536);
        mc_msg.pwm3 = (uint16_t)(pwms[3]/100*65536);
        mc_pub.publish(mc_msg);
        ROS_INFO("%i %i %i %i", mc_msg.pwm0, mc_msg.pwm1, mc_msg.pwm2, mc_msg.pwm3);
        
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
