#include<Wire.h>

/* --- define pinout --- */
#define X_PIN A0
#define Y_PIN A1
#define Z_PIN A2
#define ALARM_PIN 13
#define IGNITION_PIN 12
#define CONFIRM_KEY_PIN 11

/* --- define machine states --- */
#define state_off_ok    1
#define state_on_ok     2
#define state_off_alarm 3
#define state_on_alarm  4
#define state_ok        5
#define state_alarm     6

/* --- define constants --- */
#define minVal 265
#define maxVal 402
#define alarmDelay 3000
#define triggerAlarmDelay 5000
#define armAlarmDelay 5000
#define acceDelay 250
#define debounceDelay 2
#define PASSWORD 150 //2, 3, 5, 8 | 10010110
#define XY_THRESHOLD 2.5
#define Z_THRESHOLD 15

/* --- organiza digital input that need debounce --- */
#define DIGITAL_INPUTS 2
#define IGNITION_INDEX 0
#define CONFIRM_KEY_INDEX 1

int input[DIGITAL_INPUTS] = { IGNITION_PIN, CONFIRM_KEY_PIN };

boolean inputState[DIGITAL_INPUTS];
boolean lastInputState[DIGITAL_INPUTS];
boolean reading[DIGITAL_INPUTS];
unsigned long bounceTime[DIGITAL_INPUTS];

int state = state_off_ok;
int prev_state = -1;

double x;
double y;
double z;
double lastX;
double lastY;
double lastZ;

void setup() {
  
  //enable I2C communication
  Wire.begin();
  
  //setup analog inputs
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);

  //setup digital outputs
  pinMode(ALARM_PIN, OUTPUT);
  
  //setup digital inputs
  for(int i = 0; i < DIGITAL_INPUTS; i++){
    pinMode(input[i], INPUT);
    digitalWrite(input[i], HIGH);   //turn on pull up resistor
    inputState[i] = HIGH;
    lastInputState[i] = HIGH;
  }

  getAxes(&x, &y, &z);

  lastX = x;
  lastY = y;
  lastZ = z;
  
}

boolean passAcceCheck = true;
boolean carIsOn = false;
boolean alarmTimeOut = false;
boolean alarmDisarmed = false;

void loop() {  
  switch (state){    
    case state_off_ok:

     if(prev_state != state_off_ok){
        startAlarmTimer();
      }
      
      passAcceCheck = checkAcce();      
      if(passAcceCheck){
        state = state_ok;
      } else {
        state = state_alarm;
      }

      prev_state = state;
      break;
    case state_on_ok:

      alarmTimeOut = isTimeOut();
      alarmDisarmed = checkAlarmKey();

      if(prev_state == state_off_ok){
        startAlarmTimer();
      }

      if(alarmDisarmed){
          state = state_ok;
      } else {
        if(alarmTimeOut){
          state = state_alarm;
        } else {
          state = state_ok;
        }
      }

      prev_state = state;
      break;
    case state_off_alarm:
      
      passAcceCheck = checkAcce();
      alarmTimeOut = isTimeOut();
      alarmDisarmed = checkAlarmKey();

      if(!passAcceCheck){
        resetAlarmTimer();
      } 

      if(alarmDisarmed){
        state = state_ok;
      } else {
        if(alarmTimeOut){
          state = state_ok;
        } else {
          state = state_alarm;
        }
      }

      prev_state = state;
      break;
      
    case state_on_alarm:
      
      alarmDisarmed = checkAlarmKey();
      if(alarmDisarmed){
        state = state_ok;
      } else {
        state = state_alarm;
      }

      prev_state = state;
      break;
      
    case state_ok:

      carIsOn = isOn();
      if(carIsOn){
        state = state_on_ok;
      } else {
        state = state_off_ok;
      }
      
      break;

    case state_alarm:

      carIsOn = isOn();
      if(carIsOn){
        state = state_on_alarm;  
      } else {
        state = state_off_alarm;
      }
      
      break;
  }

  if(alarmTimeOut || alarmDisarmed){
    stopAlarm();
  } else {
    triggerAlarm();
  }

  readDigitalInputs();
}
