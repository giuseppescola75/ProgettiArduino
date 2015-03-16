//#include <SPI.h>
//#include <Dhcp.h>
//#include <Dns.h>
#include <Wire.h>
#include <String.h>
#include "RTClib.h"
//#include <Ethernet.h>
//#include <EthernetClient.h>
//#include <Temboo.h>
//#include "TembooAccount.h" // Contains Temboo account information

#define BUFFER_SIZE 20
#define STATE_OFF  0
#define STATE_ON   1

// Buffer for incoming data
char serial_buffer[BUFFER_SIZE];
int buffer_position;


//Giornate di programmazione
const int numeroProgrammazioni = 4;
String lunedi[numeroProgrammazioni] = { 
  "20:35", "20:30","20:35","20:40"};
String martedi[numeroProgrammazioni] = {  
  "11:00", "11:35","13:10","13:50"};
String mercoledi[numeroProgrammazioni] = {  
  "10:00", "10:35","12:10","15:00"};
String giovedi[numeroProgrammazioni] = {  
  "10:00", "10:35","17:10","20:00"};
String venerdi[numeroProgrammazioni] = {  
  "10:00", "10:35","14:10","15:00"};
String sabato[numeroProgrammazioni] = {  
  "21:40", "21:42","21:45","21:4"};
String domenica[numeroProgrammazioni] = {  
  "10:00", "10:35","12:10","15:00"};
bool isRandom= false;
//Fine giornate di programmazione

RTC_DS1307 rtc;
int fsm_state;
String START_TIME;
String END_TIME;  
int nSize = numeroProgrammazioni / 2;
String giornoTemp[numeroProgrammazioni];

