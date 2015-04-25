// OLED Libraries
#include <Wire.h>
#include <OzOLED.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//Radio Libraries
#include <SPI.h>
#include "nRF24L01.h";
#include <RF24_config.h>
#include "RF24.h"
#include "printf.h"
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 8
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
//                             Writing        Temp+Light          Unused          Unused
const uint64_t pipes[4] = {0xF0F0F0F0E4LL, 0xE8E8F0F0E1LL,  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
double Fahrenheit(double celsius){
	return 1.8 * celsius + 32;
}
struct payload_t
{
  int16_t id;
  int16_t var1;
  int16_t var2;
};
void setup(void){
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.stopListening();
}
void loop(void){
    int temp;
    int chk = DHT11.read(DHT11PIN);
    if(chk==DHTLIB_OK){
       temp = int(Fahrenheit(DHT11.temperature));
    }
    else{
      temp = 1;
    }
    int dater = analogRead(6);
    payload_t dat = {(int)1, temp, dater};

    //radio.stopListening();
    int wr = radio.write(&dat, sizeof(dat));
    radio.startListening();
    OzOled.clearDisplay();
    printf("%d", dat.var1);
    OzOled.printString("Temp:", 0, 0);
    OzOled.printNumber((long)dat.var1, 7, 0);
    OzOled.printString("Light:", 0, 2);
    OzOled.printNumber((long)dat.var2, 7, 2);
    OzOled.printString("Sent:", 0, 4);
    OzOled.printNumber((long)wr, 7, 4);
    delay(4000);
    radio.stopListening();
}

