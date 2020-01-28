#include <Servo.h>
Servo Thing;
int step_number = 0;
#define turnSpeed 5
#define wait 1000
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

#define steps 530  // Amount of steps the stepper motor can do in 90 degrees: Change at your discretion                       CHANGE THESE
int flipDeg = 10;  // How far the servo will travel when flipping cube: Change at your discretion                                  |
int holdDeg = 105; // The angle at which the servo grips the cube: Change at your discretion                                       |
int restDeg = 130; // The angle at which the servo is at rest and the stepper can move freely: Change at your discretion           |
                                                                                                                                   |
const int algLength = 18;      // <<----- PUT LENGTH OF ALGORITHM HERE!!!                                                          |
String raw = "U D B2 L B U2 R2 L' U2 R L2 U2 F L U' D' R2 B2 ";     // <<----- PUT ALGORITHM FOR SOLVING CUBE HERE!!!              |

int moveCounter = 0;
int a = 0;
int b = 0;
String parsed[algLength]; //moves to perform for solving

enum orientation{
  BW, BO, BY, BR, YB, YO, YG, YR, GY, GO, GW, GR,
  WG, WO, WB, WR, OW, OG, OY, OB, RY, RG, RW, RB
};
String orientation[] = {
  "BW","BO","BY","BR",
  "YB","YO","YG","YR",
  "GY","GO","GW","GR",
  "WG","WO","WB","WR",
  "OW","OG","OY","OB",
  "RY","RG","RW","RB"
};
orientation cube = YG;           //(down)(front)=(Y)(G)

int numturns = 0;

void setup() {
  delay(3000);
  Serial.begin(9600);
  Thing.attach(7);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT); 
}

void loop(){
  parseAlg();
  solveCube();
  showOff();
  while(true){
  }
}
void Dclock(){   //rotates stepper counterclockwise
  for(int i = 0; i < steps; i++){
    OneStep(true);
    delay(turnSpeed);
  }
  delay(wait);
}
void Dcount(){  //rotates stepper clockwise
  for(int i = 0; i < steps; i++){
    OneStep(false);
    delay(turnSpeed);
  }
  delay(wait);
}
void parseAlg() {
    for (int i = 0; i < algLength; i++) {
      while(raw.charAt(b) != ' ') {
        b++;
      }
      parsed[moveCounter] = raw.substring(a, b);
      a = b + 1;
      b++;
      moveCounter++;
    }
    Serial.print("Algorithm to perform: ");
    for (int i = 0; i < algLength; i++){
      Serial.print(parsed[i] + " ");
    }
    Serial.println();
  }
void printCube(){
  Serial.println(ori[cube]);
}
void showOff(){
  rest();
  for (int i = 0; i < 2048; i++){
    OneStep(true);
    delay(2);
  }
}
void solveCube(){ // takes string of moves and converts it to an array for processing by makeMove()
  for (int i = 0; i < algLength; i++){
    makeMove(parsed[i]);
    Serial.println(parsed[i]);
  }
}
void checkerboard(){
  turnClock(2);
  flip(2);
  turnClock(2);
  flip(1);
  turnClock(2);
  flip(2);
  turnClock(2);
  rotClock();
  flip(1);
  turnClock(2);
  flip(2);
  turnClock(2);
}
void OneStep(bool dir){
  if(dir){   //true = counterclockwise; false = CW
      switch(step_number){
        case 0:
          digitalWrite(STEPPER_PIN_1, HIGH);
          digitalWrite(STEPPER_PIN_2, LOW);
          digitalWrite(STEPPER_PIN_3, LOW);
          digitalWrite(STEPPER_PIN_4, LOW);
          break;
        case 1:
          digitalWrite(STEPPER_PIN_1, LOW);
          digitalWrite(STEPPER_PIN_2, HIGH);
          digitalWrite(STEPPER_PIN_3, LOW);
          digitalWrite(STEPPER_PIN_4, LOW);
          break;
        case 2:
          digitalWrite(STEPPER_PIN_1, LOW);
          digitalWrite(STEPPER_PIN_2, LOW);
          digitalWrite(STEPPER_PIN_3, HIGH);
          digitalWrite(STEPPER_PIN_4, LOW);
          break;
        case 3:
          digitalWrite(STEPPER_PIN_1, LOW);
          digitalWrite(STEPPER_PIN_2, LOW);
          digitalWrite(STEPPER_PIN_3, LOW);
          digitalWrite(STEPPER_PIN_4, HIGH);
          break;
      }
  }else{
    switch(step_number){
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
 
  
    } 
  }
  step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}
