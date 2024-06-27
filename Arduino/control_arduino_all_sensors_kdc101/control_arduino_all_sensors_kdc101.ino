#include <Wire.h>

#define MPU6050_ADRESS 0x68

// General variables
int freq = 2.5; // [ms]   oder 25 oder 125, 2.5 für handheld
byte trig_state = LOW;
unsigned long curr_ms = 0;
unsigned long prev_ms_2 = 0;

// Constants for pins and signals
int const REED_front_0 = 24;
int const REED_front_1 = 26;
int const REED_back_0 = 28;
int const REED_back_1 = 30;
int const PT_inside = A10;
int const IlluminatingLED = 40;
int const LaserON = 42;
int const LaserINDICATOR = 44;
int const LaserWARNING = 38;
int const FootPedal = 46;
int const trig = 52;

// Variables for sensors
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_PT_inside;
int val_fPedal_Button;
int reed_sum;

// Phototransistor calculations
int threshold_PT = 50; // convert to Volt with [V] = threshold *(5/1023) 
float movingAverage_i_PT;
float derivative_i_PT;

int const BUFFER_SIZE_PT = 10; // dieser Buffer kann aus IRGENDEINEM GRUND maximal size 32 haben
int const BUFFER_SIZE_DEV_PT = 10;
int cBuffer_i_PT[BUFFER_SIZE_PT] = {0};
int cBuffer_avg_i_PT[BUFFER_SIZE_PT] = {0};
int cBuffer_dev_i_PT[BUFFER_SIZE_DEV_PT] = {0};
int bIndex_i_PT = 0;
int bIndex_avg_i_PT = 0;
int bIndex_dev_i_PT = 0;

// Accelerometer calculations
int threshold_ACC = 110;

int const NUM_STREAMS = 6;
int const BUFFER_SIZE_ACC = 5;
long SerialValue_ACC[NUM_STREAMS];  //accValue [accX, accY, accZ, gyrX, gyrY, gyrZ]
long movingAverage_ACC[NUM_STREAMS];
int derivative_ACC[NUM_STREAMS];
unsigned long vec_acc_curr = 0;
unsigned long vec_acc_prev = 0;
int temperature;

// temporary values for derivative calculation
int curr;
int last;

// [accX, accY, accZ, gyrX, gyrY, gyrZ]
long buffer[NUM_STREAMS][BUFFER_SIZE_ACC] = {0};
long bufferAvgs[NUM_STREAMS][BUFFER_SIZE_ACC] = {0};
int bufferDevs[NUM_STREAMS][BUFFER_SIZE_ACC] = {0};

int bufferIndex[NUM_STREAMS] = {0};
int bufferIndexAvgs[NUM_STREAMS] = {0};
int bufferIndexDevs[NUM_STREAMS] = {0};


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
  pinMode(PT_inside, INPUT);
  pinMode(FootPedal, INPUT);
  pinMode(IlluminatingLED, OUTPUT);
  pinMode(LaserON, OUTPUT);
  pinMode(LaserINDICATOR, OUTPUT);
  pinMode(LaserWARNING, OUTPUT);
  pinMode(trig, OUTPUT);

  digitalWrite(IlluminatingLED, HIGH);      
  digitalWrite(LaserON, LOW);
  digitalWrite(LaserINDICATOR, LOW);
  digitalWrite(LaserWARNING, LOW);
}

void loop(){
  curr_ms = millis();
  toggle_trig();
  digitalWrite(trig, trig_state);
}

// Update buffer acceleromer
void updateBuffer_ACC(int newData, int streamIndex){
  buffer[streamIndex][bufferIndex[streamIndex]] = newData;
  bufferIndex[streamIndex] = (bufferIndex[streamIndex] + 1) % BUFFER_SIZE_ACC;
}

// Compute moving average of accelerometer
float computeMovingAverage_ACC(int streamIndex){
  long sum = 0;
  for (int i = 0; i < BUFFER_SIZE_ACC; i++){
    sum += buffer[streamIndex][i];
  }
  bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]] = (float)sum/BUFFER_SIZE_ACC;
  bufferIndexAvgs[streamIndex] = (bufferIndexAvgs[streamIndex] + 1) % BUFFER_SIZE_ACC;
  return (float)sum/BUFFER_SIZE_ACC;
}

// Compute derivative of raw data 
float computeDerivative_ACC(int streamIndex){
  // test 
  curr = bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]];
  last = bufferAvgs[streamIndex][(bufferIndexAvgs[streamIndex] - 1 + BUFFER_SIZE_ACC) % BUFFER_SIZE_ACC];
  int derivative = curr - last;
  bufferDevs[streamIndex][bufferIndexDevs[streamIndex]] = derivative;
  bufferIndexDevs[streamIndex] = (bufferIndexDevs[streamIndex] + 1) & BUFFER_SIZE_ACC;
  return derivative;
}

// Compute derivative of acceleration vector
float computeDerivativeVec_ACC(long curr, long prev){
  long derivative = curr - prev;
  return derivative;
}

// Update Buffer Phototransistor
void updateBuffer_PT(int newData, int* bIndex, int* cB) {
    cB[*bIndex] = newData;
    *bIndex = (*bIndex + 1) % BUFFER_SIZE_PT;
}

// Compute moving average phototransistor
float computeMovingAverage_PT(int* cBuffer, int* cBuffer_avg, int* bIndex_avg) {
    int sum = 0;
    for (int i = 0; i < BUFFER_SIZE_PT; i++) {
        sum += cBuffer[i];
    }
    // update average_buffer
    float avg = (float)sum/BUFFER_SIZE_PT;
    cBuffer_avg[*bIndex_avg] = avg;
    *bIndex_avg = (*bIndex_avg + 1) % BUFFER_SIZE_PT;

    return avg;
}

