/*
 Ricezione dai sensori porte
 Trasmissione verso telecamera
 
 http://code.google.com/p/rc-switch/
 
 Need help? http://forum.ardumote.com
 */

#include <RCSwitch.h>
#include <Time.h>  

RCSwitch mySwitch = RCSwitch();
int SERIAL_BAUD        = 9600; 
int TRASMIT_PIN       = 10;
int RECEIVE_PIN       = 10;
int NOISE_DIGITAL_PIN =    2; 

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(SERIAL_BAUD, INPUT);  
  mySwitch.enableReceive(RECEIVE_PIN);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(TRASMIT_PIN);  // Using Pin #10
}

void loop() {
  if (detectNoise()){
    Serial.print("Rumore");
    accendiCam() ;
  }
  
  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } 
    else {

      if (mySwitch.getReceivedValue() == 3557625) {
        Serial.print("Attenzione! Porta cucina aperta!");
         Serial.print("Invio: ");
     accendiCam() ;
       delay(1000); 
      } 
    
      Serial.print("Received ");
       Serial.print( mySwitch.getReceivedValue() );
       Serial.print(" / ");
       Serial.print( mySwitch.getReceivedBitlength() );
       Serial.print("bit ");
       Serial.print("Protocol: ");
       Serial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  }
  
}


bool detectNoise () 
{
      bool rit = false;
       if (digitalRead(NOISE_DIGITAL_PIN) == HIGH) 
       {                                                                                                      
          Serial.print("Sound detected ");                                       
          Serial.print("\n"); 
          rit = true;          
                                                                                  
          // Wait a short bit to avoid multiple detection of the same sound.      
          //delay(SOUND_DELAY);                                                     
      }   
  return rit;    
} 


void accendiCam() 
{
    mySwitch.send(1394001, 24);
       mySwitch.send(1394001, 24);
       mySwitch.send(1394001, 24);
       mySwitch.send(1394001, 24);
       mySwitch.send(1394001, 24);
} 




