#include "Atuadores.h"

bool exaustorOn = false;
unsigned long startTime = 0;

void Atuadores::ledPainel12(int hora) {
    bool start = (hora >= 0);
    bool end = (hora < 12);

    if (start && end) {
        digitalWrite(2, LOW);
        digitalWrite(5, HIGH);
        Serial.print(", Painel12: 1");
    }
    else {
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
        Serial.print(", Painel12: 0");
    }
}


void Atuadores::ledPainel6(int hora) {
    bool start1 = (hora >= 0);
    bool end1 = (hora < 6);
    bool start2 = (hora >= 12);
    bool end2 = (hora < 18);

    if (start1 && end1){
        digitalWrite(2, LOW);
        digitalWrite(5, HIGH);
        Serial.print(", Painel6 : 1");
    }

    if (start2 && end2) {
        digitalWrite(2, LOW);
        digitalWrite(5, HIGH);
        Serial.print(", Painel6 : 1");
    }

    if (!(start1 && end1) && !(start2 && end2)) {
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
        Serial.print(", Painel6 : 0");
    }
}


void Atuadores::ledPainel0() {
    digitalWrite(2, HIGH);
    digitalWrite(5, HIGH);
    Serial.print(", Painel0: 0");
}


void Atuadores::exaustor(Sensores& sensor) {
    int temperature = sensor.readTemperature();
    int humidity = sensor.readHumidity();
    bool cond = ((temperature >= tp) || (humidity > hd));
    unsigned long currentTime = millis();

    if (cond) {
        digitalWrite(3, LOW);
        Serial.print(", Exaustor: 1");
        exaustorOn = true;
        startTime = currentTime;
    } else {
        if (exaustorOn && currentTime - startTime >= 60000) {
            digitalWrite(3, HIGH);
            Serial.print(", Exaustor: 0");
            exaustorOn = false;
        } else if (exaustorOn && currentTime - startTime <= 60000) {
            Serial.print(", Exaustor: 1");
        } else {
            Serial.print(", Exaustor: 0");
        }
    }
}


void Atuadores::acionarBomba(int bombaPin, int boiaPin) {
    digitalWrite(bombaPin, LOW);
    while (digitalRead(boiaPin) == HIGH) {
        delay(100);
    }
    digitalWrite(bombaPin, HIGH);
}

void Atuadores::acionarBombaE(int bombaPin, int boiaPin) {
    digitalWrite(bombaPin, LOW);
    while (digitalRead(boiaPin) == LOW) {
        delay(100);
    }
    digitalWrite(bombaPin, HIGH);
}

void Atuadores::regar() {
    int estadoBotao = digitalRead(botaoPin);
    estadoBotao = !estadoBotao;

    Serial.print(", Rega: ");
    Serial.print(estadoBotao);

    if (estadoBotao) {
        digitalWrite(4, LOW);
    } else {
        digitalWrite(4, HIGH);
    }
    delay(100);
}

void Atuadores::boiaAlta() {
    int estadoBoia = digitalRead(boiaPin1);

    Serial.print(", BoiaAlta: ");
    Serial.println(!estadoBoia);
    delay(100);
}

void Atuadores::boiaBaixa() {
    int estadoBoia = digitalRead(boiaPin2);

    Serial.print(", BoiaBaixa: ");
    Serial.print(!estadoBoia);
}