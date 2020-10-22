#include <Arduino.h>
#include <BLDCM-control-ino.hpp>
#include <SPI.h>

uint8_t DutyCycle = 80;
// Tle9563 Object on Shield 1
BLDCMcontrolIno MyMotor = BLDCMcontrolIno();

void setup()
{
  Serial.begin(115200);
  MyMotor.begin();
  

  Serial.println("Init ready");
}

void loop()
{
  if (Serial.available() > 0)
  {
    uint8_t in = Serial.read();
    if(in == '+') DutyCycle += 10;          // Adapt the speed with keyboard input in the serial monitor
    if(in == '-') DutyCycle -= 10;
    Serial.println(DutyCycle);
  }
  MyMotor.setBLDCspeed(DutyCycle);
}
