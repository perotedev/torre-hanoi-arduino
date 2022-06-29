/*
Hanoi tower game with arduino
modified on 77 June 2022
by @perotedev @Lojlvenom

Color Sensor 1     Arduino
-----------      --------
VCC               5V
GND               GND
s0                50
s1                51
s2                52
s3                53
OUT               49
OE                GND

Color Sensor 2     Arduino
-----------      --------
VCC               5V
GND               GND
s0                50
s1                51
s2                47
s3                46
OUT               48
OE                GND

Color Sensor 3     Arduino
-----------      --------
VCC               5V
GND               GND
s0                50
s1                41
s2                43
s3                42
OUT               41
OE                GND

LCD               Arduino
-----------      --------
VSS               GND
VDD               5V
V0                GND
RS                8
RW                GND
E                 9
D4                10
D5                11
D6                12
D7                13
A                 5V + resistor
K                 GND
*/

// include lcd lib
#include<LiquidCrystal.h>

// Structs
struct SENSOR {
  int S0;
  int S1;
  int S2;
  int S3;
  int OUT;
  int sensorNumber;
  String lastColorRead;
};

struct DISK {
  String color;
  String topColor;
};

struct TOWER {
  String below;
  String middle;
  String top;
  String colorOnTop;
};

// Constants
const int messageOffset = 400;
const int timeToNextMove = 1000; // one second after last move is permited more actions

// Variables
int red = 0;
int green = 0;
int blue = 0;
int actionsCount = 0;
int lastActionTime = 1001; 
DISK blueDisk;
DISK yellowDisk;
DISK redDisk;
SENSOR sensor1;
SENSOR sensor2;
SENSOR sensor3;
TOWER tower1;
TOWER tower2;
TOWER tower3;

// Pointers to access tower struct by reference array
TOWER *t1 = &tower1;
TOWER *t2 = &tower2;
TOWER *t3 = &tower3;
TOWER towers[3] = { *t1, *t2, *t3 };

// Lcd definition
LiquidCrystal lcd(8,9,10,11,12,13);

void setup() {
  // init game variables
  initGameValues();
  
  // init sensor pin mode
  sensor1 = {50, 51, 52, 53, 49 , 1, "unknow"};
  sensor2 = {50, 51, 47, 46, 45 , 2, "unknow"};
  sensor3 = {50, 51, 43, 42, 41 , 3, "unknow"};  
  setSensorPinMode(sensor1);
  setSensorPinMode(sensor2);
  setSensorPinMode(sensor3);

  // start lcd screen
  lcd.begin(16,2);
  
  // show default message
  printDefaultMessage();
}

void loop() {
  readColor(sensor1);
  delay(100);
  readColor(sensor2);
  delay(100);
  readColor(sensor3);
  delay(100);
  if (lastActionTime > timeToNextMove){
    playGame();
    delay(100);
    lastActionTime = 100;
  }
  lastActionTime += 300;
}

void initGameValues() {
  // disk rules
  blueDisk = { "blue", "all" }; 
  yellowDisk = { "yellow", "red" };
  redDisk = { "red", "none" };

  // initial postion disks in towers
  tower1 = { "blue", "yellow", "red", "red" };
  tower2 = { "none", "none", "none", "none" };
  tower3 = { "none", "none", "none", "none" };
}

void setSensorPinMode(SENSOR sensor) {
  Serial.begin(9600);
  pinMode(sensor.S0, OUTPUT);
  pinMode(sensor.S1, OUTPUT);
  pinMode(sensor.S2, OUTPUT);
  pinMode(sensor.S3, OUTPUT);
  pinMode(sensor.OUT, INPUT);
  digitalWrite(sensor.S0, HIGH);
  digitalWrite(sensor.S1, HIGH);
}

