/*
TCS230 color recognition sensor
modified on 7 May 2019
by Mohammadreza Akbari @ Electropeak
https://electropeak.com/learn/
Color Sensor      Arduino
-----------      --------
VCC               5V
GND               GND
s0                8
s1                9
s2                10
s3                11
OUT               12
OE                GND
*/

const int s0 = 8;
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;
const int out = 12;

// Variables
int red = 0;
int green = 0;
int blue = 0;

void color(){
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
void setup(){
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
