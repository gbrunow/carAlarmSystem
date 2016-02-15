/* --- define pinout --- */
#define X_PIN A15
#define Y_PIN A14
#define Z_PIN A13
#define ALARM_PIN 13
#define IGNITION_PIN 12
#define CONFIRM_KEY_PIN 11
#define KEY1_PIN 10
//#define KEY2_PIN 9
//#define KEY3_PIN 8
//#define KEY4_PIN 7
//#define KEY5_PIN 6
//#define KEY6_PIN 5
//#define KEY7_PIN 4
//#define KEY8_PIN 3

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
#define ADCDelay 250

/* --- configuration --*/
#define DIGITAL_INPUTS 3

int input[DIGITAL_INPUTS] = {IGNITION_PIN, CONFIRM_KEY_PIN, KEY1_PIN};
boolean inputState[DIGITAL_INPUTS];
boolean lastInputState[DIGITAL_INPUTS];
boolean reading[DIGITAL_INPUTS];

int state = state_off_ok;
int prev_state = state_off_ok;

void setup() {
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
  
}

boolean passAcceCheck = true;
boolean carIsOn = false;
boolean alarmTimeOut = false;
boolean alarmDisarmed = false;

void loop() {  
  switch (state){    
    case state_off_ok:
    
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
}
