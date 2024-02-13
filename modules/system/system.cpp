//=====[Libraries]=============================================================

#include "system.h"
#include "engine.h"
#include "windshield_wiper.h"
#include "display.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static void displaySetup(); //Format display with mode and delay time selector feature
static void displayUpdate(); //Update the current mode (HI, LO, INT, OFF) and current delay (Short, Medium, Long)


//=====[Implementations of public functions]===================================

void systemInit(){
    engineInit();
    displayInit();
    displaySetup();
    wiperInit();
}

void systemUpdate(){
    engineUpdate();
    displayUpdate();
    updateMode();
}


//=====[Implementations of private functions]==================================
static void displaySetup(){
    displayCharPositionWrite(0,0);
    displayStringWrite("Mode:");

    displayCharPositionWrite(0,1);
    displayStringWrite("Delay:");
}


static void displayUpdate(){

}