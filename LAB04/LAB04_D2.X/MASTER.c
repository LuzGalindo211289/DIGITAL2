#pragma config  FOSC    = INTRC_NOCLKOUT
#pragma config  WDTE    = OFF
#pragma config  PWRTE   = OFF
#pragma config  MCLRE   = OFF
#pragma config  CP      = OFF
#pragma config  CPD     = OFF
#pragma config  BOREN   = OFF
#pragma config  IESO    = OFF
#pragma config  FCMEN   = OFF
#pragma config  LVP     = OFF

#pragma config  BOR4V   = BOR40V
#pragma config  WRT     = OFF

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <xc.h>
#include "i2c_lib.h"
#include <xc.h>
#include "LCD.h"
#include "RTC.h"

//*****************************************************************************
// Definición de variables
//*****************************************************************************
// Definición de frecuencia del cristal
#define CRYSTAL_FREQ 8000000

// Definición de los pines para los botones
#define BUTTON1 RB0
#define BUTTON2 RB1
//#define BUTTON3 RB2


// Declaración de variables globales
uint8_t potentiometer;
uint8_t unit, ten, hundred;

uint8_t seconds, minutes, hours;
uint8_t day, month, year;

uint8_t menuSelection, menuParameter, temp;

// Cadena de caracteres para almacenar la hora y fecha
static char Time[] = "00:00:00";
static char Date[] = "00/00/2000";

//*****************************************************************************
// Definición de funciones
//*****************************************************************************

void initialize(void);
__bit debounce(void);

void printTimeAndDate(void);

uint8_t adjustValue(uint8_t parameter, uint8_t rtc);
uint8_t configureValue(uint8_t x, uint8_t y, uint8_t parameter, uint8_t rtc);
void blink(void);

uint8_t decimalToBCD(uint8_t num);
uint8_t BCDToDecimal(uint8_t num);

// Rutina de interrupción
void __interrupt() ISR(void){
    if (INTCONbits.RBIF == 1){
        if (BUTTON1 == 0){
            if(debounce())
                menuSelection++;
        } else if (BUTTON2 == 0){
            if(menuSelection != 0)
                if(debounce())
                    menuParameter++;
        }
//        else if (BUTTON3 == 0) {            
//            if (menuSelection != 0)
//                if (debounce())
//                    menuParameter--;
//        }
        if(menuSelection > 5)
            menuSelection = 0;
        if(menuSelection == 1 && menuParameter > 23)
            menuParameter = 0;
        if(menuSelection == 2 && menuParameter > 59)
            menuParameter = 0;
        if(menuSelection == 3 && menuParameter > 12)
            menuParameter = 1;
        if(menuSelection == 3 && menuParameter == 0)
            menuParameter = 1;
        if(menuSelection == 4 && menuParameter > 99)
            menuParameter = 0;
                
        INTCONbits.RBIF = 0;
    }
}

// Función principal
void main(void) {
    initialize();
    
    while(1){
        
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        potentiometer = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);

        if(menuSelection != 0){
            if(menuSelection == 1){                        
                menuParameter = adjustValue(menuParameter, hours);
                hours = configureValue(1, 8, menuParameter, hours);
            }
            if(menuSelection == 2){
                menuParameter = adjustValue(menuParameter, minutes);
                minutes = configureValue(1, 11, menuParameter, minutes);
            }
            if(menuSelection == 3){
                menuParameter = adjustValue(menuParameter, month);
                month = configureValue(2, 9, menuParameter, month);
            }
            if(menuSelection == 4){
                menuParameter = adjustValue(menuParameter, year);
                year = configureValue(2, 14, menuParameter, year);
            }
            if(menuSelection == 5){
                writeRTC(0xD0, 0x02, hours);
                writeRTC(0xD0, 0x01, minutes);
                writeRTC(0xD0, 0x05, month);
                writeRTC(0xD0, 0x06, year);
                writeRTC(0xD0, 0x00, 0x00);
                menuParameter = 0;
                menuSelection = 0;
            }
        } else {                     
            seconds = readRTC(0xD0, 0x00);
            minutes = readRTC(0xD0, 0x01);
            hours = readRTC(0xD0, 0x02);
            day = readRTC(0xD0, 0x04);
            month = readRTC(0xD0, 0x05);
            year = readRTC(0xD0, 0x06); 
        }

        hundred = CENTENA(potentiometer);
        ten = DECENA(potentiometer);
        unit = UNIDAD(potentiometer);
       
        hundred += '0';
        ten += '0';
        unit += '0';
        
        LCD_CLEAR();
        LCD_XY(1, 0);
        LCD_STRING("temp:");
        LCD_XY(2, 0);
        LCD_CHAR(hundred);
        LCD_STRING(".");
        LCD_CHAR(ten);
        LCD_CHAR(unit);
        
        printTimeAndDate();
    }
    return;
}

