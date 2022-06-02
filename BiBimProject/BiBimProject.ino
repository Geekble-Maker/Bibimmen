// 동작 시나리오
// 시작 > 물이 나오고 > 버너가 켜짐 > 면을 보냄 > 타이머가 작동 > 3분 대기 > 타이머가 꺼짐/버너가 꺼짐/온수기 꺼짐 > 투석기 작동 >물펌프 동작 > 물펌프 정지
// 스피너 뒤집기 > 축이동 > 소스머신 작동 > 김가루 머신 > 참기름 머신 > 면 저어주는 기계
// 프로세서간의 통신은 High Low 정도로 줄 생각
// < 버너 시스템 >
// 초기 150 > 작동 0 > 1초뒤 > 작동 30

#include<Servo.h>
Servo burnerServo;
// ========== PIN DEFINE ==========
// START
#define _pinBIBIMStart 2
// KETTLE
#define _pinKettle_AC 3
//#define _pinKettle_Temp
#define _pinKettle_PowerLock 4
#define _pinKettle_mLSupply 5
// BURNER
#define _pinBurner 6 //
// SENDER
#define _pinSender_CLK 7
#define _pinSender_DIR 8
// TIMER
#define _pinTimer_Reset 9
#define _pinTimer_Start 10
#define _pinTimer_Mode 11
// CATAPULT
#define _pinCatapult_DIR 12
#define _pinCatapult_ENA 13
// PUMP
#define _pinPump A0
// ARDUINO
#define _pinArduino A1
// ========== PARAMETER ==========
#define _click_ms 600
#define _longClick_ms 3500
#define _cookTime 240000 //180000
#define _kettleTime 1000 //62000
#define _waterTime 27000
// ========== SYSTEM STATUS==========
enum CURRENT_STATE
{
  START,
  KETTLE_ON,
  BURNER_ON,
  SENDER,
  TIMER_ON,
  KETTLE_BURNER_TIMER_OFF,
  CATAPULT,
  PUMP,
  NEXT,
  END
} state;
// ==================== SYSTEM SETUP ======================
void setup()
{
    Serial.begin(9600);
    Init_Start();
    Init_Kettle();
    Init_Burner();
    Init_Sender();
    Init_Timer();
    Init_Catapult();
    Init_Pump();
    Init_Arduino();
    state = START;
    Serial.println(state);
    Serial.println("Initialize Completed!");
    Serial.println("============================");
    delay(1000);
}
void Init_Start ()
{
    pinMode(_pinBIBIMStart, INPUT_PULLUP);
    Serial.println("Initialize Start Module");
    delay(500);
}
void Init_Kettle()
{
    pinMode(_pinKettle_AC, OUTPUT);
    //pinMode(_pinKettle_Temp, OUTPUT);
    pinMode(_pinKettle_PowerLock, OUTPUT);
    pinMode(_pinKettle_mLSupply, OUTPUT);
    delay(10);
    digitalWrite(_pinKettle_AC, LOW);
    //digitalWrite(_pinKettle_Temp, LOW);
    digitalWrite(_pinKettle_PowerLock, LOW);
    digitalWrite(_pinKettle_mLSupply, LOW);
    Serial.println("Initialize Kettle Module");
    delay(500);
}
void Init_Burner ()
{
    burnerServo.attach(_pinBurner);
    delay(10);
    burnerServo.write(150);
    Serial.println("Initialize Burner Module");
    delay(500);
}
void Init_Sender ()
{
    pinMode(_pinSender_CLK, OUTPUT);
    pinMode(_pinSender_DIR, OUTPUT);
    delay(10);
    digitalWrite(_pinSender_CLK, HIGH);
    digitalWrite(_pinSender_DIR, LOW);
    Serial.println("Initialize Sender Module");
    delay(500);
}
void Init_Timer ()
{
    pinMode(_pinTimer_Reset, OUTPUT);
    pinMode(_pinTimer_Start, OUTPUT);
    pinMode(_pinTimer_Mode, OUTPUT);
    delay(10);
    digitalWrite(_pinTimer_Reset, LOW);
    digitalWrite(_pinTimer_Start, LOW);
    digitalWrite(_pinTimer_Mode, LOW);
    Serial.println("Initialize Timer Module");
    delay(500);
}
void Init_Catapult()
{
    pinMode(_pinCatapult_DIR, OUTPUT);
    pinMode(_pinCatapult_ENA, OUTPUT);
    delay(10);
    digitalWrite(_pinCatapult_DIR, LOW);
    digitalWrite(_pinCatapult_ENA, LOW);
    Serial.println("Initialize Catapult Module");
    delay(500);
}
void Init_Pump(){
    pinMode(_pinPump, OUTPUT);
    delay(10);
    digitalWrite(_pinPump, LOW);
    Serial.println("Initialize Pump Module");
    delay(500);
}
void Init_Arduino(){
    pinMode(_pinArduino,OUTPUT);
    delay(10);
    digitalWrite(_pinArduino,LOW);
    Serial.println("Initialize Arduino Module");
    delay(500);
}
// ==================== SYSTEM LOOP ======================
void loop()
{
   if (Serial.available() > 0) {
      // read the incoming byte:
      char input = Serial.read();
      if(input == 'a'){
          Serial.println("Run Catapult Motor CCW!");
          digitalWrite(_pinCatapult_DIR, HIGH);
          digitalWrite(_pinCatapult_ENA, HIGH);
          delay(500);
          digitalWrite(_pinCatapult_ENA, LOW);
      }
      else if(input == 's'){
          Serial.println("Run Catapult Motor CW!");
          digitalWrite(_pinCatapult_DIR, LOW);
          digitalWrite(_pinCatapult_ENA, HIGH);
          delay(500);
          digitalWrite(_pinCatapult_ENA, LOW);
      }
      else if(input == 'o'){
        Serial.println("Run Pump Motor!");
        digitalWrite(_pinPump, HIGH);
      }
      else if(input == 'p'){
        Serial.println("Stop Pump Motor!");
        digitalWrite(_pinPump, HIGH);
        delay(1000);
        digitalWrite(_pinPump, LOW);
      }
      else if(input == 'n'){
        Serial.println("Run Next Arduino");
        digitalWrite(_pinArduino,HIGH);
        delay(1000);
        digitalWrite(_pinArduino,LOW);
      }
      else if(input == 'q'){
        Serial.println("Run Burner On");
        burnerServo.write(0);
      }
      else if(input == 'w'){
        Serial.println("Run Burner off");
        burnerServo.write(150);
      }
    }
    switch (state)
    {
    case START:
        Action_Start();
        break;
    case KETTLE_ON:
        Action_Kettle_On();
        break;
    case BURNER_ON:
        Action_Burner_On();
        break;
    case SENDER:
        Action_Sender();
        break;
    case TIMER_ON:
        Action_Timer_On();
        break;
    case KETTLE_BURNER_TIMER_OFF:
        Action_Kettle_Off();
        Action_Burner_Off();
        Action_Timer_Off();
        break;
    case CATAPULT:
        Action_Catapult();
        break;
    case PUMP:
        Action_Pump();
        break;
    case NEXT:
        Action_Next();
    default:
        break;
    }
}
void Action_Start(){
    Serial.println("Ready to Start!");
    if (digitalRead(_pinBIBIMStart) == 0)
    {
        Serial.println("Action Start!");
        delay(2000);
        state = KETTLE_ON;
    }
    delay(100);
}
void Action_Kettle_On(){
    Serial.println("Action Kettle On!");
    // AC전원 공급
    digitalWrite(_pinKettle_AC, HIGH);
    delay(_longClick_ms);
    // Power Lock
    ButtonClick(_pinKettle_PowerLock);
    // 물 공급하기
    ButtonClick(_pinKettle_mLSupply);
    delay(_kettleTime);
    state = BURNER_ON;
}
void Action_Burner_On(){
    Serial.println("Action Burner On!");
    burnerServo.write(0);
    delay(2000);
    state = SENDER;
}
void Action_Sender(){
    Serial.println("Action Sender!");
    digitalWrite(_pinSender_DIR, LOW);
    for (uint16_t _servo_steps = 0; _servo_steps < 2000; _servo_steps ++)
    {
        digitalWrite(_pinSender_CLK, HIGH);
        delayMicroseconds(3000);
        digitalWrite(_pinSender_CLK, LOW);
        delayMicroseconds(3000);
    }
    digitalWrite(_pinSender_DIR, HIGH);
    for (uint16_t _servo_steps = 0; _servo_steps < 700; _servo_steps ++)
    {
        digitalWrite(_pinSender_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(_pinSender_CLK, LOW);
        delayMicroseconds(700);
    }
    delay(2000);
    state = TIMER_ON;
}
void Action_Timer_On()
{
    Serial.println("Action Timer On!");
    ButtonClick(_pinTimer_Mode);
    ButtonClick(_pinTimer_Start);
    delay(_cookTime);
    state = KETTLE_BURNER_TIMER_OFF;
}
void Action_Kettle_Off(){
    digitalWrite(_pinKettle_AC, LOW);
}
void Action_Burner_Off(){
    burnerServo.write(150);
}
void Action_Timer_Off()
{
    Serial.println("Action kettle/Burner/Timer Off!");
    ButtonClick(_pinTimer_Start);
    ButtonClick(_pinTimer_Reset);
    delay(2000);
    state = CATAPULT;
}
void Action_Catapult(){
    Serial.println("Action Catapult!");
    digitalWrite(_pinCatapult_DIR, LOW);
    digitalWrite(_pinCatapult_ENA, HIGH);
    delay(3000);
    digitalWrite(_pinCatapult_ENA, LOW);
    delay(2000);
    state = PUMP;
}
void Action_Pump(){
    Serial.println("Action Pump!");
    digitalWrite(_pinPump, HIGH);
    delay(_waterTime);
    digitalWrite(_pinPump, LOW);
    delay(10000);
    state = NEXT;
}
void Action_Next(){
    Serial.println("Action Next!");
    digitalWrite(_pinArduino,HIGH);
    delay(1000);
    digitalWrite(_pinArduino,LOW);
    delay(2000);
    state = START;
}
// ======== SUB FUNCTION =========
void ButtonClick (uint8_t _button)
{
    digitalWrite(_button, HIGH);
    delay(_click_ms);
    digitalWrite(_button, LOW);
    delay(_click_ms);
}
void ButtonLongClick (uint8_t _button)
{
    digitalWrite(_button, HIGH);
    delay(_longClick_ms);
    digitalWrite(_button, LOW);
    delay(_click_ms);
}// 동작 시나리오
// 시작 > 물이 나오고 > 버너가 켜짐 > 면을 보냄 > 타이머가 작동 > 3분 대기 > 타이머가 꺼짐/버너가 꺼짐/온수기 꺼짐 > 투석기 작동 >물펌프 동작 > 물펌프 정지
// 스피너 뒤집기 > 축이동 > 소스머신 작동 > 김가루 머신 > 참기름 머신 > 면 저어주는 기계
// 프로세서간의 통신은 High Low 정도로 줄 생각
// < 버너 시스템 >
// 초기 150 > 작동 0 > 1초뒤 > 작동 30

#include<Servo.h>
Servo burnerServo;
// ========== PIN DEFINE ==========
// START
#define _pinBIBIMStart 2
// KETTLE
#define _pinKettle_AC 3
//#define _pinKettle_Temp
#define _pinKettle_PowerLock 4
#define _pinKettle_mLSupply 5
// BURNER
#define _pinBurner 6 //
// SENDER
#define _pinSender_CLK 7
#define _pinSender_DIR 8
// TIMER
#define _pinTimer_Reset 9
#define _pinTimer_Start 10
#define _pinTimer_Mode 11
// CATAPULT
#define _pinCatapult_DIR 12
#define _pinCatapult_ENA 13
// PUMP
#define _pinPump A0
// ARDUINO
#define _pinArduino A1
// ========== PARAMETER ==========
#define _click_ms 600
#define _longClick_ms 3500
#define _cookTime 240000 //180000
#define _kettleTime 1000 //62000
#define _waterTime 27000
// ========== SYSTEM STATUS==========
enum CURRENT_STATE
{
  START,
  KETTLE_ON,
  BURNER_ON,
  SENDER,
  TIMER_ON,
  KETTLE_BURNER_TIMER_OFF,
  CATAPULT,
  PUMP,
  NEXT,
  END
} state;
// ==================== SYSTEM SETUP ======================
void setup()
{
    Serial.begin(9600);
    Init_Start();
    Init_Kettle();
    Init_Burner();
    Init_Sender();
    Init_Timer();
    Init_Catapult();
    Init_Pump();
    Init_Arduino();
    state = START;
    Serial.println(state);
    Serial.println("Initialize Completed!");
    Serial.println("============================");
    delay(1000);
}
void Init_Start ()
{
    pinMode(_pinBIBIMStart, INPUT_PULLUP);
    Serial.println("Initialize Start Module");
    delay(500);
}
void Init_Kettle()
{
    pinMode(_pinKettle_AC, OUTPUT);
    //pinMode(_pinKettle_Temp, OUTPUT);
    pinMode(_pinKettle_PowerLock, OUTPUT);
    pinMode(_pinKettle_mLSupply, OUTPUT);
    delay(10);
    digitalWrite(_pinKettle_AC, LOW);
    //digitalWrite(_pinKettle_Temp, LOW);
    digitalWrite(_pinKettle_PowerLock, LOW);
    digitalWrite(_pinKettle_mLSupply, LOW);
    Serial.println("Initialize Kettle Module");
    delay(500);
}
void Init_Burner ()
{
    burnerServo.attach(_pinBurner);
    delay(10);
    burnerServo.write(150);
    Serial.println("Initialize Burner Module");
    delay(500);
}
void Init_Sender ()
{
    pinMode(_pinSender_CLK, OUTPUT);
    pinMode(_pinSender_DIR, OUTPUT);
    delay(10);
    digitalWrite(_pinSender_CLK, HIGH);
    digitalWrite(_pinSender_DIR, LOW);
    Serial.println("Initialize Sender Module");
    delay(500);
}
void Init_Timer ()
{
    pinMode(_pinTimer_Reset, OUTPUT);
    pinMode(_pinTimer_Start, OUTPUT);
    pinMode(_pinTimer_Mode, OUTPUT);
    delay(10);
    digitalWrite(_pinTimer_Reset, LOW);
    digitalWrite(_pinTimer_Start, LOW);
    digitalWrite(_pinTimer_Mode, LOW);
    Serial.println("Initialize Timer Module");
    delay(500);
}
void Init_Catapult()
{
    pinMode(_pinCatapult_DIR, OUTPUT);
    pinMode(_pinCatapult_ENA, OUTPUT);
    delay(10);
    digitalWrite(_pinCatapult_DIR, LOW);
    digitalWrite(_pinCatapult_ENA, LOW);
    Serial.println("Initialize Catapult Module");
    delay(500);
}
void Init_Pump(){
    pinMode(_pinPump, OUTPUT);
    delay(10);
    digitalWrite(_pinPump, LOW);
    Serial.println("Initialize Pump Module");
    delay(500);
}
void Init_Arduino(){
    pinMode(_pinArduino,OUTPUT);
    delay(10);
    digitalWrite(_pinArduino,LOW);
    Serial.println("Initialize Arduino Module");
    delay(500);
}
// ==================== SYSTEM LOOP ======================
void loop()
{
   if (Serial.available() > 0) {
      // read the incoming byte:
      char input = Serial.read();
      if(input == 'a'){
          Serial.println("Run Catapult Motor CCW!");
          digitalWrite(_pinCatapult_DIR, HIGH);
          digitalWrite(_pinCatapult_ENA, HIGH);
          delay(500);
          digitalWrite(_pinCatapult_ENA, LOW);
      }
      else if(input == 's'){
          Serial.println("Run Catapult Motor CW!");
          digitalWrite(_pinCatapult_DIR, LOW);
          digitalWrite(_pinCatapult_ENA, HIGH);
          delay(500);
          digitalWrite(_pinCatapult_ENA, LOW);
      }
      else if(input == 'o'){
        Serial.println("Run Pump Motor!");
        digitalWrite(_pinPump, HIGH);
      }
      else if(input == 'p'){
        Serial.println("Stop Pump Motor!");
        digitalWrite(_pinPump, HIGH);
        delay(1000);
        digitalWrite(_pinPump, LOW);
      }
      else if(input == 'n'){
        Serial.println("Run Next Arduino");
        digitalWrite(_pinArduino,HIGH);
        delay(1000);
        digitalWrite(_pinArduino,LOW);
      }
      else if(input == 'q'){
        Serial.println("Run Burner On");
        burnerServo.write(0);
      }
      else if(input == 'w'){
        Serial.println("Run Burner off");
        burnerServo.write(150);
      }
    }
    switch (state)
    {
    case START:
        Action_Start();
        break;
    case KETTLE_ON:
        Action_Kettle_On();
        break;
    case BURNER_ON:
        Action_Burner_On();
        break;
    case SENDER:
        Action_Sender();
        break;
    case TIMER_ON:
        Action_Timer_On();
        break;
    case KETTLE_BURNER_TIMER_OFF:
        Action_Kettle_Off();
        Action_Burner_Off();
        Action_Timer_Off();
        break;
    case CATAPULT:
        Action_Catapult();
        break;
    case PUMP:
        Action_Pump();
        break;
    case NEXT:
        Action_Next();
    default:
        break;
    }
}
void Action_Start(){
    Serial.println("Ready to Start!");
    if (digitalRead(_pinBIBIMStart) == 0)
    {
        Serial.println("Action Start!");
        delay(2000);
        state = KETTLE_ON;
    }
    delay(100);
}
void Action_Kettle_On(){
    Serial.println("Action Kettle On!");
    // AC전원 공급
    digitalWrite(_pinKettle_AC, HIGH);
    delay(_longClick_ms);
    // Power Lock
    ButtonClick(_pinKettle_PowerLock);
    // 물 공급하기
    ButtonClick(_pinKettle_mLSupply);
    delay(_kettleTime);
    state = BURNER_ON;
}
void Action_Burner_On(){
    Serial.println("Action Burner On!");
    burnerServo.write(0);
    delay(2000);
    state = SENDER;
}
void Action_Sender(){
    Serial.println("Action Sender!");
    digitalWrite(_pinSender_DIR, LOW);
    for (uint16_t _servo_steps = 0; _servo_steps < 2000; _servo_steps ++)
    {
        digitalWrite(_pinSender_CLK, HIGH);
        delayMicroseconds(3000);
        digitalWrite(_pinSender_CLK, LOW);
        delayMicroseconds(3000);
    }
    digitalWrite(_pinSender_DIR, HIGH);
    for (uint16_t _servo_steps = 0; _servo_steps < 700; _servo_steps ++)
    {
        digitalWrite(_pinSender_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(_pinSender_CLK, LOW);
        delayMicroseconds(700);
    }
    delay(2000);
    state = TIMER_ON;
}
void Action_Timer_On()
{
    Serial.println("Action Timer On!");
    ButtonClick(_pinTimer_Mode);
    ButtonClick(_pinTimer_Start);
    delay(_cookTime);
    state = KETTLE_BURNER_TIMER_OFF;
}
void Action_Kettle_Off(){
    digitalWrite(_pinKettle_AC, LOW);
}
void Action_Burner_Off(){
    burnerServo.write(150);
}
void Action_Timer_Off()
{
    Serial.println("Action kettle/Burner/Timer Off!");
    ButtonClick(_pinTimer_Start);
    ButtonClick(_pinTimer_Reset);
    delay(2000);
    state = CATAPULT;
}
void Action_Catapult(){
    Serial.println("Action Catapult!");
    digitalWrite(_pinCatapult_DIR, LOW);
    digitalWrite(_pinCatapult_ENA, HIGH);
    delay(3000);
    digitalWrite(_pinCatapult_ENA, LOW);
    delay(2000);
    state = PUMP;
}
void Action_Pump(){
    Serial.println("Action Pump!");
    digitalWrite(_pinPump, HIGH);
    delay(_waterTime);
    digitalWrite(_pinPump, LOW);
    delay(10000);
    state = NEXT;
}
void Action_Next(){
    Serial.println("Action Next!");
    digitalWrite(_pinArduino,HIGH);
    delay(1000);
    digitalWrite(_pinArduino,LOW);
    delay(2000);
    state = START;
}
// ======== SUB FUNCTION =========
void ButtonClick (uint8_t _button)
{
    digitalWrite(_button, HIGH);
    delay(_click_ms);
    digitalWrite(_button, LOW);
    delay(_click_ms);
}
void ButtonLongClick (uint8_t _button)
{
    digitalWrite(_button, HIGH);
    delay(_longClick_ms);
    digitalWrite(_button, LOW);
    delay(_click_ms);
}
