
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

void Stop(){
  //enable standby  
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, HIGH);
}

// Encoder interrupts
void Encod1Isr()
{
   unsigned long dt = micros() - gLastInterrupt;
   if (dt > 1.1*VITESSEMAX)
   {
     gTimeBetweenTicks = (dt + (3*gTimeBetweenTicks))/4;
     gLastInterrupt = micros();
   }
}

void Encod2Isr()
{

}

void ISRIrFront()
{
  Serial.println("INTERRUPT");
  long currentTime = millis();
  if ((currentTime - gIRFLastInterrupt) >= IR_ISR_DELAY)
  {
    gIRFLastInterrupt = currentTime;
    gIRFrontCount++;
  }
}

void ISRIrBack()
{
  Serial.println("INTERRUPT");
  long currentTime = millis();
  if ((currentTime - gIRBLastInterrupt) >= IR_ISR_DELAY)
  {
    gIRBLastInterrupt = currentTime;
    gIRBackCount++;
  }
}
