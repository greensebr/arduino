#ifndef Sensores_h
#define Sensores_h

#include "Arduino.h"
#include "DHT11.h"
#include "Thermistor.h"

extern int tp;
extern int hd;
extern int pinSensorA;

class Sensores {
    public:
        Sensores(int pinDHT, int pinThermistor) : dht(pinDHT), ThermistorPin(pinThermistor) {}

        int readTemperature();
        int readHumidity();
        void printTempAndHumAr();
        void soloTemp();
        void soloUmidade();
    
    private:
        DHT11 dht;
        int ThermistorPin;
        int pinSensorA;
        static void printError(int temperature, int humidity);

};

#endif