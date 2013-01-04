#include "pinchangeInterrupt.h"
#include "Servo.h"
#include "utility.h"
#include "PID_v1.h"

void setMovement(int nbPales, DirectionMovement movement, Speed spd = NORMALSPEED, Offset surplus=OFFSET_NULL);
void setMovement(Speed spd);
void setDebounce(long spd);

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
#define pinLed          13
#define pinIRBack       14 // Analog 0
#define pinIRFront      15 // Analog 1

// Movements
const unsigned long MINDELTAT = 1000;//microseconds
const double NB_TICK_BTWN_PALES = 382.0;//ticks

// TUNING
const double consKp = 0.5;
const double consKi = 1.5;
const double consKd = 0.0;

// INPUT - OUTPUT
double pidOutput                = 0;          // COMMAND
double SpeedSetpoint            = 0;          // SET POINT
unsigned long gTimeBetweenTicks = 0;          // FEEDBACK

// GLOBAL VARIABLES
unsigned long gLastInterrupt = 0;
unsigned int gSurplus = 0;
int gLastSurplus;
bool gCurrentMovement = FORWARD;
bool gLastMovement = FORWARD;
bool isPressed = false;
bool movementDone = false;
bool nextStateReady = false;
int lastTilt = HORIZONTAL;
int lastYaw  = FRONT;

// INSTANCE
PID pid(&gTimeBetweenTicks, &pidOutput, &SpeedSetpoint, consKp, consKi, consKd, DIRECT);

//---------------------------------------------------------------------------
//                                INFRA RED
//---------------------------------------------------------------------------

// IR GLOBAL VARIABLES
long gIRDelay = 100; // ms
long gIRFirstDelay;
unsigned int gIRFrontCount = 0;
unsigned long gIRFLastInterrupt = 0;

// PALES
unsigned int nbPalesToSee = 0;

// STATES
bool Running = false;
Trials state = INITIALIZE;

//---------------------------------------------------------------------------
//                                LOOP
//---------------------------------------------------------------------------

void loop()
{ 
  if (pushButtonPressed()) {
    if (Running) {
      Serial.println("Stop!");
      setMovement(0, FORWARD, NOSPEED);
      Running = false;
      nextStateReady = false;
      state = INITIALIZE;
      initializeServos();
    }
    else {
      Serial.println("Go!");
      Running = true;
      nextStateReady = true;
    }

  }
  if (Running)
  { 
    pid.Compute();
    
    if (nextStateReady)
    {
      switch(state) {
        case(INITIALIZE) :
          //Aucun déplacement à effectuer
          movementDone = true;
          break;
        case(TOUR) :
          setMovement(PALES_TOUR, FORWARD, NORMALSPEED);
          break;
        case(PREBARRIERE) :
          setMovement(PALES_PREBARRIERE, FORWARD, NORMALSPEED);
          break;
        case(BARRIERE) :
          setMovement(PALES_BARRIERE, FORWARD, NORMALSPEED, OFFSET_BARRIERE);
          break;
        case(BATTERIE_PRENDRE) :
          setMovement(PALES_BATTERIE_PRENDRE, BACKWARD, NORMALSPEED, OFFSET_BARRIERE_PRENDRE);
          break;
        case(POST_BAT_PRENDRE) :
          setMovement(PALES_POST_BATTERIE_PRENDRE, FORWARD, NORMALSPEED, OFFSET_POST_BAT_PRENDRE);
          break;
        case(BALLE) :
          setMovement(PALES_BALLE, FORWARD, NORMALSPEED, OFFSET_BALLE);
          break;
        case(SHORT) :
          setMovement(PALES_SHORT, FORWARD, NORMALSPEED, OFFSET_SHORT);
          break;
        case(BATTERIE_DEPOSER) :
          setMovement(PALES_BATTERIE_DEPOSER, FORWARD, NORMALSPEED);
          break;
        case(OEUF) :
          setMovement(PALES_OEUF, FORWARD, NORMALSPEED, OFFSET_OEUF);
          break;
        case(LASER) :
          //setMovement(PALES_LASER, BACKWARD, NORMALSPEED, OFFSET_LASER);
          bloquerLaserPasClean(NORMALSPEED);
          break;
        case(DESTRUCTION) :
          setMovement(PALES_DESTRUCTION, BACKWARD, QUICKSPEED, OFFSET_POSTDESTRUCTION);
          break;
        case(POST_DESTRUCTION) :
          setMovement(PALES_DESTRUCTION, BACKWARD, LOWSPEED);
          break;
        default:
          break;
      }
      nextStateReady = false;
    }
    
    if (movementDone == true) {
      stopMovement();
      movementDone = false;
    }
  }
}


void bloquerLaserPasClean(long spd)
{
  Serial.println("Boucle laser pas cleannn");
  gSurplus = 0;
  gTimeBetweenTicks = 0; // On simule une vitesse presque nulle
  gLastInterrupt = micros();
  gIRFrontCount = 0;
  SpeedSetpoint = double(spd);
  Backward();
  long tempsDebut = millis();
  long tmp = 0;
  long stopCondition = ((750L*NORMALSPEED)/spd);
  Serial.print("Stop cond ");  Serial.println(stopCondition);
  while(tmp <= stopCondition)
  {
    pid.Compute();
    tmp = millis() - tempsDebut;
  }
  movementDone = true;
}


void executeActionCorrespondingToState() {
  switch(state) {
    case(INITIALIZE) :
      state = initialize();
      break;
    case(TOUR) :
      state = tour();
      break;
    case(PREBARRIERE) :
      state = prebarriere();
      break;
    case(BARRIERE) :
      state = barriere();
      break;
    case(BATTERIE_PRENDRE) :
      state = batteriePrendre();
      break;
    case(POST_BAT_PRENDRE) :
      state = postBatPrendre();
      break;
    case(BALLE) :
      state = balle();
      break;
    case(SHORT) :
      state = doShort();
      break;
    case(BATTERIE_DEPOSER) :
      state = batterieDeposer();
      break;
    case(OEUF) :
      state = oeuf();
      break;
    case(LASER):
      state = laser();
      break;
    case(DESTRUCTION) :
      state = destruction();
      break;
    case(POST_DESTRUCTION) :
      state = postdestruction();
      break;
    default:
      break;
  }
  if (state != IDLE) {
    nextStateReady = true;
  }
}


