/*
Hanoi tower game with arduino
modified on 77 June 2022
by @perotedev @Lojlvenom

Color Sensor 1     Arduino
-----------      --------
VCC               5V
GND               GND
s0                30
s1                31
s2                32
s3                33
OUT               20
OE                GND

Color Sensor 2     Arduino
-----------      --------
VCC               5V
GND               GND
s0                30
s1                31
s2                32
s3                33
OUT               21
OE                GND

Color Sensor 3     Arduino
-----------      --------
VCC               5V
GND               GND
s0                30
s1                31
s2                32
s3                33
OUT               22
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

struct LAST_READ {
  String color;
  int sensorNumber;
};

// Constants
const int s0 = 30;
const int s1 = 31;
const int s2 = 32;
const int s3 = 33;
const int sensorOut1 = 20;
const int sensorOut2 = 21;
const int sensorOut3 = 22;

// Variables
int red = 0;
int green = 0;
int blue = 0;
int actionsCount = 0;
int sensorTimerDelay = 1501;
LAST_READ lastRead;
DISK blueDisk;
DISK yellowDisk;
DISK redDisk;
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

  // start lcd screen
  lcd.begin(16,2);

  // lcd usage example
  lcd.print("Torre de Hanoi");
  lcd.setCursor(0,1);
  lcd.print("Equipe 4");

  Serial.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sensorOut1, INPUT);
  pinMode(sensorOut2, INPUT);
  pinMode(sensorOut3, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

void loop() {
  readColor(sensorOut1);
  readColor(sensorOut2);
  readColor(sensorOut3);
  checkGameRoles();
  delay(50);
}

void initGameValues() {
  blueDisk = { "red", "all" };
  yellowDisk = { "blue", "red" };
  redDisk = { "blue", "none" };
  tower1 = { "blue", "yellow", "red", "red" };
  tower2 = { "none", "none", "none", "none" };
  tower3 = { "none", "none", "none", "none" };
}

int readColor(int sensorNumber) {
  pulseSensor(sensorNumber);
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);

  if (red < green && red < blue && green < blue){
    Serial.println(" - (Yellow Color)");
    lastRead.color = "yellow";
  } else if (red < blue && red < green && red < 20){
    Serial.println(" - (Red Color)");
    lastRead.color = "red";
  } else if (blue < red && blue < green) {
    Serial.println(" - (Blue Color)");
    lastRead.color = "blue";
  } else if (green < red && green < blue){
    lastRead.color = "green";
    Serial.println(" - (Green Color)");
  } else{
    lastRead.color = "unknow";
    Serial.println(" - (Unknow Color)");
  }
}

void pulseSensor(int sensorOut) {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count sensorOut, pRed, RED
  red = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count sensorOut, pBLUE, BLUE
  blue = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count sensorOut, pGreen, GREEN
  green = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  setLastSensorRead(sensorOut);
}

void setLastSensorRead(int sensorNumber) {
  if (sensorNumber == sensorOut1){
    lastRead.sensorNumber = 1;
  } else if (sensorNumber = sensorOut2){
    lastRead.sensorNumber = 2;
  } else {
    lastRead.sensorNumber = 3;
  }
}

void checkGameRoles() {
  switch (lastRead.sensorNumber) {
    case 1:
      tryMoveDisk(towers[0]);
      break;
    case 2:
      tryMoveDisk(towers[1]);
      break;
    case 3:
      tryMoveDisk(towers[2]);
      break;
    default:
      break;
  }
}

void tryMoveDisk(TOWER tower) {
  DISK diskOnTop;
  diskOnTop = getDisk(tower.colorOnTop);
  bool validateAction = isValidInsertAction(diskOnTop);
  if (validateAction) {
    insertDiskOnTower(lastRead.sensorNumber, lastRead.color);
  } else {
    removeDiskFromTower(lastRead.sensorNumber, lastRead.color);
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

bool isValidInsertAction(DISK diskOnTop){
  if (diskOnTop.topColor ==  "all" || diskOnTop.topColor == lastRead.color){
    return true;
  } else {
    printAlert("ACAO INVALIDA");
    return false;
  }
}

void insertDiskOnTower(int towerNumber, String color){
  if (towers[towerNumber-1].below == "none"){
    towers[towerNumber-1].top = color;
  } else if (towers[towerNumber-1].middle == "none") {
    towers[towerNumber-1].middle = color;
  } else {
    towers[towerNumber-1].top = color;
  }
  towers[towerNumber-1].colorOnTop = color;
  actionsCount ++;
  printActionsCount();
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
  }
}

void printActionsCount() {

}

void printAlert(String message) {

}