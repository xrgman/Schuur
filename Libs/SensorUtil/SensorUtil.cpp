#include "Arduino.h"
#include "SensorUtil.h"
#include "Wire.h"


//******************************************************************
//** Constructor.
//** param timeModuleAdress - The I2C adress of the clock module.
//******************************************************************
SensorUtil::SensorUtil(int timeModuleAdress, int lightSensorAdress) {
    this->timeModuleAdress = timeModuleAdress;
    this->lightSensorAdress = lightSensorAdress;
    Wire.begin();
}

//********************************************************************
//** Handle the motion sensor event.
//********************************************************************
void SensorUtil::handleMotionSensorEvent(int state) {
    if(state)
        motionDetected = true;
    else
        motionDetected = false;
}

//********************************************************************
//** Get the current hour of the day.
//** Return - The hour of the day.
//********************************************************************
int SensorUtil::getCurrentHour() {
    int second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    getTimeModule(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    return hour;
}

//********************************************************************
//** Set the time in the time module.
//********************************************************************
void SensorUtil::setTimeModule(int second,int minute,int hour,int dayOfWeek,int dayOfMonth,int month,int year) {
    Wire.beginTransmission(timeModuleAdress);
    Wire.write(0);
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year));
    Wire.endTransmission();
}

//********************************************************************
//** Get the time from the time module.
//********************************************************************
void SensorUtil::getTimeModule(int *second, int *minute, int *hour, int *dayOfWeek, int * dayOfMonth, int *month, int *year) {
    Wire.beginTransmission(timeModuleAdress);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(timeModuleAdress,7);
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

//********************************************************************
//** Displays the current time of the RTC chip through serial monitor
//********************************************************************
void SensorUtil::displayTime() {
    int second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    getTimeModule(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
    Serial.print("Current time in the RTC chip: ");
    if(hour < 10)
        Serial.print("0");
    Serial.print(hour,DEC);
    Serial.print(":");
    if(minute < 10)
        Serial.print("0");
    Serial.print(minute,DEC);
    Serial.print(":");
    if(second < 10)
        Serial.print("0");
    Serial.print(second,DEC);
    Serial.print(" ");
    if(dayOfMonth < 10)
        Serial.print("0");
    Serial.print(dayOfMonth,DEC);
    Serial.print("-");
    if(month < 10)
        Serial.print("0");
    Serial.print(month,DEC);
    Serial.print("-");
    Serial.print("20");
    Serial.print(year,DEC);
    Serial.print("\n");
}

//********************************************************************
//** Get the temperature from the time module.
//** Return - The temperature in C degrees.
//********************************************************************
float SensorUtil::getTemperature() {
    float temperature;

    Wire.beginTransmission(timeModuleAdress);
    Wire.write(0x11);
    Wire.endTransmission();

    Wire.requestFrom(timeModuleAdress, 2);

    int firstRegister = Wire.read();
    int secondRegister = Wire.read() >> 6;

    if((firstRegister & 0x80) != 0)
        firstRegister |= ~((1 << 8) - 1); //Negative temperature;

    temperature = 0.25 * secondRegister + firstRegister ;
    return temperature;
}

//********************************************************************
//** Initialize the light sensor.
//** Turning the device on.
//** Setting the monitioring mode of the device.
//********************************************************************
void SensorUtil::initializeLightSensor() {
    Wire.beginTransmission(lightSensorAdress);
    Wire.write(0x01); //Enabling light sensor
    Wire.endTransmission();
    Wire.beginTransmission(lightSensorAdress);
    Wire.write(0x10); //Setting light sensor mode
    Wire.endTransmission();
}

//********************************************************************
//** Get the current light intensity.
//** Return - The current light intensity in LUX.
//********************************************************************
uint16_t SensorUtil::getLightSensorLux() {
    Wire.beginTransmission(lightSensorAdress);
    Wire.requestFrom(lightSensorAdress,2);
    uint16_t Intensity_value;
    Intensity_value = Wire.read();
    Intensity_value <<= 8;
    Intensity_value |= Wire.read();
    Wire.endTransmission();
    return Intensity_value/1.2;
}


//********************************************************************
//** Convert binary number to a decimal number.
//** Return - The decimal number.
//********************************************************************
int SensorUtil::bcdToDec(int val) {
    return( (val/16*10) + (val%16) );
}

//********************************************************************
//** Convert decimal number to a binary number.
//** Return - The binary number.
//********************************************************************
int SensorUtil::decToBcd(int val) {
    return( (val/10*16) + (val%10) );
}
