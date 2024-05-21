#ifndef Atuadores_h
#define Atuadores_h

#include "Arduino.h"
#include "Sensores.h"

extern int hs;
extern int tp;
extern int hd;

class Atuadores {

    public:
        static void ledPainel12(int hora);
        static void ledPainel6(int hora);
        static void ledPainel0();
        static void exaustor(Sensores& sensor);
        void regar();
        void boiaAlta();
        void boiaBaixa();
        void acionarBomba(int bombaPin, int boiaPin);
        void acionarBombaE(int bombaPin, int boiaPin);

    private:
        const int botaoPin = 6; // Pino do botão
        const int boiaPin1 = 8; // Pino da boia 1
        const int boiaPin2 = 9; // Pino da boia 2
        const int devolverAguaPin = 11; // Pino do sensor para devolver água

};


#endif