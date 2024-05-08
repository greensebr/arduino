/**
 * Author: Marcelino Andrade
 * Version: 1.0.0
 * License: MIT
 */

// Inclusão das bibliotecas:

//#include <SD.h>
#include <DHT11.h>
#include <TimeLib.h>
#include <Thermistor.h>



int ThermistorPin = 3;
int Vo, Tc;
float R1 = 10000;
float logR2, R2, T, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


//Variaveis globais:
Thermistor temp(2);
DHT11 dht11(10); // Definição do pino para o sensor DHT11
int mark = 0;
int hs, tp, hd;
String param = "in 30 60 12";
const int botaoPin = 6; // botao de regar a planta
#define pinSensorA A0
static bool exaustorOn = false; // variável para guardar o estado do exaustor
static unsigned long startTime = 0; // variável para guardar o tempo de início


// Função de configuração:
void setup() {

    Serial.begin(9600); // Inicialização da comunicação serial

    setTime(23, 59, 50, 12, 12, 72);   // Definição do tempo (HH:MM:SS, DD/MM/20YY)
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(botaoPin, INPUT);
    digitalWrite(3, HIGH);

    pinMode(8, INPUT); //Boia 1
    pinMode(9, INPUT); //Boia 2
    
}

// Função principal:
void loop() {
  
    digitalClockDisplay(); // Exibição do horário atual
    
    String receivedValue = Serial.readString(); // Read the integer value from serial
    Serial.print(", Parametros: ");
    
    if ((receivedValue!="") & (mark == 1)) {
     Serial.print(receivedValue);
     extrairValores(receivedValue, &tp, &hd, &hs);
     param = receivedValue;
     //[Iluminação: 8/24, Temperatura: 25°C, Humidade: 50%]
    ledSerial(1);
    }
    
    if (mark == 0) {
    Serial.print(param);
    ledSerial(1);
    extrairValores(param, &tp, &hd, &hs); // Passa o endereço das variáveis
    mark = 1;
    }
    
    if ((receivedValue=="") & (mark == 1)){
    Serial.print(param);
    ledSerial(0);
    extrairValores(param, &tp, &hd, &hs); // Passa o endereço das variáveis
    mark = 1;
    }
    
    if (hs==12) {ledPainel12(hour());}
    if (hs==6)  {ledPainel6(hour());}
    if (hs==0)  {ledPainel0();}

 regar();
 soloTemp1();
 soloHumidade();
 boiaBaixa();
 boiaAlta();


 
    // Leitura da temperatura e umidade
     int temperature = dht11.readTemperature();
     int humidity = dht11.readHumidity();

   exaustor(temperature, humidity, tp, hd);   

    // Verificação se a leitura foi bem-sucedida
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT &&
        humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT) {
            
        //Impressão da temperatura e umidade
        Serial.print(", Temperature: ");
        Serial.print(temperature);
        Serial.print("/");
        Serial.print(tp);
        Serial.print("°C");

        Serial.print(", Humidity: ");
        Serial.print(humidity);
        Serial.print("/");
        Serial.print(hd);
        Serial.println("%, ");
    } 
    
    else {
        // Caso haja erro na leitura de temperatura ou umidade
        if (temperature == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_CHECKSUM) {
            Serial.print("Temperature Reading Error: ");
            Serial.println(DHT11::getErrorString(temperature));
        }
        if (humidity == DHT11::ERROR_TIMEOUT || humidity == DHT11::ERROR_CHECKSUM) {
            Serial.print("Humidity Reading Error: ");
            Serial.println(DHT11::getErrorString(humidity));
        }
    }

    // Aguarda 1 segundo antes da próxima leitura
    delay(1000);
    
} // fim da funcao loop

