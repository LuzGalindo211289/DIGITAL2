#include <stdint.h>
#include "i2c_lib.h"
#include <xc.h>
#include "RTC.h"
#include "LCD.h"
#define _XTAL_FREQ  8000000

// Función para escribir datos en el RTC
void writeRTC(unsigned I2C, unsigned d, char parameter){
    
    I2C_Master_Start();
    I2C_Master_Write(I2C);
    I2C_Master_Write(d);
    I2C_Master_Write(parameter);
    I2C_Master_Stop();
    __delay_ms(20);
}


// Función para leer datos desde el RTC
unsigned short readRTC(unsigned I2C, unsigned d){
    unsigned short RTC;
    
    I2C_Master_Start();
    I2C_Master_Write(I2C);
    I2C_Master_Write(d);
    I2C_Master_Stop();
    __delay_ms(10);
       
    I2C_Master_Start();
    I2C_Master_Write(I2C + 0x01);
    RTC = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(10);

    return RTC;
}
