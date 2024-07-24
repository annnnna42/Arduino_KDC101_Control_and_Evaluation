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

//Variables
int val_REED_front_0;
int val_REED_front_1;
int val_REED_back_0;
int val_REED_back_1;
int val_REED_back_2;
int val_PD_inside;

void setup(){
 Serial.begin(9600);
 Serial.println("Hello World, this five Reedswitches!");
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

}

void loop(){
  val_REED_front_0 = analogRead(REED_front_0);
  val_REED_front_1 = analogRead(REED_front_1);  
  val_REED_back_0 = analogRead(REED_back_0);
  val_REED_back_1 = analogRead(REED_back_1);
  val_REED_back_2 = analogRead(REED_back_2);
  val_PD_inside = analogRead(PD_inside);

  /*Serial.print("val_REED_front_0: ");
  Serial.println(val_REED_front_0);
  Serial.print("val_REED_front_1: ");
  Serial.println(val_REED_front_1);
  Serial.print("val_REED_back_0: ");
  Serial.println(val_REED_back_0);
  Serial.print("val_REED_back_1: ");
  Serial.println(val_REED_back_1);
  Serial.print("val_REED_back_2: ");
  Serial.println(val_REED_back_2);
  */
  Serial.print("val_PD_inside: ");
  Serial.println(val_PD_inside);


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

  delay(300); //Small delay
}