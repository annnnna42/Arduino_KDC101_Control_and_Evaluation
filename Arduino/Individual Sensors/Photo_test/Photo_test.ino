int sensorPin = A10;
int sensorValue = 0;

void setup(void) {
   Serial.begin(9600);
   pinMode(sensorPin, INPUT); 
}
 
void loop(void) {
   //sensorValue = analogRead(sensorPin);
   Serial.print(volt(A10));
   Serial.println(" Volt");
   delay(500);
}

float volt(int adPin)
{
  return float(analogRead(adPin)) * 5.0/1024;
}
