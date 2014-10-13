/** FC 04**/

int SERIAL_BAUD        = 9600;                                                  
int SENSOR_DIGITAL_PIN =    2;                                                  
int SOUND_DELAY        =   50; /* Delay to avoid duplicate detection */ 


void setup() {                                                                  
    Serial.begin(SERIAL_BAUD);                                                  
    pinMode(SERIAL_BAUD, INPUT);                                                
}                                                                               
                                                                                
void loop() {
    int count = 0;  
    if (digitalRead(SENSOR_DIGITAL_PIN) == HIGH) {                               
                                                          
        count++;                                                                         
        Serial.print("Sound detected " + count);                                       
        Serial.print("\n");                                              
                                                                                
        // Wait a short bit to avoid multiple detection of the same sound.      
        delay(SOUND_DELAY);                                                     
    }                                                                           
}        
