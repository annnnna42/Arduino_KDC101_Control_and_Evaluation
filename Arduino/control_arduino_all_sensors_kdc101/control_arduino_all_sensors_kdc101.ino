// Constants for pins and signals
int const REED_front_0 = 30;
int const REED_front_1 = 28;
int const REED_back_0 = 26;
int const REED_back_1 = 24;
int const REED_back_2 = 22;

int const PD_inside = A10;
int const PD_outside = A11;

int const LED_red = 40;
int const LED_LASER = 44;
int const trig = 52;

// set frequency 
int freq = 20;   // [ms]

// initialize buffer
int const BUFFER_SIZE = 5;
int circularBuffer[BUFFER_SIZE] = {0};
int bufferIndex = 0;

int circularBuffer_avg[BUFFER_SIZE] = {0};
int bufferIndex_avg = 0;

// Variables
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_REED_back_2;
int val_PD_inside;
int val_PD_outside;

// Inits
byte trig_state = LOW;
unsigned long curr_ms = 0;
unsigned long prev_ms_2 = 0;

void setup(){
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
  pinMode(LED_LASER, OUTPUT);
  pinMode(trig, OUTPUT);

  digitalWrite(LED_red, HIGH);      
  digitalWrite(LED_LASER, HIGH);

}

void loop(){
  curr_ms = millis();
  toggle_trig();
  digitalWrite(trig, trig_state);
}

void updateBuffer(int newData) {
    circularBuffer[bufferIndex] = newData;
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
}

float computeMovingAverage() {
    int sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sum += circularBuffer[i];
    }

// update average_buffer
    circularBuffer_avg[bufferIndex_avg] = (float)sum / BUFFER_SIZE;
    bufferIndex_avg = (bufferIndex_avg + 1) % BUFFER_SIZE;

    return (float)sum / BUFFER_SIZE;
}

float computeDerivative() {
    // Compute the difference between consecutive data points
    int derivative = circularBuffer_avg[bufferIndex_avg] - circularBuffer_avg[(bufferIndex_avg - 1 + BUFFER_SIZE) % BUFFER_SIZE];
    return derivative;
}

void toggle_trig(){
  switch(trig_state){
    case LOW:
      if (curr_ms - prev_ms_2 >= freq/2){       // switch high/low every 50ms   
        // read and print once per clock cycle
        trig_state = HIGH;
        read_and_print_sensors();
        prev_ms_2 = curr_ms;
      }
    case HIGH:
      if (curr_ms - prev_ms_2>= freq/2){   
        trig_state = LOW;
        prev_ms_2 = curr_ms;
      }
  }
}

void read_and_print_sensors(){  
  val_REED_front_0 = digitalRead(REED_front_0) * 1023;
  val_REED_front_1 = digitalRead(REED_front_1)* 1023;  
  val_REED_back_0 = digitalRead(REED_back_0)* 1023;
  val_REED_back_1 = digitalRead(REED_back_1)* 1023;
  val_REED_back_2 = digitalRead(REED_back_2)* 1023;

  val_PD_inside = analogRead(PD_inside);
  val_PD_outside = analogRead(PD_outside);

// normale Reihenfolge: f0, f1, b0, b1, b2, pd, pd_average
  Serial.print(val_PD_inside);  
  Serial.print(",");

  updateBuffer(val_PD_inside);
  float movingAverage = computeMovingAverage();
  Serial.print(movingAverage);
  Serial.print(",");

  float derivative = computeDerivative();
  Serial.print(derivative);
  Serial.print(",");

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

  // scaling serial plotter
  Serial.print(400);
  Serial.print(",");
  Serial.print(-10);
  Serial.println(",");


  if ((movingAverage < 80) && (derivative < 3)){
      digitalWrite(LED_LASER, HIGH);
  } else {
      digitalWrite(LED_LASER, LOW);
  }

}

