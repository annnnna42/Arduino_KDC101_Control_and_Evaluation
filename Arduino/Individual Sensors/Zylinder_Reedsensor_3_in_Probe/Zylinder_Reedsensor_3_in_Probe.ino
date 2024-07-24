// constants won't change. They're used here to set pin numbers:
const int SENSOR_PIN0 = 24; // the Arduino's input pin that connects to the sensor's SIGNAL pin
const int SENSOR_PIN1 = 26; // the Arduino's input pin that connects to the sensor's SIGNAL pin
const int SENSOR_PIN2 = 28; // the Arduino's input pin that connects to the sensor's SIGNAL pin
const int LED_PIN0 = 30; 
const int LED_PIN1 = 31; 
const int LED_PIN2 = 32; 
int value0;
int value1;
int value2;


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Hello World, this is a reed switch!");

  pinMode(SENSOR_PIN0, INPUT);
  pinMode(SENSOR_PIN1, INPUT);
  pinMode(SENSOR_PIN2, INPUT);
  pinMode(LED_PIN0, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
}

void loop() {
  value0 = digitalRead(SENSOR_PIN0);
  value1 = digitalRead(SENSOR_PIN1);
  value2 = digitalRead(SENSOR_PIN2);

  Serial.print(value0);
  Serial.print(",");
  Serial.print(value1);
  Serial.print(",");
  Serial.print(value2);
  Serial.println(" ");


  if (value0 < 80){
    digitalWrite(LED_PIN0, LOW);  //Turn led off
  }
  else{
    digitalWrite(LED_PIN0, HIGH); //Turn led on
  }

  if (value1 < 120){
    digitalWrite(LED_PIN1, LOW);  //Turn led off
  }
  else{
    digitalWrite(LED_PIN1, HIGH); //Turn led on
  }

  if (value2 < 80){
    digitalWrite(LED_PIN2, LOW);  //Turn led off
  }
  else{
    digitalWrite(LED_PIN2, HIGH); //Turn led on
  }

  delay(100); //Small delay

}