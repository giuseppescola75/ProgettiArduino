const int greenLEDPin = 11;
const int redLEDPin = 10;
const int blueLEDPin = 9;


const int redSensorPin = A0;
const int greenSensorPin = A1;
const int blueSensorPin = A2;


int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int redSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;

void setup(){
  Serial.begin(9600);
  
  pinMode(greenLEDPin,OUTPUT);
  pinMode(redLEDPin,OUTPUT);
  pinMode(blueLEDPin,OUTPUT);
  }
  
  void loop(){
    redSensorValue = analogRead(redSensorPin);
    greenSensorValue = analogRead(greenSensorPin);
    blueSensorValue = analogRead(blueSensorPin);
    
    Serial.print("Red : \t");
    Serial.println(redSensorValue);
    
    Serial.print("Green : \t");
    Serial.println(greenSensorValue);
    
    Serial.print("Blue : \t");
    Serial.println(blueSensorValue);
    
    redValue= redSensorValue / 4;
    greenValue= greenSensorPin / 4;
    blueValue= blueSensorValue / 4;
    
    analogWrite(redLEDPin,redValue);
    analogWrite(greenLEDPin,greenValue);
    analogWrite(blueLEDPin,blueValue);
  }  
  
  

