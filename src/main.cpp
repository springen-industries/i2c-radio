#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <PXX.h>


#define PPM_PIN 9

int16_t channels[8] = { 0,0,0,0,0,0,0,0 };
Wcppm wCPPM;


// the data pin for the NeoPixels
int neoPixelPin = 6;
// How many NeoPixels we will be using, charge accordingly
int numPixels = 1;
// Instatiate the NeoPixel from the ibrary
// create a one pole (RC) lowpass filter

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);


void requestEvent(){
    // break up channel line recieved via i2c
    // translate it into the conteents of the channels array
    // reset the last-heard-from failsafe
}

void setup() {
  strip.begin();
  strip.setPixelColor(0, 25, 255, 25);
  strip.show();
  pinMode(PPM_PIN, OUTPUT);
  wCPPM.begin(PPM_PIN);
}

void loop() {

    // constantly pump the last state of the control signal we have
    // recieved
    for(int i = 0; i < 7; i++) {
        wCPPM.setChannel(i,channels[i]);
    }

}
