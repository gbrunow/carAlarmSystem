
/* --- define pinout --- */
#define IGNITION_PIN 7
#define X_PIN A15
#define Y_PIN A14
#define Z_PIN A13
#define ALARM_PIN 6

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

int state = state_off_ok;
int prev_state = state_off_ok;

void setup() {
  // put your setup code here, to run once:

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

}
