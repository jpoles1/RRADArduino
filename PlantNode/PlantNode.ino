// OLED Libraries
#include <Wire.h>
#include <OzOLED.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_FAST);
// Timer
#include <SimpleTimer.h>
SimpleTimer timer;
int hum;
int maxhum;
int minhum;
int warnhum;
int redled = 7;
int greenled = 3;
void setup(){
 Serial.begin(9600);
 pinMode(2, OUTPUT);
 digitalWrite(2, LOW);
 hum = readHum();
 minhum = hum;
 maxhum = hum;
 checkKnob();
 writeDisplay();
 timer.setInterval(120000, checkHum);
 timer.setInterval(10000, checkKnob);
}
void loop(){
  timer.run();
}
void checkHum(){
  hum = readHum();
  if(hum>maxhum){
    maxhum = hum; 
  }
  if(hum<minhum){
    minhum = hum; 
  }
  Serial.println(String(hum));
  writeDisplay();
}
int readHum(){
  digitalWrite(2, HIGH);
  delay(400);
  hum = 100-(analogRead(7)/10.24);
  delay(100);
  digitalWrite(2, LOW);
  return hum;
}
void checkKnob(){
   warnhum = analogRead(1)/10.24;
   writeDisplay();
}
void writeDisplay(){
  OzOled.clearDisplay();
  OzOled.printString("Humidity: ", 0, 0);
  OzOled.printNumber((long)hum, 11, 0);
  OzOled.printString("Min Humid.: ", 0, 2);
  OzOled.printNumber((long)minhum, 11, 2);
  OzOled.printString("Max Humid.: ", 0, 4);
  OzOled.printNumber((long)maxhum, 11, 4);
  OzOled.printString("Warn lvl.: ", 0, 6);
  OzOled.printNumber((long)warnhum, 11, 6);
  if(hum<warnhum){
    digitalWrite(greenled, LOW);
    digitalWrite(redled, HIGH);
  }
  else{
    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
  }
}
