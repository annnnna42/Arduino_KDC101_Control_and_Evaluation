/* Use a photoresistor (or photocell) to turn on an LED in the dark
   More info and circuit schematic: http://www.ardumotive.com/how-to-use-a-photoresistor-en.html
   Dev: Michalis Vasilakis // Date: 8/6/2015 // www.ardumotive.com */
   

//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0

//Variables
int value;				  // Store value from photoresistor (0-1023)

void setup(){
  Serial.begin(9600);
  Serial.println("Hello World, this a a Photoresistor!");
 pinMode(LED_BUILTIN, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
}

void loop(){
  value = analogRead(pResistor);
  Serial.println(value);
  //You can change value "25"
  if (value > 500){
    digitalWrite(LED_BUILTIN, LOW);  //Turn led off
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH); //Turn led on
  }

  delay(500); //Small delay
}