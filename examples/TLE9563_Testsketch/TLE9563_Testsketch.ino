//Origin

#include <SPI.h>

//Define Inputs & Outputs
#define PWM_U 3
#define PWM_V 9
#define PWM_W 10
#define BEMF_U 5
#define BEMF_V 6
#define BEMF_W 7
#define CSN 8


//Define Motor parameters
#define MotorPoles 6
#define HallPoleShift 0



//PrintInfo


//StartUp - Commutation-Counts to switch over to closed-loop
#define OpenLoopToClosedLoopCount 50



uint8_t Commutation = 0;
uint8_t ClosedLoop = 0;
uint8_t DutyCycle = 100;
uint8_t Dir = 0;
void setup() {

  Serial.begin(115200);
  setPwmFrequency(PWM_U, 1);
  setPwmFrequency(PWM_V, 1);
  setPwmFrequency(PWM_W, 1);
  // put your setup code here, to run once:


  pinMode(CSN, OUTPUT);
  digitalWrite(CSN, HIGH);
  pinMode(PWM_U, OUTPUT);
  pinMode(PWM_V, OUTPUT);
  pinMode(PWM_W, OUTPUT);
  pinMode(BEMF_U, INPUT);
  pinMode(BEMF_V, INPUT);
  pinMode(BEMF_W, INPUT);

  SPI.begin();
  //SPI.setBitOrder(LSBFIRST);
  //SPI.setDataMode(SPI_MODE1);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);

  SPISettings settingsA(8000000, LSBFIRST, SPI_MODE1);
  SPI.beginTransaction(settingsA);

  
  analogWrite(PWM_U,0);
  analogWrite(PWM_V,0);
  analogWrite(PWM_W,0);
  SBC_CRC_Disable();
  //genctrl
  SBC_SPI(0b00010000,0b1000111000110000);
  //ls_vds mon
  SBC_SPI(0b00010010,0b0000000110110110);
  //hs_vds min
  SBC_SPI(0b00010011,0b0000000110110110);
  //hbmode
  SBC_SPI(0b00010101,0b0000101110111011);

  //clear stat regs
  SBC_SPI(0b01000000,0);
  


}

void loop() {

uint16_t i = 5000;
uint8_t CommStartup = 0;
while (i>1200) {
  delayMicroseconds(i);
  Commutation = CommStartup;
  UpdateHardware (CommStartup,0);
  CommStartup++;
  if (CommStartup==6) CommStartup=0;
  i=i-200;
  
}
while(1) {
  DoCommutation();
}
}



void DoCommutation() {

  if (Dir == 0) {
    switch (Commutation) {
      case 0:
        if (digitalRead(BEMF_W)==1) {
          Commutation = 1;
          UpdateHardware(Commutation,0);
        }
        break;
      case 1:
        if (digitalRead(BEMF_V)==0) { 
          Commutation=2;
          UpdateHardware(Commutation,0);
        }
        break;
      case 2:
        if (digitalRead(BEMF_U)==1) { 
          Commutation=3;
          UpdateHardware(Commutation,0);
        }
        break;
      case 3:
        if (digitalRead(BEMF_W)==0) { 
          Commutation=4;
          UpdateHardware(Commutation,0);
        }
        break;
      case 4:
        if (digitalRead(BEMF_V)==1) { 
          Commutation=5;
          UpdateHardware(Commutation,0);
        }
        break;
      case 5:
        if (digitalRead(BEMF_U)==0) { 
          Commutation=0;
          UpdateHardware(Commutation,0);
        }
        break;
      default:
      break;
      
    }
  }
}
//defining commutation steps according to HALL table
void UpdateHardware(uint8_t CommutationStep, uint8_t Dir) {
  //CW direction
  if (Dir == 0) {

    switch (CommutationStep) {
      case 0:
        
        SBC_SPI(0b00010101,0b0000110001001011);
        analogWrite(PWM_U, DutyCycle);
        analogWrite(PWM_V, 0);
        analogWrite(PWM_W, 0);
        break;

      case 1:
        SBC_SPI(0b00010101,0b0000010011001011);
        analogWrite(PWM_U, DutyCycle);
        analogWrite(PWM_V, 0);
        analogWrite(PWM_W, 0);
        break;

      case 2:
        SBC_SPI(0b00010101,0b0000010010111100);
        analogWrite(PWM_U, 0);
        analogWrite(PWM_V, DutyCycle);
        analogWrite(PWM_W, 0);
        break;

      case 3:
      SBC_SPI(0b00010101,0b0000110010110100);
        analogWrite(PWM_U, 0);
        analogWrite(PWM_V, DutyCycle);
        analogWrite(PWM_W, 0);
        break;

      case 4:
        SBC_SPI(0b00010101,0b0000101111000100);
        analogWrite(PWM_U, 0);
        analogWrite(PWM_V, 0);
        analogWrite(PWM_W, DutyCycle);
        break;

      case 5:
       SBC_SPI(0b00010101,0b0000101101001100);
        analogWrite(PWM_U, 0);
        analogWrite(PWM_V, 0);
        analogWrite(PWM_W, DutyCycle);
        break;

      default:
        break;
    }

  }
  }


void SBC_CRC_Disable() {
  digitalWrite(CSN,LOW);
  SPI.transfer(0xe7);
  SPI.transfer(0x55);
  SPI.transfer(0x55);
  SPI.transfer(0xc3);
  digitalWrite(CSN,HIGH);
}
uint16_t SBC_SPI(uint8_t addr, uint16_t data) {
  uint16_t outdata = 0;
  digitalWrite(CSN,LOW);
  SPI.transfer(addr|0x80);
  outdata = (uint16_t) SPI.transfer(data&0xFF);
  outdata |= (uint16_t) (SPI.transfer((data>>8)&0xFF)<<8);
  SPI.transfer(0xA5);
  digitalWrite(CSN,HIGH);
}
/**
   Divides a given PWM pin frequency by a divisor.

   The resulting frequency is equal to the base frequency divided by
   the given divisor:
     - Base frequencies:
        o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
        o The base frequency for pins 5 and 6 is 62500 Hz.
     - Divisors:
        o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
          256, and 1024.
        o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
          128, 256, and 1024.

   PWM frequencies are tied together in pairs of pins. If one in a
   pair is changed, the other is also changed to match:
     - Pins 5 and 6 are paired on timer0
     - Pins 9 and 10 are paired on timer1
     - Pins 3 and 11 are paired on timer2

   Note that this function will have side effects on anything else
   that uses timers:
     - Changes on pins 3, 5, 6, or 11 may cause the delay() and
       millis() functions to stop working. Other timing-related
       functions may also be affected.
     - Changes on pins 9 or 10 will cause the Servo library to function
       incorrectly.

   Thanks to macegr of the Arduino forums for his documentation of the
   PWM frequency divisors. His post can be viewed at:
     https://forum.arduino.cc/index.php?topic=16612#msg121031
*/
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}