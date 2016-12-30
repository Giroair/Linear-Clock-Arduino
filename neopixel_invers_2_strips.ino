#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
 
#define NEOPIN 6          // Tira Neopixel a Pin 6.
#define NEOPIN2 7         // Second strip.
#define BRIGHTNESS 64     // Defineix brillantor màxima.
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIN, NEO_GRB + NEO_KHZ800); // Crea tira de Neopixel.
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, NEOPIN2, NEO_GRB + NEO_KHZ800); // Create 2nd strip of Neopixel


RTC_DS1307 RTC;           // Crea rellotge.
DateTime Clock;           // Defineix rellotge.
byte hora, minut, segon;  // Variables per gestionar temps.
int estatbotohora,estatbotominut = 0;  //Variables per estat polsadors Hora i 

void setup () {
  Wire.begin();        // Activa I2C.
  RTC.begin();         // Activa rellotge.
  strip.begin();       // Activa tira Neopixel.
  strip2.begin();
  pinMode(NEOPIN, OUTPUT);
  pinMode(NEOPIN2, OUTPUT);
  pinMode(2, INPUT);   // Polsador HORES.
  pinMode(4, INPUT);   // Polsador MINUTS.
  
  strip.setBrightness(BRIGHTNESS);   // Defineix lluentor.
  
  borra_tot();
//  if (! RTC.isrunning()) {
//    //Serial.println("RTC is NOT running!");
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }
}

//------- LOOP-------------- 
void loop () {
  Clock = RTC.now();         // Obtè hora del DS1307.
  hora = Clock.hour();       // Separa l'hora.
  minut = Clock.minute();    // Separa els minuts.
  segon = Clock.second();    // Separa els segons.

  estatbotohora = digitalRead(2);  // Pin 2: polsador increment hores.
  if (estatbotohora == HIGH) {
    if (hora <23){            // No passar de 24 h!!!
      hora++;
    }
    else{
      hora = 0;
    }
    segon = 0;  
    RTC.adjust(DateTime(0,0,0,hora,minut,segon));
  }
  delay(10);                // Evita rebots del polsador.
  
  estatbotominut = digitalRead(4);  // Pin 4: polsador increment minuts.
  if (estatbotominut == HIGH) {
    if (minut <59){          // No passar de 60 m!!!
      minut++;
    }
    else{
      minut = 0;
    }
    segon = 0;
    RTC.adjust(DateTime(0,0,0,hora,minut,segon));
  }
  delay(10);                // Evita rebots del polsador.
  
  pinta_hora(hora);
  pinta_minut(minut);
  pinta_segon(segon);
  delay(1000);               // Menys d'1 seg es veu parpadeig.
  borra_tot();               // Després d'ensenyar la H/M/S, esborra tot.
}
 
 
//----------------- PINTA_HORA Per encendre LEDs corresponents a l'hora.
void pinta_hora(int hora) {
  if (hora >= 12)              // Per forçar hora a estar entre 0 i 12.
    hora = hora-12;
    
  switch (hora) {
    case 0:
      pinta_leds_hora(0);    // Les 0 hores es mostren al final de la tira.
      break;
    case 1:
      pinta_leds_hora(55);     // La hora "1" es mostra al començament de la tira, i la resta a continuació. 
      break;
    case 2:
      pinta_leds_hora(50);
      break;
    case 3:
      pinta_leds_hora(45);
      break;
    case 4:
      pinta_leds_hora(40);
      break;
    case 5:
      pinta_leds_hora(35);
      break;      
    case 6:
      pinta_leds_hora(30);
      break;      
    case 7:
      pinta_leds_hora(25);
      break;
    case 8:
      pinta_leds_hora(20);
      break;
    case 9:
      pinta_leds_hora(15);
      break;
    case 10:
      pinta_leds_hora(10);
      break;
    case 11:
      pinta_leds_hora(5);
      break;
    default:
    break;
  }
}

//------------------ PINTA_LEDS_HORA
//-Es juga amb la lluentor per fer cobrir una zona de la tira.
void pinta_leds_hora(int primer_led){
  strip.setPixelColor(primer_led,10,0,0);
  strip.setPixelColor(primer_led+1,70,0,0);
  strip.setPixelColor(primer_led+2,100,0,0);
  strip.setPixelColor(primer_led+3,70,0,0);  
  strip.setPixelColor(primer_led+4,10,0,0);
  strip.show();
}

//------------------ PINTA_MINUT Per encendre LEDs corresponents a Minuts.
void pinta_minut(int minut){
  if (minut == 0)          // Per pintar els minuts complerts al final de la barra.
    minut = 60;
  strip2.setPixelColor(60-minut-1,0,10,0);
  strip2.setPixelColor(60-minut,0,100,0);
  strip2.setPixelColor(60-minut+1,0,10,0);
  strip2.show();
}

//------------------ PINTA_SEGON Per encendre LEDs corresponents a Segons.
void pinta_segon(int segon){
  if (segon == 0)        // Per pintar els segons complerts al final de la barra.
    segon = 60;
  strip2.setPixelColor(60-segon,0,0,100);
  strip2.show();
}

//------------------ BORRA_TOT Apaga tota la tira de LEDs Neopixel.
void borra_tot(){
    for(int i=0;i<60;i++){
      strip.setPixelColor(i, 0, 0, 0);
      strip2.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    strip2.show();
}
