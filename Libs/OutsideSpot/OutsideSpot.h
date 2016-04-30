#ifndef SCHUURZOOI_OUTSIDESPOT_H
#define SCHUURZOOI_OUTSIDESPOT_H

class OutsideSpot {
    public:
        OutsideSpot(int);
        void turnOn();
        void turnOff();
        void fadeSpot();
        void changeBrightness(int);
        void setBrightness(int);
        int getBrightness();
        bool getSpotFading();
    private:
        int spotPin, spotValue;
        bool spotState, spotFading;
};
#endif //SCHUURZOOI_OUTSIDESPOT_H
