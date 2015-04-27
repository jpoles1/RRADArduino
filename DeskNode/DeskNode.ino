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
//Setup IR+codes
#include <IRremote.h>
IRsend irsend;
#define POWER             0XFF02FD
#define FADE3             0xFF609F
#define FADE7             0xFFE01F
#define RED               0xFF1AE5
#define GREEN             0XFF9A65
#define BLUE              0xFFA25D
#define WHITE             0xFF22DD
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
//                             Writing        Temp+Light          Hub          Unused
const uint64_t pipes[4] = {0xF0F0F0F0E4LL, 0xF0F0F0F0AALL,  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
struct payload_t
{
  short id;
  short var1;
  short var2;
  short var3;
  short var4;
  short var5;
};
int dccount=0;
int pirpin = 2;
int lightpin = 4;
int fanpin = 8;
int butpin = 5;
int pirtime = 250;
int radtime = 50;
int scrtime = 100;
int dctime = 500;
int sent = 3;
payload_t dat;
int pir = 0;
int pirct = 0;
int radct = 0;
int scrct = 0;
int transmit = 1;
int sleep = 0;
int lightover = 0;
int fanover = 0;
int fanset = 0;
int lightset = 0;
int butstate = 0;

void sendRadio(short a, short b, short c, short d, short e, short f){
  payload_t dat = {a, b, c, d, e, f};
  printf("SENT ID: %d, var1=%d, var2=%d, var3=%d, var4=%d, var5=%d \n", dat.id, dat.var1, dat.var2, dat.var3, dat.var4, dat.var5);
  radio.stopListening();
  sent = radio.write(&dat, sizeof(dat));
  radio.startListening();
  radct=0;
}
void readRadio(){
  
}
void relayCtrl(){
  if(sleep==0 && !(lightover==1 && lightset==0) && (pir==1 ||(lightover==1 && lightset==1))){
    digitalWrite(lightpin, LOW);
    //digitalWrite(fanpin, HIGH);
  }
  else{
    digitalWrite(lightpin, HIGH);
    //digitalWrite(fanpin, LOW);
  }
  /*if(fanover==1 && fanset==1){
    digitalWrite(fanpin, LOW); 
  }*/
}
void scrRef(int type){
  if(type==0){
    OzOled.clearDisplay();
    OzOled.printString("PIR:", 0, 0);
    OzOled.printNumber((long)pir, 7, 0);
    OzOled.printString("Sleep:", 0, 2);
    OzOled.printNumber((long)sleep, 7, 2);
    OzOled.printString("PIRCt:", 0, 4);
    OzOled.printNumber((long)pirct, 7, 4);
  }
  else if(type==1){
    OzOled.clearDisplay();
    OzOled.printString("ID", 0, 0);
    OzOled.printNumber((long)dat.id, 7, 0);
    OzOled.printString("Temp:", 0, 2);
    OzOled.printNumber((long)dat.var1, 7, 2);
    OzOled.printString("Light:", 0, 4);
    OzOled.printNumber((long)dat.var2, 7, 4);
    if(dat.var3==1){
      printf("remote PIR is on");
      if(pir==0){
        transmit=1;
      }
      pirct = pirtime;
    }
  }
  if(type==6){
    sleep = dat.var1;
    lightover = dat.var2;
    lightset = dat.var3;
    fanover = dat.var4;
    fanset = dat.var5;
    OzOled.clearDisplay();
    OzOled.printString("Sleep:", 0, 0);
    OzOled.printNumber((long)sleep, 7, 0);
    OzOled.printString("Over:", 0, 2);
    OzOled.printNumber((long)lightover, 7, 2);
    OzOled.printString("Set:", 0, 4);
    OzOled.printNumber((long)lightset, 7, 4);
  }
  if(type==30){
    irsend.sendNEC(BLUE, 32);
    delay(50);
    irsend.sendNEC(BLUE, 32);
  }
  if(type==31){
    irsend.sendNEC(GREEN, 32);
    delay(50);
    irsend.sendNEC(GREEN, 32);
  }
  if(type==32){
    irsend.sendNEC(RED, 32);
    delay(50);
    irsend.sendNEC(RED, 32);
  }
  if(type==33){
    irsend.sendNEC(WHITE, 32);
    delay(50);
    irsend.sendNEC(WHITE, 32);
  }
  if(type==34){
    irsend.sendNEC(FADE7, 32);
    delay(50);
    irsend.sendNEC(FADE7, 32);
  }
  if(type==66){
    OzOled.clearDisplay();
    OzOled.printString("No signal! \n");
    printf("No Signal"); 
  }
}
void setup(void){
  Serial.begin(57600);
  pinMode(pirpin, INPUT);
  pinMode(butpin, INPUT);
  pinMode(lightpin, OUTPUT);
  pinMode(fanpin, OUTPUT);
  digitalWrite(lightpin, HIGH);
  digitalWrite(fanpin, HIGH);
  printf_begin();
  radio.begin();
  radio.enableDynamicPayloads();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(3,pipes[2]);
  OzOled.init();  //initialze Oscar OLED display
  OzOled.printString("Startin' Up!");
  radio.startListening();
  radio.printDetails();
  delay(300);
  sendRadio(122, 0, 0, 0, 0, 0);
}
void loop(void){
  if(digitalRead(butpin)==1){
    if(butstate==0){
      sleep=!sleep;
      transmit=1;
    }
    butstate=1;
  }
  else{
    butstate=0;
  }
  if(digitalRead(pirpin)==1){
    if(pir==0){
      transmit=1;
    }
    pirct = pirtime;
  }
  if(pirct==1){
    pir=0;
    transmit=1;
    pirct--;
  }
  else if(pirct > 1){
    pir=1;
    pirct--;
  }
  else{
    pir=0;
  }
  //Read radio
  if(radio.available()){
    // Grab the response, compare, and send to debugging spew
    radio.read( &dat, sizeof(dat) );
    printf("ID: %d, var1=%d, var2=%d, var3=%d, var4=%d, var5=%d \n", dat.id, dat.var1, dat.var2, dat.var3, dat.var4, dat.var5);
    // Spew it
    scrRef(dat.id);
    dccount=0;
  }
  //Counters
  else{
    if(dccount>=dctime){
      printf("Disconnected");
      scrRef(66);
      asm volatile ("  jmp 0");
    }
    dccount++;
    scrct++;
    if(scrtime<scrct){
      scrRef(0);
      scrct=0; 
    }
  }
  if(radct>radtime){
    transmit=1;
  }
  else{
    radct++;  
  }
  //Housekeeping Functions
  relayCtrl();
  if(transmit==1){
    sendRadio(2, pir, sleep, 0, 0, 0);
    scrRef(0);
  }
  transmit=0;
  delay(800);
}








