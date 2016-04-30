#include "Arduino.h"
#include "OutsideSpot.h"

//******************************************************************
//** Constructor.
//** param spotPin - The pin which the spot mosfet is connected to.
//******************************************************************
OutsideSpot::OutsideSpot(int spotPin) {
    this->spotPin = spotPin;
}

//***********************************************
//** turnOn - Initializes fading on the spot.
//***********************************************
void OutsideSpot::turnOn() {
    //spotValue = 0;
    spotFading = true;
    spotState = true;
}

//***********************************************
//** turnOff - Initializes fading off the spot.
//***********************************************
void OutsideSpot::turnOff() {
    //spotValue = 255;
    spotFading = true;
    spotState = false;
}

//***********************************************
//** fadeSpot - Fades the spot.
//***********************************************
void OutsideSpot::fadeSpot() {
    if(spotValue <= 255  && spotValue >= 0 && spotFading) {
        analogWrite(spotPin, spotValue);
        if(spotState)
            spotValue +=5;
        else
            spotValue -=5;
    }
    else {
        spotFading = false;
        if(spotState)
            spotValue -=5;
        else
            spotValue +=5;
    }
}

//************************************************
//** getSpotFading.
//** Return whether or not the spot is fading.
//************************************************
bool OutsideSpot::getSpotFading() {
    return spotFading;
}

//************************************************
//** changeBrightness.
//** Param offset - The amount of brightness you want to add to the current brightness.
//************************************************
void OutsideSpot::changeBrightness(int offset) {
    if((spotValue + offset) <= 255 && (spotValue + offset) >= 0) {
        spotValue += offset;
        analogWrite(spotPin,spotValue);
    }
}

//************************************************
//** setBrightness.
//** Param value - The new brightness value of the spot.
//************************************************
void OutsideSpot::setBrightness(int value) {
    if(value >= 0 && value <= 255) {
        spotValue = value;
        analogWrite(spotPin, spotValue);
    }
}

//************************************************
//** getBrightness.
//** Return the current brightness of the spot.
//************************************************
int OutsideSpot::getBrightness() {
    return spotValue;
}