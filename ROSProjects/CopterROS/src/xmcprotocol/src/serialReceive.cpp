// ROS includes
#include "ros/ros.h"
#include "serialProtocol.h"
#include "xmcprotocol/IMUstates.h"
#include "xmcprotocol/IMUstatesRaw.h"
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Vector3.h>
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/MagneticField.h"
#include "copter/Motor_Voltage.h"

int uart0_filestream;

/*
 *  State Machine to parse Messages from XMC
 */
// States
#define WAIT_FOR_ID  1
#define READ_MSG     2
#define EVAL_MSG     0

#define DEG_TO_RAD   0.017453292519943295769236907684884f

struct StateMachine {
    int id1;		// TODO : int  => uint8_t
    int id2;
    int length;
    int state;
    int success;
    int calibrated;
    
    uint8_t CON_ID[2];
    uint8_t BUF_RX[32];
    uint16_t csum;
    uint16_t sum;
};
typedef struct StateMachine StateMachine;

uint8_t readByte(uint8_t &success){
    if (uart0_filestream != -1) {
        uint8_t newByte=0;
        int rx_length = read(uart0_filestream,&newByte,1);
	success=1;
        if (rx_length < 0) {
	   success=0;
           // ROS_ERROR("UART RX no data or other error");
        } else if (rx_length == 0) {
	   success=0;
           ROS_ERROR("UART RX end of file error");
        }
        return newByte;
    }
}
bool checkID(StateMachine *sm, uint8_t *id, int start=0) {
    if(id[start]==sm->id1 && id[start+1]==sm->id2) {
        return true;
    }else {
        return false;
    }
}
// Print received Data as Byte and int representation
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
    //ROS_INFO("Message as bytes: %s",outputBytes.c_str());
    ROS_INFO("Message as uints: %s\n",outputUints.c_str());
}
void stateMachine(StateMachine *sm, uint8_t newByte)
{
    switch( sm->state ) {
        case 1:
            //ROS_INFO("State 1");
            // Shift and read new byte
            sm->CON_ID[0] = sm->CON_ID[1];
            sm->CON_ID[1] = newByte;
            // Check Bytes for ID
            if (checkID(sm, sm->CON_ID)) {
                sm->state=READ_MSG;
                return;
            }
            // else sm->state=WAIT_FOR_ID;
            break;
            
        case 2:
            //ROS_INFO("State 2");
            sm->BUF_RX[0] = sm->CON_ID[0];
            sm->BUF_RX[1] = sm->CON_ID[1];
            sm->BUF_RX[2] = newByte;
            if (checkID(sm, sm->BUF_RX,sm->state-1)) {
                ROS_INFO("ID FOUND: %i",sm->state);
		sm->state=READ_MSG;
                return;
            }
            sm->state++;
            break;
            
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
            //ROS_INFO("State %i", sm->state);
	    //ROS_INFO("NEWBYTE: %i",newByte);
            sm->BUF_RX[sm->state] = newByte;;
            // check for ID
            if (checkID(sm, sm->BUF_RX,sm->state-1)) {
                sm->state=READ_MSG;
                return;
            }
            sm->state++;
            if(sm->state>=sm->length){    // length: 2xID + data length + 2 checksum
                // finished reading bytes => check checksum
                sm->csum = uint8216(sm->BUF_RX[sm->length-2],sm->BUF_RX[sm->length-1]);
                sm->sum = 0;
                for(int i=0;i<sm->length-2;i++) {
                   sm->sum += (uint16_t)sm->BUF_RX[i];
                }
		//printBitrepresentation(sm->BUF_RX);
                // If checksum correct
                if(sm->sum==sm->csum) {
                    // Return Msg
                    sm->success = 1;
		    //ROS_INFO("CHECKSUM: id=%i, cs_serial=% i, cs_calced=%i", sm->id2, (int)sm->csum, (int)sm->sum);
		    //printBitrepresentation(sm->BUF_RX);
                } else {
                    // ROS_ERROR("CHECKSUM ERROR: id=%i, cs_serial=% i, cs_calced=%i", sm->id2, (int)sm->csum, (int)sm->sum);
                    // printBitrepresentation(sm->BUF_RX);
                }
                sm->state = WAIT_FOR_ID;
	    }
            break;
            
        default:
            //ROS_INFO("State DEFAULT");
            sm->state = WAIT_FOR_ID;
    }
}

