#include "Configs.h"
#include "Sensores.h"
#include "Atuadores.h"

// Declarando objetos:
Configs config;
Atuadores atuador_painel; 
Atuadores atuador_exaustor;
Atuadores atuador_rega;

//Variáveis:
#define pinSensorA A0 
int pinDHT = 10; // sensor umidade e temperatura do ar;
int pinThermistor = 3; // sensor temperatura solo;
int mark, hs, tp, hd;
String param = "in 30 60 6"; // temp, umid, painel;
const int botaoPin = 6; //rega
const int bombaPin = 4;
const int boiaPin1 = 8; // boia1
const int boiaPin2 = 9; // boia2
const int devolverAguaPin = 11;
Sensores sensor(pinDHT, pinThermistor); // precisa ser declarado depois das variáveis;


void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(botaoPin, INPUT);
  pinMode(8, INPUT); 
  pinMode(9, INPUT); 
  pinMode(11, INPUT); 
  digitalWrite(3, HIGH);
}


void loop() {

  //Configurações iniciais:
  config.digitalClockDisplay(); // HH:MM:SS DD/MM/YYYY
  config.readSerialData(); // leitura e impressão dos parâmetros;

  //Tempo painel: 
  if (hs == 12) atuador_painel.ledPainel12(hour());
  if (hs == 6) atuador_painel.ledPainel6(hour());
  if (hs == 0) atuador_painel.ledPainel0();

  //Sensores:
  sensor.printTempAndHumAr(); // leitura e impressão da temperatura e umidade do ar;
  sensor.soloTemp(); // leitura e impressão da temperatura do solo;
  sensor.soloUmidade(); // leitura e impressão da umidade do solo;
  
  //Atuadores:
  atuador_exaustor.exaustor(sensor); // passa um objeto da classe Sensor para medir temp e umidade do ar;
  atuador_rega.regar();
  atuador_rega.boiaBaixa();
  atuador_rega.boiaAlta();
  if (digitalRead(6) == LOW) {
      atuador_rega.acionarBomba(4, 8);
  }
  if (digitalRead(11) == LOW) {
      atuador_rega.acionarBombaE(5, 9);
  }
  delay(2000);
}