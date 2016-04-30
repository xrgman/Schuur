#include <Wire.h>
#include <TimerOne.h>
#include <Door.h>
#include <SensorUtil.h>
#include <OutsideLight.h>
#include <OutsideSpot.h>

//*********************************
//** Pin number definitions: ******
//*********************************
#define doorSwitch 18 //This should be one of the following interrupt pins: 2,3,18,19,20,21
#define motionSensor 19 //This should be one of the following interrupt pins: 2,3,18,19,20,21
#define doorLight 4
#define outsideSpot1 13
#define outsideSpot2 3
#define outsideSpot3 5
#define outsideSpot4 6
#define outsideSpot5 7
#define outsideLight1 22
#define outsideLight2 24
#define outsideLight3 26
#define outsideLight4 28
#define rfRemoteA 8
#define rfRemoteB 9 
#define rfRemoteC 10 
#define rfRemoteD 11

//Defining start day time:
int startHour = 6;
//Defining end day time:
int endHour = 1;
//Defining darkLux:
int darkLux = 50;
//Defining lightLux:
int lightLux = 200;

//Defining I2C adresses:
#define clockModuleAdress 0x68 //Clock module
#define lightSensorAdress 0x23 //Light sensor

//Defining library classes:
Door door = Door(doorLight);
SensorUtil util = SensorUtil(clockModuleAdress, lightSensorAdress);
OutsideLight light1 = OutsideLight(outsideLight1);
OutsideLight light2 = OutsideLight(outsideLight2);
OutsideLight light3 = OutsideLight(outsideLight3);
OutsideLight light4 = OutsideLight(outsideLight4);
OutsideSpot spot1 = OutsideSpot(outsideSpot1);
OutsideSpot spot2 = OutsideSpot(outsideSpot2);
OutsideSpot spot3 = OutsideSpot(outsideSpot3);
OutsideSpot spot4 = OutsideSpot(outsideSpot4);
OutsideSpot spot5 = OutsideSpot(outsideSpot5);

//Poll counter:
int pollCounter = 0;
int pollInterveral = 1;
int currentHour;

void setup() {
  //Setting pinModus: 
  pinMode(doorSwitch,INPUT);
  pinMode(motionSensor,INPUT);
  pinMode(doorLight,OUTPUT);
  pinMode(outsideSpot1,OUTPUT);
  pinMode(outsideSpot2,OUTPUT);
  pinMode(outsideSpot3,OUTPUT);
  pinMode(outsideSpot4,OUTPUT);
  pinMode(outsideSpot5,OUTPUT);
  pinMode(outsideLight1,OUTPUT);
  pinMode(outsideLight2,OUTPUT);
  pinMode(outsideLight3,OUTPUT);
  pinMode(outsideLight4,OUTPUT);
  //Turning off lights:
  light1.turnOff();
  light2.turnOff();
  light3.turnOff();
  light4.turnOff();
  //Initializing light sensor:
  util.initializeLightSensor();
  //Attaching interrupts: 
  attachInterrupt(digitalPinToInterrupt(doorSwitch),doorEvent,CHANGE); //Door open/close.
  attachInterrupt(digitalPinToInterrupt(motionSensor),motionSensorEvent,CHANGE); //Motion sensor
  //Sarting timer:
  Timer1.initialize(10000); //Updates every half second.
  Timer1.attachInterrupt(timerEvent);
  //Starting serial connection:
  Serial.begin(9600);
}

int hour;
void loop() {
  //Reading rfRemote:
  handleRF();
  //Checking light intensity:
  if(pollCounter%(pollInterveral*85) == 0) {
      currentHour = util.getCurrentHour();
      if(!(currentHour <= startHour && currentHour >= endHour)) 
        handleLightSensor();
  }
  delay(10);
  pollCounter++;
}

//Triggerd when door state changes.
void doorEvent() {
  door.handleDoorEvent(digitalRead(doorSwitch));
}

//Triggered when motion is detected.
void motionSensorEvent() {
  bool motion = digitalRead(motionSensor);
  util.handleMotionSensorEvent(motion);
  if(currentHour <= startHour && currentHour >= endHour) {
    if(motion) {
      light1.turnOn();
      light2.turnOn();
      light3.turnOn();
      light4.turnOn();
    }
    else {
      light1.turnOff();
      light2.turnOff();
      light3.turnOff();
      light4.turnOff();
    }  
  }
}