void correct(bool dir){
  for (int i = 0; i < (steps - 512); i++){
    OneStep(dir);
    delay(5);
  }
}
void hold(){
  Thing.write(holdDeg);
  delay(wait);
}
void rest(){
  Thing.write(restDeg);
  delay(wait);
}
void turnClock(int x){  //D
  for (int i = 0; i < x; i++){
    rest();
    hold();
    Dclock();
    rest();
    correct(false);
  }
}
void turnCount(int x){  //D'
  for (int i = 0; i < x; i++){ 
    rest();
    hold();
    Dcount();
    rest();
    correct(true);
  }
}
void turnToSide(char y){  //makes input side (FRULBD) the bottom side for turning
  switch (y){
    case 'F':
      switch (cube){
        case BW:
          flip(2);
          break;
        case BO:
          flip(2);
          break;
        case BY:
          flip(2);
          break;
        case BR:
          flip(2);
          break;
        case YB:
          flip(1);
          break;
        case YO:
          rotCount();
          flip(1);
          break;
        case YG:
          flip(3);
          break;
        case YR:
          rotClock();
          flip(1);
          break;
        case GY:
          break;
        case GO:
          break;
        case GW:
          break;
        case GR:
          break;
        case WG:
          flip(3);
          break;
        case WO:
          rotCount();
          flip(3);
          break;
        case WB:
          flip(1);
          break;
        case WR:
          rotCount();
          flip(1);
          break; 
        case OW:
          rotCount();
          flip(1);
          break;
        case OG:
          flip(3);
          break;
        case OY:
          rotCount();
          flip(3);
          break;
        case OB:
          flip(1);
          break;
        case RY:
          rotCount();
          flip(1);
          break;
        case RG:
          flip(3);
          break;
        case RW:
          rotCount();
          flip(3);
          break;
        case RB:
          flip(1);
          break;
      }
      break;
    case 'R':
      switch (cube){
        case BW:
          rotClock();
          flip(1);
          break;
        case BO:
          flip(1);
          break;
        case BY:
          rotCount();
          flip(1);
          break;
        case BR:
          flip(3);
          break;
        case YB:
          rotClock();
          flip(1);
          break;
        case YO:
          flip(1);
          break;
        case YG:
          rotCount();
          flip(1);
          break;
        case YR:
          flip(3);
          break;
        case GY:
          rotClock();
          flip(1);
          break;
        case GO:
          flip(1);
          break;
        case GW:
          rotCount();
          flip(1);
          break;
        case GR:
          flip(3);
          break;
        case WG:
          rotClock();
          flip(1);
          break;
        case WO:
          flip(1);
          break;
        case WB:
          rotCount();
          flip(1);
          break;
        case WR:
          flip(3);
          break; 
        case OW:
          flip(2);
          break;
        case OG:
          flip(2);
          break;
        case OY:
          flip(2);
          break;
        case OB:
          flip(2);
          break;
        case RY:
          break;
        case RG:
          break;
        case RW:
          break;
        case RB:
          break;
      }
      break;
    case 'U':
      switch (cube){
        case BW:
          flip(3);
          break;
        case BO:
          rotClock();
          flip(1);
          break;
        case BY:
          flip(1);
          break;
        case BR:
          rotCount();
          flip(1);
          break;
        case YB:
          flip(2);
          break;
        case YO:
          flip(2);
          break;
        case YG:
          flip(2);
          break;
        case YR:
          flip(2);
          break;
        case GY:
          flip(1);
          break;
        case GO:
          rotCount();
          flip(1);
          break;
        case GW:
          flip(3);
          break;
        case GR:
          rotClock();
          flip(1);
          break;
        case WG:
          break;
        case WO:
          break;
        case WB:
          break;
        case WR:
          break; 
        case OW:
          flip(3);
          break;
        case OG:
          rotClock();
          flip(1);
          break;
        case OY:
          flip(1);
          break;
        case OB:
          rotCount();
          flip(1);
          break;
        case RY:
          flip(1);
          break;
        case RG:
          rotCount();
          flip(1);
          break;
        case RW:
          flip(3);
          break;
        case RB:
          rotClock();
          flip(1);
          break;
      }
      break;
    case 'D':
      switch (cube){
        case BW:
          flip(1);
          break;
        case BO:
          rotCount();
          flip(1);
          break;
        case BY:
          flip(3);
          break;
        case BR:
          rotCount();
          flip(3);
          break;
        case YB:
          break;
        case YO:
          break;
        case YG:
          break;
        case YR:
          break;
        case GY:
          flip(3);
          break;
        case GO:
          rotCount();
          flip(3);
          break;
        case GW:
          flip(1);
          break;
        case GR:
          rotCount();
          flip(1);
          break;
        case WG:
          flip(2);
          break;
        case WO:
          flip(2);
          break;
        case WB:
          flip(2);
          break;
        case WR:
          flip(2);
          break; 
        case OW:
          flip(1);
          break;
        case OG:
          rotCount();
          flip(1);
          break;
        case OY:
          flip(3);
          break;
        case OB:
          rotClock();
          flip(1);
          break;
        case RY:
          flip(3);
          break;
        case RG:
          rotClock();
          flip(1);
          break;
        case RW:
          flip(1);
          break;
        case RB:
          rotCount();
          flip(1);
          break;
      }
      break;
    case 'L':
      switch (cube){
        case BW:
          rotCount();
          flip(1);
          break;
        case BO:
          flip(3);
          break;
        case BY:
          rotClock();
          flip(1);
          break;
        case BR:
          flip(1);
          break;
        case YB:
          rotCount();
          flip(1);
          break;
        case YO:
          flip(3);
          break;
        case YG:
          rotClock();
          flip(1);
          break;
        case YR:
          flip(1);
          break;
        case GY:
          rotCount();
          flip(1);
          break;
        case GO:
          flip(3);
          break;
        case GW:
          rotClock();
          flip(1);
          break;
        case GR:
          flip(1);
          break;
        case WG:
          rotCount();
          flip(1);
          break;
        case WO:
          flip(3);
          break;
        case WB:
          rotClock();
          flip(1);
          break;
        case WR:
          flip(1);
          break; 
        case OW:
          break;
        case OG:
          break;
        case OY:
          break;
        case OB:
          break;
        case RY:
          flip(2);
          break;
        case RG:
          flip(2);
          break;
        case RW:
          flip(2);
          break;
        case RB:
          flip(2);
          break;
      }
      break;
    case 'B':
      switch (cube){
        case BW:
          break;
        case BO:
          break;
        case BY:
          break;
        case BR:
          break;
        case YB:
          flip(3);
          break;
        case YO:
          rotClock();
          flip(1);
          break;
        case YG:
          flip(1);
          break;
        case YR:
          rotCount();
          flip(1);
          break;
        case GY:
          flip(2);
          break;
        case GO:
          flip(2);
          break;
        case GW:
          flip(2);
          break;
        case GR:
          flip(2);
          break;
        case WG:
          flip(1);
          break;
        case WO:
          rotCount();
          flip(1);
          break;
        case WB:
          flip(3);
          break;
        case WR:
          rotCount();
          flip(3);
          break; 
        case OW:
          rotClock();
          flip(1);
          break;
        case OG:
          flip(1);
          break;
        case OY:
          rotCount();
          flip(1);
          break;
        case OB:
          flip(3);
          break;
        case RY:
          rotClock();
          flip(1);
          break;
        case RG:
          flip(1);
          break;
        case RW:
         rotCount();
         flip(1);
          break;
        case RB:
          flip(3);
          break;
      }
  }
}
void makeMove(String x){  //makes a move with cubing notation as input (F, B2, R', etc...)
  Serial.println();
  Serial.print("Current Move: ");
  Serial.println(x);
  Serial.print("Start Orientation: ");
  printCube();
  if (x.length() == 1){
    turnToSide(x.charAt(0));
    turnClock(1);
  }else
  if ((x.length() == 2) && (x.charAt(1) == '\'')){
    turnToSide(x.charAt(0));
    turnCount(1);
  }
  else
  if ((x.length() == 2) && (x.charAt(1) == '2')){
    turnToSide(x.charAt(0));
    for (int i = 0; i < 2; i++){
      turnClock(1);
    }
  }
  Serial.print("End Orientation: ");
  printCube();
}
void flip(int times){  //flips the cube (x)
  for (int i = 0; i < times; i++){
    rest();
    for(int i = restDeg; i > flipDeg; i--){    //flipping
      Thing.write(i);
      delay(15);
    }
    delay(200);
    for(int i = flipDeg; i < restDeg; i++){    //retracting
      Thing.write(i);
      delay(2);
    }
    x();
    rest();
    Serial.print("flip to: ");
    printCube();
  }
}
void straighten(){
  Thing.write(restDeg);
  delay(wait);
  Thing.write(holdDeg);
  delay(wait);
  Thing.write(restDeg);
  delay(wait);
}
void rotClock(){  //y
  rest();
  Dcount();
  correct(true);
  y();
  Serial.print("rotClock to: ");
  printCube();
}
void rotCount(){  //y'
  rest();
  Dclock();
  correct(false);
  yp();
  Serial.print("rotCount to: ");
  printCube();
}
void yp(){  //simulates y'
  switch (cube){
    case BW:
      cube = BR;
      break;
    case BO:
      cube = BW;
      break;
    case BY:
      cube = BO;
      break;
    case BR:
      cube = BY;
      break;
    case YB:
      cube = YR;
      break;
    case YO:
      cube = YB;
      break;
    case YG:
      cube = YO;
      break;
    case YR:
      cube = YG;
      break;
    case GY:
      cube = GR;
      break;
    case GO:
      cube = GY;
      break;
    case GW:
      cube = GO;
      break;
    case GR:
      cube = GW;
      break;
    case WG:
      cube = WR;
      break;
    case WO:
      cube = WG;
      break;
    case WB:
      cube = WO;
      break;
    case WR:
      cube = WB;
      break; 
    case OW:
      cube = OB;
      break;
    case OG:
      cube = OW;
      break;
    case OY:
      cube = OG;
      break;
    case OB:
      cube = OY;
      break;
    case RY:
      cube = RB;
      break;
    case RG:
      cube = RY;
      break;
    case RW:
      cube = RG;
      break;
    case RB:
      cube = RW;
      break;
  }
}
void y(){   //simulates y
  switch (cube){
    case BW:
      cube = BO;
      break;
    case BO:
      cube = BY;
      break;
    case BY:
      cube = BR;
      break;
    case BR:
      cube = BW;
      break;
    case YB:
      cube = YO;
      break;
    case YO:
      cube = YG;
      break;
    case YG:
      cube = YR;
      break;
    case YR:
      cube = YB;
      break;
    case GY:
      cube = GO;
      break;
    case GO:
      cube = GW;
      break;
    case GW:
      cube = GR;
      break;
    case GR:
      cube = GY;
      break;
    case WG:
      cube = WO;
      break;
    case WO:
      cube = WB;
      break;
    case WB:
      cube = WR;
      break;
    case WR:
      cube = WG;
      break; 
    case OW:
      cube = OG;
      break;
    case OG:
      cube = OY;
      break;
    case OY:
      cube = OB;
      break;
    case OB:
      cube = OW;
      break;
    case RY:
      cube = RG;
      break;
    case RG:
      cube = RW;
      break;
    case RW:
      cube = RB;
      break;
    case RB:
      cube = RY;
      break;
  }
}
void x(){   //simulates x
  switch (cube){
    case BW:
      cube = YB;
      break;
    case BO:
      cube = RB;
      break;
    case BY:
      cube = WB;
      break;
    case BR:
      cube = OB;
      break;
    case YB:
      cube = GY;
      break;
    case YO:
      cube = RY;
      break;
    case YG:
      cube = BY;
      break;
    case YR:
      cube = OY;
      break;
    case GY:
      cube = WG;
      break;
    case GO:
      cube = RG;
      break;
    case GW:
      cube = YG;
      break;
    case GR:
      cube = OG;
      break;
    case WG:
      cube = BW;
      break;
    case WO:
      cube = RW;
      break;
    case WB:
      cube = GW;
      break;
    case WR:
      cube = OW;
      break; 
    case OW:
      cube = YO;
      break;
    case OG:
      cube = BO;
      break;
    case OY:
      cube = WO;
      break;
    case OB:
      cube = GO;
      break;
    case RY:
      cube = WR;
      break;
    case RG:
      cube = BR;
      break;
    case RW:
      cube = YR;
      break;
    case RB:
      cube = GR;
      break;
  }
}
