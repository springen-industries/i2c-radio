#include <Arduino.h>
#include <Wire.h>
#include <CPPM.h>


int16_t channels[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
bool prepare = true;


void requestEvent(){
    // break up request from whatever they are in to the current array
}

void setup()
{
    //start i2c on address 33
    Wire.begin(33);
    //hook up response to a request
    Wire.onRequest(requestEvent);
    //fire up the cppm library
    CPPM.begin();
}

void loop() {

    // constantly pump the last state of the control signal we have
    // recieved
    CPPM.cycle();
    if (!CPPM.synchronized())
    {
        for(int i = 0; i < 15; i++)
        {
             CPPM.write(i,channels[i]);
        }
    } else {
      Serial.println("synchronized");
    }
    delay(2);
}
