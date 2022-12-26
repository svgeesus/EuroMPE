// working 02 Mar 2018
// Sparkfun RGB encoder
// 10k from A, B pins to output
// 50nF from output to 0V
// C pin to 0V
// no pullup (library uses internal pullup)
// "+" pin on breakout seems to be an (unconnected) 0V
// so connect to Teensy 0V

#include <Encoder.h> 
#include <Bounce.h>

Encoder enc(0,1); // create an encoder object

const int switchPin = 2;
Bounce pushbutton = Bounce(switchPin, 5);  // 5 ms debounce

int32_t current_value; // current read
int32_t previous_value = -999; // previous read

byte previousState = HIGH;         // what state was the button last time
unsigned int count = 0;            // how many times has it changed to low
unsigned long countAt = 0;         // when count changed
unsigned int countPrinted = 0;     // last count printed

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  Serial.begin(57600); // open serial port
  while(!Serial);
}

void loop() {
   
  current_value = enc.read(); // read the value of the encoder
  
  if (current_value != previous_value) { // compare to previous reading
    Serial.println(current_value); // if it's changed, print to serial port
    previous_value = current_value; // store the reading for next comparison
  }
  if (pushbutton.update()) {
    if (pushbutton.fallingEdge()) {
      count = count + 1;
      countAt = millis();
      current_value = 0;  
      enc.write(current_value); // reset
      Serial.println(current_value); 
      previous_value = current_value; // store the reading for next comparison
    }
  } else {
    if (count != countPrinted) {
      unsigned long nowMillis = millis();
      if (nowMillis - countAt > 100) {
        Serial.print("count: ");
        Serial.println(count);
        countPrinted = count;
      }
    }
  }
}
