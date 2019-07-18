#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#include "FastLED.h"
#define NUM_LEDS 8//
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

#include <Bounce2.h>
Bounce D2 = Bounce();
bool firestate = 0;
int s = 0;
byte ledstep = 0 ;
byte ledfade[8] = {32, 32, 32, 32, 32, 32, 32, 32};

byte seuilFrappe = 100; //////////////////// ==> SENSIBILITE FRAPPE PADS !!!!!!!!!!

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode(2, INPUT_PULLUP);
  D2.attach(2);
  D2.interval(50); // interval in ms

  pinMode(5, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(14, LOW);
  digitalWrite(13, LOW);
  delay(100);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = ledfade[i];
    leds[i].g = 0;
    leds[i].b = 0;
  }
  FastLED.show();
}

void loop() {
  if (MIDI.read())                // Is there a MIDI message incoming ?
  {
    switch (MIDI.getType())     // Get the type of the message we caught
    {
      case midi::NoteOn:       // If it is a note on
        if (firestate == 1 && s < 32 && MIDI.getData1() == 47 && MIDI.getData2() > seuilFrappe) {// DTXPRESSIII => 4-Tom2 + VELOCITY
          digitalWrite(13, HIGH);
          digitalWrite(5, HIGH);
          delay(50);
          digitalWrite(13, LOW);
          digitalWrite(5, LOW);
          ledstep = s / 4;
          ledfade[ledstep] = 2 * sq((3 - (s % 4)));
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i].g = ledfade[i];
          }
          s++;
          FastLED.show();
        }
        else if (firestate == 1  && MIDI.getData1() == 48 && MIDI.getData2() > seuilFrappe) {// DTXPRESSIII => 3-Tom1 + VELOCITY
          digitalWrite(13, HIGH);
          digitalWrite(14, HIGH);
          delay(50);
          digitalWrite(13, LOW);
          digitalWrite(14, LOW);
        }
        else if (MIDI.getData2() > seuilFrappe){
          digitalWrite(13, HIGH);
          delay(100);
          digitalWrite(13, LOW);
        }
        break;
      default:
        break;
    }
  }
  D2.update(); //D3.update(); D4.update();
  if ( D2.fell() ) {
    firestate = !firestate;
    if (s >= 32) {
      ledfade[0] = 0; ledfade[1] = 0; ledfade[2] = 32; ledfade[3] = 32; ledfade[4] = 32; ledfade[5] = 32; ledfade[6] = 0; ledfade[7] = 0;
    }
    if (firestate == 0) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = ledfade[i];
        leds[i].g = 0;
        //leds[i].b = 0;
      }
    }
    else  {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = 0;
        leds[i].g = ledfade[i];
        //leds[i].b = 0;
      }
    }
    FastLED.show();
  }
}
