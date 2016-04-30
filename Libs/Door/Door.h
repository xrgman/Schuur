#ifndef SCHUURZOOI_DOOR_H
#define SCHUURZOOI_DOOR_H

class Door {
    public:
        Door(int);
        void handleDoorEvent(bool);
        void fadeLed();
        bool getDoorLedFading();
    private:
        bool doorLedOn, doorLedFading;
        int doorLedPin, doorLedValue;
        void fadeLedOnStart(int);
        void fadeLedOffStart(int);
};
#endif //SCHUURZOOI_DOOR_H
