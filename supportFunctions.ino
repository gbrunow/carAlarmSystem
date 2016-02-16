boolean readDigitalInputs(){
  //read and debounce digital inputs
  for( int i = 0; i < DIGITAL_INPUTS; i++){
      reading[i] = digitalRead(input[i]);
      if(reading[i] != lastInputState[i]){
        lastInputState[i] = reading[i];
        bounceTime[i] = millis();
      }
    }
    
    for( int i = 0; i < DIGITAL_INPUTS; i++){
     if(millis() - bounceTime[i] > debounceDelay && reading[i] != inputState[i]){
       inputState[i] = reading[i];
     }
  }
}

