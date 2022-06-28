/*
Hanoi tower game with arduino
modified on 77 June 2022
by @perotedev @Lojlvenom

Color Sensor 1     Arduino
-----------      --------
VCC               5V
GND               GND
s0                8
s1                9
s2                10
s3                11
OUT               12
OE                GND

Color Sensor 2     Arduino
-----------      --------
VCC               5V
GND               GND
s0                8
s1                9
s2                10
s3                11
OUT               13
OE                GND

Color Sensor 3     Arduino
-----------      --------
VCC               5V
GND               GND
s0                8
s1                9
s2                10
s3                11
OUT               14
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
const int s0 = 8;
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;
const int out = 12;

// Variables
int red = 0;
int green = 0;
int blue = 0;
LAST_READ lastRead;
DISK blueDisk;
DISK yellowDisk;
DISK redDisk;
TOWER tower1;
TOWER tower2;
TOWER tower3;
TOWER* towers[3] = { &tower1, &tower2, &tower3 };

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
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

void loop() {
  readColor();
  setLastColorRead();
  checkGameRoles();
  delay(100);
}

void initGameValues() {
  blueDisk = { "red", "all" };
  yellowDisk = { "blue", "red" };
  redDisk = { "blue", "none" };
  tower1 = { "blue", "yellow", "red", "red" };
  tower2 = { "none", "none", "none", "none" };
  tower3 = { "none", "none", "none", "none" };
}

void readColor() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}

int setLastColorRead() {
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);

  if (red < green && red < blue && green < blue){
    Serial.println(" - (Yellow Color)");
    lastRead.color = "yellow";
    // GET YELLOW 
  } else if (red < blue && red < green && red < 20){
    Serial.println(" - (Red Color)");
    lastRead.color = "red";
    // GET RED
  } else if (blue < red && blue < green) {
    Serial.println(" - (Blue Color)");
    lastRead.color = "blue";
    // GET BLUE
  } else if (green < red && green < blue){
    lastRead.color = "green";
    Serial.println(" - (Green Color)");
    // GET GREEN;
  } else{
    lastRead.color = "unknow";
    Serial.println(" - (Unknow Color)");
    // GET UNKNOW;
  }
}

void checkGameRoles() {
  switch (lastRead.sensorNumber) {
    case 1:
      checkTower(1);
      break;
    case 2:
      checkTower(2);
      break;
    case 3:
      checkTower(3);
      break;
    default:
      break;
  }
}

void checkTower(int towerNumber) {
  
  
}

void checkTopDisk(TOWER tower) {
  if (tower.colorOnTop != "none") {
    DISK diskOnTop;
    diskOnTop = getDisk(tower.colorOnTop);

  } else {

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

bool isValidAction(DISK diskOnTop){
  if (diskOnTop.topColor ==  "all" || diskOnTop.topColor == lastRead.color){
    return true;
  } else {
    return false;
  }
}