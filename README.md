# Bibimmen
TUKOREA X GEEKBLE 'Bibimmen' Project

## Requirements

* Arduino Uno(BibimProject1)
* Arduino Mega(BiBimProject2)

## Getting Started

### Pinmap

* BibimProject1
```C
// START
#define _pinBIBIMStart 2
// KETTLE
#define _pinKettle_AC 3
//#define _pinKettle_Temp
#define _pinKettle_PowerLock 4
#define _pinKettle_mLSupply 5
// BURNER
#define _pinBurner 6
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
```

* BibimProject2
```C
// START 
#define _pinBIBIMStart 52
// Move
#define _pinMoveIntSpeed 3
#define _pinMoveAlarm 4
#define _pinMoveAlarmReset 5
#define _pinMoveRunBreak 6
#define _pinMoveStartStop 7
#define _pinMoveSpeedIn 8
#define _pinMove_LIMIT_START 32
#define _pinMove_LIMIT_STOP 33
// SPINNER 
#define _pinSpinner_DIR 22
#define _pinSpinner_ENA 23
#define _pinSpinner_LIMIT_DOWN 34
#define _pinSpinner_LIMIT_UP 35
// SAUCE
#define _pinSauce_DIR 24
#define _pinSauce_ENA 25
#define _pinSauce_LIMIT_BACK 36
#define _pinSauce_LIMIT_FRONT 37
// GIM
#define _pinGim_Servo 9
#define _pinGim_Sol 26
// OIL
#define _pinOil_Servo 10
// MIX
#define _pinMIX_DIR1 28
#define _pinMIX_ENA1 29
#define _pinMIX_DIR2 30
#define _pinMIX_ENA2 31
#define _pinMIX_LIMIT_UP 38
#define _pinMIX_LIMIT_DOWN 39 
```
