int sensor1 = A4;
int sensor2 = A5;
int sensor3 = A6;

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
}

//globals
int data1;
int data2;
int data3;
int freq = 50; //data collection frequency ~x milliseconds

void loop(){
    data1 = analogRead(sensor1);
    data2 = analogRead(sensor2);
    data3 = analogRead(sensor3);



    //Display Data to Serial Monitor
    Serial.print(data1);
    Serial.print(",");
    Serial.print(data2);    
    Serial.print(",");
    Serial.println(data3);

    delay(freq);
}