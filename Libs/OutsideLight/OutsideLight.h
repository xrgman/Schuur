#ifndef SCHUURZOOI_OUTSIDELIGHT_H
#define SCHUURZOOI_OUTSIDELIGHT_H

class OutsideLight {
    public:
        OutsideLight(int);
        void turnOn();
        void turnOff();
        bool getState();
    private:
        bool state;
        int lightPin;
};
#endif //SCHUURZOOI_OUTSIDELIGHT_H
