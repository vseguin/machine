//---------------------------------------------------------------------------
//                             INCLUDES
//---------------------------------------------------------------------------
#include "pinchangeInterrupt.h"
#include <Servo.h>
#include "PID_v1.h"

//---------------------------------------------------------------------------
//                             PIN DEFINITION
//---------------------------------------------------------------------------

#define pinEncoder1     3 //  interrupt 0
#define pinEncoder2     2 //  interrupt 1
#define pinPBGO         4
#define pinServoTilt    5
#define pinServoYaw     9
#define pinIn1          7
#define pinIn2          8
#define pinSpeedMotor   6  // PWM
#define pinServoPreh    10
#define pinServoFlag    11
#define pinServoPong    12
#define pinMagnet       13
#define pinIRBack       14 // Analog 0
#define pinIRFront      15 // Analog 1

//---------------------------------------------------------------------------
//                                  PID
//---------------------------------------------------------------------------

// CONSTANTS
const unsigned long VITESSEMAX = 843; // [ticks/s]

// TUNING
double consKp = 0.5;
double consKi = 1.5;
double consKd = 0.0;

// INPUT - OUTPUT
double pidOutput                = 0;          // COMMAND
double SpeedSetpoint            = 0;          // SET POINT
unsigned long gTimeBetweenTicks = 0;          // FEEDBACK

// VARIABLES
unsigned long gLastInterrupt    = 0;    

// INSTANCE
PID pid(&gTimeBetweenTicks, &pidOutput, &SpeedSetpoint, consKp, consKi, consKd, DIRECT);

//---------------------------------------------------------------------------
//                                 SERVOS
//---------------------------------------------------------------------------

// CONSTANTS
const int SERVOFLAG_DOWN   = 800;
const int SERVOFLAG_UP     = 2200;

const int SERVOPRE_PULL    = 650;
const int SERVOPRE_RELEASE = 1900;

const int SERVOTILT_HORIZ  = 860;
const int SERVOTILT_VERT   = 1690;

const int SERVOYAW_CW      = 1900;
const int SERVOYAW_CCW     = 1100;

// INSTANCES
Servo gServoTilt;
Servo gServoYaw;
Servo gServoPreh;
Servo gServoFlag;
Servo gServoPong;

//---------------------------------------------------------------------------
//                                INFRA RED
//---------------------------------------------------------------------------

// CONSTANTS
const int IR_ISR_DELAY = 50; // ms

// VARIABLES
int gIRFrontCount = 0;
int gIRBackCount  = 0;
unsigned long gIRFLastInterrupt = 0;
unsigned long gIRBLastInterrupt = 0;

//---------------------------------------------------------------------------
//                               TESTS
//---------------------------------------------------------------------------

// BUTTON CONSTANT
bool Running = false;

//#define TESTIR       // UNCOMMENT TO TEST IR SENSOR
//#define TESTPID      // UNCOMMENT TO TEST PID
//#define FULLTHROTTLE // UNCOMMENT TO RUN AT FULL SPEED ONLY.
#define TESTSERVO

//---------------------------------------------------------------------------
//                               FULL THROTTLE
//---------------------------------------------------------------------------
#ifdef FULLTHROTTLE 
void loop()
{
   if (digitalRead(pinPBGO) == HIGH)
   {
     if (!Running) 
     {
       Forward();
       analogWrite(pinSpeedMotor,255);
       Running = true;
       delay(1000);
     }
     else
     {
       Stop();
       Running = false;
       delay(1000);
     }
   }
} 
#endif
   
//---------------------------------------------------------------------------
//                               TEST PID
//---------------------------------------------------------------------------

#ifdef TESTPID
void loop()
{
   if (digitalRead(pinPBGO) == HIGH)
   {
     if (!Running) 
     {
       Forward();
       Running = true;
       delay(1000);
     }
     else
     {
       Stop();
       Running = false;
       delay(1000);
     }
   }
   
   if (Running)
   { 
     pid.Compute();
     analogWrite(pinSpeedMotor,pidOutput);
   }
}
    
#endif
   
//---------------------------------------------------------------------------
//                               INFRA RED
//---------------------------------------------------------------------------

#ifdef TESTIR
void loop()
{
   delay(250);
   Serial.print("Avant: "); Serial.println(gIRFront);
   Serial.print("Arriere: "); Serial.println(gIRBack);
}
#endif

//---------------------------------------------------------------------------
//                               TEST SERVO
//---------------------------------------------------------------------------

#ifdef TESTSERVO

//#define DFLAG
//define DTILT
//#define DYAW
#define DPREH
//#define DPONG
void loop()
{  
 #ifdef DFLAG
   gServoFlag.writeMicroseconds(SERVOFLAG_UP);
   delay(2000);
   gServoFlag.writeMicroseconds(SERVOFLAG_DOWN);
   delay(2000);
 #endif
 
 #ifdef DPONG
   gServoPong.writeMicroseconds(700);
   delay(2000);
   gServoPong.writeMicroseconds(1500);
   delay(2000);
 #endif
 
 #ifdef DTILT
   gServoTilt.writeMicroseconds(1800);
   delay(2000);
   gServoTilt.writeMicroseconds(SERVOTILT_HORIZ);
   delay(2000);
   delay(10000);
  #endif
 
 #ifdef DYAW
  //ccw
  gServoYaw.writeMicroseconds(1500);
  delay(8000);
  while(1);
 #endif
  
 #ifdef DPREH
   gServoPreh.writeMicroseconds(1000);
 #endif  
}
#endif

