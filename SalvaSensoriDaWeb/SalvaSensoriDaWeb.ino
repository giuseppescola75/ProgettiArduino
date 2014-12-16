#include <String.h>
#include <Time.h>  
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h> 
#include <RCSwitch.h>

byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
int SERIAL_BAUD        = 9600; 
EthernetServer server(8081);
RCSwitch mySwitch = RCSwitch();
int RECEIVE_PIN       = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  setupComm();
  mySwitch.enableReceive(RECEIVE_PIN); 
}

void loop() {
  getClientConnection();
  if (mySwitch.available()) {
    long receivedValue = mySwitch.getReceivedValue();
    /*Serial.println("sensore trovato in loop");
    Serial.print(receivedValue);*/
  }
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
        //postString.concat(c); 
        if(postString.length()<15){
          postString +=c;
        }

        if (c == '\n' && currentLineIsBlank) {
          //if(readString.indexOf("id=1") > 0){ 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //client.println("<h1>Settaggi</h1><br>");
          client.println("<h1>Configurazione</h1>");
          client.print("<br>");
          client.println("<a href=\"./?save1\">Salva Sensore 1</a>");
          client.println("<a href=\"./?save2\">Salva Sensore 2</a>");
          client.println("<br/>");
          client.println("<a href=\"./?elenco\">Visualizza dati sensori</a>");
          client.println("</html>");
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

    Serial.println("-------------");
    Serial.println(postString);
    Serial.println("-------------");

    if(postString.indexOf("?save1") > 0){ 
      long valore = salvaSensore(0);
      client.println("<br/>");
      client.println("<p>Salvataggio sensore 1 effettuato</p>");
      client.println("<br/>");
      client.println("Codice sensore 1:" );
      client.print(valore);
    }

    if(postString.indexOf("?save2") > 0){ 
      long valore = salvaSensore(4);
      client.println("<br/>");
      client.println("<p>Salvataggio sensore 2 effettuato</p>");
      client.println("<br/>");
      client.println("Codice sensore 2:");
      client.print(valore);
    } 

    if(postString.indexOf("?elenco") > 0){ 
      client.println("<p>Sensore 1 : </p>");
      client.print( EEPROMReadlong(0));
      client.println("<p>Sensore 2 : </p>");
      client.print( EEPROMReadlong(4));
    }

    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected"); 
  }
}

long salvaSensore(int addressTosSave)
{
  long valore = 0;
  Serial.println("salvaSensore");
  if (mySwitch.available()) {
    Serial.println("mySwitch.available");
    valore = mySwitch.getReceivedValue();
    Serial.println("valore ");
    Serial.print(valore);
    EEPROMWritelong(addressTosSave,valore);
  } 
  delay(1000);
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














