// Definimos los pines para los LEDs
const int pinRojo = RED_LED;
const int pinAzul = BLUE_LED;
const int pinVerde = GREEN_LED;
const int buttonPin = PUSH2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status


void setup() {
  // Configuramos los pines como salidas
  pinMode(pinRojo, OUTPUT);
  pinMode(pinAzul, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {        
    // Encendemos el LED rojo (se supone que es el estado "ROJO")
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAzul, LOW);
    delay(2000);  // Esperamos 5 segundos
  
    // Apagamos el verde y encendemos el amarillo (se supone que es el estado "amarillo")
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAzul, LOW);
    delay(2000);  // Esperamos 2 segundos
  
    // Apagamos el amarillo y encendemos el verde (se supone que es el estado "rojo")
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAzul, LOW);
    delay(5000);  // Esperamos 5 segundos
  
    // Reiniciamos el ciclo
    digitalWrite(pinRojo, LOW);      
  } 
  else {
    // turn LED off:
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAzul, LOW); 
  }



}