int main(int argc, char **argv)
{
    // Init serial
    uart0_filestream = initSerial();
    uint8_t success=0;

    // Init ROS
    ros::init(argc, argv, "serialReceive");
    ros::NodeHandle nh;
    ros::Rate loop_rate(400);
    
    // Set first byte of msg id for all msgs
    uint8_t id1 = 85;

    // Init state machine for imu states
    struct StateMachine imuSM;
    imuSM.id1 = id1;
    imuSM.id2 = 242;
    imuSM.length = 16;
    imuSM.state = 1;
    imuSM.success = 0;
    imuSM.csum = 0;
    imuSM.sum = 0;
    StateMachine *imuSMptr = &imuSM;
    ros::Publisher IMUstates_pub = nh.advertise<xmcprotocol::IMUstates>("xmcIMUstates", 1);
    xmcprotocol::IMUstates euler_msg;

    // Init state machine for imu raw
    struct StateMachine imurawSM;
    imurawSM.id1 = id1;
    imurawSM.id2 = 207;
    imurawSM.length = 22;
    imurawSM.state = 1;
    imurawSM.success = 0;
    imurawSM.calibrated = 1;
    imurawSM.csum = 0;
    imurawSM.sum = 0;
    StateMachine *imurawSMptr = &imurawSM;
    ros::Publisher imu_pub = nh.advertise<sensor_msgs::Imu>("Imu", 1);
    ros::Publisher mag_pub = nh.advertise<sensor_msgs::MagneticField>("Magnetic_Field",1);
    sensor_msgs::Imu imu_msg;
    sensor_msgs::MagneticField mag_msg;

    
    // calculate rotation matrix from sensor frame to copter frame.
    double imu_rotation_yaw=0, imu_rotation_pitch=0, imu_rotation_roll=0;
    nh.getParam("imu_rotation_yaw",imu_rotation_yaw);
    nh.getParam("imu_rotation_pitch",imu_rotation_pitch);
    nh.getParam("imu_rotation_roll",imu_rotation_roll);
    tf::Matrix3x3 Copter_R_Imu;
    Copter_R_Imu.setEulerYPR(imu_rotation_yaw,imu_rotation_pitch, imu_rotation_roll);
    tf::Vector3 acc = tf::Vector3(0,0,0);
    tf::Vector3 gyro = tf::Vector3(0,0,0);
    tf::Vector3 mag = tf::Vector3(0,0,0); 

    double g=0;
    nh.getParam("gravity",g);
    
    // Init state machine for mag calibration
    struct StateMachine magSM;
    magSM.id1 = id1;
    magSM.id2 = 155;
    magSM.length = 16;
    magSM.state = 1;
    magSM.success = 0;
    magSM.csum = 0;
    magSM.sum = 0;
    StateMachine *magSMptr = &magSM;
 
    float aRes = 2/32768.0;
    float gRes = 250/32768.0;
    float mRes = 12290/32768.0;
    float magbias[3] = {0,0,0};
    float magCalibration[3] = {1.175781,1.175781,1.136719};
    
    // Init state machine for battery voltage
    struct StateMachine batSM;
    batSM.id1 = id1;
    batSM.id2 = 199;
    batSM.length = 10;
    batSM.state = 1;
    batSM.success = 0;
    batSM.csum = 0;
    batSM.sum = 0;
    StateMachine *batSMptr = &batSM;
    ros::Publisher bat_pub = nh.advertise<copter::Motor_Voltage>("Motor_Voltage", 1);
    copter::Motor_Voltage bat_msg;
    float conversion = 3.3*4.0/4096.0;
 
    int loop = 440;
    while (ros::ok()){
    while (loop)
    {
        /*
         * Read Byte from Serial
         */
        uint8_t newByte = readByte(success);
        if(!success){
            usleep(10);
            loop--;
            continue;
        }
        // ROS_INFO("NEWBYTE: %i",newByte);
        
        /*
         * IMU State Machine
         */
        stateMachine(imuSMptr,newByte);
        if (imuSM.success) {
            // Parse pay load
            euler_msg.yaw = uintToFloat(imuSM.BUF_RX,2);
            euler_msg.pitch = uintToFloat(imuSM.BUF_RX,6);
            euler_msg.roll = uintToFloat(imuSM.BUF_RX,10);
            IMUstates_pub.publish(euler_msg);
            // ROS_INFO("yaw: %f, pitch: %f, roll: %f\n",imu_msg.yaw,imu_msg.pitch,imu_msg.roll);
            imuSM.success = 0;
        }
        
        /*
         * Mag State Machine
         */
        /*stateMachine(magSMptr,newByte);
        if (magSM.success) {
            // Parse pay load
            xmcprotocol::IMUstates imu_msg;
            magCalibration[0] = uintToFloat(imuSM.BUF_RX,2);
            magCalibration[1] = uintToFloat(imuSM.BUF_RX,6);
            magCalibration[2] = uintToFloat(imuSM.BUF_RX,10);
            ROS_INFO("magC1: %f, magC2: %f, magC3: %f\n",magCalibration[0],magCalibration[1],magCalibration[2]);
            imurawSM.calibrated = 1;
        }*/
        
        /*
         * IMU raw State Machine
         */
        stateMachine(imurawSMptr,newByte);
        if (imurawSM.success && imurawSM.calibrated) {
            // Parse pay load
            xmcprotocol::IMUstatesRaw imuraw_msg;

            acc.setX(int8216(imurawSM.BUF_RX[2],imurawSM.BUF_RX[3])*aRes*g);
            acc.setY(int8216(imurawSM.BUF_RX[4],imurawSM.BUF_RX[5])*aRes*g);
            acc.setZ(int8216(imurawSM.BUF_RX[6],imurawSM.BUF_RX[7])*aRes*g);
 
            gyro.setX(int8216(imurawSM.BUF_RX[8],imurawSM.BUF_RX[9])*gRes*DEG_TO_RAD);
            gyro.setY(int8216(imurawSM.BUF_RX[10],imurawSM.BUF_RX[11])*gRes*DEG_TO_RAD);
            gyro.setZ(int8216(imurawSM.BUF_RX[12],imurawSM.BUF_RX[13])*gRes*DEG_TO_RAD);
 
            mag.setX(int8216(imurawSM.BUF_RX[14],imurawSM.BUF_RX[15])*mRes*magCalibration[0]-magbias[0]);
            mag.setY(int8216(imurawSM.BUF_RX[16],imurawSM.BUF_RX[17])*mRes*magCalibration[1]-magbias[1]);
            mag.setZ(int8216(imurawSM.BUF_RX[18],imurawSM.BUF_RX[19])*mRes*magCalibration[2]-magbias[2]);
            
            acc=Copter_R_Imu*acc;
            gyro=Copter_R_Imu*gyro;
            mag=Copter_R_Imu*mag;
            
            // publish IMU data
            imu_msg.header.frame_id = "base_link";
            
            imu_msg.linear_acceleration.x=acc.getX();
            imu_msg.linear_acceleration.y=acc.getY();
            imu_msg.linear_acceleration.z=acc.getZ();
	   
            imu_msg.angular_velocity.x=gyro.getX();
            imu_msg.angular_velocity.y=gyro.getY();
            imu_msg.angular_velocity.z=gyro.getZ();
            
            imu_pub.publish(imu_msg);
            
	    // publish magnetic Field data
            mag_msg.header.frame_id = "base_link";   

            mag_msg.magnetic_field.x=mag.getX();
            mag_msg.magnetic_field.y=mag.getY();
            mag_msg.magnetic_field.z=mag.getZ();
          
            mag_pub.publish(mag_msg);

            //ROS_INFO("acc -- x: %f, y: %f, z: %f",imuraw_msg.accx,imuraw_msg.accy,imuraw_msg.accz);
            //ROS_INFO("gyr -- x: %f, y: %f, z: %f",imuraw_msg.gyrx,imuraw_msg.gyry,imuraw_msg.gyrz);
            //ROS_INFO("mag -- x: %f, y: %f, z: %f\n",imuraw_msg.magx,imuraw_msg.magy,imuraw_msg.magz);
            imurawSM.success = 0;
        }
        
        /*
         * Battery Voltage State Machine
         */
        stateMachine(batSMptr,newByte);
        if (batSM.success) {
            // Parse pay load
            // conversion = 3.3*4/4096;
	    bat_msg.cell1  =(float)((float)uint8216(batSM.BUF_RX[2],batSM.BUF_RX[3]))*conversion;
	    bat_msg.cell12 =(float)((float)uint8216(batSM.BUF_RX[4],batSM.BUF_RX[5]))*conversion;
            bat_msg.cell123=(float)((float)uint8216(batSM.BUF_RX[6],batSM.BUF_RX[7]))*conversion;
            bat_pub.publish(bat_msg);
            // ROS_INFO("Battery Voltage: Cell1: %f, Cell12: %f, Cell123: %f \n", bat_msg.cell1, bat_msg.cell12, bat_msg.cell123);
            batSM.success = 0;
        }
    loop--;
    }
    loop_rate.sleep();
    loop = 440;
    }
    close(uart0_filestream);
    return 0;
}
