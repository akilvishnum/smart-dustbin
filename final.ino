#include <Servo.h>
#include<SoftwareSerial.h>
#define mobile 6385490321
SoftwareSerial mySerial(9,10);
Servo servo;
//define Pins
int trigPin =5;
int echoPin = 6;
int servoPin=7;
int trigPinLid = 3;
int echoPinLid = 4;

long duration, durationLid, dist, distLid, average, averageLid;
long aver[3], averLid[3];
void readResponse() {
  //simple function for read modem response. we will check work or not
  delay(1000);
  String response = mySerial.readStringUntil("OK");
  Serial.println(response);
}
void sendSMS() 
{
  mySerial.println("AT");
  readResponse();
  delay(1000);
  mySerial.println("AT+CMGF=1");
  readResponse();
  delay(1000);
  mySerial.println("AT+CMGS=\"6385490321\"\r");
  readResponse();
  delay(1000);
  mySerial.println("Bin is full. You are requested to clean it. Thank You!");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){;}
  mySerial.begin(9600);
  servo.attach(servoPin);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPinLid,OUTPUT);
  pinMode(echoPinLid,INPUT);
  servo.write(0);
  delay(100);
  servo.detach();
}
void measureLid(){
digitalWrite(trigPinLid,LOW);
delayMicroseconds(5);
digitalWrite(trigPinLid,HIGH);
delayMicroseconds(15);
digitalWrite(trigPinLid,LOW);
pinMode(echoPinLid,INPUT);
durationLid = pulseIn(echoPinLid,HIGH);
distLid = (durationLid/2)/29.1;
}

bool check() {
 for(int i=0;i<=2;i++) {
  measureLid();
  averLid[i]=distLid;
  delay(10);
} 
distLid = (averLid[0]+averLid[1]+averLid[2])/3;
if(distLid<5)
  return false;
else
   return true;
}

void measure() {
//digitalWrite(10,HIGH);
digitalWrite(trigPin,LOW);
delayMicroseconds(5);
digitalWrite(trigPin,HIGH);
delayMicroseconds(15);
digitalWrite(trigPin,LOW);
pinMode(echoPin,INPUT);
duration = pulseIn(echoPin,HIGH);
dist = (duration/2)/29.1;
}
void loop() {
  // put your main code here, to run repeatedly:
for(int i=0;i<=2;i++) {
  measure();
  aver[i]=dist;
  delay(10);
}
dist = (aver[0]+aver[1]+aver[2])/3;
if(dist<50) 
{
  if(check())
  {
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    delay(3000);
    servo.write(150);
    delay(1000);
    servo.detach();
  }
  else
  {
    sendSMS();
    delay(5000);
  }
}
}
