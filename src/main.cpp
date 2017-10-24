#include <Arduino.h>
#include <Wire.h>
#include <CPPM.h>
#include <PXX.h>

int16_t channels[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
bool prepare = true;


void requestEvent(){
  
}

void setup()
{
    Wire.begin(33);
    Wire.onRequest(requestEvent);
    CPPM.begin();
    PXX.begin();
}

void loop() {
    if(prepare)
    {
        CPPM.cycle();
        if (CPPM.synchronized())
        {
            for(int i = 0; i < 7; i++)
            {
                channels[i] = CPPM.read_us(i);
            }

            PXX.prepare(channels);
        }
    }
    else
    {
        PXX.send();
    }

    prepare = !prepare;

    delay(2);
}
