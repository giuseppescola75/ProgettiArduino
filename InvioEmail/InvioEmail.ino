/*************************************************************
Arduino sketch by jake@coffshire.com
Corrected by Nicolaj.joergensen@gmail.com
*************************************************************/

#include <SPI.h>
#include <Ethernet.h>

// Arduino network information
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
EthernetClient client;
char smtpServer[] = "smtpcorp.com";

void setup()
{
  Serial.begin(9600);  // for debug
  setupComm();
}

void loop()
{
  email("ciao");
  delay(1000);
}

// Call to initialize the communication with the email server
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
      if(i > 998){Serial.println("error: No response");}
      if(client.read() > 0)
        break;
    } 
    success = true;
    client.println();
    Serial.println("end");
  } else {
    Serial.println("Failed");
    client.println("QUIT"); //attempt to cleanup the connection
  }
  client.stop();
  return success;
}

