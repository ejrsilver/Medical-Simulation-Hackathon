#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

unsigned long timer = 0;
float timeStep = 0.01;
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

float pitch = 0;
float roll = 0;
float yaw = 0;
float accX = 0;
float accY = 0;
float accZ = 0;
float rollRad = 0;
float pitchRad = 0;
float yawRad = 0;
float gX = 0;
float gY = 0;
float gZ = -9.8;
int count = 0;

float accP(float value) {
    int x = (int)(value * 10);
    return ((float)(x)) / 10;
}

Vector minusGravity(Vector vec) {
  float x = vec.XAxis, y = vec.YAxis, z = 0;
  vec.XAxis -= x * cos(yawRad) - y * sin(yawRad);
  vec.YAxis -= x * sin(yawRad) - y * cos(yawRad);

  x = vec.XAxis; z = vec.ZAxis;
  vec.XAxis -=  x * cos(pitchRad) + z * sin(pitchRad);
  vec.ZAxis -= - x * sin(pitchRad) + z * cos(pitchRad);

  y = vec.YAxis, z = vec.ZAxis;
  vec.YAxis -=  y * cos(rollRad) - z * sin(rollRad);
  vec.ZAxis -=  y * sin(rollRad) + z * cos(rollRad);
  return vec;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  mpu.calibrateGyro();
  mpu.setThreshold(1);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance=duration*0.034/2;

  timer = millis();
  Vector norm=mpu.readNormalizeGyro();
  pitch=pitch+norm.YAxis*timeStep;
  roll=roll+norm.XAxis*timeStep;
  yaw=yaw+norm.ZAxis*timeStep;
  Vector normAccel=minusGravity(mpu.readNormalizeAccel());
  rollRad=roll*0.0174533;
  pitchRad=pitch*0.0174533;
  yawRad=yaw*0.0174533;

  accX = accP(normAccel.XAxis);
  accY = accP(normAccel.YAxis);
  accZ = accP(normAccel.ZAxis);
  
  Serial.print(" accX = ");
  Serial.print(accX);
  Serial.print(" accY = ");
  Serial.print(accY);
  Serial.print(" accZ = ");
  Serial.print(accZ);

  Serial.print(" Distance: ");
  Serial.println(distance);
  
  delay(10);
}
