// Encoder interrupts
void Encod1Isr()
{
  unsigned long dt = micros() - gLastInterrupt;
  if (dt > MINDELTAT)
  {
    if (gTimeBetweenTicks == 0){
      gTimeBetweenTicks = dt;
    } 
    else {
      gTimeBetweenTicks = (dt + (3*gTimeBetweenTicks))/4;
    }
    gLastInterrupt = micros();
  }
}


void setDebounce(const double spd)
{
  if (gCurrentMovement == gLastMovement)
  {
    gIRFirstDelay = max(0,long(1000.0 *((NB_TICK_BTWN_PALES / (spd * 1.5)) - (double(gLastSurplus)/SpeedSetpoint))));
  }
  else
  {
    gIRFirstDelay = long(1000.0 *((NB_TICK_BTWN_PALES / (spd * 1.5)) + (double(gLastSurplus)/SpeedSetpoint)));
  }
  gIRDelay = long(1000.0 * NB_TICK_BTWN_PALES / (spd * 1.5));
  Serial.print("Debounce=");
  Serial.print(gIRDelay);
  Serial.println("ms");
  Serial.print("First Dbounce=");
  Serial.print(gIRFirstDelay);
  Serial.println("ms");  
}


void ISRIrFront()
{
  unsigned long currentTime = millis();
  if ((currentTime - gIRFLastInterrupt) >= gIRDelay && gIRFrontCount > 0)
  {
    delayMicroseconds(5000);
    if (digitalRead(pinIRFront))
    {
      togglePin(pinLed);
      gIRFrontCount++;
      Serial.print("PALE #");
      Serial.println(gIRFrontCount);
      
      gIRFLastInterrupt = currentTime;
      if (nbPalesToSee <= gIRFrontCount) {
        movementDone = true;
      } 
    }
  }
  if (((currentTime - gIRFLastInterrupt) >= gIRFirstDelay) && gIRFrontCount == 0)
  {
    delayMicroseconds(5000);
    if (digitalRead(pinIRFront))
    {
      togglePin(pinLed);
      gIRFrontCount++;
      Serial.println("PREMIERE PALE #");      
      gIRFLastInterrupt = currentTime;
      if (nbPalesToSee <= gIRFrontCount) {
        movementDone = true;
      } 
    }
  }
}
