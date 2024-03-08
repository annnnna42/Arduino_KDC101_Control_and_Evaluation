// Rename to 5_reed_PD_inside

//Constants
int const REED_front_0 = A2;
int const REED_front_1 = A3;
int const REED_back_0 = A4;
int const REED_back_1 = A5;
int const REED_back_2 = A6;
int const PD_inside = A7;

int const LED_REED_front_0 = 32;
int const LED_REED_front_1 = 33;
int const LED_REED_back_0 = 34;
int const LED_REED_back_1 = 35;
int const LED_REED_back_2 = 36;
int const LED_LASER = 37;
int const LED_PD_inside = 38;

int const trig1 = 51;
int const trig2 = 52;

//Variables
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_REED_back_2;
int val_PD_inside;

byte trig1_state = LOW;
byte trig2_state = LOW;

unsigned long curr_ms = 0;
unsigned long prev_ms_1 = 0;
unsigned long prev_ms_2 = 0;

int freq = 400;
int wait_time = 20000;

void setup(){
 Serial.begin(9600);
  pinMode(REED_front_0, INPUT);
  pinMode(REED_front_1, INPUT);
  pinMode(REED_back_0, INPUT);
  pinMode(REED_back_1, INPUT);
  pinMode(REED_back_2, INPUT);
  pinMode(PD_inside, INPUT);

  pinMode(LED_REED_front_0, OUTPUT);
  pinMode(LED_REED_front_1, OUTPUT);
  pinMode(LED_REED_back_0, OUTPUT);
  pinMode(LED_REED_back_1, OUTPUT);
  pinMode(LED_REED_back_2, OUTPUT);
  pinMode(LED_LASER, OUTPUT);
  pinMode(LED_PD_inside, OUTPUT);

  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);

// der Teil war für die Kommunikation und ramping up mit Stage gedacht
//0.1s True, polling for get_position alle 1s


  digitalWrite(trig2, LOW);
  delay(wait_time);
  //Serial.println("Done, send init pulse");
  digitalWrite(trig2, HIGH);
  //Serial.println("True");
  delay(freq/2);
  digitalWrite(trig2, LOW);
  //Serial.println("False");
  delay(freq/2);


}
void loop(){
  
  curr_ms = millis();
  //toggle_trig1();
  toggle_trig2();
  write_states();   
  //read_and_print_sensors();
  
/*
  digitalWrite(trig1, HIGH);
  Serial.println("True");
  delay(100);
  digitalWrite(trig1, LOW);
  Serial.println("False");
  delay(1000);

  delay(CLK);
*/

}

void write_states(){
  //Serial.print(trig1_state);
  //Serial.print(" ");
  //Serial.println(trig2_state);
  //digitalWrite(trig1, trig1_state);
  digitalWrite(trig2, trig2_state);
}

void toggle_trig1(){
  switch(trig1_state){
    case LOW:
      if (curr_ms - prev_ms_1 >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        trig1_state = HIGH;
        //read_and_print_sensors();
        //Serial.println("HIGH");
        prev_ms_1 = curr_ms;
      }

    case HIGH:
      if (curr_ms - prev_ms_1 >= freq/2){   
        trig1_state = LOW;
        //Serial.println("LOW");
        prev_ms_1 = curr_ms;
      }
  }
}

void toggle_trig2(){
  switch(trig2_state){
    case LOW:
      if (curr_ms - prev_ms_2 >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        trig2_state = HIGH;
        read_and_print_sensors();
        //Serial.println("HIGH");
        prev_ms_2 = curr_ms;
      }

    case HIGH:
      if (curr_ms - prev_ms_2>= freq/2){   
        trig2_state = LOW;
        //Serial.println("LOW");
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

  Serial.print(val_REED_front_0);
  Serial.print(",");
  Serial.print(val_REED_front_1);
  Serial.print(",");
  Serial.print(val_REED_back_0);
  Serial.print(",");
  Serial.print(val_REED_back_1);
  Serial.print(",");
  Serial.println(val_REED_back_2);
  //Serial.print(",");
  //Serial.println(val_PD_inside);
}



void set_LEDs(){
  if (val_PD_inside > 800){
    digitalWrite(LED_PD_inside, HIGH);
  }else{
    digitalWrite(LED_PD_inside, LOW);
  }

  if (val_REED_front_0 < 80){
    digitalWrite(LED_REED_front_0, LOW); 
  }else{
    digitalWrite(LED_REED_front_0, HIGH); 
  }

  if (val_REED_front_1 < 80){
    digitalWrite(LED_REED_front_1, LOW); 
  }else{
    digitalWrite(LED_REED_front_1, HIGH);
  }

  if (val_REED_back_0 < 80){
    digitalWrite(LED_REED_back_0, LOW);
  }else{
    digitalWrite(LED_REED_back_0, HIGH);
  }

  if (val_REED_back_1 < 80){
    digitalWrite(LED_REED_back_1, LOW);
  }else{
    digitalWrite(LED_REED_back_1, HIGH);
  }

  if (val_REED_back_2 < 80){
    digitalWrite(LED_REED_back_2, LOW);
  }else{
    digitalWrite(LED_REED_back_2, HIGH);
  }
}