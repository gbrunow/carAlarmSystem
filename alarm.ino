boolean checkAlarmKey(){
  byte reading = readAlarmKey();
  return reading == PASSWORD;
}

byte readAlarmKey(){
  byte x = 0;
  Wire.requestFrom(32,1); 
  if(Wire.available()){     //If the request is available
    x=Wire.read();       //Receive the data
  }
  return x;
}

unsigned long alarmTimer = 0;

boolean isTimeOut(){

  switch(state){
    case state_on_ok:
      return (millis() - alarmTimer) > triggerAlarmDelay;
    case state_off_ok:
      return (millis() - alarmTimer) > armAlarmDelay;
    default:
      return (millis() - alarmTimer) > alarmDelay;
  }
}

void resetAlarmTimer(){
  alarmTimer = millis();
}

void startAlarmTimer(){
  alarmTimer = millis();
}

void triggerAlarm(){
  digitalWrite(ALARM_PIN, HIGH);  
}

void stopAlarm(){
  digitalWrite(ALARM_PIN, LOW);
}

boolean checkConfirmKey(){
  return inputState[CONFIRM_KEY_INDEX];
}


