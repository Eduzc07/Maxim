#ifndef MPU6050_H
#define MPU6050_H


#define ADRESS 0x68

//Accel:
#define LSB_ACC 16384.0f
#define ACCURACY_ACC 0 

//GYRO
#define ACCURACY_GYRO 0 
	//set to:
	//2000°/s => 18, 
	//1000°/s => 10, 
	//500°/s => 8, 
	//250°/s => 0

#define LSB_GYRO 131.0f
	//2000°/s => 16,4
	//1000°/s => 32,8, 
	//500°/s => 65,5, 
	//250°/s => 131

#define GRAV 9.806f //m/s^2

class MPU6050{
	public:
		MPU6050(void);

		int init(void);
	
		int calibrationAccel(int times);

		int calibrationGyro(int times, float* gyroQ);

		int readAccel(float &x, float &y, float &z);

		int readGyro(float &x, float &y, float &z);

		int getInitError();

	private:
		int file;
		float xOffsetAcc;
		float yOffsetAcc;
		float zOffsetAcc;

		float xOffsetGyro;
		float yOffsetGyro;
		float zOffsetGyro;
		int init_error;
};
#endif
