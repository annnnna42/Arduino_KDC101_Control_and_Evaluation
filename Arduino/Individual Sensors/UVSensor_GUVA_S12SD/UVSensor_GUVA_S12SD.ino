void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");

}

void loop() {
  // put your main code here, to run repeatedly:
  float sensorVoltage;
  float sensorValue;

  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*5;
  Serial.print("sensor reading + ");
  Serial.print(sensorValue);
  Serial.println("");
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  delay(1000);
}

