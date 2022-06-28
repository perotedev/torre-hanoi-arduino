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
OUT               12
OE                GND

Color Sensor 3     Arduino
-----------      --------
VCC               5V
GND               GND
s0                8
s1                9
s2                10
s3                11
OUT               12
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

const int s0 = 8;
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;
const int out = 12;

// Variables
int red = 0;
int green = 0;
int blue = 0;

struct DISK {
  String color;
  String topColor;
  String belowColor;
};

DISK blueDisk;
DISK yellowDisk;
DISK redDisk;

struct TOWER {
  String below;
  String middle;
  String top;
};

TOWER tower1;
TOWER tower2;
TOWER tower3;

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
  color();
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);
  //Serial.println();
  if (red < green && red < blue && green < blue){
    Serial.println(" - (Yellow Color)");
    // Turn YELLOW LED ON
  } else if (red < blue && red < green && red < 20){
    Serial.println(" - (Red Color)");
    // Turn RED LED ON
  }
  else if (blue < red && blue < green) {
    Serial.println(" - (Blue Color)");
    // Turn BLUE LED ON
  }
  else if (green < red && green < blue){
    Serial.println(" - (Green Color)");
    // Turn GREEN LED ON;
  } else{
    Serial.println("(Unknow Color)");
  }
  delay(300);
}

void color() {
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

void initGameValues() {
  blueDisk = { "red", "all", "none" };
  yellowDisk = { "blue", "red", "azul" };
  redDisk = { "blue", "none", "all" };
  tower1 = {"blue", "yellow", "red"};
  tower2 = {"none", "none", "none"};
  tower3 = {"none", "none", "none"};
}
