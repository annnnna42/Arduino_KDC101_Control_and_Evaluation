#include <Wire.h>

#define MPU6050_ADRESS 0x68

int const NUM_STREAMS = 6;
int const BUFFER_SIZE_gyro = 5;

/////// PD and REED
// Constants for pins and signals
int const REED_front_0 = 24;
int const REED_front_1 = 26;
int const REED_back_0 = 28;
int const REED_back_1 = 30;


int const PD_inside = A10;
int const PD_outside = A11;

int const LED_IR = 40;
int const LED_LASER = 42;
int const LED_LASER_SAFE = 44;
int const fPedal_Button = 46;
int const trig = 52;

// set frequency 
int freq = 125; // [ms]   oder 25 oder 125, 2.5 für handheld
int threshold_inside = 50;

int const BUFFER_SIZE_PS = 10; // dieser Buffer kann aus IRGENDEINEM GRUND maximal size 32 haben
int const BUFFER_SIZE_DEV_PS = 10;

int cBuffer_i_PS[BUFFER_SIZE_PS] = {0};
int cBuffer_avg_i_PS[BUFFER_SIZE_PS] = {0};
int cBuffer_dev_i_PS[BUFFER_SIZE_DEV_PS] = {0};

int bIndex_i_PS = 0;
int bIndex_avg_i_PS = 0;
int bIndex_dev_i_PS = 0;



///////// GYRO
// initialize buffer

long SerialValue_gyro[NUM_STREAMS];  //accValue [accX, accY, accZ, gyrX, gyrY, gyrZ]
long movingAverage_gyro[NUM_STREAMS];
int derivative_gyro[NUM_STREAMS];

unsigned long vec_acc_curr = 0;
unsigned long vec_acc_prev = 0;

int temperature;

// temporary values for derivative calculation
int curr;
int last;

// [accX, accY, accZ, gyrX, gyrY, gyrZ]
long buffer[NUM_STREAMS][BUFFER_SIZE_gyro] = {0};
long bufferAvgs[NUM_STREAMS][BUFFER_SIZE_gyro] = {0};
int bufferDevs[NUM_STREAMS][BUFFER_SIZE_gyro] = {0};

int bufferIndex[NUM_STREAMS] = {0};
int bufferIndexAvgs[NUM_STREAMS] = {0};
int bufferIndexDevs[NUM_STREAMS] = {0};



/////// PINS and signals
// Variables for sensors
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_PD_inside;
int val_PD_outside;
int val_fPedal_Button;

float val_PD_inside_volt;
float movingAverage_i_PS;
float derivative_i_PS;

// Inits
byte trig_state = LOW;
unsigned long curr_ms = 0;
unsigned long prev_ms_2 = 0;

void setup(){

  Wire.begin();
  Wire.beginTransmission(MPU6050_ADRESS);  // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B);                        // PWR_MGMT_1 register
  Wire.write(0);                           // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(115200);   //9600, 19200, 115200
  Serial.flush();
  delay(100);

  pinMode(REED_front_0, INPUT);
  pinMode(REED_front_1, INPUT);
  pinMode(REED_back_0, INPUT);
  pinMode(REED_back_1, INPUT);
  pinMode(PD_inside, INPUT);
  pinMode(PD_outside, INPUT);
  pinMode(fPedal_Button, INPUT);
  pinMode(LED_IR, OUTPUT);
  pinMode(LED_LASER, OUTPUT);
  pinMode(LED_LASER_SAFE, OUTPUT);
  pinMode(trig, OUTPUT);

  digitalWrite(LED_IR, HIGH);      
  digitalWrite(LED_LASER, HIGH);
  digitalWrite(LED_LASER_SAFE, HIGH);

}

void loop(){
  curr_ms = millis();
  toggle_trig();
  digitalWrite(trig, trig_state);
}

void updateBuffer_gyro(int newData, int streamIndex){
  buffer[streamIndex][bufferIndex[streamIndex]] = newData;
  bufferIndex[streamIndex] = (bufferIndex[streamIndex] + 1) % BUFFER_SIZE_gyro;
}

float computeMovingAverage_gyro(int streamIndex){
  long sum = 0;
  for (int i = 0; i < BUFFER_SIZE_gyro; i++){
    sum += buffer[streamIndex][i];
  }

  bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]] = (float)sum/BUFFER_SIZE_gyro;
  bufferIndexAvgs[streamIndex] = (bufferIndexAvgs[streamIndex] + 1) % BUFFER_SIZE_gyro;
  // Serial.print((float)sum/BUFFER_SIZE_gyro);
  // Serial.print(", ");
  return (float)sum/BUFFER_SIZE_gyro;
}

float computeDerivative_gyro(int streamIndex){
  // test 
  curr = bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]];
  last = bufferAvgs[streamIndex][(bufferIndexAvgs[streamIndex] - 1 + BUFFER_SIZE_gyro) % BUFFER_SIZE_gyro];
  int derivative = curr - last;
  bufferDevs[streamIndex][bufferIndexDevs[streamIndex]] = derivative;
  bufferIndexDevs[streamIndex] = (bufferIndexDevs[streamIndex] + 1) & BUFFER_SIZE_gyro;
  return derivative;
}

float computeDerivativeVec_gyro(long curr, long prev){
  long derivative = curr - prev;
  return derivative;
}