//Timer for effects.
void timerEvent() {
  if(door.getDoorLedFading())  //Door led fading
    door.fadeLed();
  //Outside spots fading:
  if(spot1.getSpotFading())
    spot1.fadeSpot();
  if(spot2.getSpotFading())
    spot2.fadeSpot();
  if(spot3.getSpotFading())
    spot3.fadeSpot();
  if(spot4.getSpotFading())
    spot4.fadeSpot();
  if(spot5.getSpotFading())
    spot5.fadeSpot();
  
}

//Triggered when serial communication is present.
void serialEvent() {
  while (Serial.available()) {
     String word = "";
     while(Serial.available() > 0) {
       char letter = Serial.read();
        word += letter;
        delay(3); 
     }
     handleSerialEvent(word);
  }
}

//Handeling incomming serial commands
void handleSerialEvent(String word) {
  String command = word.substring(0,3);
  if(command.equals("VER")) { //Version
       Serial.println("Schuur automation, version 1.0");
  }
  else if(command.equals("DOO")) { //Door status
       Serial.print("Door status: ");
       Serial.println(digitalRead(doorSwitch)); 
  }
  else if(command.equals("GTM")) {
      util.displayTime();
  }
  else if(command.equals("STM")) {
      int second = word.substring(3,5).toInt();
      int minute = word.substring(5,7).toInt();
      int hour = word.substring(7,9).toInt();
      int dayOfWeek = word.substring(9,10).toInt();
      int dayOfMonth = word.substring(10,12).toInt();
      int month = word.substring(12,14).toInt();
      int year = word.substring(14,16).toInt();
      util.setTimeModule(second,minute,hour,dayOfWeek,dayOfMonth,month,year);
      util.displayTime();
  }
  else if(command.equals("LUX")) {
       Serial.print("LUX: ");
       Serial.println(util.getLightSensorLux());
  }
  else if(command.equals("MOT")) {
       Serial.print("Is there currently any motion? ");
       if(digitalRead(motionSensor))
         Serial.println("Yes");
       else
         Serial.println("No");
  }
  else if(command.equals("GIN")) {
       Serial.print("Amount of time between each time light intensity is checked: ");
       Serial.print(pollInterveral);
       Serial.println(" second('s)");
  }
  else if(command.equals("SIN")) {
       int seconds = word.substring(3,9).toInt();
       pollInterveral = seconds;
       Serial.print("New amount of time between each time light intensity is checked: ");
       Serial.print(pollInterveral);
       Serial.println(" second('s)");
  }
  else if(command.equals("SLL")) {
       int lux = word.substring(3,9).toInt();
       lightLux = lux;
       Serial.print("New amount of lux that's needed to turn off the lights: ");
       Serial.print(lightLux);
       Serial.println(" LUX");
  }
  else if(command.equals("SLD")) {
       int lux = word.substring(3,9).toInt();
       darkLux = lux;
       Serial.print("New amount of lux that's needed to turn on the lights: ");
       Serial.print(darkLux);
       Serial.println(" LUX");
  }
  else if(command.equals("GLL")) {
       Serial.print("Amount of lux needed to turn off lights: ");
       Serial.print(lightLux);
       Serial.println(" LUX");
  }
  else if(command.equals("GLD")) {
       Serial.print("Amount of lux needed to turn on lights: ");
       Serial.print(darkLux);
       Serial.println(" LUX");
  }
  else if(command.equals("STS")) {
       int hour = word.substring(3,5).toInt();
       if(hour >=0 && hour <= 23) {
         startHour = hour;
         Serial.print("Light sensor will now start working after: ");
         if(hour < 10)
           Serial.print("0");
         Serial.print(hour);
         Serial.println(":00");
       }
       else
         Serial.println("Wrong time input, only time between 00:00 and 23:00 allowed");
  }
  else if(command.equals("STE")) {
       int hour = word.substring(3,5).toInt();
       if(hour >=0 && hour <= 23) {
         startHour = hour;
         Serial.print("Motion sensor will now start working after: ");
         if(startHour < 10)
           Serial.print("0");
         Serial.print(hour);
         Serial.println(":00");
       }
       else
         Serial.println("Wrong time input, only time between 00:00 and 23:00 allowed");
  }
  else if(command.equals("GTS")) {
       Serial.print("The Light sensor will start working after: ");
       if(startHour < 10)
         Serial.print("0");
       Serial.print(startHour);
       Serial.println(":00");
  }
  else if(command.equals("GTE")) {
       Serial.print("The motion sensor will start working after: ");
       if(startHour < 10)
         Serial.print("0");
       Serial.print(endHour);
       Serial.println(":00");
  }
  else if(command.equals("GSP")) {
       int spotNr = word.substring(3,4).toInt();
       displaySpot(spotNr,false,0);
  }
  else if(command.equals("SSP")) {
       int spotNr = word.substring(3,4).toInt();
       int value = word.substring(4,7).toInt();
       displaySpot(spotNr,true,value);
  }
  else if(command.equals("GLG")) {
       int lightNr = word.substring(3,4).toInt();
       displayLight(lightNr,false,0);
  }
  else if(command.equals("SLG")) {
       int lightNr = word.substring(3,4).toInt();
       int value = word.substring(4,5).toInt();
       displayLight(lightNr,true,value);
  }
  else if(command.equals("TMP")) {
       float temp = util.getTemperature();
       Serial.print("Current temperature inside: ");
       Serial.print(temp);
       Serial.println(" C");
  }
}

