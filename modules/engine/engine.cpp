//=====[Libraries]=============================================================

#include "engine.h"
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

typedef enum {
   WAIT,
   BUTTON_PRESSED,
   BUTTON_RELEASED
} ignitionButtonState_t;

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignitionButton(BUTTON1);
DigitalIn driverSeatButton(PB_11);

DigitalOut engineLed(LED2);

ignitionButtonState_t ignitionButtonState;

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static bool engineButtonUpdate();
static void inputsInit();
static void outputsInit();

//=====[Implementations of public functions]===================================

void engineInit(){
    inputsInit();
    outputsInit();
}

void engineUpdate(){ 
  if (engineButtonUpdate()) {
      if (!engineLed) {
          if (driverSeatButton) {
              engineLed = ON;
          }
      }
      else {
          engineLed = OFF;
      }
  }
}

//=====[Implementations of private functions]==================================

static void inputsInit()
{
   ignitionButton.mode(PullDown);
   driverSeatButton.mode(PullDown);
   ignitionButtonState = WAIT;
}

static void outputsInit()
{
    engineLed = OFF;
}

static bool engineButtonUpdate(){

   switch(ignitionButtonState){
       case WAIT :
           if(ignitionButton){
               ignitionButtonState = BUTTON_PRESSED;
           }
       break;

       case BUTTON_PRESSED :
           if(!ignitionButton){
               ignitionButtonState = BUTTON_RELEASED;
           }
       break;
    
       case BUTTON_RELEASED :
           ignitionButtonState = WAIT;
           return true;
       break;
   }
   return false;
}