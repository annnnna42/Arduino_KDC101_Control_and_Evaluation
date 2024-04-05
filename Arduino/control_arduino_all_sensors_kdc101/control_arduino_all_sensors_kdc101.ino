// Constants
int const REED_front_0 = A2;
int const REED_front_1 = A3;
int const REED_back_0 = A4;
int const REED_back_1 = A5;
int const REED_back_2 = A6;
int const PD_inside = A10;
int const PD_outside = A11;

int const LED_red = 40;
int const trig1 = 51;
int const trig2 = 52;

int freq = 200;

// Variables
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_REED_back_2;
int val_PD_inside;
int val_PD_outside;

// Inits
byte trig2_state = LOW;
unsigned long curr_ms = 0;
unsigned long prev_ms_2 = 0;

void setup(){
  //Serial.setTimeout(2); //Milliseconds
  Serial.begin(115200);   //9600, 19200, 115200
  delay(500);

  pinMode(REED_front_0, INPUT);
  pinMode(REED_front_1, INPUT);
  pinMode(REED_back_0, INPUT);
  pinMode(REED_back_1, INPUT);
  pinMode(REED_back_2, INPUT);
  pinMode(PD_inside, INPUT);
  pinMode(PD_outside, INPUT);

  pinMode(LED_red, OUTPUT);
  pinMode(trig2, OUTPUT);

  digitalWrite(LED_red, HIGH);

}
void loop(){
  curr_ms = millis();
  toggle_trig2();
  write_states();   
}

void write_states(){
  digitalWrite(trig2, trig2_state);
}


void toggle_trig2(){
  switch(trig2_state){
    case LOW:
      if (curr_ms - prev_ms_2 >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        trig2_state = HIGH;
        read_and_print_sensors();
        prev_ms_2 = curr_ms;
      }
    case HIGH:
      if (curr_ms - prev_ms_2>= freq/2){   
        trig2_state = LOW;
        prev_ms_2 = curr_ms;
      }
  }
}


void read_and_print_sensors(){
  val_REED_front_0 = analogRead(REED_front_0);
  val_REED_front_1 = analogRead(REED_front_1);  
  val_REED_back_0 = analogRead(REED_back_0);
  val_REED_back_1 = analogRead(REED_back_1);
  val_REED_back_2 = analogRead(REED_back_2);
  val_PD_inside = analogRead(PD_inside);
  val_PD_outside = analogRead(PD_outside);

  Serial.print(val_REED_front_0);
  Serial.print(",");
  Serial.print(val_REED_front_1);
  Serial.print(",");
  Serial.print(val_REED_back_0);
  Serial.print(",");
  Serial.print(val_REED_back_1);
  Serial.print(",");
  Serial.print(val_REED_back_2);
  Serial.print(",");
  Serial.print(val_PD_inside);  
  Serial.print(",");
  Serial.println(val_PD_outside);
}

