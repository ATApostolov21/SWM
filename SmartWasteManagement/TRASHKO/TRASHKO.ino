#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL4XiEPhmiZ"
#define BLYNK_TEMPLATE_NAME "SWM"
#define BLYNK_AUTH_TOKEN "5RMoDbAKRPdB4FG_FIJfmWow5RKWsJFG"

//This code is for the other template called "Smart Waste"
//#define BLYNK_TEMPLATE_ID "TMPL4GpGMojLT"
//#define BLYNK_TEMPLATE_NAME "Smart Waste"
//#define BLYNK_AUTH_TOKEN "zy4ARrY0muysKQxdcz29xFA1EazL0ZNQ"

#include <Servo.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi Note 8";
char pass[] = "12345678";

BlynkTimer timer;

#define echoPin1 D7
#define trigPin1 D8
#define echoPin2 D5
#define trigPin2 D6

Servo servo;
long duration2, duration1;
int distance1, distance2;
int binLevel = 0;

// Function prototypes
void ultrasonicSensor1();
void ultrasonicSensor2();

void setup()
{
  Serial.begin(9600);
  servo.attach(D2);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Blynk.begin(auth, ssid, pass);
  delay(2500);

  // Set up the timer intervals for each sensor function
  timer.setInterval(100L, ultrasonicSensor1);
  timer.setInterval(100L, ultrasonicSensor2);
}

void loop()
{
  Blynk.run();
  timer.run();
}


BLYNK_WRITE(V1){
  servo.write(param.asInt());
  if (param.asInt() > 0)
  {
    servo.write(180);
    Blynk.virtualWrite(V1, 180);
  }
  else{
    servo.write(0);
    Blynk.virtualWrite(V1, 0);
  }
}

int getDistance(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2; // formula to calculate the distance for the ultrasonic sensor
  return distance1;
}

// Function to handle the first ultrasonic sensor
void ultrasonicSensor1()
{
  distance1 = getDistance();
  Blynk.virtualWrite(V0, distance1);

  // If the distance from the first ultrasonic sensor is under 10 cm, turn the servo 180 degrees
  if (distance1 < 30 && servo.read() == 0)
  {
    servo.write(180);
    Blynk.virtualWrite(V1, 180);
    delay(3000); // Wait for the servo to reach the desired position
    while(getDistance() < 30){
      delay(1);
    }
    servo.write(0); // Move the servo back to the initial position (0 degrees)
    Blynk.virtualWrite(V1, 0); // Sync the app with the servo position
  }
}

// Function to handle the second ultrasonic sensor
void ultrasonicSensor2()
{
  

  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  binLevel = map(distance2, 30, 0, 0, 100); // ADJUST BIN HEIGHT HERE for the second sensor
  
  if(servo.read() == 0){

  Blynk.virtualWrite(V2, binLevel);
  }
  if (binLevel > 70){
    if(servo.read() == 0){
    Blynk.logEvent("full_bin");
    servo.write(180);
    Blynk.virtualWrite(V1, 180);
    
        }
      }
}