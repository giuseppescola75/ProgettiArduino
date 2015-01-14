/*
 Ricezione dai sensori porte
 Trasmissione verso telecamera
 rivelazione rumore
 
 http://code.google.com/p/rc-switch/
 
 Need help? http://forum.ardumote.com
 */

#include <String.h>
#include <RCSwitch.h>
#include <Time.h>  
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h> 

/* Informazioni Ethernet*/

byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
int SERIAL_BAUD        = 9600; 
int TRASMIT_PIN       = 9;
int RECEIVE_PIN       = 0;
int NOISE_DIGITAL_PIN =    5; 
//Sensori
//long PORTA_INGRESSO_SENSORE  = 3557625;
//long PORTA_CUCINA_SENSORE = 10521177;
long SEGNALE_ACCENZIONE_WEBCAM = 1394001;
long SEGNALE_SPEGNIMENTO_WEBCAM= 1394004;
int nSensori = 5;

EthernetClient client;
EthernetServer server(8081);
char smtpServer[] = "smtpcorp.com";
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(SERIAL_BAUD, INPUT);  
  mySwitch.enableReceive(RECEIVE_PIN);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(TRASMIT_PIN);  // Using Pin #10
  setupComm();
}

void loop() {
  getClientConnection();
  //Serial.print(detectNoise());
  /*if (detectNoise()){
   Serial.print("Rumore");
   email("Attenzione, rilevato rumore in casa!");
   accendiCam(SEGNALE_ACCENZIONE_WEBCAM) ;
   }*/

  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    //Serial.print(value); 
    if (value == 0) {
      Serial.print("Unknown encoding");
      Serial.print("\n"); 
    } 
    else {
      long receivedValue = mySwitch.getReceivedValue();
      int isPresente = isPresenteSensore(receivedValue);
      if (isPresente != -1)
      {
        //Trovato codice
        String messaggioRilevazione = "Attenzione! Sensore " +String(isPresente)+ " rilevato!";
        Serial.println(messaggioRilevazione);
        email(messaggioRilevazione);
        accendiCam(SEGNALE_ACCENZIONE_WEBCAM) ;
        delay(1000); 
      }
      /*
      if (receivedValue == PORTA_INGRESSO_SENSORE) {
       Serial.print("Attenzione! Porta ingresso aperta!");
       Serial.print("\n"); 
       email("Attenzione, porta ingresso aperta!");
       accendiCam(SEGNALE_ACCENZIONE_WEBCAM) ;
       delay(1000); 
       }
       else if(receivedValue == PORTA_CUCINA_SENSORE) {
       Serial.print("Attenzione! Porta cucina aperta!");
       Serial.print("\n"); 
       email("Attenzione, porta cucina aperta!");
       accendiCam(SEGNALE_ACCENZIONE_WEBCAM) ;
       delay(1000); 
       } */

      /*Serial.print("Received ");
       Serial.print( receivedValue);
       Serial.print(" / ");
       Serial.print( mySwitch.getReceivedBitlength() );
       Serial.print("bit ");
       Serial.print("Protocol: ");
       Serial.println( mySwitch.getReceivedProtocol() );*/
    }
    //mySwitch.resetAvailable();
  }
}


bool detectNoise () 
{
  bool rit = false;
  if (digitalRead(NOISE_DIGITAL_PIN) == HIGH) 
  {                                                                                                      
    rit = true;          

    // Wait a short bit to avoid multiple detection of the same sound.      
    //delay(SOUND_DELAY);                                                     
  }   
  return rit;    
} 


void accendiCam(long value) 
{
  //Serial.print("accendiCam");
  //Serial.print("\n"); 
  mySwitch.send(value, 24);
  mySwitch.send(value, 24);
  mySwitch.send(value, 24);
  mySwitch.send(value, 24);
  mySwitch.send(value, 24);
} 

