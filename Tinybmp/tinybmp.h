/*
Header-File
tinybmp180.h
2015.06.18
*/

//Wird benötigt um eine "doppel-definierung"
//zu verhindern
#ifndef tinybmp180_H
#define tinybmp180_H

#include "Arduino.h"
#include "TinyWireM.h"

//I2C add ausgelesen mit I2C-Scanner
#define bmp_add 0x77

//mode = 0 -> ulltralow
#define bmp_mode 0

//Adressen aus Datenblatt ausgelesen
#define bmp_ac1 0xAA
#define bmp_ac2 0xAC
#define bmp_ac3 0xAE
#define bmp_ac4 0xB0
#define bmp_ac5 0xB2
#define bmp_ac6 0xB4
#define bmp_b1 0xB6
#define bmp_b2 0xB8
#define bmp_mb 0xBA
#define bmp_mc 0xBC
#define bmp_md 0xBE

/*
Adressen um in register zu schreiben,
bzw zu lesen
*/
#define bmp_reg 0xF4
#define bmp_temperatur_raw 0x2E
#define bmp_pressure_raw 0x34
#define bmp_temp 0xF6
#define bmp_pres 0xF6

/*
Die Klasse bmp180 soll dazu dienen die temperatur
und den druck zu lesen.
Die "unkompensierten"-Daten werden für die spätere berechnung
der Realwerte benötigt.
Temperatur wird in Grad=*C berechnet.
Druck wird in Pascale=Pa berechnet.
*/
class tinybmp180
{
  private:
  uint16_t read16(uint8_t add);
  uint8_t read8(uint8_t add);
  void write8(uint8_t add, uint8_t data);
  
  int16_t ac1,ac2,ac3,b1,b2,mb,mc,md;
  uint16_t ac4,ac5,ac6;
  
  uint8_t oss;
  
  public:
  tinybmp180();
  boolean begin();
  int32_t readPressure();
  float readTemp();
  uint16_t readRawTemp();
  uint32_t readRawPres();
  };
  
#endif
