//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include <cwchar>

//=====[Defines potato]===============================================================

#define HEADLAMP_ON_LEVEL 0.8
#define HEADLAMP_OFF_LEVEL 0.2
#define DAYLIGHT_LEVEL 0.6 //ADJUST!!
#define DUSK_LEVEL 0.4 //ADJUST!!


//=====[Declaration of public data types]======================================

typedef enum {
    WAIT, 
    BUTTON_PRESSED, 
    BUTTON_RELEASED
} ignitionButtonState_t;

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignitionButton(BUTTON1);
DigitalIn driverSeatButton(D2);

DigitalOut engineLed(LED2);
DigitalOut leftHeadlight(D3);
DigitalOut rightHeadlight(D4);


UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

AnalogIn potentiometer(A0);  
AnalogIn LDR(A1);

ignitionButtonState_t ignitionButtonState;

//=====[Declaration and initialization of public global variables]=============


//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void engineUpdate();
void headlightUpdate();
bool engineButtonUpdate();
bool driverSitting(); //Determine if the driver is seated

//void uartTask();
//void availableCommands();


//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        engineUpdate();
        if(engineLed){
            headlightUpdate();
        }
        else{
            leftHeadlight = OFF;
            rightHeadlight = OFF;
        }
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    ignitionButton.mode(PullDown);
    driverSeatButton.mode(PullDown);
    ignitionButtonState = WAIT;
}

void outputsInit()
{
    engineLed = OFF;
    leftHeadlight = OFF;
    rightHeadlight = OFF;
}

void engineUpdate(){ //NEEDS FIXING

   if(engineButtonUpdate()){
       engineLed = !engineLed;
   }


/*
    if(!engineLed){
        if(driverSeatButton && E()){
            engineLed = ON;
        }
    }
    else{
        if(E()){
            engineLed = OFF;
        }
    }
    */
}

bool engineButtonUpdate(){ //NEEDS FIXING

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

/*
    static bool buttonstatetracker = false;
    //static bool buttonReleased = false;
    if(buttonstatetracker && !ignitionButton){
        buttonstatetracker = false;
        return true;
    }

    if(ignitionButton){
        buttonstatetracker = true;
    }

    return false;
    */
    
    return false;
}

void headlightUpdate(){
    float readP = potentiometer.read(); //headlights are turned on
    if(readP >= HEADLAMP_ON_LEVEL){
        leftHeadlight = ON;
        rightHeadlight = ON;
    }

    if(readP <= HEADLAMP_OFF_LEVEL){ //headlights are turned off
        leftHeadlight = OFF;
        rightHeadlight = OFF;
    }

    else{ //Headlights are set to auto
        float readLDR = LDR.read();//Get the analog signal from the LDR

        if(readLDR >= DAYLIGHT_LEVEL){
            delay(2000); //Wait 
            leftHeadlight = OFF;
            rightHeadlight = OFF;
        }
        if(readLDR <= DUSK_LEVEL){
            delay(1000); //Wait
            leftHeadlight = ON;
            rightHeadlight = ON;
        }
    }
}

bool driverSeated(){
if(driverSeatButton){
    return true;
}

    return false;
}

