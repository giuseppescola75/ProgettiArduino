/*
 Provo a scrivere un long e a leggerlo nella eeprom
 */

#include <EEPROM.h> 
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

}

// the loop routine runs over and over again forever:
void loop() {
  delay(5000);
  
  long sensore1= 3557625;
  long sensore2= 10521177;
  long sensore3 = 1398036;
  long address=0;
  EEPROMWritelong(address, sensore1);
  Serial.println("Scrittura 1 riuscita");
  address+=4;
  EEPROMWritelong(address, sensore2);
  Serial.println("Scrittura 2 riuscita");
  address+=4;
  EEPROMWritelong(address, sensore3);
  Serial.println("Scrittura 3 riuscita");
  delay(5000);
  address=0;
  Serial.println("Sensore 1");
  Serial.println(EEPROMReadlong(address));
  Serial.println("Sensore 2");
  address+=4;
  Serial.println(EEPROMReadlong(address));
  Serial.println("Sensore 3");
  address+=4;
  Serial.println(EEPROMReadlong(address));
  delay(5000);
}

void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

Serial.println(four);
Serial.println(three);
Serial.println(two);
Serial.println(one);
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

Serial.println(four);
Serial.println(three);
Serial.println(two);
Serial.println(one);
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}




