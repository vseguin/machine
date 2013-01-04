#include "utility.h"

void setTilt(const TiltValue angle) {
  setTilt(angle, 2700);
}


void setTilt(const TiltValue angle, const int pDelay) {
  if (angle > lastTilt) {
    for(int pos = lastTilt; pos < angle; pos ++)  
    {                                  
      ArmTilt.write(pos); 
      delayMicroseconds(pDelay);
    } 
  } 
  else {
    for(int pos = lastTilt; pos > angle; pos --)  
    {                                  
      ArmTilt.write(pos);
      delayMicroseconds(pDelay);
    } 
  }
  lastTilt = angle;
}


void setYaw(const YawValue direct) {
  if (direct > lastYaw) {
    for(int pos = lastYaw; pos < direct; pos ++)  
    {                                  
      ArmYaw.write(pos);               
      delayMicroseconds(2700);
    } 
  } 
  else {
    for(int pos = lastYaw; pos > direct; pos --)  
    {                                  
      ArmYaw.write(pos);               
      delayMicroseconds(2700);
    } 
  }
  lastYaw = direct;
}


void raiseFlag() {
  Flag.writeMicroseconds(SERVOFLAG_UP);
}


void dropFlag() {
  Flag.writeMicroseconds(SERVOFLAG_DOWN);
}


void setPingPongReady() {
  PingPong.writeMicroseconds(SERVOPINGPONG_READY);
}


void relacherBatterie() {
  ArmPrehenseur.writeMicroseconds(SERVOPRE_RELEASE);  
}


void shootPingPong() {
  PingPong.writeMicroseconds(SERVOPINGPONG_SHOOT);
}


void pullPrehenseur() {
  ArmPrehenseur.writeMicroseconds(SERVOPRE_PULL);
}


void releasePrehenseur() {
  ArmPrehenseur.writeMicroseconds(SERVOPRE_RELEASE);
}


void initializeServos() {
  raiseFlag();
  setPingPongReady();
  ArmPrehenseur.writeMicroseconds(SERVOPRE_PULL);
  ArmTilt.writeMicroseconds(HORIZONTAL);
  lastTilt = HORIZONTAL;
  delay(500);
  setYaw(BEGINNING);
}


bool pushButtonPressed()
{
 if (digitalRead(pinPBGO) == HIGH) {
   if (!isPressed) {
     isPressed = true;
     return true;
   } else {
     return false;
   }
 } else {
   isPressed = false;
   return false;
 }
}

void togglePin(const int pin)
{
  bool nextValue = digitalRead(pin) ^ true;//bitwise XOR
  digitalWrite(pin,nextValue);
}
