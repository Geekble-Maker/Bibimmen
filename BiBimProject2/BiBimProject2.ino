// 동작 시나리오2
// 시작 > 물이 나오고 > 버너가 켜짐 > 면을 보냄 > 타이머가 작동 > 3분 대기 > 타이머가 꺼짐/버너가 꺼짐/온수기 꺼짐 > 투석기 작동 >물펌프 동작 > 물펌프 정지
// 스피너 뒤집기 > 축이동 > 소스머신 작동 > 김가루 머신 > 참기름 머신 > 면 저어주는 기계
// 프로세서간의 통신은 High Low 정도로 줄 생각

// < 버너 시스템 >
// 초기 150 > 작동 0 > 1초뒤 > 작동 30

#include<Servo.h>
Servo gimServo;
Servo oilServo;

// ========== PIN DEFINE ==========
// START 
#define _pinBIBIMStart 52
// Move
#define _pinMoveIntSpeed 3
#define _pinMoveAlarm 4
#define _pinMoveAlarmReset 5
#define _pinMoveRunBreak 6
#define _pinMoveStartStop 7
#define _pinMoveSpeedIn 8 // PWM 127보다 클 때 전진 / 127보다 작 때 복귀
#define _pinMove_LIMIT_START 32 // 시작
#define _pinMove_LIMIT_STOP 33 // 끝
// SPINNER 
#define _pinSpinner_DIR 22 // 엎는게 HIGH / 돌아오는게 LOW
#define _pinSpinner_ENA 23
#define _pinSpinner_LIMIT_DOWN 34 // 엎어짐
#define _pinSpinner_LIMIT_UP 35 // 일어남
// SAUCE
#define _pinSauce_DIR 24 // 짜는게 HIGH / 푸는게 LOW
#define _pinSauce_ENA 25
#define _pinSauce_LIMIT_BACK 36 // 후방
#define _pinSauce_LIMIT_FRONT 37 // 전방
// GIM
#define _pinGim_Servo 9 // PWM 0 ->180
#define _pinGim_Sol 26
// OIL
#define _pinOil_Servo 10 // PWM 115 -> 180
// MIX
#define _pinMIX_DIR1 28 // 내려가는게 LOW / 올라가는게 HIGH
#define _pinMIX_ENA1 29
#define _pinMIX_DIR2 30
#define _pinMIX_ENA2 31
#define _pinMIX_LIMIT_UP 38
#define _pinMIX_LIMIT_DOWN 39 

// ========== PARAMETER ==========
#define _moveSauce_ms 23000
#define _moveGim_ms 17500
#define _moveOil_ms 17000

// ========== SYSTEM STATUS==========
int stateNum = 0;

void MoveMotorStop();
void MoveMotorRun(uint8_t _MotorSpeed);
void Action_Start();
void Action_Spinner();
void Action_Move_Sauce();
void Action_Sauce();
void Action_Move_Gim();
void Action_Gim();
void Action_Move_Oil();
void Action_Oil();
void Action_Move_Mix();
void Action_Mix();

// ==================== SYSTEM SETUP ======================
void setup()
{
    Serial.begin(9600);
    
    Init_Start();
    Init_Move();
    Init_Spinner();
    Init_Sauce();
    Init_Gim();
    Init_Oil();
    Init_Mix();

    stateNum = 0;
    Serial.println(stateNum);
    Serial.println("Initialize Completed!");
    Serial.println("============================");
    delay(1000);
}

void Init_Start ()
{
    pinMode(_pinBIBIMStart, INPUT);
    Serial.println("Initialize Start Module");
    delay(500);
}

void Init_Move(){
    pinMode(_pinMoveIntSpeed, OUTPUT);
    pinMode(_pinMoveAlarm, INPUT);
    pinMode(_pinMoveAlarmReset, OUTPUT);
    pinMode(_pinMoveRunBreak, OUTPUT);
    pinMode(_pinMoveStartStop, OUTPUT);
    pinMode(_pinMoveSpeedIn, OUTPUT);
    pinMode(_pinMove_LIMIT_START, INPUT_PULLUP);
    pinMode(_pinMove_LIMIT_STOP, INPUT_PULLUP);
    delay(10);
    digitalWrite(_pinMoveIntSpeed, HIGH);
    digitalWrite(_pinMoveAlarmReset, LOW);
    digitalWrite(_pinMoveRunBreak, LOW);
    digitalWrite(_pinMoveStartStop, HIGH);
    analogWrite(_pinMoveSpeedIn, 127);

    Serial.println("Initialize Move Module");
    delay(500);
}

