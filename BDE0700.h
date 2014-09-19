
#include <REG51.H>
#include <stdio.h>
#include "DELAY.h"



#ifndef	__BDE0700_H__
#define	__BDE0700_H__


sbit LP_rd = P3^3;
sbit LP_set = P3^4;

extern uint get_Temperature();

#endif
