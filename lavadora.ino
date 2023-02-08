#include <LedControl.h>

#define FIVE_MIN 5
#define TEN_MIN 10
#define FIFTEEN_MIN 15

int pin5min = 4;   // pin por el cual se va a leer la orden de 5 min
int pin15min = 3;  // pin por el cual se va a leer la orden de 15 min
int tiempoMin = 0; // almacena el tiempo de lavado
int pin4seg = 12;  // pin de salida de 4 segundos
int pin8seg = 13;  // pin de salida de 8 segundos
int pinBtn = 2;    // pin de pausar y despausar
int pinDIN = 8;
int pinLed = 5;
int pinCLK = 6;
int pinLOAD = 7;
unsigned long t4seg = 0; // tiempo ultimo del cambio de salida de 4 segundos
unsigned long t8seg = 0;
int state4seg = HIGH; // establezco en alto para que lave desde el principio
int state8seg = HIGH; // igual que arriba
int segundosTiempo = tiempoMin * 60;
unsigned long tPrevReloj = 0;                           // marca el ultimo momento de cambio del contador del reloj
bool pausa = true;                                      // por defecto pausado
LedControl lc = LedControl(pinDIN, pinCLK, pinLOAD, 1); // pines del arduino DIN,CLK,LOAD,Cantidad de max7219

void setup()
{
  // put your setup code here, to run once:
  digitalWrite(pin5min, LOW);
  digitalWrite(pin15min, LOW);
  digitalWrite(pin4seg, state4seg);
  digitalWrite(pin8seg, state8seg);
  pinMode(pin15min, INPUT);
  pinMode(pin5min, INPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(pinBtn)) // si toca el boton para prender o despausar
  {
    delay(100);               // espera para evitar rebote
    pausa = !pausa;           // despausar
    if (digitalRead(pin5min)) // si se cumple significa que no puede cumplirse nadamas
    {
      tiempoMin = FIVE_MIN;
      segundosTiempo = tiempoMin * 60; // convierte los minutos a segundos
    }
    else if (digitalRead(pin15min))
    {
      tiempoMin = FIFTEEN_MIN;
      segundosTiempo = tiempoMin * 60;
    }
    else
    { // si no se cumple ninguno de los anteriores significa que es 10 min
      tiempoMin = TEN_MIN;
      segundosTiempo = tiempoMin * 60;
    }
  }

  /*Este codigo es para cambiar cada 4 u 8 segundos los estados de las salidas*/
  // Y lavar
  if (!pausa && tiempoMin > 0)
  { // codigo para lavar
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
    if (digitalRead(pinBtn))
    {
      delay(100); // antirebote
      pausa = !pausa;
    }
  }
  if (!pausa)//si no esta en pausa significa que esta lavando
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }else{
    digitalWrite(LED_BUILTIN,LOW);
  }
  displayTime(); // muestra el tiempo
}

// funcion que muestra el tiempo en decremento por el display de 7 segmentos
void displayTime()
{
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