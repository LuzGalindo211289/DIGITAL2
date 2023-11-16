#ifndef LEDSERVER_H
#define LEDSERVER_H

/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
#include <SevSeg.h>

// Configuración del segundo puerto serie
#define SerialTIVA1 Serial
#define SerialTIVA2 Serial2
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "JuanpapiL";  // Enter your SSID here
const char* password = "gago123P";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

// Definir el objeto para el display de 7 segmentos
SevSeg sevseg;


uint8_t LED1pin = 2;
bool LED1status = LOW;

//Indicadores LED y status
//Parking 1
bool parking1_LED1 = true;
bool parking1_LED2 = false;
bool parking1_LED3 = false;
bool parking1_LED4 = false;

//Parking 2
bool parking2_LED1 = false;
bool parking2_LED2 = false;
bool parking2_LED3 = false;
bool parking2_LED4 = false;

//Variables comunicacion serial
static const int BufferSerial_SIZE1 = 256;
static const int Lectura_SIZE1 = 5;
static const int BufferSerial_SIZE = 256;
static const int Lectura_SIZE = 5;

uint8_t BufferSerial1[BufferSerial_SIZE1];
uint8_t Lectura1[Lectura_SIZE1];
uint8_t BufferSerial[BufferSerial_SIZE];
uint8_t Lectura[Lectura_SIZE];

uint8_t ptr_BufferSerial1 = 0;
uint8_t ptr_Lectura1 = 0;
uint8_t ptr_BufferSerial = 0;
uint8_t ptr_Lectura = 0;

uint8_t cont1 = 0;
uint8_t cont = 0;

uint8_t numActiveLEDs = 0;

int countActiveLEDs() {
  int count = 0;

  // Sumar las variables que tienen el valor true
  if (parking1_LED1) count++;
  if (parking1_LED2) count++;
  if (parking1_LED3) count++;
  if (parking1_LED4) count++;

  if (parking2_LED1) count++;
  if (parking2_LED2) count++;
  if (parking2_LED3) count++;
  if (parking2_LED4) count++;

  return count;
}
#endif