#include "LEDServer.h"

#include <WiFi.h>
#include <WebServer.h>

//#include <Timer.h>

void setup() {
  Serial.begin(115200);
  SerialTIVA1.begin(115200); // Inicializar el segundo puerto serie2
  SerialTIVA2.begin(115200); // Inicializar el segundo puerto serie2

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  // Configuración del objeto SevSeg
  byte numDigits = 2; // Dos dígitos para mostrar el conteo
  byte digitPins[] = {18, 19}; // Pines conectados a los dígitos 
  byte segmentPins[] = {4, 0, 2, 5, 15, 12, 14, 27}; // Pines conectados a los segmentos 
  
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);

  delay(100);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
  

  pinMode(LED1pin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handle_OnConnect);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
server.handleClient();
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }

    // Leer el segundo puerto serie para recibir mensajes del dispositivo TIVA
  while (SerialTIVA1.available() > 0) {
    //char incomingChar = SerialTIVA.read();    //Lectura mensajes de TIVA
    uint8_t b = Serial.read();
    BufferSerial1[ptr_BufferSerial1++] = b;
    Serial.println(b);
  }
  while (SerialTIVA2.available() > 0) {
    //char incomingChar = SerialTIVA.read();    //Lectura mensajes de TIVA
    uint8_t c = Serial2.read();
    BufferSerial[ptr_BufferSerial++] = c;
    Serial.println(c);
  }
  if (ptr_BufferSerial1 != ptr_Lectura1){
    Serial.println("Activado");
    Serial.println(BufferSerial1[ptr_Lectura1]);
    if (BufferSerial1[ptr_Lectura1] == 0xDD){
      uint8_t r = ptr_BufferSerial1 - ptr_Lectura1;
      if (r > Lectura_SIZE1){

        for (int i = 0; i < Lectura_SIZE1; i++){
            
            Lectura1[i] = BufferSerial1[ptr_Lectura1];
            Serial.print(Lectura1[i]);
            Serial.print(", ");
            ptr_Lectura1++;
            }
          Serial.println();
        if (Lectura1[4]==0xFF){
          Serial.println("Lectura correcta");
          if (Lectura1[1]==0x00){
            Serial.println("Parking 1");
            if (Lectura1[2]==0x01){
              Serial.println("LED 1, Parking 1");
              if (Lectura1[3]==0x00){
                parking1_LED1 = false;
              }
              else if (Lectura1[3]==0x01){
                parking1_LED1 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x02){
              Serial.println("LED 2, Parking 1");
                if (Lectura1[3]==0x00){
                parking1_LED2 = false;
              }
              else if (Lectura1[3]==0x01){
                parking1_LED2 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x03){
              Serial.println("LED 3,Parking 1");
                if (Lectura1[3]==0x00){
                parking1_LED3 = false;
              }
              else if (Lectura1[3]==0x01){
                parking1_LED3 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x04){
            Serial.println("LED 4,Parking 1");
                if (Lectura1[3]==0x00){
                parking1_LED4 = false;
              }
              else if (Lectura1[3]==0x01){
                parking1_LED4 = true;
              }
              else; ptr_Lectura1++;
            }
            else; ptr_Lectura1++;
          }
          else if(Lectura1[1]==0x10){
            Serial.println("Parking 2");
            if (Lectura1[2]==0x01){
              Serial.println("LED 1, Parking 2");
              if (Lectura1[3]==0x00){
                parking2_LED1 = false;
              }
              else if (Lectura1[3]==0x01){
                parking2_LED1 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x02){
              Serial.println("LED 2, Parking 2");
              if (Lectura1[3]==0x00){
                parking2_LED2 = false;
              }
              else if (Lectura1[3]==0x01){
                parking2_LED2 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x03){
              Serial.println("LED 3,Parking 2");
                if (Lectura1[3]==0x00){
                parking2_LED3 = false;
              }
              else if (Lectura1[3]==0x01){
                parking2_LED3 = true;
              }
              else; ptr_Lectura1++;
            }
            else if (Lectura1[2]==0x04){
            Serial.println("LED 4,Parking 2");
                if (Lectura1[3]==0x00){
                parking1_LED4 = false;
              }
              else if (Lectura1[3]==0x01){
                parking1_LED4 = true;
              }
              else; ptr_Lectura1++;
            }
          }
          else; ptr_Lectura1++;
        }
        else{
            Serial.println("Lectura incorrecta");
            ptr_Lectura1++;
        }
      }
      else{
        cont1++;
        if(cont1 >= 5){
          ptr_Lectura1++;
          cont1 = 0;
        }
      }
    }
    else{
      ptr_Lectura1++;
      Serial.println("");
    }; 
  }
  if (ptr_BufferSerial != ptr_Lectura){
    Serial.println("Activado");
    Serial.println(BufferSerial[ptr_Lectura]);
    if (BufferSerial[ptr_Lectura] == 0xDD){
      uint8_t r = ptr_BufferSerial - ptr_Lectura;
      if (r > Lectura_SIZE){

        for (int i = 0; i < Lectura_SIZE; i++){
            
            Lectura[i] = BufferSerial[ptr_Lectura];
            Serial.print(Lectura[i]);
            Serial.print(", ");
            ptr_Lectura++;
            }
          Serial.println();
        if (Lectura[4]==0xFF){
          Serial.println("Lectura correcta");
          if (Lectura[1]==0x00){
            Serial.println("Parking 1");
            if (Lectura[2]==0x01){
              Serial.println("LED 1, Parking 1");
              if (Lectura[3]==0x00){
                parking1_LED1 = false;
              }
              else if (Lectura[3]==0x01){
                parking1_LED1 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x02){
              Serial.println("LED 2, Parking 1");
                if (Lectura[3]==0x00){
                parking1_LED2 = false;
              }
              else if (Lectura[3]==0x01){
                parking1_LED2 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x03){
              Serial.println("LED 3,Parking 1");
                if (Lectura[3]==0x00){
                parking1_LED3 = false;
              }
              else if (Lectura[3]==0x01){
                parking1_LED3 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x04){
            Serial.println("LED 4,Parking 1");
                if (Lectura[3]==0x00){
                parking1_LED4 = false;
              }
              else if (Lectura[3]==0x01){
                parking1_LED4 = true;
              }
              else; ptr_Lectura++;
            }
            else; ptr_Lectura++;
          }
          else if(Lectura[1]==0x10){
            Serial.println("Parking 2");
            if (Lectura[2]==0x01){
              Serial.println("LED 1, Parking 2");
              if (Lectura[3]==0x00){
                parking2_LED1 = false;
              }
              else if (Lectura[3]==0x01){
                parking2_LED1 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x02){
              Serial.println("LED 2, Parking 2");
              if (Lectura[3]==0x00){
                parking2_LED2 = false;
              }
              else if (Lectura[3]==0x01){
                parking2_LED2 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x03){
              Serial.println("LED 3,Parking 2");
                if (Lectura[3]==0x00){
                parking2_LED3 = false;
              }
              else if (Lectura[3]==0x01){
                parking2_LED3 = true;
              }
              else; ptr_Lectura++;
            }
            else if (Lectura[2]==0x04){
            Serial.println("LED 4,Parking 2");
                if (Lectura[3]==0x00){
                parking1_LED4 = false;
              }
              else if (Lectura[3]==0x01){
                parking1_LED4 = true;
              }
              else; ptr_Lectura++;
            }
          }
          else; ptr_Lectura++;
        }
        else{
            Serial.println("Lectura incorrecta");
            ptr_Lectura++;
        }
      }
      else{
        cont++;
        if(cont >= 5){
          ptr_Lectura++;
          cont = 0;
        }
      }
    }
    else{
      ptr_Lectura++;
      Serial.println("Primera lectura no es DD");
    }; 
  }
  //Contar numero de LEDs activos
numActiveLEDs = countActiveLEDs();

// Mostrar el número en el display de 7 segmentos
sevseg.setNumber(numActiveLEDs);
sevseg.refreshDisplay();

}
void handle_OnConnect() {
  String html = "<html><body><h1>Estado Parqueomatic &#128664</h1>";
  html += "<h3>Parking 1</h3>";
  html += "<table border='1'><tr><th>Variable</th><th>Estado</th></tr>";

  // Agrega las primeras 4 variables a la tabla con su estado actualizado
  html += "<tr><td>Parqueo 1</td><td>" + getCircle(parking1_LED1) + "</td></tr>";
  html += "<tr><td>Parqueo 2</td><td>" + getCircle(parking1_LED2) + "</td></tr>";
  html += "<tr><td>Parqueo 3</td><td>" + getCircle(parking1_LED3) + "</td></tr>";
  html += "<tr><td>Parqueo 4</td><td>" + getCircle(parking1_LED4) + "</td></tr>";

  html += "</table>";

  html += "<h3>Parking 2</h3>";
  html += "<table border='1'><tr><th>Variable</th><th>Estado</th></tr>";

  // Agrega las siguientes 4 variables a la segunda tabla con su estado actualizado
  html += "<tr><td>Parqueo 5</td><td>" + getCircle(parking2_LED1) + "</td></tr>";
  html += "<tr><td>Parqueo 6</td><td>" + getCircle(parking2_LED2) + "</td></tr>";
  html += "<tr><td>Parqueo 7</td><td>" + getCircle(parking2_LED3) + "</td></tr>";
  html += "<tr><td>Parqueo 8</td><td>" + getCircle(parking2_LED4) + "</td></tr>";

  html += "</table></body></html>";
  server.send(200, "text/html", html);
}

String getCircle(bool status) {
  // Devuelve el código HTML para un círculo verde o rojo según el estado
  if (status) {
    return "<div style='width: 20px; height: 20px; border-radius: 50%; background-color: green;'></div>";
  } else {
    return "<div style='width: 20px; height: 20px; border-radius: 50%; background-color: red;'></div>";
  }
}