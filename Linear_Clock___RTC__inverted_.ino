// LINEAR CLOCK.
// This project was originally a Week-end project using elements from other projects: Arduino, Neopixel and wood support are re-used. DS1307 is later added just to play.
// Is a SIMPLE CLOCK using three colors: 5 Red LED indicates the HOUR. 1 Green LED indicates the MINUTE and 1 Blue LED indicates the SECOND. Neither date nor AM/PM indication is showed.
// 2 buttons are added to increase Hour and Minute.
// 7 LEDs are showed at a time: this allows to use +5V output from Arduino UNO pin. If more LEDs are activated, you are advised to check if an external +5Vdc power is needed to supply the Neopixel strip.
// You will need:
// 1 Arduino UNO.
// 1 NEOPIXEL LED Strip (60 LEDs per meter).
// 1 DS1307 RTC board.
// 2 Buttons
// 2 1K Ohm resistance.
// 1 5Vdc Power supply and USB cable to Arduino.
// Optional material: 1 prototyping board + 1 piece of wood to act as a support of the LED strip and Arduino.
// This code works for the Neopixel strip "upside down": the cables are in the inferior part of the strip (so the Arduino is placed near the base and closer to power plug).
// The code for Neopixel strip "straight" is called "Linear_Clock_RTC".
// Youtube video at:
// Images and more information at: Instructables.
// THIS CODE IS CREATIVE COMMONS: DISTRIBUTE AND USE IT MENTIONING THE AUTHOR @LluisLlimargas / www.llimargas.com (I'll be glad to know your opinion and suggestions).
// Stay Geek!


#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
 
#define NEOPIN 6          // Neopixel strip control to Pin 6.
#define BRIGHTNESS 64     // Maximum LED brightness definition.
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIN, NEO_GRB + NEO_KHZ800); // Neopixel strip definitiol.

RTC_DS1307 RTC;           // Real Time Clock (RTC) used.
DateTime Clock;           // RTC defined.
byte hora, minut, segon;  // Time managing variables.
int estatbotohora,estatbotominut = 0;  //Hour and Minutes button stat variables. 

void setup () {
  Wire.begin();        // Activate I2C.
  RTC.begin();         // Activate RTC.
  strip.begin();       // Activate Neopixel.
  pinMode(NEOPIN, OUTPUT);
  pinMode(2, INPUT);   // HOUR button attached at Pin 2.
  pinMode(4, INPUT);   // MINUTE button attached at Pin 4.
  
  strip.setBrightness(BRIGHTNESS);   // Set Brightness.
  
  borra_tot();                        // FYI: if the Arduino is powered from a computer, uncomment to send the computer time to Arduino.
//  if (! RTC.isrunning()) {
//    //Serial.println("RTC is NOT running!");
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }
}

//------- LOOP-------------- 
void loop () {
  Clock = RTC.now();         // Arduino gets time from DS1307. Notice that no am/pm discrimination and no date is obtained.
  hora = Clock.hour();       // Obtain Hour.
  minut = Clock.minute();    // Obtain Minutes.
  segon = Clock.second();    // Obtain Seconds.

  estatbotohora = digitalRead(2);  // Pin 2: if the button is pressed, the Hour is increased.
  if (estatbotohora == HIGH) {
    if (hora <23){            // Don't exceed from 24 h!!! (for obvious reasons... unless you live in other planet).
      hora++;
    }
    else{
      hora = 0;
    }
    segon = 0;  
    RTC.adjust(DateTime(0,0,0,hora,minut,segon));
  }
  delay(10);                // Rebounds from button avoided.
  
  estatbotominut = digitalRead(4);  // Pin 4: if the button is pressed, the Minute is increased.
  if (estatbotominut == HIGH) {
    if (minut <59){          // Don't exceed 60 minutes (unless you're workoholic and need more time to do your job).
      minut++;
    }
    else{
      minut = 0;
    }
    segon = 0;
    RTC.adjust(DateTime(0,0,0,hora,minut,segon));
  }
  delay(10);                // Rebounds from button avoided.
  
  pinta_hora(hora);
  pinta_minut(minut);
  pinta_segon(segon);
  delay(1000);               // if set to less than 1second, there is a blimp.
  borra_tot();               // After showing the LEDs, clear it all.
}
 
 
//----------------- PINTA_HORA Per encendre LEDs corresponents a l'hora.
void pinta_hora(int hora) {
  if (hora >= 12)              // Hour from 0 to 12.
    hora = hora-12;
    
  switch (hora) {
    case 0:
      pinta_leds_hora(0);    // 0 hour is showed at the extreme of the strip.
      break;
    case 1:
      pinta_leds_hora(55);     // Hour 1 is showed at the other extreme of the strip.
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

//------------------ TURNS ON THE HOUR LEDS
// Play with the Brightness parameter.
void pinta_leds_hora(int primer_led){
  strip.setPixelColor(primer_led,10,0,0);
  strip.setPixelColor(primer_led+1,70,0,0);
  strip.setPixelColor(primer_led+2,100,0,0);
  strip.setPixelColor(primer_led+3,70,0,0);  
  strip.setPixelColor(primer_led+4,10,0,0);
  strip.show();
}

//------------------ TURNS ON THE MINUTE LED
void pinta_minut(int minut){
  if (minut == 0)          // Paint the complete minute at the "end" of the strip.
    minut = 60;
  strip.setPixelColor(60-minut-1,0,10,0);
  strip.setPixelColor(60-minut,0,100,0);
  strip.setPixelColor(60-minut+1,0,10,0);
  strip.show();
}

//------------------ TURNS ON THE SECOND LED.
void pinta_segon(int segon){
  if (segon == 0)        // Paint the complete second at the "end" of the strip.
    segon = 60;
  strip.setPixelColor(60-segon,0,0,100);
  strip.show();
}

//------------------ TURNS OFF ALL THE LEDs.
void borra_tot(){
    for(int i=0;i<60;i++){
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}
