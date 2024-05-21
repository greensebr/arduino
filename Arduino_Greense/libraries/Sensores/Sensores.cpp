#include "Sensores.h"


int Sensores::readTemperature() {
    int temperature = round(dht.readTemperature());
    if (isnan(temperature)) {
        printError(temperature, -1);
        return -1;
    }
    return temperature;
}


int Sensores::readHumidity() {
    int humidity = round(dht.readHumidity());
    if (isnan(humidity)) {
        printError(-1, humidity);
        return -1;
    }
    return humidity;
}


void Sensores::printTempAndHumAr() {
    int temperature = readTemperature();
    int humidity = readHumidity();

    Serial.print(", Temperatura: ");
    Serial.print(temperature);
    Serial.print("°C, Umidade: ");
    Serial.print(humidity);
    Serial.print("%");
}


void Sensores::printError(int temperature, int humidity) {
    Serial.print("Error: ");

    if (temperature == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_CHECKSUM) {
        Serial.print("Temperature Reading ");
    }

    if (humidity == DHT11::ERROR_TIMEOUT || humidity == DHT11::ERROR_CHECKSUM) {
        Serial.print("Humidity Reading ");
    }
    Serial.println("Error");
}


void Sensores::soloTemp() {
    const int nAmostras = 5;
    int soma = 0;
    for (int i = 0; i < nAmostras; i++){
        soma += analogRead(ThermistorPin);
        delay(10);
    }

    int Vo = soma / nAmostras;

    float R2 = 10000 * (1023.0 / (float)Vo - 1.0);
    float logR2 = log(R2);
    float T = (1.0 / (1.009249522e-03 + 2.378405444e-04*logR2 + 2.019202697e-07*logR2*logR2*logR2));
    int Tc = round(T - 273.15);

    Serial.print(", TempSolo: ");
    Serial.print(Tc);
}


void Sensores::soloUmidade(){
    int valor_analogico;
    const int nAmostras = 5;
    int soma = 0;

    for (int i = 0; i < nAmostras; i++){
        soma += analogRead(pinSensorA);
        delay(10);
    }

    valor_analogico = soma / nAmostras;

    int valor_convertido = map(valor_analogico, 1023, 460, 0, 100); 
    Serial.print(", UmidSolo: ");
    Serial.print(valor_convertido);
}