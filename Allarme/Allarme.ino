/*
 Ricezione dai sensori porte
 Trasmissione verso telecamera
 
 http://code.google.com/p/rc-switch/
 
 Need help? http://forum.ardumote.com
 */

#include <RCSwitch.h>
#include <Time.h>  
#include <SPI.h>
#include <Ethernet.h>

/* Informazioni Ethernet*/
byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
EthernetClient client;
char smtpServer[] = "smtpcorp.com";

RCSwitch mySwitch = RCSwitch();
int SERIAL_BAUD        = 9600; 
int TRASMIT_PIN       = 10;
int RECEIVE_PIN       = 10;
int NOISE_DIGITAL_PIN =    2; 
//Sensori
int PORTA_INGRESSO_SENSORE = 3557625;
int SEGNALE_ACCENZIONE_WEBCAM = 1394001;


void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(SERIAL_BAUD, INPUT);  
  mySwitch.enableReceive(RECEIVE_PIN);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(TRASMIT_PIN);  // Using Pin #10
}

void loop() {
  if (detectNoise()){
    Serial.print("Rumore");
	mail("Attenzione, rilevato rumore in casa!");
    accendiCam() ;
  }
  
  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } 
    else {

      if (mySwitch.getReceivedValue() == PORTA_INGRESSO_SENSORE) {
        Serial.print("Attenzione! Porta cucina aperta!");
        email("Attenzione, porta cucina aperta!");
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
	mySwitch.send(SEGNALE_ACCENZIONE_WEBCAM, 24);
    mySwitch.send(SEGNALE_ACCENZIONE_WEBCAM, 24);
    mySwitch.send(SEGNALE_ACCENZIONE_WEBCAM, 24);
    mySwitch.send(SEGNALE_ACCENZIONE_WEBCAM, 24);
    mySwitch.send(SEGNALE_ACCENZIONE_WEBCAM, 24);
} 

void setupComm()
{
  Serial.println("Trying to connect");
  if (!Ethernet.begin(mac)){
    Serial.println("Failed to DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  delay(1000);

  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println(); 
}

// Call to send an email. 
bool email(char* text)
{
  bool success = false;
  Serial.println("Sending email...");

  if (client.connect(smtpServer, 2525)){
    Serial.println("connected");
    delay(100);
    client.println("EHLO arduino"); 
    for(int i=0; i<999; ++i){
      if(client.read() > 0)
        break;
    } 
    Serial.println("responded");
    //http://arduino.cc/forum/index.php/topic,126829.0.html
    client.println("AUTH LOGIN");                 //see "http://base64-encoder-online.waraxe.us/"
    client.println("bGFmYWFkZWxjYW8xOTc1");           //Type kltan@pph.com.my and encode it
    client.println("bGFmYWFkZWxjYW8xOTc1");        

    // Put your "from" email address here
    client.println("MAIL FROM:<dumm@gmail.com>"); //Does not seem to matter what email stands here
    for(int i=0; i<999; ++i){ 
      if(client.read() > 0)      
        break;
    } 

    // Put where you are sending it here
    client.println("RCPT TO:<giuseppe.scola@gmail.com>"); //Must be the reciever ID

    for(int i=0; i<999; ++i){ 
      if(client.read() > 0) 
        break; 
    } 

    client.println("DATA"); 
    for(int i=0; i<999; ++i){
      if(client.read() > 0)
        break;
    } 
    //This is the email that is listed in the sender 
    client.println("from: giuseppe.scola@gmail.com"); //Sender adress posted in email
    client.println("to: giuseppe.scola@gmail.com");  //This is what is listed in the To head. Does not have to ne the real adress.
    client.println("SUBJECT: From your arduino"); 
    client.println("");
    client.println(text); 
    client.println("."); 
    client.println("QUIT"); 
    for(int i=0; i<999; ++i){
      if(i > 998){
        Serial.println("error: No response");
      }
      if(client.read() > 0)
        break;
    } 
    success = true;
    client.println();
    Serial.println("end");
  } 
  else {
    Serial.println("Failed");
    client.println("QUIT"); //attempt to cleanup the connection
  }
  client.stop();
  return success;
}