void Init_Spinner()
{
    pinMode(_pinSpinner_DIR, OUTPUT);
    pinMode(_pinSpinner_ENA, OUTPUT);
    pinMode(_pinSpinner_LIMIT_DOWN, INPUT_PULLUP);
    pinMode(_pinSpinner_LIMIT_UP, INPUT_PULLUP);
    delay(10);
    digitalWrite(_pinSpinner_DIR, LOW);
    digitalWrite(_pinSpinner_ENA, LOW);

    Serial.println("Initialize Spinner Module");
    delay(500);
}

void Init_Sauce()
{
    pinMode(_pinSauce_DIR, OUTPUT);
    pinMode(_pinSauce_ENA, OUTPUT);
    pinMode(_pinSauce_LIMIT_BACK, INPUT_PULLUP);
    pinMode(_pinSauce_LIMIT_FRONT, INPUT_PULLUP);

    delay(10);
    digitalWrite(_pinSauce_DIR, LOW);
    digitalWrite(_pinSauce_ENA, LOW);

    Serial.println("Initialize Sauce Module");
    delay(500);
}

void Init_Gim()
{
    pinMode(_pinGim_Servo, OUTPUT);
    pinMode(_pinGim_Sol, OUTPUT);
    delay(10);
    gimServo.attach(_pinGim_Servo);
    gimServo.write(0);
    digitalWrite(_pinGim_Sol, LOW);
    
    Serial.println("Initialize Gim Module");
    delay(500);
}

void Init_Oil()
{
    pinMode(_pinOil_Servo, OUTPUT);
    delay(10);
    oilServo.attach(_pinOil_Servo);
    oilServo.write(125);
    Serial.println("Initialize Oil Module");
    delay(500);
}

void Init_Mix()
{
    pinMode(_pinMIX_DIR1, OUTPUT);
    pinMode(_pinMIX_DIR2, OUTPUT);
    pinMode(_pinMIX_ENA1, OUTPUT);
    pinMode(_pinMIX_ENA2, OUTPUT);
    pinMode(_pinMIX_LIMIT_UP, INPUT_PULLUP);
    pinMode(_pinMIX_LIMIT_DOWN, INPUT_PULLUP);
    delay(10);
    digitalWrite(_pinMIX_DIR1, LOW);
    digitalWrite(_pinMIX_DIR2, LOW);
    digitalWrite(_pinMIX_ENA1, LOW);
    digitalWrite(_pinMIX_ENA2, LOW);

    Serial.println("Initialize Mix Module");
    delay(500);
}

// ==================== SYSTEM LOOP ======================
void loop()
{
    ModeMap();
    inputTest();
    SwitchLog();
    delay(100);  
}

