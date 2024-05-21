#ifndef Configs_h
#define Configs_h

#include "Arduino.h"
#include "TimeLib.h"

class Configs {

    public:
        void readSerialData();
        static void ledSerial(bool status);
        void digitalClockDisplay();
        void printDigits(int digits);

    private:
        void extrairValores(String texto, int* valor1, int* valor2, int* valor3);
};

#endif