void setupComm()
{
  Serial.println("Trying to connect");
  Serial.print("\n"); 
  if (!Ethernet.begin(mac)){
    Serial.println("Failed to DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }

  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println("fine"); 
}

// Call to send an email. 
bool email(String text)
{
  bool success = false;
  Serial.println("Sending email...");
  Serial.print("\n"); 
  Serial.println("SMTP server...");
  Serial.print(smtpServer); 

  if (client.connect(smtpServer, 2525)){
    Serial.println("connected");
    delay(100);
    client.println("EHLO arduino"); 
    for(int i=0; i<999; ++i){
      if(client.read() > 0)
        break;
    } 
    Serial.println("responded");
    Serial.print("\n"); 
    //http://arduino.cc/forum/index.php/topic,126829.0.html
    client.println("AUTH LOGIN");                 //see "http://base64-encoder-online.waraxe.us/"
    client.println("bGFmYWFkZWxjYW8xOTc1");           //Type kltan@pph.com.my and encode it
    client.println("c3RhcndhcnMxOTc1");        

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
    client.println("SUBJECT: ArduAlarm"); 
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
    Serial.print("\n"); 
  } 
  else {
    Serial.println("Failed");
    Serial.print("\n"); 
    client.println("QUIT"); //attempt to cleanup the connection
  }
  client.stop();
  return success;
}


void getClientConnection(){

  EthernetClient client = server.available();
  if (client) {
    String postString ="";
    Serial.println("nuova richiesta");

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.println(c);
        //postString.concat(c); 
        // Serial.println("=========");
        //Serial.println(postString.length());
        //Serial.println("=========");
        if(postString.length()<20){
          Serial.println(c);
          postString +=c;
        }
        //Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          //if(readString.indexOf("id=1") > 0){ 
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close"));  // the connection will be closed after completion of the response          
          client.println();
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html>"));          
          client.println(F("<h1>AllarDuino</h1>"));
          client.print(F("<br>"));
          client.println(F("<a href=\"./?on\">Accendi CAM</a>"));
          client.println(F("<a href=\"./?off\">Spegni CAM</a>"));
          client.print(F("<br>"));
          client.println(F("<h2>Configurazione</h2>"));
          client.print(F("<br>"));
          char linkCompleto[50];
          for (int i=0; i<nSensori; i++)
          {          
            strcpy (linkCompleto, "<a href=\"./?save");            //copio la stringa in array char
            sprintf(linkCompleto, "%s%d", linkCompleto, i);        //concateno int a stringa
            strcat(linkCompleto, "\">Salva Sensore ");            //concateno due stringhe
            sprintf(linkCompleto, "%s%d", linkCompleto, i);    
            strcat(linkCompleto, "</a><br/><br/>");

            client.println(linkCompleto);
            //Serial.println();            
          }
          client.println(F("<a href=\"./?elenco\">dati sensori</a></html>"));
          break;
          //}  
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }   
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }  //fine client.connected 

    //Gestione querystring

    if(postString.indexOf("?on") > 0){ 
      Serial.println("accendi CAM"); 
      accendiCam(SEGNALE_ACCENZIONE_WEBCAM); 
      client.println("<br/>");
      client.println("<p>Cam accesa</p>");
    }
    if(postString.indexOf("?off") > 0){ 
      accendiCam(SEGNALE_SPEGNIMENTO_WEBCAM); 
      client.println("<br/>");
      client.println("<p>Cam spenta</p>");
    } 
    if(postString.indexOf("?save") > 0){ 
      int indexSave = postString.indexOf("?save");
      Serial.println("indexOf");
      Serial.println(indexSave);
      Serial.println("=====");
      //cerco valore del sensore
      String sSensore = postString.substring(indexSave+5 ,indexSave+6);
      Serial.println(sSensore);
      int iSensore = sSensore.toInt();
      long valore = salvaSensore(iSensore);
      client.println("<br/>");
      client.println("<p>Salvataggio sensore effettuato</p>");
      client.println("<br/>");
      client.println("Codice sensore " + sSensore);
      Serial.println("=====");
      client.print(valore);
    }
    //Serial.println(postString);
    if(postString.indexOf("?elenco") > 0){ 
      //Serial.println("elenco");
      for (int i=0; i<nSensori; i++)
      {
        Serial.println("<p>Sensore " + String(i)+" : </p>");
        client.println("<p>Sensore " + String(i)+" : </p>");
        client.print(String(EEPROMReadlong(i*4)));
        Serial.println(EEPROMReadlong(i*4));
      }   
    }
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected"); 
  }      
}

long salvaSensore(int iSensore)
{
  int addressTosSave = iSensore*4;
  long valore = 0;
  Serial.println("salvaSensore");
  if (mySwitch.available()) {
    Serial.println("mySwitch.available");
    valore = mySwitch.getReceivedValue();
    Serial.println("valore ");
    Serial.println(valore);
    EEPROMWritelong(addressTosSave,valore);
  } 
  delay(1000);
  Serial.println(valore);
  return valore;
} 

void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

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

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

//Se trova sensore ritorna il numero del sensore
//Se non lo trova ritorna -1
int isPresenteSensore(long valoreRicevuto)
{
  for (int i=0; i<nSensori; i++)
  {
    long valoreRegistrato = EEPROMReadlong(i*4);
    if (valoreRegistrato == valoreRicevuto)
    {
      return i;  
    }
    return -1;
  } 
}












