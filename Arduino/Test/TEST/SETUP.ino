// Setup method
void setup()
{
  // Définir la direction des pins

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
  pinMode(pinMagnet, OUTPUT);
  //pinMode(pinServoPONG, OUTPUT);
  //pinMode(pinServoPONG, OUTPUT);

  // Définir les états par défaut
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, HIGH);
  digitalWrite(pinSpeedMotor, LOW);
  digitalWrite(pinMagnet, LOW);
  
  // Attacher les servomoteurs
  gServoTilt.attach(pinServoTilt);
  gServoYaw.attach(pinServoYaw);
  gServoPreh.attach(pinServoPreh);
  gServoFlag.attach(pinServoFlag);
  gServoPong.attach(pinServoPong);
  
  // Initialiser les interruptions
  // Encodeur Moteur
  attachInterrupt(0, Encod1Isr, RISING);//pin 2
  attachInterrupt(1, Encod2Isr, RISING);//pin 3
  
  // IR
  PCattachInterrupt(pinIRFront,ISRIrFront,RISING);
  PCattachInterrupt(pinIRBack,ISRIrBack,RISING);  
  
  Serial.begin(115200);
  Serial.println("Serial Started.");


  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(13); // 75Hz pid update
  
  SpeedSetpoint = 300.0; // [ticks/s]
  
}