void inputTest(){
  if (Serial.available() > 0) {
      // read the incoming byte:
      char input = Serial.read();
  
      if(input == 'p'){
        Serial.println("Input 'P'!");
        stateNum = 1;
        input = 'm';
      }
      else if(input == 'q'){
        Serial.println("Spinner Go!");
        digitalWrite(_pinSpinner_DIR, HIGH);
        digitalWrite(_pinSpinner_ENA, HIGH);
        delay(10);
        digitalWrite(_pinSpinner_ENA, LOW);
      }
      else if(input == 'w'){
        Serial.println("Spinner Back!");
        digitalWrite(_pinSpinner_DIR, LOW);
        digitalWrite(_pinSpinner_ENA, HIGH);
        delay(10);
        digitalWrite(_pinSpinner_ENA, LOW);
      }
      else if(input == 'e'){
        digitalWrite(_pinSauce_DIR, HIGH);
        digitalWrite(_pinSauce_ENA, HIGH);
        delay(10);
        digitalWrite(_pinSauce_ENA, LOW);
      }
      else if(input == 'r'){
        digitalWrite(_pinSauce_DIR, LOW);
        digitalWrite(_pinSauce_ENA, HIGH);
        delay(10);
        digitalWrite(_pinSauce_ENA, LOW);
      }
      else if(input == 't'){
        digitalWrite(_pinMIX_DIR1, LOW);
        digitalWrite(_pinMIX_ENA1, HIGH);
        delay(100);
        digitalWrite(_pinMIX_ENA1, LOW);
      }
      else if(input == 'y'){
        digitalWrite(_pinMIX_DIR1, HIGH);
        digitalWrite(_pinMIX_ENA1, HIGH);
        delay(100);
        digitalWrite(_pinMIX_ENA1, LOW);
      }
      else if(input == 'g'){
        digitalWrite(_pinMIX_DIR2, LOW);
        digitalWrite(_pinMIX_ENA2, HIGH);
        delay(1000);
        digitalWrite(_pinMIX_ENA2, LOW);
      }
      else if(input == 'h'){
        digitalWrite(_pinMIX_DIR2, HIGH);
        digitalWrite(_pinMIX_ENA2, HIGH);
        delay(1000);
        digitalWrite(_pinMIX_ENA2, LOW);
      }
      else if(input == 'a'){ // 전진  
        int inputValue = 77;
        Serial.print("MoveMotor Go: ");
        Serial.println(inputValue);
        MoveMotorRun(inputValue);
        delay(1000);
        MoveMotorStop();
      }
      else if(input == 's'){ // 복귀
        int inputValue = 177;
        Serial.print("MoveMotor Back!: ");
        Serial.println(inputValue);
        MoveMotorRun(inputValue);
        delay(1000);
        MoveMotorStop();
      }
      else if(input == 'd'){ // 김가루
        gimServo.write(180);
        delay(1000);
        gimServo.write(0);
      }
      else if(input == 'f'){ // 오일
        oilServo.write(180);
        delay(1000);
        oilServo.write(125);
      }
      else if(input == 'j'){
        digitalWrite(_pinSauce_DIR, HIGH);
        digitalWrite(_pinSauce_ENA, HIGH);
        delay(2500);
        digitalWrite(_pinSauce_DIR, LOW);
        while(true){
          if(digitalRead(_pinSauce_LIMIT_FRONT) == LOW){
            digitalWrite(_pinSauce_ENA, LOW);
            break;
          }
        digitalWrite(_pinSauce_ENA, HIGH);
        delay(10);
        digitalWrite(_pinSauce_ENA, LOW);
        }
      }
      else if(input == 'k'){
        digitalWrite(_pinSpinner_DIR, HIGH);
        while(true){
          if(digitalRead(_pinSpinner_LIMIT_UP) == LOW){
            digitalWrite(_pinSpinner_ENA, LOW);
            break;
          }
          digitalWrite(_pinSpinner_ENA, HIGH);
          delay(10);
          digitalWrite(_pinSpinner_ENA, LOW);
        }
    
        delay(2000);
        digitalWrite(_pinSpinner_DIR, LOW);
        while(true){
          if(digitalRead(_pinSpinner_LIMIT_DOWN) == LOW){
            digitalWrite(_pinSpinner_ENA, LOW);
            break;
          }
          digitalWrite(_pinSpinner_ENA, HIGH);
          delay(10);
          digitalWrite(_pinSpinner_ENA, LOW);
        }
        delay(2000);
      }
      else if(input == 'i'){
        InitializeAll();
      }
  }
}

void ModeMap(){
    Serial.print("stateNum: ");
    Serial.println(stateNum);
    switch (stateNum)
    {
    case 0:
        Action_Start();
        break;
    case 1:
        Action_Spinner();
        break;
    case 2:
        Action_Move_Sauce();
        break;
    case 3:
        Action_Sauce();
        break;
    case 4:
        Action_Move_Gim();
        break;
    case 5:
        Action_Gim();
        break;
    case 6:
        Action_Move_Oil();
        break;
    case 7:
        Action_Oil();
        break;
    case 8:
        Action_Move_Mix();
        break;
    case 9:
        Action_Mix();
        break;
    default:
        break;
    }
}

void SwitchLog(){
  Serial.print("MOVE_S: ");
  Serial.print(digitalRead(_pinMove_LIMIT_START));
  Serial.print(" MOVE_D: ");
  Serial.print(digitalRead(_pinMove_LIMIT_STOP));
  Serial.print(" SPIN_UP: ");
  Serial.print(digitalRead(_pinSpinner_LIMIT_UP));
  Serial.print(" SPIN_DOWN: ");
  Serial.print(digitalRead(_pinSpinner_LIMIT_DOWN));
  Serial.print(" SAUCE_F: ");
  Serial.print(digitalRead(_pinSauce_LIMIT_FRONT));
  Serial.print(" SAUCE_B: ");
  Serial.print(digitalRead(_pinSauce_LIMIT_BACK));
  Serial.print(" MIX_U: ");
  Serial.print(digitalRead(_pinMIX_LIMIT_UP));
  Serial.print(" MIX_D: ");
  Serial.println(digitalRead(_pinMIX_LIMIT_DOWN));
}

