#include <QTRSensors.h>


// To Set Up kd , kp and baseSpeed
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial ESP8266(4, 2);
DynamicJsonDocument doc(200);
// Ends Here

uint8_t baseSpeed = 200;
float Kd = 2;
float Kp = 0.2;
float Kp_sound = 50;
float Kd_sound =3;
const uint8_t minSpeedDashed = 60;
const uint8_t qtrSensorsArray[] = {11, 10, A0, A1, A2, A3, A4, A5};
const uint8_t sideSensorsArray[] = {A6, A7}; // Right -> Left
const uint16_t optimalPosition_sound = 5; // Distance from wall in cm

#define useDistanceSensor false 
#define rightmotorforward 6
#define rightmotorback 5
#define leftmotorforward 3
#define leftmotorback 9
#define startButton 13

#if useDistanceSensor
  #define trigpin 7
  #define echopin 8
#endif
#define maxSpeed 255


const uint8_t numberOfQtrSensors = sizeof(qtrSensorsArray) / sizeof(qtrSensorsArray[0]);
const uint8_t numberOfSideSensors = sizeof(sideSensorsArray) / sizeof(sideSensorsArray[0]);
const uint16_t optimalPosition = (numberOfQtrSensors - 1) * 500;
bool work = true;

bool isArduinoNano = false;
#if defined(A7)
  isArduinoNano = true;
#endif

uint16_t position;
int lastError = 0;
QTRSensors qtrSensors;
QTRSensors sideSensors;
uint16_t sideSensorValues[numberOfSideSensors];
uint16_t qtrSensorValues[numberOfQtrSensors];
enum LineColor {Black,White};

bool detect_Line(LineColor color = Black, uint8_t times = 2);  
void PID_control(LineColor color = Black);
void PID_control_dashedLine(LineColor color = Black);
void steerCar(int motorspeedl, int motorspeedr, int delayTime = 0);
uint8_t positionFromSideSensors(LineColor color = Black);

void setup() {
  Serial.begin(9600);
  ESP8266.begin(4800); 

  setPinModes();

  while(!startButtonClicked());
  qtrcalibration();
  while(!startButtonClicked());
  delay(60); // For the First Jump
}

void loop() {
  while (ESP8266.available() > 0) {
    String val = ESP8266.readStringUntil('\n');
    manageSerial(val);
  }
  PID_control();
}

void setPinModes(){
  pinMode(rightmotorforward, OUTPUT);
  pinMode(rightmotorback, OUTPUT);
  pinMode(leftmotorback, OUTPUT);
  pinMode(leftmotorforward, OUTPUT);
  pinMode(startButton,INPUT_PULLUP);

  qtrSensors.setTypeRC();
  qtrSensors.setSensorPins(qtrSensorsArray, numberOfQtrSensors);
  
  // if (useDistanceSensor){
  //   pinMode(trigpin,OUTPUT);
  //   pinMode(echopin,INPUT);
  // }
  if (isArduinoNano) {
    sideSensors.setTypeAnalog();
    sideSensors.setSensorPins(sideSensorsArray, numberOfSideSensors);
  } else {
    for (int i = 0; i < numberOfSideSensors; i++) {
      pinMode(sideSensorsArray[i], INPUT);
    }
  }
}

bool startButtonClicked(){
  return !digitalRead(startButton);
}

int lastMotorSpeedL = 0;
int lastMotorSpeedR = 0;

void steerCar(int motorspeedl, int motorspeedr, int delayTime = 0) {
  if (!work){
    motorspeedl = 0;
    motorspeedr = 0;
  }
  if (motorspeedl != lastMotorSpeedL || motorspeedr != lastMotorSpeedR) {
    analogWrite(rightmotorforward, max(motorspeedr, 0));
    analogWrite(leftmotorforward, max(motorspeedl, 0));
    analogWrite(rightmotorback, max(0, -motorspeedr));
    analogWrite(leftmotorback, max(0, -motorspeedl));
    
    lastMotorSpeedL = motorspeedl;
    lastMotorSpeedR = motorspeedr;
  }

  delay(delayTime);
}

void PID_control(LineColor color = Black) {
  if (color == Black) {
    position = qtrSensors.readLineBlack(qtrSensorValues);
  } else {
    position = qtrSensors.readLineWhite(qtrSensorValues);
  }

  int error = optimalPosition - position;
  int motorspeed = error * Kp  + (error - lastError) * Kd;
  lastError = error;

  int motorspeedr = constrain(baseSpeed + motorspeed, -maxSpeed, maxSpeed);
  int motorspeedl = constrain(baseSpeed - motorspeed, -maxSpeed, maxSpeed);  
  
  steerCar(motorspeedl, motorspeedr);
}

