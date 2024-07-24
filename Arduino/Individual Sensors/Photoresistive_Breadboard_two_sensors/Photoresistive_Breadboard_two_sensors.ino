/* Use a photoresistor (or photocell) to turn on an LED in the dark
   More info and circuit schematic: http://www.ardumotive.com/how-to-use-a-photoresistor-en.html
   Dev: Michalis Vasilakis // Date: 8/6/2015 // www.ardumotive.com */
   

//Constants
const int pResistor_inside = A0; // Photoresistor at Arduino analog pin A0
const int pResistor_outside = A1;
const int ledPin=30;       // Led pin at Arduino pin 9

//Variables
int value;
int value_inside;				  // Store value from photoresistor (0-1023)
int value_outside;				  // Store value from photoresistor (0-1023)

void setup(){
 Serial.begin(9600);
 Serial.println("Hello World, this two Photoresistors!");
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(pResistor_inside, INPUT);// Set pResistor - A0 pin as an in put (optional)
 pinMode(pResistor_outside, INPUT);// Set pResistor - A0 pin as an in put (optional)

}

void loop(){
  value_inside = analogRead(pResistor_inside);
  value_outside = analogRead(pResistor_outside);
  Serial.print("Value inside: ");
  Serial.println(value_inside);
  Serial.print("Value outside: ");
  Serial.println(value_outside);
 
  value = value_outside - value_inside;
  Serial.print("difference: ");
  Serial.println(value);

  //You can change value "25"
  if ((value_inside < 100) && (value > 100)){
    Serial.println("Laser can be turned on");
    digitalWrite(ledPin, HIGH); //Turn led on
  }
  else{
    digitalWrite(ledPin, LOW);  //Turn led off
  }

  delay(500); //Small delay
}