void InitializeAll(){
    Serial.println("Initialize All Status!");

    stateNum = 0;

    // 비빔이 초기화 LOW/HIGH
    digitalWrite(_pinMIX_DIR1, HIGH);
    while(true){
      if(digitalRead(_pinMIX_LIMIT_UP) == HIGH){      
        digitalWrite(_pinMIX_ENA1, LOW);
        break;
      }
      digitalWrite(_pinMIX_ENA1, HIGH);
      delay(100);
      digitalWrite(_pinMIX_ENA1, LOW);
//      delay(100);
    }
    Serial.println("Initialize Mix");
    delay(1000);
    
    // 축 초기화 HIGH/LOW
    MoveMotorRun(207);
    while(true){
      if(digitalRead(_pinMove_LIMIT_START) == HIGH){
        MoveMotorStop();
        break;
      }
      delay(10);
    }

    MoveMotorRun(27);
    delay(10000);
    MoveMotorStop();
    
    Serial.println("Initialize MoveModule");
    delay(1000);

    // 뒤집기 초기화 HIGH/LOW
    digitalWrite(_pinSpinner_DIR, LOW);
    while(true){
      if(digitalRead(_pinSpinner_LIMIT_DOWN) == LOW){
        digitalWrite(_pinSpinner_ENA, LOW);
        break;
      }
      digitalWrite(_pinSpinner_ENA, HIGH);
      delay(10);
      digitalWrite(_pinSpinner_ENA, LOW);
      delay(500);
    }
    Serial.println("Initialize Spinner");
    delay(1000);

    // 소스 초기화 HIGH/LOW
    digitalWrite(_pinSauce_DIR, LOW);
    while(true){
      if(digitalRead(_pinSauce_LIMIT_FRONT) == LOW){
        digitalWrite(_pinSauce_ENA, LOW);
        break;
      }
      digitalWrite(_pinSauce_ENA, HIGH);
      delay(10);
      digitalWrite(_pinSauce_ENA, LOW);
      delay(100);
    }
    Serial.println("Initialize Sauce");
    delay(1000);

    // 김가루 초기화
    gimServo.write(0);
    Serial.println("Initialize Gim");
    delay(1000);

    // 참기름 초기화
    oilServo.write(115);
    Serial.println("Initialize Oil");
    delay(1000);
}


void Action_Start(){
  
    Serial.print("Ready to Next Start: ");
    Serial.println(digitalRead(_pinBIBIMStart));
    if (digitalRead(_pinBIBIMStart) == 1)
    {
        Serial.println("Action Next Start!");
        delay(2000);
        stateNum = 1;
    }
    delay(100);
}

int spinnerMode = 0;
void Action_Spinner(){ // HIGH/LOW
  if(spinnerMode == 0){
    Serial.println("Action Spinner!!");

    MoveMotorRun(207);
    while(true){
      if(digitalRead(_pinMove_LIMIT_START) == HIGH){
        MoveMotorStop();
        break;
      }
      delay(10);
    }
    delay(2000);
    
    digitalWrite(_pinSpinner_DIR, HIGH);
    while(true){
      if(digitalRead(_pinSpinner_LIMIT_UP) == LOW){
        digitalWrite(_pinSpinner_ENA, LOW);
        break;
      }
      digitalWrite(_pinSpinner_ENA, HIGH);
      delay(10);
      digitalWrite(_pinSpinner_ENA, LOW);
    }

    delay(2000);
    spinnerMode = 1;
  }
  else if(spinnerMode == 1){
    digitalWrite(_pinSpinner_DIR, LOW);
    while(true){
      if(digitalRead(_pinSpinner_LIMIT_DOWN) == LOW){
        digitalWrite(_pinSpinner_ENA, LOW);
        break;
      }
      digitalWrite(_pinSpinner_ENA, HIGH);
      delay(10);
      digitalWrite(_pinSpinner_ENA, LOW);
    }
    delay(2000);
    spinnerMode = 2;
    stateNum = 2;
  }
}

void Action_Move_Sauce(){
  Serial.println("Action Move Sauce!!");
  MoveMotorRun(77);
  delay(_moveSauce_ms);
  MoveMotorStop();

  delay(2000);
  stateNum = 3;
}

