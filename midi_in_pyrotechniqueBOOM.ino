#include "FastLED.h"
#define NUM_LEDS 8//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define DATA_PIN 16
CRGB leds[NUM_LEDS];

#include <Bounce2.h>
Bounce D2 = Bounce();
Bounce D3 = Bounce();
Bounce D4 = Bounce();
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
byte data;
bool firestate = 0;
int s = 0;
byte ledstep = 0 ;
byte ledfade[8] = {32, 32, 32, 32, 32, 32, 32, 32};

void setup() {
  Serial.begin(31250);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  D2.attach(2);
  D2.interval(25); // interval in ms
  D3.attach(3);
  D3.interval(25); // interval in ms
  D4.attach(4);
  D4.interval(25); // interval in ms


  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(17, LOW);
  delay(500);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = ledfade[i];
    leds[i].g = 0;
    leds[i].b = 0;
  }
  FastLED.show();
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();
    data = data & B11110000;
    //Serial.print(data);
    if (data == B10010000 && firestate == 1 && s<32) { // midi note on + go vert
      digitalWrite(7, LOW);
      digitalWrite(17, HIGH);
      ledstep = s/4;
      
      ledfade[ledstep] = 2*sq((3-(s % 4)));
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].g = ledfade[i];
      }
      s++;

      delay(50);
      digitalWrite(7, HIGH);
      digitalWrite(17, LOW);
      FastLED.show();
    }
  }

  D2.update(); D3.update(); D4.update();
  if ( D2.fell() ) {
    firestate = !firestate;
    if (firestate == 0) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = ledfade[i];
        leds[i].g = 0;
        leds[i].b = 0;
      }
    }
    else  {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = 0;
        leds[i].g = ledfade[i];
        leds[i].b = 0;
      }
    }
    FastLED.show();
    digitalWrite(6, LOW);
    delay(50);
    digitalWrite(6, HIGH);
  }

}

