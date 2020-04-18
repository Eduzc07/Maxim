#include "MPU6050.h"
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/time.h>

MPU6050::MPU6050(void){
	init_error=init();
}



int MPU6050::init(void){
	const char *devName = "/dev/i2c-2";
	file = open(devName, O_RDWR);
	if(file == -1){ printf("error"); return -1;}
	char buf[10];
	if(ioctl(file, I2C_SLAVE,ADRESS)<0){printf("Err2");return -2;}
	buf[0]=0x6B; buf[1]=0;
	if(write(file,buf,2)!=2){printf("err1n");return -2;}
	buf[0]=0x1A; buf[1]=6;
	if(write(file,buf,2)!=2){printf("err1n");return -2;}
	buf[0]=0x18; buf[1]=8|0xE0;
	if(write(file,buf,2)!=2){printf("err1n");return -2;}
	// ACC accuracy
	buf[0]=0x1C; buf[1]=ACCURACY_ACC;
	if(write(file,buf,2)!=2){printf("err1n");return -2;}
	// Gyro accuracy	
	buf[0]=0x1B; buf[1]=ACCURACY_GYRO;
	if(write(file,buf,2)!=2){printf("err1n");return -2;}
	return 0;
}

int MPU6050::calibrationGyro(int times, float* gyroQ){
	if(file == -1){ printf("error"); return -1;}	
	char buf[10];

	xOffsetGyro=0;
	yOffsetGyro=0;
	zOffsetGyro=0;
	
	float gyroVal[times][3];
	int count=0;
	for(int n=0; n<times; n++){
		buf[0]=0x43;
		if(write(file,buf,1)!=1){printf("err1");return -2;}
		if(read(file,buf,6) != 6){printf("err4"); return -3;}
		usleep(1000);
		short int x = (buf[0]&0xFF)<<8|(buf[1]&0xFF);
		short int y = (buf[2]&0xFF)<<8|(buf[3]&0xFF);
		short int z = (buf[4]&0xFF)<<8|(buf[5]&0xFF);
		// xOffsetGyro+= x;
		// yOffsetGyro+= y;
		// zOffsetGyro+= z;
		
		// save values for sample covariance calculation
		gyroVal[n][0]=x;
		gyroVal[n][1]=y;
		gyroVal[n][2]=z;

		usleep(4000);
	}
	// calculate mean
	float x_mean=0,y_mean=0,z_mean=0;

	for (int i=0; i<times; i++){
		x_mean+=gyroVal[i][0];
		y_mean+=gyroVal[i][1];
		z_mean+=gyroVal[i][2];
	}
	x_mean=x_mean/times;
	y_mean=y_mean/times;
	z_mean=z_mean/times;

	float mean[3];
	mean[0]=x_mean;
	mean[1]=y_mean;
	mean[2]=z_mean;

        // Sample covariance calculation	
        for (int j=0; j<3; j++){
               	for (int k=0; k<3; k++){
			for (int i=0; i<times; i++){
                       		gyroQ[3*j+k]=(gyroVal[i][j]-mean[j])*(gyroVal[i][k]-mean[k]);
			}
              	}
        }
	for (int i; i<9; i++){
		gyroQ[i]=gyroQ[i]/(times-1);
	}
	xOffsetGyro=-x_mean;
	yOffsetGyro=-y_mean;
	zOffsetGyro=-z_mean;
	return 0;
}

int MPU6050::readGyro(float &x, float &y, float &z){
	if(file == -1){ printf("error"); return -1;}
	char buf[10];
	buf[0]=0x43;

	if(write(file,buf,1)!=1){printf("err1");return -2;}
	if(read(file,buf,6) != 6){printf("err4");return -3;}
	//usleep(1000);
	
	short int i = (buf[0]&0xFF)<<8|(buf[1]&0xFF);
	short int j = (buf[2]&0xFF)<<8|(buf[3]&0xFF);
	short int k = (buf[4]&0xFF)<<8|(buf[5]&0xFF);
	
	i+=+xOffsetGyro;
	j+=+yOffsetGyro;
	k+=+zOffsetGyro;

	x= ((float) i)/(LSB_GYRO)/180*M_PI;
	y= ((float) j)/(LSB_GYRO)/180*M_PI;
	z= ((float) k)/(LSB_GYRO)/180*M_PI;

	return 0;
}

int MPU6050::calibrationAccel(int times){
	if(file == -1){ printf("error"); return -1;}
	char buf[10];

	xOffsetAcc=0;
	yOffsetAcc=0;
	zOffsetAcc=0;

	int count=0;
	while(count<times){
		buf[0]=0x3B;
		if(write(file,buf,1)!=1){printf("err1");return -2;}
		if(read(file,buf,6) != 6){printf("err4");return -3;}
		usleep(1000);
		short int i = (buf[0]&0xFF)<<8|(buf[1]&0xFF);
		short int j = (buf[2]&0xFF)<<8|(buf[3]&0xFF);
		short int k = (buf[4]&0xFF)<<8|(buf[5]&0xFF);
		xOffsetAcc+= (float)i/(LSB_ACC)*GRAV;
		yOffsetAcc+= (float)j/(LSB_ACC)*GRAV;
		zOffsetAcc+= (float)k/(LSB_ACC)*GRAV-(float)GRAV;
		count++;

	}
	
	xOffsetAcc = xOffsetAcc/times;
	yOffsetAcc = yOffsetAcc/times;
	zOffsetAcc = zOffsetAcc/times;	
	//printf("xOffset: %d, yOffset: %d, zOffset: %d\n"  , (int)i,(int)j,(int)k);
	return 0;
}

int MPU6050::readAccel(float &x, float &y, float &z){
	if(file == -1){ printf("error"); return -1;}
	char buf[10];
	buf[0]=0x3B;

	if(write(file,buf,1)!=1){printf("err1");return -2;}
	if(read(file,buf,6) != 6){printf("err4"); return -3;}
	// usleep(1000);
	
	short int i = (buf[0]&0xFF)<<8|(buf[1]&0xFF);
	short int j = (buf[2]&0xFF)<<8|(buf[3]&0xFF);
	short int k = (buf[4]&0xFF)<<8|(buf[5]&0xFF);
	
	x= ((float)i/(LSB_ACC)*GRAV-xOffsetAcc);
	y= ((float)j/(LSB_ACC)*GRAV-yOffsetAcc);
	z= ((float)k/(LSB_ACC)*GRAV-zOffsetAcc);
	
	//printf(" xr: %d, yr: %d, zr: %d\n"  , (int)i,(int )j,(int)k);
	//printf(" x: %d, y: %d, z: %d\n"  , (int)x,(int )y,(int)z);
	return 0;
}

int MPU6050::getInitError(){
	return init_error;
}