void Action_Sauce(){
  digitalWrite(_pinSauce_DIR, HIGH);
  digitalWrite(_pinSauce_ENA, HIGH);
  delay(2500);
  digitalWrite(_pinSauce_DIR, LOW);
  while(true){
    if(digitalRead(_pinSauce_LIMIT_FRONT) == LOW){
      digitalWrite(_pinSauce_ENA, LOW);
      break;
    }
  digitalWrite(_pinSauce_ENA, HIGH);
  delay(10);
  digitalWrite(_pinSauce_ENA, LOW);
  }

  delay(2000);
  stateNum = 4;
  
}

void Action_Move_Gim(){
  MoveMotorRun(77);
  delay(_moveGim_ms);
  MoveMotorStop();

  delay(2000);
  stateNum = 5;
}

void Action_Gim(){
  gimServo.write(180);

  delay(1000);

  for(int i=0;i<20;i++){
    digitalWrite(_pinGim_Sol, HIGH);
    delay(50);
    digitalWrite(_pinGim_Sol, LOW);
    delay(50);
  }

  delay(1000);
  gimServo.write(0);

  delay(2000);
  stateNum = 6;
}

void Action_Move_Oil(){
  MoveMotorRun(77);
  delay(_moveOil_ms);
  MoveMotorStop();

  delay(2000);
  stateNum = 7;
}

void Action_Oil(){
  oilServo.write(180);
  delay(2000);
  oilServo.write(125);
  delay(1000);
  oilServo.write(180);
  delay(2000);
  oilServo.write(125);
  
  delay(2000);
  stateNum = 8;
}

void Action_Move_Mix(){

  MoveMotorRun(77);
  while(true){
    if(digitalRead(_pinMove_LIMIT_STOP) == HIGH){
      MoveMotorStop();
      break;
    }
    delay(10);
  }
    
  delay(2000);
  stateNum = 9;
}

int mixMode = 0;
void Action_Mix(){
  if(mixMode == 0){
    digitalWrite(_pinMIX_DIR1, LOW);
    while(true){
      if(digitalRead(_pinMIX_LIMIT_DOWN) == LOW){      
        digitalWrite(_pinMIX_ENA1, LOW);
        break;
      }
      digitalWrite(_pinMIX_ENA1, HIGH);
      delay(100);
      digitalWrite(_pinMIX_ENA1, LOW);
    }

    delay(2000);
    mixMode = 1;
  }
  else if(mixMode == 1){
    digitalWrite(_pinMIX_DIR2, LOW);
    digitalWrite(_pinMIX_ENA2, HIGH);
    delay(10000);
    digitalWrite(_pinMIX_ENA2, LOW);
    delay(2000);
    digitalWrite(_pinMIX_DIR2, HIGH);
    digitalWrite(_pinMIX_ENA2, HIGH);
    delay(10000);
    digitalWrite(_pinMIX_ENA2, LOW);
    delay(2000);

    mixMode = 2; 
  }
  else if(mixMode == 2){
    digitalWrite(_pinMIX_DIR1, HIGH);
    while(true){
      if(digitalRead(_pinMIX_LIMIT_UP) == HIGH){      
        digitalWrite(_pinMIX_ENA1, LOW);
        break;
      }
      digitalWrite(_pinMIX_ENA1, HIGH);
      delay(100);
      digitalWrite(_pinMIX_ENA1, LOW);
    }

    delay(2000);
    mixMode = 3;
    stateNum = 0;
  }
}

void MoveMotorStop()
{
  digitalWrite(_pinMoveRunBreak, HIGH);
  digitalWrite(_pinMoveStartStop, HIGH);
  analogWrite(_pinMoveSpeedIn, 127);
  Serial.println("MoveMotor Stop");
}

void MoveMotorRun(uint8_t _MotorSpeed)
{
  if(digitalRead(_pinMoveAlarm) == 1)
  {
    digitalWrite(_pinMoveStartStop, HIGH);
    analogWrite(_pinMoveSpeedIn, 127);
    digitalWrite(_pinMoveAlarmReset, HIGH);
    delay(500);
    digitalWrite(_pinMoveAlarmReset, LOW);
    Serial.println("Alarm Reset");
  }
  analogWrite(_pinMoveSpeedIn, _MotorSpeed);
  digitalWrite(_pinMoveRunBreak, LOW);
  digitalWrite(_pinMoveStartStop, LOW);
}
