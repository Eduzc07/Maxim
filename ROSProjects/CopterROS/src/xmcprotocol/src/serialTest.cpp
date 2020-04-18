#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <bitset>
#include <iostream>
#include <sstream>

// ROS includes
#include "ros/ros.h"
#include "ros/console.h"

template<typename T,int N>
void printBitrepresentation(T (&uintArray)[N]) {
    std::string outputBytes = "";
    std::string outputUints = "";
    for(int i=0;i<N;i++) {
        std::bitset<8> bitrep(uintArray[i]);
        outputBytes += bitrep.to_string() + " ";
	std::stringstream ss;
	ss << (unsigned int)uintArray[i];
        outputUints += ss.str() + ", ";
    }
    ROS_DEBUG("Message as bytes: %s",outputBytes.c_str());
    ROS_DEBUG("Message as uints: %s\n",outputUints.c_str());
}

int main(int argc, char **argv)
{
    // Init serial
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        ROS_ERROR("UART open() failed");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = 0; //IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    // Bytes senden
    
    uint8_t BUF_TX[3];
    BUF_TX[0] = 3;
    BUF_TX[1] = 127;
    BUF_TX[2] = 10;
    
    // Init ROS
    ros::init(argc, argv, "serialTest");

    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
        ros::console::notifyLoggerLevelsChanged();
    }
    
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);

    while (ros::ok())
    {
	// Send message via UART
	
        // Read serial
        if (uart0_filestream != -1)	{
            int out = write(uart0_filestream, &BUF_TX[0], 3); //
            if (out < 0) {
                ROS_ERROR("UART TX error");
            } else {
                ROS_DEBUG("TX %i Bytes sent: %i,%i,%i", out, BUF_TX[0], BUF_TX[1], BUF_TX[2]);
            }
        } // if uart0
	
        // Receive message from UART
        if (uart0_filestream != -1) {
            uint8_t BUF_RX[3] = {0};
            int rx_length = read(uart0_filestream,BUF_RX,3);
            if (rx_length < 0) {
                ROS_ERROR("UART RX error");
            } else if (rx_length == 0) {
                ROS_ERROR("UART RX no data");
            } else {
                printBitrepresentation(BUF_RX);
            }
            ros::spinOnce();
            loop_rate.sleep();
        } //if uart0
    }

    close(uart0_filestream);
    return 0;
}
