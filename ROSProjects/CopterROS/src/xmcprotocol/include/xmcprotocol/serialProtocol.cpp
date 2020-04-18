#include "serialProtocol.h"

/*
 * Set ROS log level to DEBUG
 */
void setROSloglevel2Debug() {
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
        ros::console::notifyLoggerLevelsChanged();
    }
}
 /*
  * Type casting
 */
// Convert int8_t array[4] to float32
float intToFloat(int8_t* ints, int s) {
    int8_t buffer[4] = {0};
    buffer[0] = ints[s];
    buffer[1] = ints[s+1];
    buffer[2] = ints[s+2];
    buffer[3] = ints[s+3];
    float ret = *(float*)&buffer;
    return ret;
}
// Convert uint8_t array[4] to float32
float uintToFloat(uint8_t* uint, int s) {
    uint8_t buffer[4] = {0};
    buffer[0] = uint[s];
    buffer[1] = uint[s+1];
    buffer[2] = uint[s+2];
    buffer[3] = uint[s+3];
    float ret = *(float*)&buffer;
    return ret;
}
// Join two int8_t's to int16_t
int16_t int8216(uint8_t h, uint8_t l) {
    return (int16_t)((h<<8)+l);
}
// Join two uint8_t's to uint16_t
uint16_t uint8216(uint8_t h, uint8_t l) {
    return (uint16_t)((h<<8)+l);
}
// Split uint16_t into two uint8_t's
void uint1628(uint16_t ui, uint8_t* buf, uint8_t s) {
    buf[s] = (uint8_t)(ui>>8);
    buf[s+1] = (uint8_t)(ui&0xFF);
}

/*
 * Serial connection
 */
// Init odroid gpio serial with 8N1 at 115200 Baud
int initSerial(void) {
    // Init serial
    int uart0_filestream = -1;
    // ODROID
    uart0_filestream = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
    // RPi 3
    //uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        ROS_ERROR("UART open() failed");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B921600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = 0; //IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    
    return uart0_filestream;
}


/*
 * State Machine
 */
