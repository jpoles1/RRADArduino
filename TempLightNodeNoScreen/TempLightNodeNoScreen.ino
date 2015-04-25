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
//                             Writing           Hub             Desk          Unused
const uint64_t pipes[4] = {0xF0F0F0F0AALL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E4LL, 0xF0F0F0F0D2LL};
double Fahrenheit(double celsius){
	return 1.8 * celsius + 32;
}
struct payload_t
{
  short id;
  short var1;
  short var2;
  short var3;
  short var4;
  short var5;
};
void setup(void){
  Serial.begin(57600);
  pinMode(6, INPUT);
  printf_begin();
  radio.begin();
  radio.enableDynamicPayloads();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
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
      temp = 666;
    }
    int light = analogRead(6);
    int pir = digitalRead(6);
    payload_t dat = {(int)1, temp, light, pir};
    radio.stopListening();
    int wr = radio.write(&dat, sizeof(dat));
    radio.startListening();
    delay(5000);
    if(wr==1){
       printf("Sent\n");
      delay(30000);
    }
    else{
      printf("Sending Failed\n");
    }
}

