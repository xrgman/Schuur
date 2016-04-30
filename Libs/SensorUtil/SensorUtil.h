#ifndef SCHUURZOOI_SENSORUTIL_H
#define SCHUURZOOI_SENSORUTIL_H

class SensorUtil {
    public:
        SensorUtil(int,int);
        void handleMotionSensorEvent(int);
        void setTimeModule(int,int,int,int,int,int,int);
        void getTimeModule(int*,int*,int*,int*,int*,int*,int*);
        void displayTime();
        int getCurrentHour();
        float getTemperature();
        void initializeLightSensor();
        uint16_t getLightSensorLux();
    private:
        bool motionDetected;
        int timeModuleAdress, lightSensorAdress;
        int second, minute, hour, dayOfWeek, dayOfMonth, month, year;
        int bcdToDec(int);
        int decToBcd(int);
};

#endif //SCHUURZOOI_SENSORUTIL_H
