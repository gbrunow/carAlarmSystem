void getAxes(double *x, double *y, double *z){
  // put your main code here, to run repeatedly:
    int xRead = analogRead(X_PIN);
    int yRead = analogRead(Y_PIN);
    int zRead = analogRead(Z_PIN);
  
    //convert read values to degrees -90 to 90 - Needed for atan2
    int xAng = map(xRead, minVal, maxVal, -90, 90);
    int yAng = map(yRead, minVal, maxVal, -90, 90);
    int zAng = map(zRead, minVal, maxVal, -90, 90);
  
    //Caculate 360deg values like so: atan2(-yAng, -zAng)
    //atan2 outputs the value of -π to π (radians)
    //We are then converting the radians to degrees
    *x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    *y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
    *z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
}

boolean checkAcce(){

  return true;
}

