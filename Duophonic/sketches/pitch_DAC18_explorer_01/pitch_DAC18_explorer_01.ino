// Control AD5781 bipolar 18bit DAC for Pitch CV output
// VRefPos = +5V, VRefNeg = -5V
// Just test +Vref, 4V, 0V, -4V, -Vref
// Teensy 4.1
#include <SPI.h>

// CS1 10
// CS2 8
// SCLK 13
// MOSI 11
#define CS1 10

// Comment out this line for normal operation, 
// uncomment for serial-over-USB debug
//#define DEBUG

#define vals 5
const float volts[vals] = {-5.0, -4.0, 0.0, 4.0, 5.0};

uint32_t volts_to_code (float volts) {
  // trap out of range inputs
  if (volts < -5.0) {
    return 0;
  }
  else if (volts > 5.0) {
    return 262143;
  }
  else {
    // convert from -5 .. 5 to 0 .. 10
    float abs_volts = volts + 5.0;
    const float maxval = 262143.0; // 2^18 -1
    float code = abs_volts * maxval / 10.0;
    return static_cast<uint32_t>(round(code));
  }
}

SPISettings AD5781(10000000, MSBFIRST, SPI_MODE0); // max 35MHz, start at 10 MHz

void send (uint32_t value) {
  // value is 18-bit value to send  to DAC
  uint8_t buffer[3];
  // pad to 20 bits
  value = value << 2;
  // MSB has R/W and Address bits
  buffer[2] = (value >> 16) & 16;
  buffer[1] = value >> 8;
  buffer[0] = value;
  SPI.beginTransaction(AD5781);
  digitalWriteFast(CS1, LOW);
  SPI.transfer (buffer, 3); 
  digitalWriteFast(CS1, HIGH);
  SPI.endTransaction();
}


void setup() {
  pinMode(CS1, OUTPUT);
  digitalWrite(CS1, HIGH);
#ifdef DEBUG
  while (!Serial);
#endif
  delay(2000);
  SPI.begin();
}

void loop() {
  // 5 steps
  uint32_t val;
  for (int i=0; i<vals; i++) {
    val = volts_to_code(volts[i]);
    send(val);
#ifdef DEBUG
    Serial.println(val);
    Serial.println(val, HEX);
    Serial.println();
#endif
    delay(10000);   
  }

}
