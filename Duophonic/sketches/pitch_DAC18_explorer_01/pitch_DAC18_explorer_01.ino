// Control AD5781 bipolar 18bit DAC for Pitch CV output
// VRefPos = +5V, VRefNeg = -5V
// Just test +Vref, 4V, 0V, -4V, -Vref
// Teensy 4.1
// See https://wiki.analog.com/resources/quick-start/ad5791

#include <SPI.h>

// CS1 10
// CS2 8
// SCLK 13
// MOSI 11
#define CS1 10

// Comment out this line for normal operation, 
// uncomment for serial-over-USB debug
#define DEBUG

#define vals 5
const float volts[vals] = {-5.0, -4.0, 0.0, 4.0, 5.0};

uint32_t volts_to_code (float volts) {
  // trap out of range inputs
  // offset binary, not 2s complement
  if (volts < -5.0) {
    return 0; // gives Vref_N
  }
  else if (volts > 5.0) {
    return 262143; // gives Vref_P
  }
  else {
    // first convert from -5 .. 5 to 0 .. 10
    float abs_volts = volts + 5.0;
    // now generate 18-bitcode value
    const float maxval = 262143.0; // 2^18 -1
    float code = abs_volts * maxval / 10.0;
    return static_cast<uint32_t>(round(code));
  }
}

SPISettings AD5781(10000000, MSBFIRST, SPI_MODE1); // max 35MHz, start at 10 MHz

void enable () {
  // See datasheet Table 11. Control Register and Table 12. Control Register Functions
  uint8_t buffer[3];
  // remove ground clamp, remove tristate, use offset binary rather than 2's complement
  // see https://ez.analog.com/data_converters/precision_dacs/f/q-a/110156/spi-mode-of-ad5791
  buffer[2] = 0B00100000; // write to control register
  buffer[1] = 0;
  buffer[0] = 0B00010010; // OPGND to 0, 1 is the default. DACTRI to 0, 1 is default. Leave RBUF at 1. BIN/2sC to binary
  SPI.beginTransaction(AD5781);
    // See https://ez.analog.com/data_converters/precision_dacs/f/q-a/27109/ad5781---not-working
  delayMicroseconds(50);
  digitalWriteFast(CS1, LOW);
  SPI.transfer (buffer, 3); 
  digitalWriteFast(CS1, HIGH);
  SPI.endTransaction();

}

void send (uint32_t value) {
  // See datasheet Table 10. DAC Register
  // LDAC tied low, CLR, RESET tied high.
  // value is 18-bit value to send  to DAC
  uint8_t buffer[3];
  // pad to 20 bits
  value = value << 2;// //
  // MSB has R/W and Address bits
  // 0 0 0 1 = The DAC register is loaded with the clearcode register value, and the output is set accordingly
  buffer[2] = (value >> 16) | 0B00010000;
  buffer[1] = value >> 8;
  buffer[0] = value;
#ifdef DEBUG
    Serial.print("value ");
    Serial.println(value);
    Serial.println(value, HEX);
    Serial.println();
    Serial.print("buffer (hex) ");
    Serial.println(buffer[0], HEX);
    Serial.println(buffer[1], HEX);
    Serial.println(buffer[2], HEX);
    Serial.println();
#endif
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
  enable();
}

void loop() {
  // 5 steps
  uint32_t val;
  for (int i=0; i<vals; i++) {
#ifdef DEBUG
    Serial.print("volts ");
    Serial.println(volts[i]);
    Serial.println();
#endif
    val = volts_to_code(volts[i]);
    send(val);
#ifdef DEBUG
    Serial.print("val ");
    Serial.println(val);
    Serial.println(val, HEX);
    Serial.println();
#endif
    delay(10000);   
  }

}