void setup () {
  Serial.println("Inizio Setup");
  fsm_state = STATE_OFF;
  Serial.begin(57600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  buffer_position = 0;
  rtc.begin();

  if (! rtc.isrunning()) {        
    Serial.println("RTC is NOT running!");
    //setto la data prendendola da internet!! con temboo (https://www.temboo.com/library/Library/Utilities/Dates/GetDate/)
    // o la prendo da GUI
    String DataRecuperata = "2015-03-11 16:02:06";

    //rtc.adjust(DateTime(DataRecuperata));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:    
    //    rtc.adjust(DateTime(DataRecuperata.substring(0, 3).toInt() , DataRecuperata.substring(5, 6).toInt() , DataRecuperata.substring(8, 9).toInt() , DataRecuperata.substring(11, 12).toInt() , DataRecuperata.substring(14, 15).toInt() , DataRecuperata.substring(17, 18).toInt() ));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop () {
  if (Serial.available() > 0) {
    // Read the incoming character
    char incoming_char = Serial.read();

    // End of line?
    if(incoming_char == '\n') {
      if(serial_buffer[0] == '#' && serial_buffer[1] == 'T') {

        String time_string = String(serial_buffer);
        int day = time_string.substring(2, 4).toInt();
        int month = time_string.substring(4, 6).toInt();        
        int year = time_string.substring(6, 10).toInt();
        int hour = time_string.substring(10, 12).toInt();
        int minute = time_string.substring(12, 14).toInt();
        int second = time_string.substring(14, 16).toInt();
        DateTime set_time = DateTime(year, month, day, hour, minute, second);
        rtc.adjust(set_time);
        Serial.println("OK");
      }
      else if(serial_buffer[0] == '#' && serial_buffer[1] == 'P') {
        String time_string = String(serial_buffer);
        //Ricevo da GUI per programmazione
        //#P01000110015001900    è lunedi dalle 10 alle 11 dalle 15 alle 19
        //#P11000110015001900    è martedi dalle 10 alle 11 dalle 15 alle 19
        settaProgrammazione(time_string);
      }
      else if(serial_buffer[0] == '#' && serial_buffer[1] == 'P') {
        //Ritorna programmazione alla GUI
        Serial.println("P11000110015001900");      //inviata la programmazione del martedi alla GUI
      }

      // Reset the buffer
      buffer_position = 0;
    }
    // Carriage return, do nothing
    else if(incoming_char == '\r');

    // Normal character
    else {

      // Buffer full, we need to reset it
      if(buffer_position == BUFFER_SIZE - 1) buffer_position = 0;

      // Store the character in the buffer and move the index
      serial_buffer[buffer_position] = incoming_char;
      buffer_position++;      
    }
  } // Fine serial avaible


  // Ricavo il time attuale
  DateTime now = rtc.now();
  /*Serial.println("data attuale");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();*/

  int giorno = now.dayOfWeek();      //0 = lunedi 1 = martedi 2 = mercoledi ....
  Serial.println( giorno);
  if (giorno == 1){                  //lunedi
    memcpy(giornoTemp,lunedi, sizeof(lunedi));
  }
  else if (giorno == 2){            //martedi
    memcpy(giornoTemp,martedi, sizeof(martedi));  
  }
  else if (giorno == 3){            //mercoledi
    memcpy(giornoTemp,mercoledi, sizeof(mercoledi));
  }
  else if (giorno == 4){            //giovedi
    memcpy(giornoTemp,giovedi, sizeof(giovedi));
  }
  else if (giorno == 5){            //venerdi
    memcpy(giornoTemp,venerdi, sizeof(venerdi));
  }
  else if (giorno == 6){            //sabato
    memcpy(giornoTemp,sabato, sizeof(sabato));
  }
  else if (giorno == 7){            //domenica
    memcpy(giornoTemp,domenica, sizeof(domenica));
  }



  //verifico lo stato che dovrà assumere 
  bool isON = false;
  for (int i= 0; i< nSize;i++){
    //Serial.println("giornoTemp");
    //Serial.println(giornoTemp[i]);
    START_TIME = giornoTemp[i*2];
    END_TIME = giornoTemp[(i*2)+1];   
    isON = setStatus(now,START_TIME,END_TIME);

    if (isON == true)
    {
      Serial.println("Accendo"); 
      //Accendo quello che voglio fare
      gestisciCarico();
      break;
    }
    else{
      Serial.println("Spengo");
    }
  }
  delay(3000);
}


bool setStatus(DateTime timeOra, String START_TIME, String END_TIME)
{
  //recupero l'ora 
  DateTime dSTART_TIME = DateTime(timeOra.year(),timeOra.month(),timeOra.day(),START_TIME.substring(0, 2).toInt(),START_TIME.substring(3, 5).toInt(),0);
  DateTime dEND_TIME = DateTime(timeOra.year(),timeOra.month(),timeOra.day(),END_TIME.substring(0, 1).toInt(),END_TIME.substring(3, 5).toInt(),0);
  long lSTART_TIME = dSTART_TIME.unixtime();
  long lEND_TIME = dEND_TIME.unixtime();


 Serial.println("timeOra");
  Serial.print(timeOra.year(), DEC);
  Serial.print('/');
  Serial.print(timeOra.month(), DEC);
  Serial.print('/');
  Serial.print(timeOra.day(), DEC);
  Serial.print(' ');
  Serial.print(timeOra.hour(), DEC);
  Serial.print(':');
  Serial.print(timeOra.minute(), DEC);
  Serial.print(':');
  Serial.print(timeOra.second(), DEC);
  Serial.println();
  Serial.println("timeOra FINE");


  Serial.println("dSTART_TIME");
  Serial.print(dSTART_TIME.year(), DEC);
  Serial.print('/');
  Serial.print(dSTART_TIME.month(), DEC);
  Serial.print('/');
  Serial.print(dSTART_TIME.day(), DEC);
  Serial.print(' ');
  Serial.print(dSTART_TIME.hour(), DEC);
  Serial.print(':');
  Serial.print(dSTART_TIME.minute(), DEC);
  Serial.print(':');
  Serial.print(dSTART_TIME.second(), DEC);
  Serial.println();
  Serial.println("dSTART_TIME FINE");

  Serial.println();
  Serial.println("dSTART_TIME UNIXTIME");
  Serial.println("dSTART_TIME UNIXTIME");

  if (isRandom == true)
  {
    long casuale = random(0, 20);
    //TODO: aggiungere a dSTART_TIME e dEND_TIME
    lSTART_TIME = dSTART_TIME.unixtime() + casuale;
    lEND_TIME = dEND_TIME.unixtime() + casuale;
  }
  switch(fsm_state) {
  case STATE_OFF:
    if(timeOra.unixtime() > lSTART_TIME && timeOra.unixtime() < lEND_TIME) {      
      fsm_state = STATE_ON;
    }
    break;
  case STATE_ON:
    if(timeOra.unixtime() > lEND_TIME) {
      fsm_state = STATE_OFF;
    }
    break;
  }
}

bool settaProgrammazione(String stringa)
{
  //P01000110015001900
  char giornoSettimana = stringa.charAt(1);  //0= lunedi   1 = Martedi 
  if (giornoSettimana == '0')    //Lunedi
  {
    lunedi[0] = stringa.substring(2,3) + ":" + stringa.substring(4,5);
    lunedi[1] = stringa.substring(6,7) + ":" + stringa.substring(8,9);
    lunedi[2] = stringa.substring(10,11) + ":" + stringa.substring(12,13);
    lunedi[3] = stringa.substring(14,15) + ":" + stringa.substring(16,17);
  }
}

void gestisciCarico()
{
}




