/*
void uartTask()
{
    char receivedChar = '\0';
    char str[100];
    int stringLength;
    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );
        switch (receivedChar) {
        case '1':
            if ( alarmState ) {
                uartUsb.write( "The alarm is activated\r\n", 24);
            } else {
                uartUsb.write( "The alarm is not activated\r\n", 28);
            }
            break;

        case '2':
            if ( !mq2 ) {
                uartUsb.write( "Gas is being detected\r\n", 22);
            } else {
                uartUsb.write( "Gas is not being detected\r\n", 27);
            }
            break;

        case '3':
            if ( overTempDetector ) {
                uartUsb.write( "Temperature is above the maximum level\r\n", 40);
            } else {
                uartUsb.write( "Temperature is below the maximum level\r\n", 40);
            }
            break;
            
        case '4':
            uartUsb.write( "Please enter the code sequence.\r\n", 33 );
            uartUsb.write( "First enter 'A', then 'B', then 'C', and ", 41 ); 
            uartUsb.write( "finally 'D' button\r\n", 20 );
            uartUsb.write( "In each case type 1 for pressed or 0 for ", 41 );
            uartUsb.write( "not pressed\r\n", 13 );
            uartUsb.write( "For example, for 'A' = pressed, ", 32 );
            uartUsb.write( "'B' = pressed, 'C' = not pressed, ", 34);
            uartUsb.write( "'D' = not pressed, enter '1', then '1', ", 40 );
            uartUsb.write( "then '0', and finally '0'\r\n\r\n", 29 );

            incorrectCode = false;

            for ( buttonBeingCompared = 0;
                  buttonBeingCompared < NUMBER_OF_KEYS;
                  buttonBeingCompared++) {

                uartUsb.read( &receivedChar, 1 );
                uartUsb.write( "*", 1 );

                if ( receivedChar == '1' ) {
                    if ( codeSequence[buttonBeingCompared] != 1 ) {
                        incorrectCode = true;
                    }
                } else if ( receivedChar == '0' ) {
                    if ( codeSequence[buttonBeingCompared] != 0 ) {
                        incorrectCode = true;
                    }
                } else {
                    incorrectCode = true;
                }
            }

            if ( incorrectCode == false ) {
                uartUsb.write( "\r\nThe code is correct\r\n\r\n", 25 );
                alarmState = OFF;
                incorrectCodeLed = OFF;
                numberOfIncorrectCodes = 0;
            } else {
                uartUsb.write( "\r\nThe code is incorrect\r\n\r\n", 27 );
                incorrectCodeLed = ON;
                numberOfIncorrectCodes++;
            }
            break;

        case '5':
            uartUsb.write( "Please enter new code sequence\r\n", 32 );
            uartUsb.write( "First enter 'A', then 'B', then 'C', and ", 41 );
            uartUsb.write( "finally 'D' button\r\n", 20 );
            uartUsb.write( "In each case type 1 for pressed or 0 for not ", 45 );
            uartUsb.write( "pressed\r\n", 9 );
            uartUsb.write( "For example, for 'A' = pressed, 'B' = pressed,", 46 );
            uartUsb.write( " 'C' = not pressed,", 19 );
            uartUsb.write( "'D' = not pressed, enter '1', then '1', ", 40 );
            uartUsb.write( "then '0', and finally '0'\r\n\r\n", 29 );

            for ( buttonBeingCompared = 0; 
                  buttonBeingCompared < NUMBER_OF_KEYS; 
                  buttonBeingCompared++) {

                uartUsb.read( &receivedChar, 1 );
                uartUsb.write( "*", 1 );

                if ( receivedChar == '1' ) {
                    codeSequence[buttonBeingCompared] = 1;
                } else if ( receivedChar == '0' ) {
                    codeSequence[buttonBeingCompared] = 0;
                }
            }

            uartUsb.write( "\r\nNew code generated\r\n\r\n", 24 );
            break;

        case 'c':
        case 'C':
            sprintf ( str, "Temperature: %.2f \xB0 C\r\n", lm35TempC );
            stringLength = strlen(str);
            uartUsb.write( str, stringLength );
            break;

        case 'f':
        case 'F':
            sprintf ( str, "Temperature: %.2f \xB0 F\r\n", 
                celsiusToFahrenheit( lm35TempC ) );
            stringLength = strlen(str);
            uartUsb.write( str, stringLength );
            break;

        default:
            availableCommands();
            break;

        }
    }
}

void availableCommands()
{
    uartUsb.write( "Available commands:\r\n", 21 );
    uartUsb.write( "Press '1' to get the alarm state\r\n", 34 );
    uartUsb.write( "Press '2' to get the gas detector state\r\n", 41 );
    uartUsb.write( "Press '3' to get the over temperature detector state\r\n", 54 );
    uartUsb.write( "Press '4' to enter the code sequence\r\n", 38 );
    uartUsb.write( "Press '5' to enter a new code\r\n", 31 );
    uartUsb.write( "Press 'f' or 'F' to get lm35 reading in Fahrenheit\r\n", 52 );
    uartUsb.write( "Press 'c' or 'C' to get lm35 reading in Celsius\r\n\r\n", 51 );
}

*/