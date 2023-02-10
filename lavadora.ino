#include <LedControl.h>
/*Author Code: César Fernández García
Author Electronic: Héctor Fernández Díaz
Finished: 10/02/23*/
#define FIVE_MIN 5
#define TEN_MIN 10
#define FIFTEEN_MIN 15

const int pin5min = 4;  // pin por el cual se va a leer la orden de 5 min
const int pin15min = 3; // pin por el cual se va a leer la orden de 15 min
int tiempoMin = 0;
const int pin4seg = 8;
const int pin8seg = 9;
const int pinBtn = 7;
const int pinDIN = 11;
const int pinCLK = 10;
const int pinLOAD = 12;
const int pinLed = 5;
unsigned long t4seg = 0;
unsigned long t8seg = 0;
int state4seg = HIGH;
int state8seg = HIGH;
int segundosTiempo = tiempoMin * 60;
unsigned long tPrevReloj = millis();
bool pausa = true;                                 // desde el principio esta en pausa
int stateBtn = LOW;
int tiempoUltimo = 0;
LedControl lc = LedControl(pinDIN, pinCLK, pinLOAD, 1); // pines del arduino DIN,CLK,LOAD,Cantidad de max7219
void setup()
{
  // put your setup code here, to run once:
  pinMode(pin15min, INPUT);
  pinMode(pin5min, INPUT);
  pinMode(pinBtn, INPUT);
  pinMode(pin4seg, OUTPUT);
  pinMode(pin8seg, OUTPUT);

  digitalWrite(pin5min, LOW);
  digitalWrite(pin15min, LOW);
  digitalWrite(pin4seg, state4seg);
  digitalWrite(pin8seg, state8seg);

  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);

}

void loop()
{
  escojerTiempo();

  /*Este codigo es para cambiar cada 4 u 8 segundos los estados de las salidas*/
  lavar();
  // se acaba el codigo para cambiar cada 4 u 8 segundos

  if (digitalRead(pinBtn) == HIGH)
  { delay(100);
    bool solto = false;
    while (!solto) {
      if (digitalRead(pinBtn) == LOW) {
        delay(100);
        solto = true;
      }
    }
    if (solto) {
      pausa = !pausa;
    }
  }
  if (!pausa) // si no esta en pausa significa que esta lavando
  {
    digitalWrite(pinLed, HIGH); // prende el led para visualizar el lavado
  }
  else
  {
    digitalWrite(pinLed, LOW);
  }
  if (pausa) {
    digitalWrite(pin4seg, LOW);
    digitalWrite(pin8seg, LOW);
  }
  if (!pausa) {
    displayTime(); // muestra el tiempo
  } else { //muestra el tiempo
    int s;
    s = segundosTiempo % 60;
    int m;
    m = segundosTiempo / 60;

    lc.setDigit(0, 0, s % 10, false);
    lc.setDigit(0, 1, (s / 10) % 10, false);
    lc.setDigit(0, 2, m % 10, true);
    lc.setDigit(0, 3, (m / 10) % 10, false);
  }


}

// funcion que muestra el tiempo en decremento por el display de 7 segmentos
void displayTime(){
  int s;
  s = segundosTiempo % 60;
  int m;
  m = segundosTiempo / 60;

  lc.setDigit(0, 0, s % 10, false);
  lc.setDigit(0, 1, (s / 10) % 10, false);
  lc.setDigit(0, 2, m % 10, true);
  lc.setDigit(0, 3, (m / 10) % 10, false);

  if (millis() - tPrevReloj >= 1000)
  {
    tPrevReloj = millis();

    segundosTiempo--; // le quito un segundo
  }
}
void lavar(){
  // este codigo es para lavar
  if (!pausa && segundosTiempo > 0)
  {
    if ((millis() - t4seg) >= 4000)
    {
      t4seg = millis();
      digitalWrite(pin4seg, !state4seg);
      state4seg = !state4seg; // cambia su estado
    }
    if ((millis() - t8seg) >= 8000)
    {
      t8seg = millis();
      digitalWrite(pin8seg, !state8seg);
      state8seg = !state8seg; // cambia su estado
    }
  } if (segundosTiempo <= 0) {
    digitalWrite(pin8seg, LOW);
    digitalWrite(pin4seg, LOW);
    pausa = true;
  }
}
void escojerTiempo(){
  if (pausa && tiempoUltimo != posicionActualTiempo()) {

    if (digitalRead(pin5min) == HIGH && digitalRead(pin15min) == LOW) // si se cumple significa que no puede cumplirse nadamas
    {
      delay(100);
      tiempoMin = FIVE_MIN;
      segundosTiempo = tiempoMin * 60; // convierte los minutos a segundos
    }
    else if (digitalRead(pin5min) == LOW && digitalRead(pin15min) == HIGH)
    {
      delay(100);
      tiempoMin = FIFTEEN_MIN;
      segundosTiempo = tiempoMin * 60;
    }
    else if (digitalRead(pin5min) == LOW && digitalRead(pin15min) == LOW) {
      tiempoMin = TEN_MIN;
      segundosTiempo = tiempoMin * 60;
    }
    tiempoUltimo = tiempoMin;


  }

}
int posicionActualTiempo() {
  int tiempoActual = tiempoUltimo;
  if (digitalRead(pin5min) == HIGH && digitalRead(pin15min) == LOW) // si se cumple significa que no puede cumplirse nadamas
  {
    delay(100);
    tiempoActual = FIVE_MIN;

  }
  else if (digitalRead(pin5min) == LOW && digitalRead(pin15min) == HIGH)
  {
    delay(100);
    tiempoActual = FIFTEEN_MIN;
  }
  else if (digitalRead(pin5min) == LOW && digitalRead(pin15min) == LOW) {
    delay(100);
    tiempoActual = TEN_MIN;
  }
  return tiempoActual;
}