// Compute the difference between consecutive data points
float computeDerivative_PT(int* cBuffer_dev, int* cBuffer_avg, int* bIndex_avg, int* bIndex_dev) {
    int derivative = cBuffer_avg[*bIndex_avg] - cBuffer_avg[(*bIndex_avg - 1 + BUFFER_SIZE_PT) % BUFFER_SIZE_PT];
    // update derivative buffer
    cBuffer_dev[*bIndex_dev] = derivative;
    *bIndex_dev = (*bIndex_dev + 1) % BUFFER_SIZE_DEV_PT;
    return derivative;
}

// check if last buffer_size_dev values are stable/below 4
bool allValuesInRange_PT(int* cBuffer_dev_i) {
    for (int i = 0; i < BUFFER_SIZE_DEV_PT; i++) {
        // if there is more noise this value need to be increased (with the condom)
        if (cBuffer_dev_i[i] < -4 || cBuffer_dev_i[i] > 4) {
            return false; // Return false as soon as a value is out of range
        }
    }
    return true; // Return true if all values are within range
}

// Mimic CLK behaviour, used for synchronization with stage
void toggle_trig(){
  switch(trig_state){
    case LOW:
      if (curr_ms - prev_ms_2 >= freq/2){       
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

  // Read sensors
  val_REED_front_0 = digitalRead(REED_front_0) ;
  val_REED_front_1 = digitalRead(REED_front_1);  
  val_REED_back_0 = digitalRead(REED_back_0);
  val_REED_back_1 = digitalRead(REED_back_1);
  val_fPedal_Button = digitalRead(FootPedal);
  val_PT_inside = analogRead(PT_inside);

  // Transmisson from MPU6050
  Wire.beginTransmission(MPU6050_ADRESS);
  Wire.write(0x3B);                               // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);                    // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADRESS, 7 * 2, true);  // request a total of 7*2=14 registers
  for(byte i=0; i<3; i++) {
    SerialValue_ACC[i] = Wire.read()<<8 | Wire.read(); // reading registers: ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT
  }
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  for(byte i=3; i<6; i++) {
    SerialValue_ACC[i] = Wire.read()<<8 | Wire.read(); // reading registers: GYRO_XOUT, GYRO_>OUT, GYRO_ZOUT
  }

  // Update accelerometer buffers
  for (int i = 0; i < NUM_STREAMS; i++){
    updateBuffer_ACC(SerialValue_ACC[i], i); 
    movingAverage_ACC[i] = computeMovingAverage_ACC(i);
    derivative_ACC[i] = computeDerivative_ACC(i);
  }

  // Calculate values accelerometer
  vec_acc_curr =movingAverage_ACC[0]*movingAverage_ACC[0] + movingAverage_ACC[1]*movingAverage_ACC[1] + movingAverage_ACC[2]*movingAverage_ACC[2];
  vec_acc_curr = sqrt(vec_acc_curr);
  long dev_vec_acc = computeDerivativeVec_ACC(vec_acc_curr, vec_acc_prev);
  vec_acc_prev = vec_acc_curr;

  // Update phototransistor buffer
  updateBuffer_PT(val_PT_inside, &bIndex_i_PT, cBuffer_i_PT);
  movingAverage_i_PT = computeMovingAverage_PT(cBuffer_i_PT, cBuffer_avg_i_PT, &bIndex_avg_i_PT);
  derivative_i_PT = computeDerivative_PT(cBuffer_dev_i_PT, cBuffer_avg_i_PT, &bIndex_avg_i_PT, &bIndex_dev_i_PT);

  // Calculate sum reed sensors
  reed_sum = val_REED_front_0 + val_REED_front_1 + val_REED_back_0 + val_REED_back_1;

  // Prints for serial output to host
  Serial.print(reed_sum);
  Serial.print(",");
  Serial.print(dev_vec_acc);
  Serial.print(",");
  Serial.print(val_PT_inside); 
  Serial.print(",");
  Serial.print(movingAverage_i_PT);
  Serial.print(",");
  Serial.print(derivative_i_PT);
  Serial.print(",");
  Serial.print(val_REED_front_0);
  Serial.print(",");
  Serial.print(val_REED_front_1);
  Serial.print(",");
  Serial.print(val_REED_back_0);
  Serial.print(",");
  Serial.print(val_REED_back_1);

  // Logic to decide if laser can be switched on

  if ((reed_sum < 4 ) || (movingAverage_i_PT > threshold_PT) || (dev_vec_acc > threshold_ACC) || (allValuesInRange_PT(cBuffer_dev_i_PT)){
    // Laser OFF
    digitalWrite(LaserINDICATOR, LOW);
    digitalWrite(LaserON, LOW);
  } else if ((reed_sum == 4) && (movingAverage_i_PT < threshold_PT) && (dev_vec_acc < threshold_ACC)) {
    // Laser Indicator ON 
    digitalWrite(LaserINDICATOR, HIGH);
    if (val_fPedal_Button == 1) {
      // Laser ON
      digitalWrite(LaserON, HIGH);
      digitalWrite(LaserWARNING, HIGH);
    } else {
        digitalWrite(LaserON, LOW);
    }
  } else {
    digitalWrite(LaserON, LOW);
  }

  Serial.println(" ");
}

