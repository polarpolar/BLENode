#include <REG51.H>
#include <stdio.h>
#include "I2C.h"
#include "delay.h"


sbit TX = P2^4;

void BlueTooth_Send(char* msg, uint len);