void updateBuffer_PS(int newData, int* bIndex, int* cB) {
    cB[*bIndex] = newData;
    *bIndex = (*bIndex + 1) % BUFFER_SIZE_PS;
}

float computeMovingAverage_PS(int* cBuffer, int* cBuffer_avg, int* bIndex_avg) {
    // compute moving average
    int sum = 0;
    for (int i = 0; i < BUFFER_SIZE_PS; i++) {
        sum += cBuffer[i];
    }

    // update average_buffer
    float avg = (float)sum/BUFFER_SIZE_PS;
    cBuffer_avg[*bIndex_avg] = avg;
    *bIndex_avg = (*bIndex_avg + 1) % BUFFER_SIZE_PS;

    return avg;
}

float computeDerivative_PS(int* cBuffer_dev, int* cBuffer_avg, int* bIndex_avg, int* bIndex_dev) {
    // Compute the difference between consecutive data points
    int derivative = cBuffer_avg[*bIndex_avg] - cBuffer_avg[(*bIndex_avg - 1 + BUFFER_SIZE_PS) % BUFFER_SIZE_PS];

    // update derivative buffer
    cBuffer_dev[*bIndex_dev] = derivative;
    *bIndex_dev = (*bIndex_dev + 1) % BUFFER_SIZE_DEV_PS;

    return derivative;
}

bool allValuesInRange_PS(int* cBuffer_dev_i) {
    for (int i = 0; i < BUFFER_SIZE_DEV_PS; i++) {
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
      if (curr_ms - prev_ms_2 >= freq/2){       // switch high/low 
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

float in_volt(int value){
  return value * (5.0/1023.0);
}

void read_and_print_sensors(){ 

/////// READS
  val_REED_front_0 = digitalRead(REED_front_0) ;
  val_REED_front_1 = digitalRead(REED_front_1);  
  val_REED_back_0 = digitalRead(REED_back_0);
  val_REED_back_1 = digitalRead(REED_back_1);
  val_fPedal_Button = digitalRead(fPedal_Button);
  val_PD_inside = analogRead(PD_inside);
  val_PD_inside_volt = in_volt(val_PD_inside);

  Wire.beginTransmission(MPU6050_ADRESS);
  Wire.write(0x3B);                               // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);                    // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADRESS, 7 * 2, true);  // request a total of 7*2=14 registers

  for(byte i=0; i<3; i++) {
    SerialValue_gyro[i] = Wire.read()<<8 | Wire.read(); // reading registers: ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT
  }
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  for(byte i=3; i<6; i++) {
    SerialValue_gyro[i] = Wire.read()<<8 | Wire.read(); // reading registers: GYRO_XOUT, GYRO_>OUT, GYRO_ZOUT
  }

  for (int i = 0; i < NUM_STREAMS; i++){
    updateBuffer_gyro(SerialValue_gyro[i], i); 
    movingAverage_gyro[i] = computeMovingAverage_gyro(i);
    derivative_gyro[i] = computeDerivative_gyro(i);
  }

  vec_acc_curr =movingAverage_gyro[0]*movingAverage_gyro[0] + movingAverage_gyro[1]*movingAverage_gyro[1] + movingAverage_gyro[2]*movingAverage_gyro[2];
  vec_acc_curr = sqrt(vec_acc_curr);
  long dev_vec_acc = computeDerivativeVec_gyro(vec_acc_curr, vec_acc_prev);
  vec_acc_prev = vec_acc_curr;


/////// UPDATES AND CALCS
  updateBuffer_PS(val_PD_inside, &bIndex_i_PS, cBuffer_i_PS);
  movingAverage_i_PS = computeMovingAverage_PS(cBuffer_i_PS, cBuffer_avg_i_PS, &bIndex_avg_i_PS);
  derivative_i_PS = computeDerivative_PS(cBuffer_dev_i_PS, cBuffer_avg_i_PS, &bIndex_avg_i_PS, &bIndex_dev_i_PS);

/////// PRINTS
  Serial.print(dev_vec_acc);
  Serial.print(",");

  Serial.print(val_PD_inside); 
  Serial.print(",");
  Serial.print(movingAverage_i_PS);
  Serial.print(",");
  Serial.print(derivative_i_PS);
  Serial.print(",");

  Serial.print(val_REED_front_0);
  Serial.print(",");
  Serial.print(val_REED_front_1);
  Serial.print(",");
  Serial.print(val_REED_back_0);
  Serial.print(",");
  Serial.print(val_REED_back_1);



  // Threshold control for PD_inside and derivation control for PD_inside
  if ((movingAverage_i_PS < threshold_inside) && (allValuesInRange_PS(cBuffer_dev_i_PS))) {
    
      // it is safe to use the laser
      digitalWrite(LED_LASER_SAFE, HIGH);
     // Serial.print(1);
    
      // if foot pedal is active (button in this case)
      if (val_fPedal_Button == 1) {
        digitalWrite(LED_LASER, HIGH);
      } else {
        digitalWrite(LED_LASER, LOW);
      }
 
  } else {
      digitalWrite(LED_LASER_SAFE, LOW);
      digitalWrite(LED_LASER, LOW);
      //Serial.print(0);

  }

  Serial.println(" ");




}