void soloTemp1() {

  
  const int nAmostras = 5;
  int soma = 0;
  for (int i = 0; i < nAmostras; i++) {
    soma += analogRead(ThermistorPin);
    delay (10);
  }
  Vo = soma/nAmostras;
  
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = round(T - 273.15);



  
  Serial.print(", TempSolo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(Tc); //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA

}


void soloTemp() {
  int temperature = temp.getTemp(); //VARIÁVEL DO TIPO INTEIRO QUE RECEBE O VALOR DE TEMPERATURA CALCULADO PELA BIBLIOTECA
  Serial.print(", TempSolo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(temperature); //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
//  Serial.print("*C"); //IMPRIME O TEXTO NO MONITOR SERIAL
  //delay(1000); //INTERVALO DE 1 SEGUNDO
}


void ledPainel12(int hora) {
  bool start = (hora >= 0); 
  bool end   = (hora < 12); 
  if ( start && end ){
    digitalWrite(2, LOW);
    digitalWrite(5, HIGH);
                    
    Serial.print(", Painel12: 1");
  }

    else {
    digitalWrite(2, HIGH); 
    //    digitalWrite(3, HIGH); 
          //     digitalWrite(4, HIGH); 
                digitalWrite(5, HIGH); 
    Serial.print(", Painel12: 0");
  }
}


void ledPainel6(int hora) {
  bool start1 = (hora >= 0); 
  bool end1   = (hora < 6); 
  bool start2 = (hora >= 12); 
  bool end2   = (hora < 18); 
  if ( start1 && end1 ){
    digitalWrite(2, LOW);
  //      digitalWrite(3, HIGH);
      //         digitalWrite(4, HIGH);
                digitalWrite(5, HIGH);                  
    Serial.print(", Painel6: 1");
  } 
    if ( start2 && end2 ){
    digitalWrite(2, LOW);
  //      digitalWrite(3, HIGH);
     //          digitalWrite(4, HIGH);
                digitalWrite(5, HIGH);                  
    Serial.print(", Painel6: 1");
  } 
  
  if (!(start1 && end1)  && !(start2 && end2)) {
    digitalWrite(2, HIGH); 
    //    digitalWrite(3, HIGH); 
       //        digitalWrite(4, HIGH); 
                digitalWrite(5, HIGH); 
    Serial.print(", Painel6: 0");
  }
}

void ledPainel0() {
    digitalWrite(2, HIGH);
  //      digitalWrite(3, HIGH);
  //             digitalWrite(4, HIGH);
                digitalWrite(5, HIGH);
                    
    Serial.print(", Painel0: 0");
  
}

void exaustor(int temperature, int humidity, int tp, int hd) {
  bool cond = ((temperature >= tp) || (humidity > hd)); 
  unsigned long currentTime = millis(); // variável para guardar o tempo atual
  
  if (cond){
    digitalWrite(3, LOW);
    Serial.print(", Exaustor: 1");
    exaustorOn = true; // atualiza o estado do exaustor
    startTime = currentTime; // atualiza o tempo de início
  }
  else {
    
    if (exaustorOn && currentTime - startTime >= 60000)
    { // verifica se o exaustor está ligado e se passou um minuto
        digitalWrite(3, HIGH);
      Serial.print(", Exaustor: 0");
      exaustorOn = false; // atualiza o estado do exaustor
    } 
    else
    if  (exaustorOn && currentTime - startTime <= 60000)
    { // verifica se o exaustor está ligado e se passou um minuto
      Serial.print(", Exaustor: 1");
    } 
    else
    {      
      Serial.print(", Exaustor: 0");
    }

  }
}

void regar() {
  int estadoBotao = digitalRead(botaoPin); // Ler o estado do botão
  estadoBotao = !estadoBotao;

  // Enviar o estado do botão para a porta serial
  Serial.print(", Rega: ");
  Serial.print(estadoBotao);

if (estadoBotao){
digitalWrite(4, LOW);  }
  else {
   digitalWrite(4, HIGH);
}
  delay(100); // Aguardar um breve intervalo para evitar leituras erráticas
}


void boiaAlta() {
  int estadoBoia = digitalRead(8); // Ler o estado do botão
 // estadoBotao = !estadoBotao;

  // Enviar o estado do botão para a porta serial
  Serial.print(", BoiaAlta: ");
  Serial.print(!estadoBoia);

//if (estadoBotao){
//digitalWrite(4, LOW);  }
//  else {
//   digitalWrite(4, HIGH);
//}
  delay(100); // Aguardar um breve intervalo para evitar leituras erráticas
}


void boiaBaixa() {
  int estadoBoia = digitalRead(9); // Ler o estado do botão
 // estadoBotao = !estadoBotao;

  // Enviar o estado do botão para a porta serial
  Serial.print(", BoiaBaixa: ");
  Serial.print(!estadoBoia);

//if (estadoBotao){
//digitalWrite(4, LOW);  }
//  else {
//   digitalWrite(4, HIGH);
//}
//  delay(100); // Aguardar um breve intervalo para evitar leituras erráticas
}

// Função para exibir o horário digital
void digitalClockDisplay() {
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print("/");
    Serial.print(month());
    Serial.print("/");
    Serial.print(year());
   // Serial.print();  
}

// Função para imprimir os dígitos com dois caracteres
void printDigits(int digits) {
    Serial.print(":");
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);  
}

// Função sensor humidade solo
void soloHumidade() {

  int valor_analogico = analogRead(pinSensorA);
  int valor_convertido = map(valor_analogico, 1023, 460, 0, 100); //Agua = 520, ar = 1023 , terra seca= 864 e terra molhada = 460

        Serial.print(", HumSolo: ");
        Serial.print(valor_convertido);
}

void ledSerial(bool status) {
  if ( status ){
    digitalWrite(7, HIGH);                  
    Serial.print(", Serial: 1");
  } else {
    digitalWrite(7, LOW); 
    Serial.print(", Serial: 0");
  }
}

void extrairValores(String texto, int* valor1, int* valor2, int* valor3) {
  int pos1 = texto.indexOf(' '); // Encontra o primeiro espaço
  int pos3 = texto.lastIndexOf(' '); // Encontra o último espaço
  int pos2 = texto.lastIndexOf(' ', pos3 - 1); // Encontra o segundo espaço

  //String num1 = texto.substring(0, pos1); // Extrai o número antes do primeiro espaço
  String num1 = texto.substring(pos1 + 1, pos2); // Extrai o número entre os dois espaços
  String num2 = texto.substring(pos2 + 1, pos3); // Extrai o número entre os dois últimos espaços
  String num3 = texto.substring(pos3 + 1); // Extrai o número entre os dois últimos espaços

  *valor1 = num1.toInt(); // Converte a String para um número inteiro e atualiza o valor1
  *valor2 = num2.toInt(); // Converte a String para um número inteiro e atualiza o valor2
  *valor3 = num3.toInt(); // Converte a String para um número inteiro e atualiza o valor3
}
