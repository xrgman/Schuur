#include "Arduino.h"
#include "Door.h"

//******************************************************************
//** Constructor.
//** Paramater ledPin - The pin the led of the door is connected to.
//******************************************************************
Door::Door(int ledPin) {
    doorLedPin = ledPin;
    doorLedOn = false;
}

//******************************************************************
//** handleDoorEvent - Turns led at the door on or of.
//** Paramater lstate - Whether the door is open or closed.
//******************************************************************
void Door::handleDoorEvent(bool state) {
    //Checking if its open or closed:
    if(!state) {
        if(!doorLedOn)
            fadeLedOnStart(doorLedPin);
    }
    else {
        if(doorLedOn)
            fadeLedOffStart(doorLedPin);
    }
}

//*********************************************************************
//** fadeLedOnStart - Initializes fading on the door led.
//** Paramater doorLedPin - The pin the led of the door is connected to.
//**********************************************************************
void Door::fadeLedOnStart(int doorLedPin) {
    delay(200);
    doorLedValue = 0;
    doorLedFading = true;
    doorLedOn = true;
}

//*********************************************************************
//** fadeLedOffStart - Initializes fading off the door led.
//** Paramater doorLedPin - The pin the led of the door is connected to.
//**********************************************************************
void Door::fadeLedOffStart(int doorLedPin) {
    doorLedValue = 255;
    doorLedFading = true;
    doorLedOn = false;
}

//*********************************************************************
//** fadeLed - Fades the led placed at the door.
//**********************************************************************
void Door::fadeLed() {
    if(doorLedValue <= 255  && doorLedValue >= 0)
        analogWrite(doorLedPin,doorLedValue);
    else
        doorLedFading = false;
    if(doorLedOn)
        doorLedValue +=5;
    else
        doorLedValue -=5;
}

//*********************************************************************
//** getDoorLedFading.
//** Return whether or not the door led is fading.
//**********************************************************************
bool Door::getDoorLedFading() {
    return doorLedFading;
}