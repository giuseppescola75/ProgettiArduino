#include <Wire.h>
#include <String.h>
#include "RTClib.h"

#define STATE_OFF  0
#define STATE_ON   1

RTC_DS1307 rtc;
int fsm_state;
//Giornate di programmazione
char *lunedi[] = { 
  "10:00", "10:35","12:10","15:00"};
char *martedi[] = { 
  "11:00", "11:35","13:10","13:50"};

void setup () {
  fsm_state = STATE_OFF;
  Serial.begin(57600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {        
    Serial.println("RTC is NOT running!");
    //setto la data prendendola da internet!! con temboo (https://www.temboo.com/library/Library/Utilities/Dates/GetDate/)
    String DataRecuperata = "2015-03-11 16:02:06";

    //rtc.adjust(DateTime(DataRecuperata));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:    
    rtc.adjust(DateTime(DataRecuperata.substring(0, 3).toInt() , DataRecuperata.substring(5, 6).toInt() , DataRecuperata.substring(8, 9).toInt() , DataRecuperata.substring(11, 12).toInt() , DataRecuperata.substring(14, 15).toInt() , DataRecuperata.substring(17, 18).toInt() ));
  }
}

void loop () {
  // Ricavo il time attuale
  DateTime now = rtc.now();
  int giorno = now.dayOfWeek();      //0 = lunedi 1 = martedi 2 = mercoledi ....
  if (giorno == 1){    //lunedi

  }

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
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // calculate a date which is 7 days and 30 seconds into the future
  DateTime future (now.unixtime() + 7 * 86400L + 30);

  Serial.print(" now + 7d + 30s: ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.println();
  delay(3000);
}


bool setStatus(String START_TIME, String END_TIME)
{
  switch(fsm_state) {
  case STATE_OFF:
    if(now > START_TIME && now < END_TIME) {      
        fsm_state = STATE_ON;
    }
    break;
  case STATE_ON:
    if(now > END_TIME) {
        fsm_state = STATE_OFF;
    }
    break;
  }
}




