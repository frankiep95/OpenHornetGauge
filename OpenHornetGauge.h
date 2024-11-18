/*
* Small utility to control the X27 gauge motor used in open hornet
Circuit  11/15/2024


*/

#ifndef OpenHornetGauge_h
#define OpenHornetGauge_h
#include <Arduino.h>


class OpenHornetGauge
{
    public:
        static const unsigned int STEPS = (360 *3);
        static const int pinCount = 4;
        static const int driveCount = 6;
        unsigned int driveStep;
        unsigned int pins[pinCount];
        unsigned int zeroPin;
        unsigned int position;
        unsigned int targetPos;
        unsigned int motorSpeed;
        unsigned int offset;
        bool commandComplete;

        OpenHornetGauge(int coil1, int coil2, int coil3, int coil4, int zeroPin,unsigned int offset);

        void command(unsigned int value);
        void updatePos();
        void home();

    private:
        void drive();
        void decrement();
        void increment();
        


};

#endif