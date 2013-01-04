#include "utility.h"


Trials initialize() {
  Serial.println("Trial : Initialize");
  setYaw(FRONT);
  return TOUR;
}


Trials tour() {
  Serial.println("Trial : Tour");
  setYaw(LEFT);
  return PREBARRIERE;
}


Trials prebarriere() {
  Serial.println("Trial : Prebarriere");
  setYaw(F_BARRIERE);
  setTilt(TILT_PRE_BARRIERE);
  return BARRIERE;
}


Trials barriere() {
  Serial.println("Trial : Barriere");
  setTilt(TILT_BARRIERE_FIRST);
  setYaw(F_BARRIERE2);
  delay(100);
  ArmTilt.writeMicroseconds(VERTICAL);
  lastTilt = VERTICAL;
  delay(200);
  setTilt(HORIZONTAL);
  return BATTERIE_PRENDRE;
}


Trials batteriePrendre() {
  Serial.println("Trial : Batterie Prendre");
  setYaw(R_TAKEBAT);
  setTilt(TILT_BATTERIE_PRENDRE);
  return POST_BAT_PRENDRE;
}


Trials postBatPrendre()
{
  Serial.println("Trial : Post Batterie Prendre");
  setTilt(TILT_POST_BATTERIE_PRENDRE);
  setYaw(FRONT);
  return BALLE;
}


Trials balle() {
  PingPong.attach(pinServoPong);
  Serial.println("Trial : Balle");
  shootPingPong();
  return SHORT;
}


Trials doShort() {
  Serial.println("Trial : Do Short");
  PingPong.detach();
  // Délai de 2 secondes pr bloquer
  // laser est le temps pr dropper batterie
  setYaw(L_RELBAT);
  setTilt(TILT_FIRST_DEPOSER_BATTERIE);
  relacherBatterie();
  delay(200);
  setTilt(TILT_BARRIERE_FIRST, TILT_SPEED_BATTERIE_DEPOSER);  
  return BATTERIE_DEPOSER;
}


Trials batterieDeposer() {
  Serial.println("Trial : Batterie Deposer");
  setYaw(F_OEUF);
  setTilt(TILT_BATTERIE_DEPOSER);
  return OEUF;
}


Trials oeuf() {
  Serial.println("Trial : Oeuf");
  setTilt(TILT_PRE_OEUF,3500);  
  delay(200);
  ArmPrehenseur.writeMicroseconds(600);
  delay(300);
  setTilt(HORIZONTAL);
  return LASER;
}


Trials laser() {
  Serial.println("Trial : Laser");
  Flag.attach(pinServoFlag);
  dropFlag();
  delay(2200);
  raiseFlag();
  delay(200);
  return DESTRUCTION;
}

Trials destruction() {
  Serial.println("Trial : Destruction");
  // Plus rien a faire nous sommes
  // déjà ds le chemin alternatif
  //SpeedSetpoint = double(NOSPEED);
  return POST_DESTRUCTION;
}

Trials postdestruction() {
  Serial.println("Trial : POST Destruction");
  // Plus rien a faire nous sommes
  // déjà ds le chemin alternatif
  SpeedSetpoint = double(NOSPEED);
  return IDLE;
}

