// Motor Functions 
void Forward()
{
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, LOW);
}


void Backward()
{
  digitalWrite(pinIn2, HIGH);
  digitalWrite(pinIn1, LOW);
}


void Brake()
{
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, HIGH);
}


void setMovement(int nbPales, DirectionMovement movement, Speed spd, Offset surplus)
{
  nbPalesToSee = nbPales;
  gSurplus = surplus; // ticks
  gCurrentMovement = movement;
  gTimeBetweenTicks = 0; // On simule une vitesse presque nulle
  gLastInterrupt = micros();
  gIRFrontCount = 0;
  if (spd != NOSPEED) {
    SpeedSetpoint = double(min(spd, QUICKSPEED));
    setDebounce(SpeedSetpoint);
    (movement == FORWARD) ? Forward() : Backward();
  } 
  else {
    Brake();
  }
}


void stopMovement()
{
  if (SpeedSetpoint != 0)
  {
//    Serial.println(SpeedSetpoint);
    long tmp = long((double(gSurplus)*1000.0)/SpeedSetpoint);
    Serial.print("Delais ms="); Serial.println(tmp);
    delay(tmp);
  }
  Brake();
  SpeedSetpoint = double(NOSPEED);
  gLastSurplus = gSurplus;
  gSurplus = 0;
  gLastMovement = gCurrentMovement;
  executeActionCorrespondingToState();
}


