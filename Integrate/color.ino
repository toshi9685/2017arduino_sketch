#include <Wire.h>
#include <math.h>
#define COLOR_SENSOR_ADDR  0x39//the I2C address for the color sensor 
#define REG_CTL 0x80
#define REG_TIMING 0x81
#define REG_INT 0x82
#define REG_INT_SOURCE 0x83
#define REG_ID 0x84
#define REG_GAIN 0x87
#define REG_LOW_THRESH_LOW_BYTE 0x88
#define REG_LOW_THRESH_HIGH_BYTE 0x89
#define REG_HIGH_THRESH_LOW_BYTE 0x8A
#define REG_HIGH_THRESH_HIGH_BYTE 0x8B
#define REG_BLOCK_READ 0xCF
#define REG_GREEN_LOW 0xD0
#define REG_GREEN_HIGH 0xD1
#define REG_RED_LOW 0xD2
#define REG_RED_HIGH 0xD3
#define REG_BLUE_LOW 0xD4
#define REG_BLUE_HIGH 0xD5
#define REG_CLEAR_LOW 0xD6
#define REG_CLEAR_HIGH 0xD7
#define CTL_DAT_INIITIATE 0x03
#define CLR_INT 0xE0
//Timing Register
#define SYNC_EDGE 0x40
#define INTEG_MODE_FREE 0x00
#define INTEG_MODE_MANUAL 0x10
#define INTEG_MODE_SYN_SINGLE 0x20
#define INTEG_MODE_SYN_MULTI 0x30

#define INTEG_PARAM_PULSE_1 0x00
#define INTEG_PARAM_PULSE_2 0x01
#define INTEG_PARAM_PULSE_4 0x02
#define INTEG_PARAM_PULSE_8 0x03
//Interrupt Control Register
#define INTR_STOP 40
#define INTR_DISABLE 0x00
#define INTR_LEVEL 0x10
#define INTR_PERSIST_EVERY 0x00
#define INTR_PERSIST_SINGLE 0x01
//Interrupt Souce Register
#define INT_SOURCE_GREEN 0x00
#define INT_SOURCE_RED 0x01
#define INT_SOURCE_BLUE 0x10
#define INT_SOURCE_CLEAR 0x03
//Gain Register
#define GAIN_1 0x00
#define GAIN_4 0x10
#define GAIN_16 0x20
#define GANI_64 0x30
#define PRESCALER_1 0x00
#define PRESCALER_2 0x01
#define PRESCALER_4 0x02
#define PRESCALER_8 0x03
#define PRESCALER_16 0x04
#define PRESCALER_32 0x05
#define PRESCALER_64 0x06

int readingdata[20];
int dataR, dataG, dataB, clr;
int dataR_min, dataG_min, dataB_min;
int dataR_max, dataG_max, dataB_max;


void  calibrationColorSensorManual()
{
  dataR_min = 1280;
  dataG_min = 2568;
  dataB_min = 1536;
  dataR_max = 16128;
  dataG_max = 26376;
  dataB_max = 16384;
}

void  calibrationColorSensorWhite()
{
  unsigned long timeInit;
  int count;
  unsigned long red, green, blue;

  motors.setLeftSpeed(50);
  motors.setRightSpeed(50);

  red = 0;
  green = 0;
  blue = 0;
  count = 0;
  timeInit = millis();
  while ( 1 ) {
    readRGB();
    red += dataR;
    green += dataG;
    blue += dataB;
    ++count;

    if ( millis() - timeInit > 3000 )
      break;
  }
  dataR_max = red / count;
  dataG_max = green / count;
  dataB_max = blue / count;
  
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
}
void sendcolor(){
  Serial.write('c');
  Serial.write(dataR_max >> 8);
  Serial.write(dataR_max & 255);
  Serial.write(dataG_max >> 8);
  Serial.write(dataG_max & 255);
  Serial.write(dataB_max >> 8);
  Serial.write(dataB_max & 255);
  Serial.write(dataR_min >> 8);
  Serial.write(dataR_min & 255);
  Serial.write(dataG_min >> 8);
  Serial.write(dataG_min & 255);
  Serial.write(dataB_min >> 8);
  Serial.write(dataB_min & 255);
}
void  calibrationColorSensorBlack()
{
  unsigned long timeInit;
  int count;
  unsigned long red, green, blue;

  motors.setLeftSpeed(50);
  motors.setRightSpeed(50);

  red = 0;
  green = 0;
  blue = 0;
  count = 0;
  timeInit = millis();
  while ( 1 ) {
    readRGB();
    red += dataR;
    green += dataG;
    blue += dataB;
    ++count;

    if ( millis() - timeInit > 3000 )
      break;
  }
  dataR_min = red / count;
  dataG_min = green / count;
  dataB_min = blue / count;

  
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
}

void clearInterrupt()
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(CLR_INT);
  Wire.endTransmission();
}

void readRGB()
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_BLOCK_READ);
  Wire.endTransmission();

  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.requestFrom(COLOR_SENSOR_ADDR, 8);

  if (8 <= Wire.available())  // if two bytes were received
  {
    for (int i = 0; i < 8; i++)
    {
      readingdata[i] = Wire.read();
      //Serial.println(readingdata[i],BIN);
    }
  }

  dataG = readingdata[1] * 256 + readingdata[0];
  dataR = readingdata[3] * 256 + readingdata[2];
  dataB = readingdata[5] * 256 + readingdata[4];
  clr = readingdata[7] * 256 + readingdata[6];

  red_G = map(dataR, dataR_min, dataR_max, 0, 255);
  green_G = map(dataG, dataG_min, dataG_max, 0, 255);
  blue_G = map(dataB, dataB_min, dataB_max, 0, 255);
  if ( red_G < 0 ) red_G = 0;
  if ( red_G > 255 ) red_G = 255;
  if ( green_G < 0 ) green_G = 0;
  if ( green_G > 255 ) green_G = 255;
  if ( blue_G < 0 ) blue_G = 0;
  if ( blue_G > 255 ) blue_G = 255;
}

void setupColorSensor()
{
  setTimingReg(INTEG_MODE_FREE);//Set trigger mode.Including free mode,manually mode,single synchronizition mode or so.
  setInterruptSourceReg(INT_SOURCE_GREEN); //Set interrupt source
  setInterruptControlReg(INTR_LEVEL | INTR_PERSIST_EVERY); //Set interrupt mode
  setGain(GAIN_1 | PRESCALER_4); //Set gain value and prescaler value
  setEnableADC();//Start ADC of the color sensor
}

void setTimingReg(int x)
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_TIMING);
  Wire.write(x);
  Wire.endTransmission();
  delay(100);
}
void setInterruptSourceReg(int x)
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_INT_SOURCE);
  Wire.write(x);
  Wire.endTransmission();
  delay(100);
}
void setInterruptControlReg(int x)
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_INT);
  Wire.write(x);
  Wire.endTransmission();
  delay(100);
}
void setGain(int x)
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_GAIN);
  Wire.write(x);
  Wire.endTransmission();
}
void setEnableADC()
{
  Wire.beginTransmission(COLOR_SENSOR_ADDR);
  Wire.write(REG_CTL);
  Wire.write(CTL_DAT_INIITIATE);
  Wire.endTransmission();
  delay(100);
}


