//--------------Semaforo----------------
// Definimos los pines para los LEDs
const int pinRojo = PB_2;
const int pinAmarillo = PE_0;
const int pinVerde = PF_0;

bool semaforoVerde = false; // Variable para controlar si el semáforo está en verde

const int ledPins[] = {PB_5, PB_0, PB_1, PE_4, PE_5, PB_4, PA_5, PA_6};
const int ledPins2[] = {PD_0, PD_1, PD_2, PD_3, PE_1, PE_2, PE_2, PF_1};

int contador = 0; // Variable para almacenar el contador
int contador2 = 0; // Variable para almacenar el contador

bool ledsEncendidos = false;
bool ledsEncendidos2 = false;


const int buttonPin = PB_3;     // the number of the pushbutton pin
const int buttonPin1 = PC_4;     // the number of the pushbutton pin
const int buttonPin2 = PC_5;     // the number of the pushbutton pin

const int win1 = PF_2;
const int win2 = PF_3;


int buttonState = 0;         // variable for reading the pushbutton status
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status

//********************************************************************************************

void setup() {
  // Configuramos los pines como salidas
  //CONTADOR
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT); // Configura los pines de los LEDs como salidas
  }  
   for (int i = 0; i < 8; i++) {
    pinMode(ledPins2[i], OUTPUT); // Configura los pines de los LEDs como salidas
  } 
  //BOTONES
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  //SEMAFORO
  pinMode(pinRojo, OUTPUT);
  pinMode(pinAmarillo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  //GANADOR
  pinMode(win1, OUTPUT);
  pinMode(win2, OUTPUT);
  
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 baudios (para depuración)
  leerLEDs(); 
  leerLEDs2(); 
}


//********************************************************************************************

void loop() {
 int estadoBoton = digitalRead(buttonPin1); // Lee el estado del botón
 int estadoBoton2 = digitalRead(buttonPin2); // Lee el estado del botón
 buttonState = digitalRead(buttonPin);

 //----------------SEMAFORO--------------------------------------
  if (buttonState == LOW) {        
    // Encendemos el LED rojo (se supone que es el estado "ROJO")
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAmarillo, LOW);
    delay(2000);  // Esperamos 5 segundos
  
    // Apagamos el rojo y encendemos el amarillo (se supone que es el estado "AMARILLO")
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAmarillo, HIGH);
    delay(2000);  // Esperamos 2 segundos
  
    // Apagamos el amarillo y encendemos el verde (se supone que es el estado "VERDE")
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAmarillo, LOW);
    delay(5000);  // Esperamos 5 segundos
  
    // Reiniciamos el ciclo
    digitalWrite(pinRojo, LOW);      
    semaforoVerde = true; // Marcamos que el semáforo está en verde
  } 
  else {
    // turn LED off:
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAmarillo, LOW);
    semaforoVerde = false; // Marcamos que el semáforo no está en verde
  }
//-------------------------------Jugador 1--------------------------------------------------
  if (semaforoVerde) {
      
  if (estadoBoton == LOW) { // El botón está presionado (debido a la resistencia pull-up)
    delay(100); // Espera un breve tiempo para evitar rebotes del botón
    while (digitalRead(buttonPin1) == LOW) {} // Espera a que se suelte el botón

    contador++; // Incrementa el contador
    if (contador > 7) {
      contador = 0; // Reinicia el contador cuando alcanza 255
    }

    ledsEncendidos = true; // Activa los LEDs
    leerLEDs(); // Actualiza los LEDs con el nuevo valor del contador
  }

  if (ledsEncendidos && contador >= 8) {
    ledsEncendidos = false; // Desactiva los LEDs cuando el contador llega a 8
    apagarLEDs();
  }
  }
//-------------------------------Jugador 2--------------------------------------------------
  if (semaforoVerde) {
  if (estadoBoton2 == LOW) { // El botón está presionado (debido a la resistencia pull-up)
    delay(100); // Espera un breve tiempo para evitar rebotes del botón
    while (digitalRead(buttonPin2) == LOW) {} // Espera a que se suelte el botón

    contador2++; // Incrementa el contador
    if (contador2 > 7) {
      contador2 = 0; // Reinicia el contador cuando alcanza 255
    }

    ledsEncendidos2 = true; // Activa los LEDs
    leerLEDs2(); // Actualiza los LEDs con el nuevo valor del contador
  }

  if (ledsEncendidos2 && contador2 >= 8) {
    ledsEncendidos2 = false; // Desactiva los LEDs cuando el contador llega a 8
    apagarLEDs2();
  }
  
}
// Rutina para determinar quién llegó primero a la meta
  if (!ledsEncendidos && !ledsEncendidos2 && semaforoVerde) {
    if (contador < contador2) {
      // Jugador 1 ganó
      digitalWrite(win1, HIGH); // Encender LED para indicar que el jugador 1 ganó
    } else if (contador2 < contador) {
      // Jugador 2 ganó
      digitalWrite(win2, HIGH); // Encender LED para indicar que el jugador 2 ganó
    }

}

//********************************************************************************************
//-------------------------------Jugador 1--------------------------------------------------
void leerLEDs() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], i == contador); // Enciende o apaga el LED según el valor del bit
  }
}

void apagarLEDs() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], LOW); // Apaga todos los LEDs
  }
}

//-------------------------------Jugador 2--------------------------------------------------

void leerLEDs2() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins2[i], i == contador2); // Enciende o apaga el LED según el valor del bit
  }
}

void apagarLEDs2() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins2[i], LOW); // Apaga todos los LEDs
  }
}
