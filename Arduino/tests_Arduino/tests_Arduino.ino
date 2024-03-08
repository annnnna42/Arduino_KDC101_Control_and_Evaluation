/*

int sensor1 = A4;
int sensor2 = A5;
int sensor3 = A6;

int const trig1 = 53;
int const trig2 = 52;

byte trig1_state = LOW;
byte trig2_state = LOW;

//globals
int data1;
int data2;
int data3;
int freq = 2000;                  // [ms]

unsigned long curr_ms = 0;
unsigned long prev_ms = 0;

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
}

void loop(){
  /*
  digitalWrite(trig1, HIGH);
  digitalWrite(trig2, HIGH);
  Serial.println("High");
  delay(2000);
  digitalWrite(trig1, LOW);
  digitalWrite(trig2, LOW);
  Serial.println("Low");
  delay(2000);
// 
  
  curr_ms = millis();
  //toggle_trig1();
  toggle_trig2();
  write_states(); 
  
}

void write_states(){
  //Serial.println(trig2_state);
  digitalWrite(trig2, trig2_state);
}

void toggle_trig1(){
  switch(trig1_state){
    case LOW:
      if (curr_ms - prev_ms >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        read_and_print_sensors();
        trig1_state = HIGH;
        Serial.println("HIGH");
        prev_ms = curr_ms;
      }

    case HIGH:
      if (curr_ms - prev_ms >= freq/2){   
        trig1_state = LOW;
        Serial.println("LOW");
        prev_ms = curr_ms;
      }
  }
}

void toggle_trig2(){
  switch(trig2_state){
    case LOW:
      if (curr_ms - prev_ms >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        trig2_state = HIGH;
        Serial.println("HIGH");
        prev_ms = curr_ms;
      }

    case HIGH:
      if (curr_ms - prev_ms >= freq/2){   
        trig2_state = LOW;
        Serial.println("LOW");
        prev_ms = curr_ms;
      }
  }
}

void read_and_print_sensors(){
  data1 = analogRead(sensor1);
  data2 = analogRead(sensor2);
  data3 = analogRead(sensor3);

    //Display Data to Serial Monitor
    /*
  Serial.print(data1);
  Serial.print(",");
  Serial.print(data2);    
  Serial.print(",");
  Serial.println(data3);
  
}

*/

#include <Wire.h>
 
 
void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}
 
 
void loop()
{
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}


