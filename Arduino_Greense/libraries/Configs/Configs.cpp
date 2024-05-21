#include "Configs.h"

time_t time_now = now();
int hour_now = hour(time_now);
int minute_now = minute(time_now);
int second_now = second(time_now);
extern int mark, hs, tp, hd;
extern String param;


void Configs::readSerialData(){
    String receivedValue = Serial.readString();
    Serial.print(", Parametros: ");

    if ((receivedValue != "") && (mark == 1)) {
        Serial.print(receivedValue);
        extrairValores(receivedValue, &tp, &hd, &hs);
        param = receivedValue;
        ledSerial(1);
    }

    if (mark == 0) {
        Serial.print(param);
        ledSerial(1);
        extrairValores(param, &tp, &hd, &hs);
        mark = 1;
    }

    if ((receivedValue == "") && (mark == 1)) {
        Serial.print(param);
        ledSerial(0);
        extrairValores(param, &tp, &hd, &hs);
        mark = 1;
    }
}


void Configs::extrairValores(String texto, int* valor1, int* valor2, int* valor3) {
	int pos1 = texto.indexOf(' ');
	int pos3 = texto.lastIndexOf(' ');
	int pos2 = texto.lastIndexOf(' ', pos3 - 1);
	String num1 = texto.substring(pos1 + 1, pos2);
	String num2 = texto.substring(pos2 + 1, pos3);
	String num3 = texto.substring(pos3 + 1);

    *valor1 = num1.toInt();
    *valor2 = num2.toInt();
    *valor3 = num3.toInt();
}


void Configs::ledSerial(bool status) {
    if (status) {
        digitalWrite(7, HIGH);
        Serial.print(", Serial: 1");
    }
    else {
        digitalWrite(7, LOW);
        Serial.print(", Serial: 0");
    }
}


void Configs::printDigits(int digits) {
    Serial.print(":");
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits); 
}


void Configs::digitalClockDisplay(){
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print("/");
    Serial.print(month());
    Serial.print("/");
    Serial.print(year());
}