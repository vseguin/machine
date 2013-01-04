#include "Servo.h"
#include "utility.h"

// Servomoteurs
Servo ArmTilt;
Servo ArmYaw;
Servo ArmPrehenseur;
Servo Flag;
Servo PingPong;

void setPingPongReady();
void shootPingPong();
void raiseFlag();
void dropFlag();
void pullPrehenseur();
void releasePrehenseur();
void setTilt(const int angle);
void setYaw(int direct);

void setup()
{
  pinMode(pinEncoder1, INPUT);
  pinMode(pinEncoder2, INPUT);
  pinMode(pinPBGO, INPUT);
  pinMode(pinServoTilt, OUTPUT);
  pinMode(pinServoYaw, OUTPUT);
  pinMode(pinIn1, OUTPUT);
  pinMode(pinIn2, OUTPUT);
  pinMode(pinSpeedMotor, OUTPUT);
  pinMode(pinServoPreh, OUTPUT);
  pinMode(pinServoFlag, OUTPUT);
  pinMode(pinServoPong, OUTPUT);
  pinMode(pinLed, OUTPUT);

  // Définir les états par défaut
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, HIGH);
  digitalWrite(pinSpeedMotor, LOW);
  digitalWrite(pinLed, LOW);

  ArmTilt.attach(pinServoTilt);
  ArmYaw.attach(pinServoYaw);
  ArmPrehenseur.attach(pinServoPreh);
  Flag.attach(pinServoFlag);
  PingPong.attach(pinServoPong);

  attachInterrupt(0, Encod1Isr, RISING);//pin 2

  // IR
  PCattachInterrupt(pinIRFront,ISRIrFront,RISING);

  Serial.begin(115200);
  Serial.println("Serial Started.");

  /// SET SPEED OF PID
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(20); // 50Hz pid update

  // Set default servo position
  initializeServos();

  SpeedSetpoint = double(NOSPEED); // [ticks/s]
  Brake();
  PingPong.detach();
  Flag.detach();
}


