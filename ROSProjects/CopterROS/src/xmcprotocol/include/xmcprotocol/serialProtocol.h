#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

// Serial includes
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
#include "ros/console.h"
#include "ros/ros.h"

// ROS includes
#include "ros/console.h"
#include "ros/ros.h"

void setROSloglevel2Debug();
#define ROS_LOG_LEVEL_DEBUG setROSloglevel2Debug()

float intToFloat(int8_t* ints, int s);
float uintToFloat(uint8_t* uint, int s);
int16_t int8216(uint8_t h, uint8_t l);
uint16_t uint8216(uint8_t h, uint8_t l);
void uint1628(uint16_t ui, uint8_t* buf, uint8_t s);
/*template<typename T,int N>
void printBitrepresentation(T (&uintArray)[N]);
template<typename T,int N>
void parseMessage(T (&msg)[N],float* ypr);*/

int initSerial(void);

#endif