// Función de configuración
void initialize(void){
    OSCILLATOR(1);
    
    ANSEL = 0x00;
    ANSELH = 0x00;

    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;
    
    TRISB = 0b00000111;
    PORTB = 0x00;
    
    IOC_INT(0b00000011);
    
    potentiometer = 0;
    unit = 0;
    ten = 0;
    hundred = 0;
    
    seconds = 0;
    minutes = 0;
    hours = 0;
    day = 0;
    month = 0;
    year = 0;

    menuSelection = 0;
    menuParameter = 0;

    LCD_INIT();
    I2C_Master_Init(100000);
}

// Función para imprimir la hora y fecha en el LCD
void printTimeAndDate(void){

    uint8_t seconds1, minutes1, hours1;
    uint8_t day1, month1, year1;
    seconds1 = BCDToDecimal(seconds);
    minutes1 = BCDToDecimal(minutes);
    hours1 = BCDToDecimal(hours);
    day1 = BCDToDecimal(day);
    month1 = BCDToDecimal(month);
    year1 = BCDToDecimal(year); 
    
    Time[0] = hours1/10 + '0';
    Time[1] = hours1 % 10 + '0';
    Time[3] = minutes1 / 10 + '0';
    Time[4] = minutes1 % 10 + '0';
    Time[6] = seconds1/10 + '0';
    Time[7] = seconds1 % 10 + '0';
    
    Date[0] = day1/10 + '0';
    Date[1] = day1 % 10 + '0';
    Date[3] = month1/10 + '0';
    Date[4] = month1 % 10 + '0';
    Date[8] = year1/10 + '0';
    Date[9] = year1 % 10 + '0';
    
    LCD_XY(1, 8);
    LCD_STRING(Time);
    LCD_XY(2, 6);
    LCD_STRING(Date);
    __delay_ms(100);
}

// Función para parpadeo
void blink(void){
    uint8_t j = 0;
    while(j > 100 && BUTTON1 && BUTTON2){
        j++;
        __delay_ms(5);
    }
}

uint8_t decimalToBCD(uint8_t num){
    uint8_t tens = num / 10;            // Obtener los dígitos de las decenas
    uint8_t ones = num % 10;            // Obtener los dígitos de las unidades
    
    return (tens << 4) | ones;          // Combinar y devolver el valor en formato BCD
}


// Función para convertir BCD a decimal
uint8_t BCDToDecimal(uint8_t num){
    uint8_t tens = (num >> 4) & 0x0F;   // Extraer los dígitos de las decenas
    uint8_t ones = num & 0x0F;          // Extraer los dígitos de las unidades
    
    return tens * 10 + ones;            // Calcular el valor decimal
}



// Función para realizar ajuste manual en parámetros
uint8_t adjustValue(uint8_t parameter, uint8_t rtc){
    if(!BUTTON1){
        if(debounce())
            parameter = BCDToDecimal(rtc);
    }
//    if (!BUTTON3) {
//        if (debounce())
//            parameter = BCDToDecimal(rtc) - 1; // Decrementar el valor
//    }

    
    return parameter;
    
}

// Función para configurar valores en modo de ajuste
uint8_t configureValue(uint8_t x, uint8_t y, uint8_t parameter, uint8_t rtc){
    rtc = decimalToBCD(parameter);

    LCD_XY(x, y);
    blink();
    LCD_STRING("  ");
    blink();
    return rtc;
}

// Función de antirrebote para los botones
__bit debounce(void){
    uint8_t counter = 0;
    for(uint8_t i = 0; i < 5; i++){
        if(!BUTTON1 || !BUTTON2)
            counter++;
        __delay_ms(10);
    }
    if(counter > 2)
        return 1;
    else
        return 0;
}
