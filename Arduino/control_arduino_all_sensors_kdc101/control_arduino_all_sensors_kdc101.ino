// Constants for pins and signals
int const REED_front_0 = 30;
int const REED_front_1 = 28;
int const REED_back_0 = 26;
int const REED_back_1 = 24;
int const REED_back_2 = 22;

int const PD_inside = A10;
int const PD_outside = A11;

int const LED_IR = 40;
int const LED_LASER = 42;
int const LED_LASER_SAFE = 44;
int const fPedal_Button = 46;
int const trig = 52;

// TODO: test boundaries
// set frequency 
//int freq = 0.1;   // [ms]

// initialize buffer
//int const BUFFER_SIZE = 20;
//int const BUFFER_SIZE_DEV = 50;

// set frequency 
int freq = 0.01;   // [ms]

// initialize buffer
int const BUFFER_SIZE = 50;
int const BUFFER_SIZE_DEV = 100;


int cBuffer_i[BUFFER_SIZE] = {0};
int cBuffer_avg_i[BUFFER_SIZE] = {0};
int cBuffer_dev_i[BUFFER_SIZE_DEV] = {0};

int bIndex_i = 0;
int bIndex_avg_i = 0;
int bIndex_dev_i = 0;

int threshold_inside = 70;

// Variables
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_REED_back_2;
int val_PD_inside;
int val_PD_outside;
int val_fPedal_Button;

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
  pinMode(fPedal_Button, INPUT);
  pinMode(LED_IR, OUTPUT);
  pinMode(LED_LASER, OUTPUT);
  pinMode(LED_LASER_SAFE, OUTPUT);
  pinMode(trig, OUTPUT);

  digitalWrite(LED_IR, LOW);      
  digitalWrite(LED_LASER, HIGH);
  digitalWrite(LED_LASER_SAFE, HIGH);

}

void loop(){
  curr_ms = millis();
  toggle_trig();
  digitalWrite(trig, trig_state);
}

void updateBuffer(int newData, int* bIndex, int* cB) {
    cB[*bIndex] = newData;
    *bIndex = (*bIndex + 1) % BUFFER_SIZE;
}

float computeMovingAverage(int* cBuffer, int* cBuffer_avg, int* bIndex_avg) {
    // compute moving average
    int sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sum += cBuffer[i];
    }

    // update average_buffer
    cBuffer_avg[*bIndex_avg] = (float)sum / BUFFER_SIZE;
    *bIndex_avg = (*bIndex_avg + 1) % BUFFER_SIZE;

    return (float)sum / BUFFER_SIZE;
}

float computeDerivative(int* cBuffer_dev, int* cBuffer_avg, int* bIndex_avg, int* bIndex_dev) {
    // Compute the difference between consecutive data points
    int derivative = cBuffer_avg[*bIndex_avg] - cBuffer_avg[(*bIndex_avg - 1 + BUFFER_SIZE) % BUFFER_SIZE];

    // update derivative buffer
    cBuffer_dev[*bIndex_dev] = derivative;
    bIndex_dev = (*bIndex_dev + 1) % BUFFER_SIZE_DEV;

    return derivative;
}

bool allValuesInRange(int* cBuffer_dev_i) {
    for (int i = 0; i < BUFFER_SIZE_DEV; i++) {
        // if there is more noise this value need to be increased (with the condom)
        if (cBuffer_dev_i[i] < -4 || cBuffer_dev_i[i] > 4) {
            return false; // Return false as soon as a value is out of range
        }
    }
    return true; // Return true if all values are within range
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

  val_fPedal_Button = digitalRead(fPedal_Button);

  val_PD_inside = analogRead(PD_inside);
  val_PD_outside = analogRead(PD_outside);

// normale Reihenfolge: f0, f1, b0, b1, b2, pd, pd_average

  // Serial.print(val_fPedal_Button);  
  // Serial.print(",");

  Serial.print(val_PD_inside); 
  Serial.print(",");

  // Serial.print(val_PD_outside);  
  // Serial.print(",");  

  updateBuffer(val_PD_inside, &bIndex_i, cBuffer_i);
  float movingAverage_i = computeMovingAverage(cBuffer_i, cBuffer_avg_i, &bIndex_avg_i);
  Serial.print(movingAverage_i);
  Serial.print(",");

  float derivative_i = computeDerivative(cBuffer_dev_i, cBuffer_avg_i, &bIndex_avg_i, &bIndex_dev_i);
  Serial.print(derivative_i);
  

/*
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
*/

/*
  // scaling serial plotter
  Serial.print(400);
  Serial.print(",");
  Serial.print(-10);
*/
  Serial.println(" ");


  // Threshold control for PD_inside and derivation control for PD_inside
  if ((movingAverage_i < threshold_inside) && (allValuesInRange(cBuffer_dev_i))) {
    
      // it is safe to use the laser
      digitalWrite(LED_LASER_SAFE, HIGH);

      // if foot pedal is active (button in this case)
      if (val_fPedal_Button == 1) {
        digitalWrite(LED_LASER, HIGH);
      } else {
        digitalWrite(LED_LASER, LOW);
      }
 
  } else {
      digitalWrite(LED_LASER_SAFE, LOW);
      digitalWrite(LED_LASER, LOW);
  }



}

