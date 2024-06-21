#include <Wire.h>

#define MPU6050_ADRESS 0x68

int const NUM_STREAMS = 6;
int const BUFFER_SIZE_gyro = 5;

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


void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADRESS);  // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B);                        // PWR_MGMT_1 register
  Wire.write(0);                           // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(115200);
  Serial.flush();
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


void loop() {
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
    // 6*3+1 = 19 Werte
    // [accX, ma_accX, dev_accX, accY, ... , dev_accZ, gyrX, ma_gyrX, dev_gyrX, gyrY, ... , dev_gyrZ, acc_vec]
    // Serial.print(SerialValue[i]);
    // Serial.print(", ");   

    updateBuffer_gyro(SerialValue_gyro[i], i);

    movingAverage_gyro[i] = computeMovingAverage_gyro(i);

    // Serial.print(movingAverage[i]);
    // Serial.print(", ");

    derivative_gyro[i] = computeDerivative_gyro(i);
    // Serial.print(derivative[i]);
    // Serial.print(", ");
    
  }

  vec_acc_curr =movingAverage_gyro[0]*movingAverage_gyro[0] + movingAverage_gyro[1]*movingAverage_gyro[1] + movingAverage_gyro[2]*movingAverage_gyro[2];
  vec_acc_curr = sqrt(vec_acc_curr);

  long dev_vec_acc = computeDerivativeVec_gyro(vec_acc_curr, vec_acc_prev);

  vec_acc_prev = vec_acc_curr;

  Serial.print(dev_vec_acc);

  Serial.println();
}