void PID_control_dashedLine(LineColor color = Black) {
  if (color == Black) {
    position = qtrSensors.readLineBlack(qtrSensorValues);
  } else {
    position = qtrSensors.readLineWhite(qtrSensorValues);
  }

  int error = optimalPosition - position;
  int motorspeed = error * Kp  + (error - lastError) * Kd;
  lastError = error;

  int motorspeedr = constrain(baseSpeed + motorspeed, minSpeedDashed, maxSpeed);
  int motorspeedl = constrain(baseSpeed - motorspeed, minSpeedDashed, maxSpeed);  
  
  steerCar(motorspeedl, motorspeedr);
}

float distance(){
  float duration;
  // digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  // digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  // digitalWrite(trigpin, LOW);
  // duration = pulseIn(echopin, HIGH);
  return duration * 0.034 / 2;
}

void soundPid(){
  int position_sound = distance();

  float error = optimalPosition_sound - position_sound;
  float motorspeed = error*Kp_sound  + (error - lastError)*Kd_sound;
  lastError = error;
  int motorspeedr = constrain(baseSpeed - motorspeed, -maxSpeed, maxSpeed);
  int motorspeedl = constrain(baseSpeed + motorspeed, -maxSpeed, maxSpeed);  
  steerCar(motorspeedr,motorspeedl);
}

void qtrcalibration() {
  steerCar(-120,120);
  for (uint16_t i = 0; i < 250; i++)
  {
    qtrSensors.calibrate();
    if(isArduinoNano){
      sideSensors.calibrate();
    }
  }
  steerCar(0, 0);
}

bool detect_Line(LineColor color = Black ,uint8_t times = 2){
  uint8_t countPerIteration = 0;
  uint8_t totalTimesDetected = 0;
  for (int j = 0 ; j < 7; j++){
    if (color == Black) {
      qtrSensors.readLineBlack(qtrSensorValues);
    } else {
      qtrSensors.readLineWhite(qtrSensorValues);
    }
    for (int i = 0 ; i < numberOfQtrSensors ; i++){
      if(qtrSensorValues[i] > 750){countPerIteration++;}
    }
    totalTimesDetected += countPerIteration >= times ? 1 : 0;
    countPerIteration = 0;
  }
  return totalTimesDetected > 3;
}

uint8_t positionFromSideSensors(LineColor color = Black){
  int DetectedOnRight = 0;
  int DetectedOnLeft = 0;
  if(isArduinoNano){
    for (int j = 0 ; j < 7; j++){
      sideSensors.readLineBlack(sideSensorValues);
      if (sideSensorValues[0]> 700) {DetectedOnRight++;}
      if (sideSensorValues[1]> 700) {DetectedOnLeft++;}
    }
  } else {
    for (int j = 0 ; j < 7; j++){
      if (digitalRead(sideSensorsArray[0])) {DetectedOnRight++;}
      if (digitalRead(sideSensorsArray[1])) {DetectedOnLeft++;}
    }
  }
  bool lineOnRight = DetectedOnRight >= 4;
  bool lineOnLeft = DetectedOnLeft >= 4;
  uint8_t pos = 0;
  if (lineOnRight == lineOnLeft) return pos;
  else if (lineOnRight) pos = 1;
  else pos = -1;

  return color == White ? pos : -pos;
}

void manageSerial(String val) {
  val.trim();
  String jsonData = "";
  if (val == "QTR8RC"){
    String sensorValues = "[";
    for (int i = 0; i < numberOfQtrSensors; i++) {
      sensorValues += String(qtrSensorValues[i]);
      if (i < numberOfQtrSensors - 1) sensorValues += ", ";
    }

    jsonData = "{\"qtrSensorValues\": " + sensorValues + "], \"position\":" +String(position) + "}";
    
    ESP8266.println(jsonData);
  } else if (val.substring(0, 2) == "{\"") {
    DeserializationError error = deserializeJson(doc, val);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    baseSpeed = doc["baseSpeed"].as<String>().toInt();
    Kd = doc["kd"].as<String>().toFloat();
    Kp = doc["kp"].as<String>().toFloat();
    Serial.println(val);
    
  } else if (val == "run"){
    work = true;
    ESP8266.println("true");
  } else if (val == "stop"){
    work = false;
    ESP8266.println("false");
  } else {
    Serial.println("Sending Data");
    jsonData = "{\"baseSpeed\" :"+ String(baseSpeed) + ", \"kd\": " + String(Kd) + ", \"kp\": " + String(Kp) + "}";
    Serial.println(jsonData);
    ESP8266.println(jsonData);
  }
}

