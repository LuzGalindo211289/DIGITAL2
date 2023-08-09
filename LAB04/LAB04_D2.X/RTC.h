#ifndef RTC_H
#define	RTC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <proc/pic16f887.h>
//#define _XTAL_FREQ  8000000

void writeRTC(unsigned I2C, unsigned d, char parameter);
unsigned short readRTC(unsigned I2C, unsigned d);

#endif