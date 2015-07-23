#include "tinybmp.h"

//Konstruktor
tinybmp180::tinybmp180(){
  }

/*
mode wird festgelegt auf 0
Alle kallibrierungswerte werden ausgelesen
*/
boolean tinybmp180::begin(){
  oss = bmp_mode;
  
  //Wird für weitere operationen benötigt  
  TinyWireM.begin();
  
  //kallibrierungs Werte auslesen
  ac1 = read16(bmp_ac1);
  ac2 = read16(bmp_ac2);
  ac3 = read16(bmp_ac3);
  ac4 = read16(bmp_ac4);
  ac5 = read16(bmp_ac5);
  ac6 = read16(bmp_ac6);
  b1 = read16(bmp_b1);
  b2 = read16(bmp_b2);
  mb = read16(bmp_mb);
  mc = read16(bmp_mc);
  md = read16(bmp_md);
 
return true;
}

/*
Temperatur aus den ausgelesenen Werten berechnen
Die Temperatur muss als float wert zurückgegeben werden
damit eine gleitkommazahl ausgegeben werden kann
*/
float tinybmp180::readTemp(){
  int32_t ut,b5;
  float temperatur;
  
  //ut = uncompensated temperature
  ut = readRawTemp();
  
  /*
  Berechnug der realen Temperatur in *C
  Berechnungen aus dem Datenblatt ausgelesen
  */
  int32_t x1 = ((ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
  int32_t x2 = ((int32_t)mc << 11) / (x1 + (int32_t)md);
  
  b5 = x1 + x2;

  temperatur = (b5 +8) >> 4;
  temperatur /= 10.0;
  
  return temperatur;
}

/*
Druck aus den ausgelesenen Werten berechnen
Der Druck kann als int32_t zurück gegeben werden
da er un Pascale angezeigt wird.
(Bei umrechnung in bar muss über einen neunen
Rückgabetyp nachgedacht werden!)
*/
int32_t tinybmp180::readPressure(){
 int32_t ut,up,b3,b5,b6,x1,x2,x3,p;
 uint32_t b4,b7;
 
  //ut = uncompensated temperature
  //up = uncompensated pressure
  up = readRawPres();
  ut = readRawTemp();
  
  /*
  Berechnung des realen Drucks in Pascale
  Berechnungen aus dem Datenblatt ausgelesen
  */
  int32_t x11 = ((ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
  int32_t x22 = ((int32_t)mc << 11) / (x11 + (int32_t)md);
  
  b5 = x11 + x22;
  
  b6 = b5 - 4000;
  x1 = ((int32_t)b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = ((int32_t)ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t)ac1*4) + x3) << oss) + 2) / 4;
  x1 = ((int32_t)ac3 * b6) >> 13;
  x2 = (((int32_t)b1 * (b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = ((int32_t)ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)up - b3) * (uint32_t)(50000 >> oss);
  if (b7 < 0x80000000){
    p = (b7 * 2) / b4;
  }
  else{
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p = p + ((x1 + x2 + (int32_t)3791) >> 4);
  
  return p;
}

/*
auslesung der unkompensierten TempDaten
Es wird ein uint16_t zurückgegeben
*/
uint16_t tinybmp180::readRawTemp(){
  write8(bmp_reg,bmp_temperatur_raw);
  
  delay(30);
  
  return read16(bmp_temp);
}
  
/*
auslesung der unkompensierten PresData
Es wird ein uint32_t zurückgegeben
*/
uint32_t tinybmp180::readRawPres(){
  uint32_t temp;
  write8(bmp_reg,bmp_pressure_raw + (oss << 6));
 
  delay (30);
 
  temp = read16(bmp_pres);
  temp <<= 8;
  temp |= read8(bmp_pres + 2);
  temp >>= (8 - oss);
  
 return temp;
}

/*
Allgemeine Funktionen zum lesen und schreiben von Werten über I2C
*/

/*
Werte mit 8Bit schreiben
*/
void tinybmp180::write8(uint8_t add, uint8_t data){
  TinyWireM.beginTransmission(bmp_add);
  TinyWireM.send(add);
  TinyWireM.send(data);
  TinyWireM.endTransmission();
}

/*
Werte mit 16Bit auslesen
*/
uint16_t tinybmp180::read16(uint8_t add){
  uint16_t temp;
  TinyWireM.beginTransmission(bmp_add);
  TinyWireM.send(add);
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(bmp_add);
  TinyWireM.requestFrom(bmp_add,2);
  temp = TinyWireM.receive();
  temp <<= 8;
  temp |= TinyWireM.receive();
  TinyWireM.endTransmission();
  
  return temp;
}

/*
Werte mit 8Bit auslesen
*/
uint8_t tinybmp180::read8(uint8_t add){
  uint16_t temp;
  TinyWireM.beginTransmission(bmp_add);
  TinyWireM.send(add);
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(bmp_add);
  TinyWireM.requestFrom(bmp_add,2);
  temp = TinyWireM.receive();
  TinyWireM.endTransmission();
  
  return temp;
}
