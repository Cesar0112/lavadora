/*Prueba de impresion en el display de 7 segmentos
  César Fernandez García
  2/2/23*/
#include <LedControl.h>
LedControl lc = LedControl(12, 11, 10, 1); //pines del arduino DIN,CLK,LOAD,Cantidad de max7219
int tiempoMin = 5;
int segundosTiempo = tiempoMin * 60;
unsigned long tInicial = 0;
void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

}

void loop() {
  // put your main code here, to run repeatedly:
  displayTime();

}
//funcion que muestra el tiempo en decremento por el display de 7 segmentos
void displayTime() {
  int s;
  s = segundosTiempo % 60;
  int m;
  m = segundosTiempo / 60;

  lc.setDigit(0, 0, s % 10, false);
  lc.setDigit(0, 1, (s / 10) % 10, false);
  lc.setDigit(0, 2, m % 10, true);
  lc.setDigit(0, 3, (m / 10) % 10, false);
  if (millis() - tInicial >= 1000) {
    tInicial = millis();
    segundosTiempo--;//le quito un segundo
  }

}
