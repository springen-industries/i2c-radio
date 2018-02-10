#include <Arduino.h>
#include <Wire.h>


// I2C config
#define i2c_address 8
#define serial_baud 9600
//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 16  //set the number of chanels
#define default_servo_value 1599  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 9  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////
const int channelCount = 4;
/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[chanel_number];
// buffer to read measurents into via i2c
byte vals[channelCount];
// reads message from I2C and sticks it in the buffer the radio loop uses
void receiveEvent(){
     //get bits from wire.read
     Wire.readBytes(vals,channelCount); // receive byte as a character
     for (int i=0; i < channelCount; i++){
       ppm[i] = map((int)vals[i],0,256,1000,2000);
     }
}

void setup(){
  Wire.begin(i2c_address);                // join i2c bus with address as defined above
  //Wire.onReceive(receiveEvent);           // register event
  //Serial.begin(serial_baud);                 // run serial at baud as defined above
  for(int i=0; i<chanel_number; i++){
    ppm[i]= default_servo_value;
  }



  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)

  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void loop(){
  readPPM();
  delay(10);
}

/////////// clock wizardry

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;

  TCNT1 = 0;

  if(state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= chanel_number){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PulseLen;//
      OCR1A = (PPM_FrLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }
  }
}
