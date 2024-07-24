//#define PIN_MEASURE0 A0
//#define PIN_MEASURE1 A1
#define PIN_BUTTON   2

#define V_IN   5.0   // source voltage in Volt
#define REF_R2 22000  // value of the known resistor in Ohm

const int PIN_MEASURE0 = A0;
const int PIN_MEASURE1 = A1;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_MEASURE0, INPUT);
	pinMode(PIN_MEASURE1, INPUT);
	//pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.println("Hello Resistor");
}

void loop()
{
  Serial.println("Pin0");
  measureResistance(PIN_MEASURE0);
  delay(500);
  Serial.println("Pin1");
  measureResistance(PIN_MEASURE1);
  delay(500);

	//if (digitalRead(PIN_BUTTON) == 0) {
	//	measureResistance();
	//	delay(500);
	//}
}

void measureResistance(int pin)
{
	// measure 10 times and calculate the average
	long reading = 0;
	for (byte i = 0; i < 10; i++) {
		reading += analogRead(pin);
	}
	reading = (long)(reading / 10);

	// calculate voltage
	float voltageR1 = (V_IN / 1023.0) * reading;
	Serial.print("Voltage over R1: ");
	Serial.print(voltageR1, 2);
	Serial.println(" V");

	// formula: R1 = R2 * (U2/U1)
	int resistance = REF_R2 * (voltageR1 / (V_IN - voltageR1));
	Serial.println("Resistor R1: " + String(resistance) + " Ω\n\n");
}


/*
//#define PIN_MEASURE0 A0
//#define PIN_MEASURE1 A1
#define PIN_BUTTON   2

#define V_IN   5.0   // source voltage in Volt
#define REF_R2 22000  // value of the known resistor in Ohm

const int PIN_MEASURE0 = A0;
const int PIN_MEASURE1 = A1;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_MEASURE0, INPUT);
	pinMode(PIN_MEASURE1, INPUT);
	//pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.println("Hello Resistor");
}

void loop()
{
  measureResistance();
	//if (digitalRead(PIN_BUTTON) == 0) {
	//	measureResistance();
	//	delay(500);
	//}
  delay(500);
}

void measureResistance()
{
	// measure 10 times and calculate the average
	long reading = 0;
	for (byte i = 0; i < 10; i++) {
		reading += analogRead(PIN_MEASURE0);
	}
	reading = (long)(reading / 10);

	// calculate voltage
	float voltageR1 = (V_IN / 1023.0) * reading;
	Serial.print("Voltage over R1: ");
	Serial.print(voltageR1, 2);
	Serial.println(" V");

	// formula: R1 = R2 * (U2/U1)
	int resistance = REF_R2 * (voltageR1 / (V_IN - voltageR1));
	Serial.println("Resistor R1: " + String(resistance) + " Ω\n\n");
}
*/