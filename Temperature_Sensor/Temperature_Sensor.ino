const int  sensorPin = A0;
const float baselineTemp = 20.0;

void setup()
{
  Serial.begin(9600);
  
  for (int pinNumber = 4; pinNumber <= 6;pinNumber ++)
    {
      pinMode(pinNumber,OUTPUT);
      digitalWrite(pinNumber,LOW);
    }  
}

void loop ()
{
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor value: ");
  Serial.println(sensorVal);
  float voltage = (sensorVal/1024.0) *5.0;
  Serial.print("Voltage value: ");
  Serial.println(voltage);
  float temperature = (voltage - .5) *100;
  Serial.print("Temparature value: ");
  Serial.println(temperature);
}
