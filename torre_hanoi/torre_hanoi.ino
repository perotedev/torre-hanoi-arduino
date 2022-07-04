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
OUT               45
OE                GND

Color Sensor 3     Arduino
-----------      --------
VCC               5V
GND               GND
s0                50
s1                51
s2                33
s3                32
OUT               31
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

struct RGB {
  int red;
  int green;
  int blue;
};

// Structs
struct SENSOR {
  int S0;
  int S1;
  int S2;
  int S3;
  int OUT;
  int sensorNumber;
  String lastColorRead;
  RGB rgb;
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
const int messageOffset = 1500; // alert display time
const int timeToNextMove = 1000; // one second after last move is permited more actions

// Variables
int actionsCount = 0; // count actions number
int lastActionTime = 1001; // time since last action
int disksOutTowerCount = 0; // checks the number of disks outside the towers
bool isGameOver = false;
RGB rgb1 = {0,0,0};
RGB rgb2 = {0,0,0};
RGB rgb3 = {0,0,0};
DISK blueDisk = { "blue", "all" }; 
DISK yellowDisk = { "yellow", "red" };
DISK redDisk = { "red", "none" };
DISK noneDisk = { "none", "all" }; ;
TOWER tower1 = { "blue", "yellow", "red", "red" };
TOWER tower2 = { "none", "none", "none", "none" };
TOWER tower3 = { "none", "none", "none", "none" };
TOWER towers[3];
SENSOR sensor1 = {50, 51, 52, 53, 49 , 1, "unknow", rgb1};
SENSOR sensor2 = {50, 51, 47, 46, 45 , 2, "unknow", rgb2};
SENSOR sensor3 = {50, 51, 33, 32, 31 , 3, "unknow", rgb3};
SENSOR sensors[3];

// Test Variables
// String cor;
// String torre;

// Lcd definition
LiquidCrystal lcd(8,9,10,11,12,13);

void setup() {
  // init game variables
  initGameValues();
  
  // init sensor pin mode 
  setSensorPinMode(sensor1);
  setSensorPinMode(sensor2);
  setSensorPinMode(sensor3);

  // start lcd screen
  lcd.begin(16,2);
  
  // show default message
  printDefaultMessage();
}

void printTower(int number){
  Serial.println("cor top: "+towers[number].colorOnTop);
  Serial.println("top: "+towers[number].top);
  Serial.println("middle: "+towers[number].middle);
  Serial.println("below: "+towers[number].below);
}

void loop() {
  readColorSensor1(sensors[0]);
  readColorSensor2(sensors[1]);
  readColorSensor3(sensors[2]);
  delay(50);

  //******* CODE SERIAL MONITOR TEST  *******//

  // Serial.println("cor:");
  // while (Serial.available() == 0){};
  // cor = Serial.readString();
  // Serial.println("torre:");
  // while (Serial.available() == 0){};
  // torre = Serial.readString();
  // delay(100);

  // Serial.println("cor: "+cor);
  // Serial.println("torre: "+torre);

  // if (torre == "1"){
  //   sensors[0].lastColorRead = cor;
  //   sensors[1].lastColorRead = "unknow";
  //   sensors[2].lastColorRead = "unknow";
  //   playGame();
  //   Serial.println("Printando torre 1");
  //   printTower(0);
  // } else if (torre == "2"){
  //   sensors[1].lastColorRead = cor;
  //   sensors[0].lastColorRead = "unknow";
  //   sensors[2].lastColorRead = "unknow";
  //   playGame();
  //   Serial.println("Printando torre 2");
  //   printTower(1);
  // } else if (torre == "3"){
  //   sensors[2].lastColorRead = cor;
  //   sensors[0].lastColorRead = "unknow";
  //   sensors[1].lastColorRead = "unknow";
  //   playGame();
  //   Serial.println("Printando torre 2");
  //   printTower(2);
  // }

  ////////////////////////////////////////q

  playGame();
  checkGameEnd();
}

void initGameValues() {  
  towers[0] = tower1;
  towers[1] = tower2;
  towers[2] = tower3;
  sensors[0] = sensor1;
  sensors[1] = sensor2;
  sensors[2] = sensor3;
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

int readColorSensor1(SENSOR sensor) {
  pulseSensor(sensor);
  int red = sensors[sensor.sensorNumber-1].rgb.red;
  int blue = sensors[sensor.sensorNumber-1].rgb.blue;
  int green = sensors[sensor.sensorNumber-1].rgb.green;
  Serial.print("R: ");
  Serial.print(red, DEC);
  Serial.print(" G: ");
  Serial.print(green, DEC);
  Serial.print(" B: ");
  Serial.print(blue, DEC);

  if (red < green && red < blue && green < blue && red < 15){
    Serial.print(" - (Yellow Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "yellow";
  } else if (red < blue && red < green && red > 20 && red < 34){
    Serial.print(" - (Red Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "red";
  } else if (blue < red && blue < green) {
    Serial.print(" - (Blue Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "blue";
  } else {
    sensors[sensor.sensorNumber-1].lastColorRead = "unknow";
    Serial.print(" - (Unknow Color)");
  }
  Serial.print(" - Sensor ");
  Serial.println(sensor.sensorNumber);
  printColor(sensors[sensor.sensorNumber-1].lastColorRead);
}

int readColorSensor2(SENSOR sensor) {
  pulseSensor(sensor);
  int red = sensors[sensor.sensorNumber-1].rgb.red;
  int blue = sensors[sensor.sensorNumber-1].rgb.blue;
  int green = sensors[sensor.sensorNumber-1].rgb.green;
  Serial.print("R: ");
  Serial.print(red, DEC);
  Serial.print(" G: ");
  Serial.print(green, DEC);
  Serial.print(" B: ");
  Serial.print(blue, DEC);

  if (red <= 50 && green <= 50 && blue <= 75){
    Serial.print(" - (Yellow Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "yellow";
  } else if (red <= 70 && blue <= 180 && blue >= 110 && green >= 125){
    Serial.print(" - (Red Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "red";
  } else if (blue < red && blue < green) {
    Serial.print(" - (Blue Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "blue";
  } else {
    sensors[sensor.sensorNumber-1].lastColorRead = "unknow";
    Serial.print(" - (Unknow Color)");
  }
  Serial.print(" - Sensor ");
  Serial.println(sensor.sensorNumber);
  printColor(sensors[sensor.sensorNumber-1].lastColorRead);
}

int readColorSensor3(SENSOR sensor) {
  pulseSensor(sensor);
  int red = sensors[sensor.sensorNumber-1].rgb.red;
  int blue = sensors[sensor.sensorNumber-1].rgb.blue;
  int green = sensors[sensor.sensorNumber-1].rgb.green;
  Serial.print("R: ");
  Serial.print(red, DEC);
  Serial.print(" G: ");
  Serial.print(green, DEC);
  Serial.print(" B: ");
  Serial.print(blue, DEC);
 
  if (red <= 50 && green <= 40 && blue <= 40){
    Serial.print(" - (Yellow Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "yellow";
  } else if (red <= 25 && blue <= 93 && blue >= 70 && green >= 70){
    Serial.print(" - (Red Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "red";
  } else if (red <=17 && blue <= 50 && green <= 50) {
    Serial.print(" - (Blue Color)");
    sensors[sensor.sensorNumber-1].lastColorRead = "blue";
  } else {
    sensors[sensor.sensorNumber-1].lastColorRead = "unknow";
    Serial.print(" - (Unknow Color)");
  }
  Serial.print(" - Sensor ");
  Serial.println(sensor.sensorNumber);
  printColor(sensors[sensor.sensorNumber-1].lastColorRead);
}

void pulseSensor(SENSOR sensor) {
  digitalWrite(sensor.S2, LOW);
  digitalWrite(sensor.S3, LOW);
  //count sensorOut, pRed, RED
  sensors[sensor.sensorNumber-1].rgb.red = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
  digitalWrite(sensor.S3, HIGH);
  //count sensorOut, pBLUE, BLUE
  sensors[sensor.sensorNumber-1].rgb.blue = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
  digitalWrite(sensor.S2, HIGH);
  //count sensorOut, pGreen, GREEN
  sensors[sensor.sensorNumber-1].rgb.green = pulseIn(sensor.OUT, digitalRead(sensor.OUT) == HIGH ? LOW : HIGH);
}

void playGame() {
  if (!isGameOver){
    if (sensors[0].lastColorRead != "unknow"){
      tryMoveDisk(sensors[0]);
    } else if (sensors[1].lastColorRead != "unknow"){
      tryMoveDisk(sensors[1]);
    } else if (sensors[2].lastColorRead != "unknow"){
      tryMoveDisk(sensors[2]);
    }
    delay(1000);
  } else {
    lcd.clear();
    lcd.print("JOGO CONCLUIDO!");
    lcd.setCursor(0,1);
    lcd.print("Inicie Novamente");
    delay(2500);
  }
}

void tryMoveDisk(SENSOR sensor) {
  TOWER tower = towers[sensor.sensorNumber-1];
  DISK diskOnTop = getDisk(tower.colorOnTop);
  Serial.println("Disco no topo: "+diskOnTop.color+", aceita "+diskOnTop.topColor);
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
  } else if (diskColor == "red"){
    return redDisk;
  } else {
    return noneDisk;
  }
}

bool isInsertAction(DISK diskOnTop, String color){
  if (diskOnTop.topColor ==  "all" && (diskOnTop.color != "blue" || color != "blue") || diskOnTop.topColor == color){
    Serial.println("Está inserindo");
    return true;
  } else {
    // condition check if is a disk remove
    if (diskOnTop.color != color){
      Serial.println("Não pode inserir");
      printAlert("ACAO INVALIDA");
      delay(messageOffset);
      printDefaultMessage();
    } else {
      Serial.println("Está removendo");
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
  disksOutTowerCount--;
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
    disksOutTowerCount++;
    if (disksOutTowerCount > 1){
      printAlert("ACAO INVALIDA");
      delay(messageOffset);
      printDefaultMessage();
    }
  }
}

void printColor(String color) {
  if (color == "yellow"){
    printAlert("AMARELO");
  } else if (color == "blue"){
    printAlert("AZUL");
  } else if (color == "red"){
    printAlert("VERMELHO");
  }

  if (color != "unknow"){
    delay(1000);
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
  lcd.print("Quantidade");
  lcd.setCursor(0,1);
  lcd.print("de jogadas: ");
  lcd.print(actionsCount);
}

void printAlert(String message) {
  lcd.clear();
  lcd.print(message);
}

void checkGameEnd(){
  if (towers[2].top == "red" && towers[2].middle == "yellow" && towers[2].below == "blue"){
    lcd.clear();
    lcd.print("** PARABENS **");
    lcd.setCursor(0,1);
    lcd.print("Jogo Concluido!");
    isGameOver = true;
  }
}


//Output from the sensor
// int redFrequency = 0;
// int greenFrequency = 0;
// int blueFrequency = 0;
//Values used for calibration
// int redMin;
// int redMax;
// int greenMin;
// int greenMax;
// int blueMin;
// int blueMax;
// int color;

// void decideColor() {//format color values
//   //Limit possible values:
//   redColor = constrain(redColor, 0, 255);
//   greenColor = constrain(greenColor, 0, 255);
//   blueColor = constrain(blueColor, 0, 255);
//   //find brightest color:
//   int maxVal = max(redColor, blueColor);
//   maxVal = max(maxVal, greenColor);
//   //map new values
//   redColor = map(redColor, 0, maxVal, 0, 255);
//   greenColor = map(greenColor, 0, maxVal, 0, 255);
//   blueColor = map(blueColor, 0, maxVal, 0, 255);
//   redColor = constrain(redColor, 0, 255);
//   greenColor = constrain(greenColor, 0, 255);
//   blueColor = constrain(blueColor, 0, 255);
//   //light led
//   analogWrite(redLED, redColor);
//   analogWrite(greenLED, greenColor);
//   analogWrite(blueLED, blueColor);
//   //decide which color is present (you may need to change some values here):
//   if (redColor > 250 && greenColor > 250 && blueColor > 250) {
//     color = 1;//white
//   }
//   else if (redColor < 25 && greenColor < 25 && blueColor < 25) {
//     color = 2;//black
//   }
//   else if (redColor > 200 &&  greenColor > 200 && blueColor < 100) {
//     color = 4;//yellow
//   }
//   else if (redColor > 200 &&  greenColor > 25 /*&& blueColor < 100*/) {
//     color = 3;//orange
//   }
//   else if (redColor > 200 &&  greenColor < 100 && blueColor > 200) {
//     color = 5;//purple
//   }
//   else if (redColor > 250 && greenColor < 200 && blueColor < 200) {
//     color = 6;//red
//   }
//   else if (redColor < 200 && greenColor > 250 && blueColor < 200) {
//     color = 7;//green
//   }
//   else if (redColor < 200 /*&& greenColor < 200*/ && blueColor > 250) {
//     color = 8;//blue
//   }
//   else {
//     color = 0;//unknown
//   }
// }

// void calibrate() {
//   Serial.println("Calibrating...");
//   Serial.println("White");//aim sensor at something white
//   //set calibration values:
//   digitalWrite(13, HIGH);
//   delay(2000);
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, LOW);
//   redMin = pulseIn(sensorOut, LOW);
//   delay(100);
//   digitalWrite(S2, HIGH);
//   digitalWrite(S3, HIGH);
//   greenMin = pulseIn(sensorOut, LOW);
//   delay(100);
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, HIGH);
//   blueMin = pulseIn(sensorOut, LOW);
//   delay(100);
//   Serial.println("next...");//aim sensor at something black
//   digitalWrite(13, LOW);
//   delay(2000);
//   Serial.println("Black");
//   //set calibration values:
//   digitalWrite(13, HIGH);
//   delay(2000);
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, LOW);
//   redMax = pulseIn(sensorOut, LOW);
//   delay(100);
//   digitalWrite(S2, HIGH);
//   digitalWrite(S3, HIGH);
//   greenMax = pulseIn(sensorOut, LOW);
//   delay(100);
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, HIGH);
//   blueMax = pulseIn(sensorOut, LOW);
//   delay(100);
//   Serial.println("Done calibrating.");
//   digitalWrite(13, LOW);
// }

// void readColor() {//get data from sensor
//   //red:
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, LOW);
//   redFrequency = pulseIn(sensorOut, LOW);
//   redColor = map(redFrequency, redMin, redMax, 255, 0);
//   delay(100);
//   //green:
//   digitalWrite(S2, HIGH);
//   digitalWrite(S3, HIGH);
//   greenFrequency = pulseIn(sensorOut, LOW);
//   greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
//   delay(100);
//   //blue:
//   digitalWrite(S2, LOW);
//   digitalWrite(S3, HIGH);
//   blueFrequency = pulseIn(sensorOut, LOW);
//   blueColor = map(blueFrequency, blueMin, blueMax, 255, 0);
//   delay(100);
// }