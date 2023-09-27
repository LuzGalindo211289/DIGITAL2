#include <SPI.h>
#include <SD.h>
File root;

File myFile;
File myFile2;
String opcion = "";

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  SPI.setModule(0);
  
  Serial.print("Initializing SD card...");

  pinMode(12, OUTPUT);

  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop()
{
   Serial.println("Elija la opción de su preferencia:");
   Serial.println("1 - Leer Archivo");
   Serial.println("2 - Escribir Archivo");
   opcion = leerfrase();
   
   if(opcion == "1"){
    Serial.println("\n Elija el Archivo de su preferencia:");
   
               root = SD.open("/");
               printDirectory(root, 0);
               opcion = leerfrase();
               char miArray[opcion.length() + 1];
            
                opcion.toCharArray(miArray, opcion.length() + 1);
                myFile = SD.open(miArray);
            
                if (myFile) {
                Serial.println("\n File:");
            
                while (myFile.available()) {
                  Serial.write(myFile.read());
                }
                myFile.close();
              } else {
                Serial.println("error opening File");
              }
                 
   }
   else if(opcion == "2"){
      Serial.println("Ingresa el nombre del Nuevo archivo: ");
      String fileName = leerfrase();
      String ext = ".txt";
      String fileName2;
      fileName2.concat(fileName);
      fileName2.concat(ext);
      

      myFile2 = SD.open(fileName2.c_str(), FILE_WRITE);
    
      if (myFile2) {
        Serial.print("Writing to test.txt...");
        Serial.println("Ingrese la Información que desea guardar: ");
        String fileData= leerfrase();
        char miData[fileData.length() + 1];
        myFile2.println(fileData);
        myFile2.close();
        Serial.println("done.");
      } else {
        Serial.println("error opening file");
      }
   }
    else{
     Serial.println("INVALIDO!! Vuelva a elegir");
   }
}
void readFile(){
  myFile = SD.open("Name");
  if (myFile) {
    Serial.println("File:");

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening File");
  }
}
void writeFile(){
  myFile = SD.open("menu.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to File...");
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }
}
String leerfrase ()
{
   while (Serial.available()==0);
   delay (20);
   String inString = "";
   while (Serial.available() > 0)
   {
   int inChar = Serial.read();
   if (inChar == '\n' || inChar == '\r' )
   {inString = inString;}
   else {inString = inString + (char)inChar;}
   }
 return inString;
 }

  
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
