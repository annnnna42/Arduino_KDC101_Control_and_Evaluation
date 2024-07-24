// constants won't change. They're used here to set pin numbers:
const int SENSOR_PIN = A0; // the Arduino's input pin that connects to the sensor's SIGNAL pin
const int LED_PIN = 30; 
int value;

// Variables will change:
//int lastState = LOW;      // the previous state from the input pin
//int currentState;         // the current reading from the input pin

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Hello World, this is a reed switch!");
  // initialize the Arduino's pin as aninput
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  value = analogRead(SENSOR_PIN);
  // read the state of the the input pin:
  Serial.println(value);

  //You can change value "25"
  if (value < 80){
    digitalWrite(LED_PIN, LOW);  //Turn led off
  }
  else{
    digitalWrite(LED_PIN, HIGH); //Turn led on
  }

 // delay(100); //Small delay

}