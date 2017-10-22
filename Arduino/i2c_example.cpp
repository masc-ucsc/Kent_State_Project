// Example 20.1

int dt = 2000; // used for delay duration
byte rval = 0x00; // used for value sent to potentiometer
#include "Wire.h"
#define pot_address 0x2F // each I2C object has a unique bus address, the MCP4018 is 0x2F or 0101111 in binary

void setup()
{
  Wire.begin();
  Serial.begin(9600); 
}

void potLoop()
// sends values of 0x00 to 0x7F to pot in order to change the resistance
// equates to 0~127
{
  for (rval=0; rval<128; rval++)
  {
    Wire.beginTransmission(pot_address);
    Wire.write(rval); // 
    Wire.endTransmission();
    Serial.print(" sent - ");
    Serial.println(rval, HEX);
    delay(dt);
  }
}

void loop()
{
  potLoop();
}
