//=====[Libraries]=============================================================

#include "windshield_wiper.h"
#include "mbed.h"
#include "arm_book_lib.h"
#include "string.h"

//=====[Declaration of private defines]========================================

#define MODE_HI 0.75 //Settings
#define MODE_INT 0.5
#define MODE_LO 0.25

#define DELAY_SHORT 0.33 //Settings
#define DELAY_LONG 0.66

#define MAX_TIME_MS 740 //ms - 0.74 seconds

#define PERIOD 0.02 //Motor
#define DUTY_REST 0.075
#define DUTY_HIGH_CCW 0.08
#define DUTY_HIGH_CW 0.07
#define DUTY_LOW_CCW 0.0785
#define DUTY_LOW_CW 0.0715

int accumulatedTime = 0;


//=====[Declaration of private data types]=====================================
typedef enum {
   REST,
   CYClE
} wiperState_t;
//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

AnalogIn modeDial(A0);
AnalogIn delayDial(A1);
PwmOut servo(PF_9);

wiperState_t wiperState;


int wiperMode = 0; // 0-OFF | 1-LOW | 2-INT | 3-HIGH
int wiperDelay = 0; //0-SHORT | 2-MEDIUM | 3-LONG

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static void updateDelay();
static bool returnState();

//=====[Implementations of public functions]===================================

int readMode(){
    return wiperMode;
}

int readDelay(){
    return wiperDelay;
}

void wiperInit(){
    servo.period(PERIOD);
    servo.write(DUTY_REST);
}

void updateMode(){
    float readP = modeDial.read();
    if(readP < MODE_LO){ 
        wiperMode = 0;
        if(returnState()){
            servo.write(REST);
        }
    }
    
    else if(readP >= MODE_LO && readP < MODE_INT){
        wiperMode = 1;
        if(returnState()){
            servo.write(DUTY_LOW_CW);
            delay(370);
            servo.write(DUTY_LOW_CCW);
            delay(370);
        }
    }

    else if(readP >= MODE_INT && readP < MODE_HI){
        wiperMode = 2;
        updateDelay();
    }

    else{
        wiperMode = 3;
        if(returnState()){
            servo.write(DUTY_HIGH_CW);
            delay(280);
            servo.write(DUTY_HIGH_CCW);
            delay(280);
        }
    }
}

//=====[Implementations of private functions]==================================

static void updateDelay(){
    float readP = delayDial.read();
    if(readP < DELAY_SHORT){
        wiperDelay = 0;
    }

    else if(readP >= DELAY_SHORT && readP < DELAY_LONG){
        wiperDelay = 1;
    }

    else{
        wiperDelay = 2;
    }

}

static bool returnState(){
    if(accumulatedTime >= MAX_TIME_MS){ //rest state
        return true;
        accumulatedTime = 0;
    }
    else{ //cycling
        accumulatedTime = accumulatedTime + 10;
        delay(10);
        return false;
    }  
}

/*
static bool (){

   switch(wiperState){
       case WAIT :
           
       break;

       case CYCLE:
        
       break;
    
   }
   return false;
}
*/