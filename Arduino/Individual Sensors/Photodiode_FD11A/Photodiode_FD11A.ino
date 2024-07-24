int sensorPin = A10;
int sensorValue = 0;

void setup(void) {
   Serial.begin(9600);
   pinMode(sensorPin, INPUT); 
}
 
void loop(void) {
   sensorValue = analogRead(sensorPin);
   //sensorValue = digitalRead(sensorPin);

   Serial.println(sensorValue);
   delay(100);
}