void displaySpot(int spotNumber, bool set, int value) {
  switch(spotNumber) {
     case 1:
        if(set)
           spot1.setBrightness(value);
        Serial.print("Current brightness of spot 1: ");
        Serial.println(spot1.getBrightness());
        break;
     case 2:
        if(set)
           spot2.setBrightness(value);
        Serial.print("Current brightness of spot 2: ");
        Serial.println(spot2.getBrightness());
        break;
     case 3:
        if(set)
           spot3.setBrightness(value);
        Serial.print("Current brightness of spot 3: ");
        Serial.println(spot3.getBrightness());
        break;
     case 4:
        if(set)
           spot4.setBrightness(value);
        Serial.print("Current brightness of spot 4: ");
        Serial.println(spot4.getBrightness());
        break;
     case 5:
        if(set)
           spot5.setBrightness(value);
        Serial.print("Current brightness of spot 5: ");
        Serial.println(spot5.getBrightness());
        break;  
  }
}

void displayLight(int lightNumber, bool set, int value) {
  switch(lightNumber) {
     case 1:
        if(set)
          if(value)
            light1.turnOn();
          else
            light1.turnOff();
        Serial.print("Current state of light 1: ");
        if(light1.getState())
            Serial.println("On");
          else
            Serial.println("Off");
        break;
     case 2:
        if(set)
          if(value)
            light2.turnOn();
          else
            light2.turnOff();
        Serial.print("Current state of light 2: ");
        if(light2.getState())
            Serial.println("On");
          else
            Serial.println("Off");
        break;
     case 3:
        if(set)
          if(value)
            light3.turnOn();
          else
            light3.turnOff();
        Serial.print("Current state of light 3: ");
        if(light3.getState())
            Serial.println("On");
          else
            Serial.println("Off");
        break;
     case 4:
        if(set)
          if(value)
            light4.turnOn();
          else
            light4.turnOff();
        Serial.print("Current state of light 4: ");
        if(light4.getState())
            Serial.println("On");
          else
            Serial.println("Off");
        break;
  }
}

void handleRF() {
  if(digitalRead(rfRemoteA)) {
    spot1.changeBrightness(5);
    spot2.changeBrightness(5);
    spot3.changeBrightness(5);
    spot4.changeBrightness(5);
    spot5.changeBrightness(5);
  }
  if(digitalRead(rfRemoteB)) {
    spot1.turnOn();
    spot2.turnOn();
    spot3.turnOn();
    spot4.turnOn();
    spot5.turnOn();
  }
  if(digitalRead(rfRemoteC)) {
    spot1.changeBrightness(-5);
    spot2.changeBrightness(-5);
    spot3.changeBrightness(-5);
    spot4.changeBrightness(-5);
    spot5.changeBrightness(-5);
  }
  if(digitalRead(rfRemoteD)) {
    spot1.turnOff();
    spot2.turnOff();
    spot3.turnOff();
    spot4.turnOff();
    spot5.turnOff();
  } 
}

void handleLightSensor() {
  uint16_t lux = util.getLightSensorLux();
  if(lux <= darkLux) { //Turn on lights
      light1.turnOn();
      light2.turnOn();
      light3.turnOn();
      light4.turnOn();    
  } 
  else if(lux >= lightLux) {
      light1.turnOff();
      light2.turnOff();
      light3.turnOff();
      light4.turnOff();
  } 
}
