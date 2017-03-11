/* Name       : donios-dfrobot-gps-shield
   Author     : Jeff Donios (jeff@donios.com)
   Description: The sketch for reading NMEA strings from the DFRduino GPS Shield-LEA-6H
   Repository : https://github.com/doniosjm/donios-dfrobot-gps-shield
*/

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #define WireSend(args) Wire.write(args)
  #define WireRead(args) Wire.read(args)
  #define printByte(args) Serial.write(args)
  #define printlnByte(args)  Serial.write(args),Serial.println()
#else
  #include "WProgram.h"
  #define WireSend(args) Wire.send(args)
  #define WireRead(args) Wire.receive(args)
  #define printByte(args) Serial.print(args,BYTE)
  #define printlnByte(args)  Serial.println(args,BYTE)
#endif
#include <Wire.h>
#define BUFFER_LENGTH 80 // NMEA sentences are max 80 char
int GPSAddress = 0x42; // GPS I2C Address

// initialise receiving data from the GPS device
void initReceive() 
{
  Wire.beginTransmission(GPSAddress);
  WireSend(0xff); //To send data address      
  Wire.endTransmission(); 
 
  Wire.beginTransmission(GPSAddress);
  Wire.requestFrom(GPSAddress,10); //Require 10 bytes read from the GPS device
}

// setup
void setup()
{
  Wire.begin(); //IIC Initialize
  Serial.begin(9600); //set baud rate
 
  Serial.println("NMEA statement info");
}

// read an NMEA sentence from the device
// begins with a $
// end with a \r
void readNMEASentence(char *buff){
    int i = 0; // index of data received
    bool readingData = 0; // reading data off the wire

    while(1){
      initReceive(); //Receive data initialization    
      while(Wire.available())   
      { 
        buff[i] = WireRead(); // receive serial data
        if(readingData || buff[i]=='$') // sentences start with $
        { 
          // sentences end in new-line
          if(i>0 && buff[i] =='\r') {
            i = readingData = 0; // reset counters
            Wire.endTransmission(); // end receiving
            return;
          }
          readingData = 1;
          i++;
        }   
      }
      Wire.endTransmission(); //End receiving
    }
}

// work
void loop()
{
    String sentence = "";
    char buff[BUFFER_LENGTH];
    readNMEASentence(buff);

    // clean up the sentence and convert to a string
    for(int i=0; i<(BUFFER_LENGTH); i++){
      if(buff[i] == '\r'){
        i=BUFFER_LENGTH;
      }
      else{
        sentence += buff[i];
      }
    }
    Serial.println(sentence);
}