int readColor(SENSOR sensor) {
  pulseSensor(sensor);
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);

  if (red < green && red < blue && green < blue && (blue - green) > 9){
    Serial.print(" - (Yellow Color)");
    sensor.lastColorRead = "yellow";
  } else if (red < blue && red < green && red < 20){
    Serial.print(" - (Red Color)");
    sensor.lastColorRead = "red";
  } else if (blue < red && blue < green) {
    Serial.print(" - (Blue Color)");
    sensor.lastColorRead = "blue";
  } else {
    sensor.lastColorRead = "unknow";
    Serial.print(" - (Unknow Color)");
  }
  
  Serial.print(" - Sensor ");
  Serial.println(sensor.sensorNumber);
}

void pulseSensor(SENSOR sensor) {
  digitalWrite(sensor.S2, LOW);
  digitalWrite(sensor.S3, LOW);
  //count sensorOut, pRed, RED
  red = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
  digitalWrite(sensor.S3, HIGH);
  //count sensorOut, pBLUE, BLUE
  blue = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
  digitalWrite(sensor.S2, HIGH);
  //count sensorOut, pGreen, GREEN
  green = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
}

void playGame() {
  if (sensor1.lastColorRead != "unknow"){
    tryMoveDisk(sensor1);
  } else if (sensor2.lastColorRead != "unknow"){
    tryMoveDisk(sensor2);
  } else if (sensor3.lastColorRead != "unknow"){
    tryMoveDisk(sensor3);
  }
}

void tryMoveDisk(SENSOR sensor) {
  TOWER tower = towers[sensor.sensorNumber-1];
  DISK diskOnTop = getDisk(tower.colorOnTop);
  bool isInsertion = isInsertAction(diskOnTop, sensor.lastColorRead);
  if (isInsertion) {
    insertDiskOnTower(sensor.sensorNumber, sensor.lastColorRead);
  } else {
    removeDiskFromTower(sensor.sensorNumber, sensor.lastColorRead);
  }
}

DISK getDisk(String diskColor) {
  if (diskColor == "blue") {
    return blueDisk;
  } else if (diskColor == "yellow") {
    return yellowDisk;
  } else {
    return redDisk;
  }
}

bool isInsertAction(DISK diskOnTop, String color){
  if (diskOnTop.topColor ==  "all" || diskOnTop.topColor == color){
    return true;
  } else {
    // condition check if is a disk remove
    if (diskOnTop.color != color){
      printAlert("ACAO INVALIDA");
      delay(messageOffset);
      printDefaultMessage();
    }
    return false;
  }
}

void insertDiskOnTower(int towerNumber, String color){
  if (towers[towerNumber-1].below == "none"){
    towers[towerNumber-1].below = color;
  } else if (towers[towerNumber-1].middle == "none") {
    towers[towerNumber-1].middle = color;
  } else {
    towers[towerNumber-1].top = color;
  }
  towers[towerNumber-1].colorOnTop = color;
  actionsCount ++;
  printActionsCount();
  delay(messageOffset);
  printDefaultMessage();
}

void removeDiskFromTower(int towerNumber, String color) {
  if (towers[towerNumber-1].colorOnTop == color) {
    if (towers[towerNumber-1].below == color){
      towers[towerNumber-1].below = "none";
      towers[towerNumber-1].colorOnTop = "none";
    } else if (towers[towerNumber-1].middle == color) {
      towers[towerNumber-1].middle = "none";
      towers[towerNumber-1].colorOnTop = towers[towerNumber-1].below;
    } else {
      towers[towerNumber-1].top = "none";
      towers[towerNumber-1].colorOnTop = towers[towerNumber-1].middle;
    }
    actionsCount ++;
    printActionsCount();
    delay(messageOffset);
    printDefaultMessage();
  }
}

void printDefaultMessage() {
  lcd.clear();
  lcd.print("Torre de Hanoi");
  lcd.setCursor(0,1);
  lcd.print("Faca uma jogada!");
}

  
void printActionsCount() {
  lcd.clear();
  lcd.print("Quantidade de jogadas realizadas:");
  lcd.print(actionsCount);
}

void printAlert(String message) {
  lcd.clear();
  lcd.print(message);
}
