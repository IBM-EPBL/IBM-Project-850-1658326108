#include<Servo.h>
const int pingPin = 7;
int servoPin = 8;
int const gassensor=A1;
int thresh=600;
int const LDR=A2;
Servo servo1;

void setup() {
  Serial.begin(9600);
  servo1.attach(servoPin);
  pinMode(2,INPUT);
  pinMode(4,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(8,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(11,HIGH);
  
}

void loop() {
  
  long duration, inches, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  //delay(100);
  
  servo1.write(0);
  
  if(cm < 40)
  {
    servo1.write(90);
    delay(2000);
  }
  else
  {
    servo1.write(0);
  }
  
  //Gas sensor
  int val=analogRead(gassensor);
  Serial.print("Gas sensor value");
  Serial.print(val);
  if(val>thresh)
    tone(9,800);
  delay(300);
  noTone(9);
  
  //LIGHT
  int val1=analogRead(LDR);
  if(val1>500)
  {
    digitalWrite(5,LOW);
    Serial.print("BULB ON");
    Serial.print(val1);
  }
  else
  {
    digitalWrite(5,HIGH);
    Serial.print("BULB OFF");
  }
  
  // PIR with LED starts
  int pir = digitalRead(2);
  
  if(pir == HIGH)
  {
    digitalWrite(4,HIGH);
    delay(1000);
  }
  else if(pir == LOW)
  {
    digitalWrite(4,LOW);
  }
  
  //temp with fan
  float value=analogRead(A0);
  float temperature=value*0.48;
  
  Serial.println("temperature");
  Serial.println(temperature);
  
  if(temperature > 20)
  {
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
  }
  else
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
