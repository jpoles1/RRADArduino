//Not fixed yet
#define BRIGHTNESS_UP     0xFFA05F
#define BRIGHTNESS_DOWN   0xFF20DF
#define FLASH             0xFFF00F
#define STROBE            0xFFE817 
#define ORANGE            0xFFB04F
#define YELLOW_DARK       0xFFA857
#define YELLOW_MEDIUM     0xFF9867
#define YELLOW_LIGHT      0xFF8877
#define GREEN_LIGHT       0XFF30CF
#define GREEN_BLUE1       0XFF28D7
#define GREEN_BLUE2       0XFF18E7
#define GREEN_BLUE3       0XFF08F7
#define BLUE_RED          0XFF708F
#define PURPLE_DARK       0XFF6897
#define PURPLE_LIGHT      0XFF58A7
#define PINK              0XFF48B7


IRsend irsend;
void setup()
{
  Serial.begin(9600);
  digitalWrite(4, LOW);
}
void loop(){
  irsend.sendNEC(RED, 32);
  delay(1000);
  irsend.sendNEC(GREEN, 32);
  delay(1000);
  irsend.sendNEC(BLUE, 32);
  delay(1000);
}
