import processing.serial.*;

Serial myPort; 

void setup() {
   // List all the available serial ports:
  println(Serial.list());
 
  // You may need to change the number in [ ] to match 
  // the correct port for your system
  myPort = new Serial(this, Serial.list()[1], 9600);
              }
 
void draw() {
  while (myPort.available() > 0) {
    int inByte = myPort.read();
    println(inByte);
  }
}
