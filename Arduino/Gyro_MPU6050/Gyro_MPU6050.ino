

#include <Wire.h>

#define MPU6050_ADRESS 0x68
int16_t accX, accY, accZ, gyrX, gyrY, gyrZ;
//int16_t tVal;
//double temperature = 0.0;

int const NUM_STREAMS = 6;
int const BUFFER_SIZE = 5;

int SerialValue[NUM_STREAMS];  //accValue [accX, accY, accZ, gyrX, gyrY, gyrZ, vec_acc, vec_gyr]
int movingAverage[NUM_STREAMS];
int derivative[NUM_STREAMS];

int temperature;
int accCorr;

// temporary values for derivative calculation
int curr;
int last;

float gyroAngle[3], gyroCorr;


// [accX, accY, accZ, gyrX, gyrY, gyrZ]
int buffer[NUM_STREAMS][BUFFER_SIZE] = {0};
long bufferAvgs[NUM_STREAMS][BUFFER_SIZE] = {0};
int bufferDevs[NUM_STREAMS][BUFFER_SIZE] = {0};

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

void updateBuffer(int newData, int streamIndex){
  buffer[streamIndex][bufferIndex[streamIndex]] = newData;
  bufferIndex[streamIndex] = (bufferIndex[streamIndex] + 1) % BUFFER_SIZE;
}

float computeMovingAverage(int streamIndex){
  long sum = 0;
  for (int i = 0; i < BUFFER_SIZE; i++){
    sum += buffer[streamIndex][i];
  }

  bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]] = (float)sum/BUFFER_SIZE;
  bufferIndexAvgs[streamIndex] = (bufferIndexAvgs[streamIndex] + 1) % BUFFER_SIZE;
  // Serial.print((float)sum/BUFFER_SIZE);
  // Serial.print(", ");
  return (float)sum/BUFFER_SIZE;
}

float computeDerivative(int streamIndex){
  // test 
  curr = bufferAvgs[streamIndex][bufferIndexAvgs[streamIndex]];
  last = bufferAvgs[streamIndex][(bufferIndexAvgs[streamIndex] - 1 + BUFFER_SIZE) % BUFFER_SIZE];
  int derivative = curr - last;
  bufferDevs[streamIndex][bufferIndexDevs[streamIndex]] = derivative;
  bufferIndexDevs[streamIndex] = (bufferIndexDevs[streamIndex] + 1) & BUFFER_SIZE;
  return derivative;
}


void loop() {
  Wire.beginTransmission(MPU6050_ADRESS);
  Wire.write(0x3B);                               // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);                    // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADRESS, 7 * 2, true);  // request a total of 7*2=14 registers


  for(byte i=0; i<3; i++) {
    SerialValue[i] = Wire.read()<<8 | Wire.read(); // reading registers: ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT
  }
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  for(byte i=3; i<6; i++) {
    SerialValue[i] = Wire.read()<<8 | Wire.read(); // reading registers: GYRO_XOUT, GYRO_>OUT, GYRO_ZOUT
  }


  for (int i = 0; i < NUM_STREAMS; i++){
    // Serial.print(SerialValue[i]);
    // Serial.print(", ");   

    updateBuffer(SerialValue[i], i);

    movingAverage[i] = computeMovingAverage(i);
    // Serial.print(movingAverage[i]);
    // Serial.print(", ");

    derivative[i] = computeDerivative(i);
    Serial.print(derivative[i]);
    Serial.print(", ");
  }

  // compute acceleration vector
  SerialValue[6] = sqrt(SerialValue[0]^2 + SerialValue[1]^2 + SerialValue[2]^2);



  Serial.println();
  delay(50);
}

