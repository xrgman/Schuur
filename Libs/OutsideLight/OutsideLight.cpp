#include "Arduino.h"
#include "OutsideLight.h"

//******************************************************************
//** Constructor.
//** param lightPin - The pin which the light relay is connected to.
//******************************************************************
OutsideLight::OutsideLight(int lightPin) {
    this->lightPin = lightPin;
}

//*******************************
//** Turns on the outside light.
//*******************************
void OutsideLight::turnOn() {
    digitalWrite(lightPin,LOW);
    state = true;
}

//*******************************
//** Turns off the outside light.
//*******************************
void OutsideLight::turnOff() {
    digitalWrite(lightPin,HIGH);
    state = false;
}

//********************************************
//** Whether or not the light is on.
//** Return - The current state of the light.
//********************************************
bool OutsideLight::getState() {
    return state;
}