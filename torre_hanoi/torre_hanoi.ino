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
  String belowColor;
};

struct TOWER {
  String below;
  String middle;
  String top;
  int sensorId;
};

// Constants
const int s0 = 8;
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;
const int out = 12;
const int BLUE = 1;
const int YELLOW = 2;
const int RED = 3;

// Variables
int red = 0;
int green = 0;
int blue = 0;
int actualyColor;
int actualySensor;
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
  actualyColor = getColor();
  checkGameRoles();
  delay(100);
}

void initGameValues() {
  blueDisk = { "red", "all", "none" };
  yellowDisk = { "blue", "red", "azul" };
  redDisk = { "blue", "none", "all" };
  tower1 = {"blue", "yellow", "red", 12 };
  tower2 = {"none", "none", "none", 13 };
  tower3 = {"none", "none", "none", 14 };
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

int getColor() {
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);

  if (red < green && red < blue && green < blue){
    Serial.println(" - (Yellow Color)");
    return YELLOW;
    // GET YELLOW 
  } else if (red < blue && red < green && red < 20){
    Serial.println(" - (Red Color)");
    return RED;
    // GET RED
  } else if (blue < red && blue < green) {
    Serial.println(" - (Blue Color)");
    return BLUE;
    // GET BLUE
  } else if (green < red && green < blue){
    return 0;
    Serial.println(" - (Green Color)");
    // GET GREEN;
  } else{
    return 0;
    Serial.println(" - (Unknow Color)");
    // GET UNKNOW;
  }
}

void checkGameRoles() {
  switch (actualyColor) {
    case BLUE:
      // statements
      break;
    case YELLOW:
      // statements
      break;
    case RED:
      // statements
      break;
    default:
      // statements
      break;
  }
}

void checkTowers(int towerNumber) {
  
}