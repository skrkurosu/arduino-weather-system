#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define buzzer 4
#define DHTPIN 7 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define PIN_TX    9
#define PIN_RX    8
#define BAUDRATE  9600
#define PHONE_NUMBER "+639265219841"
#define PHONE_NUMBER1 "+639758977920"
#define MESSAGE1  "Hi I'm GUKUMATZ, there's no chance of rain for the next 6hours. Enjoy your day! :)"
#define MESSAGE2 "Hi I'm GUKUMATZ, there's a chance of rain for the next 6hours. So you better prepare for rain. :)"
#define MESSAGE3 "Hi I'm GUKUMATZ, it's raining!"
#define MESSAGE_LENGTH 500
int messageIndex = 0;
char message[MESSAGE_LENGTH];
GPRS GSMTEST(PIN_TX,PIN_RX,BAUDRATE);

int nRainIn = A1;
int nRainDigitalIn = 2;
boolean bIsRaining = false;
String strRaining;
int chk;
float hum;
float temp;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  DHT begin();
  pinMode(2,INPUT);
  pinMode(buzzer, OUTPUT);
  while(!GSMTEST.init()) 
  {
      delay(1000);
      Serial.print("INIT ERROR\r\n");
  }  
  Serial.println("GSM INIT SUCCESS");
}
void loop()
{
  delay(2000);
  float hum = dht.readHumidity();
  float temp= dht.readTemperature();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("H:");
  lcd.print(hum);
  lcd.print("%,T:");
  lcd.print(temp);
  lcd.print("C");
  delay(10000);
    
  bIsRaining = !(digitalRead(nRainDigitalIn));
  
  if(bIsRaining)
  {
    strRaining = "YES";
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer, HIGH);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0,1);
    lcd.print("IT'S RAINING!");
    delay(2000);
    Serial.println("SENDING MESSAGE ...");
      GSMTEST.sendSMS(PHONE_NUMBER, MESSAGE3);
      GSMTEST.sendSMS(PHONE_NUMBER1, MESSAGE3);
      Serial.println("Message sent.");  
  }
  else
  {
    strRaining = "NO";
    lcd.setCursor(0,1);
    lcd.print("GOOD DAY!         ");
    delay(2000);
  }
  
  if (0<hum<61);
  {
    Serial.println("SENDING MESSAGE ...");
      GSMTEST.sendSMS(PHONE_NUMBER, MESSAGE1);
      GSMTEST.sendSMS(PHONE_NUMBER1, MESSAGE1);
      Serial.println("Message sent.");
      delay(21600000);
  }
  
  if (60<hum<101);
  {
    Serial.println("SENDING MESSAGE ...");
      GSMTEST.sendSMS(PHONE_NUMBER, MESSAGE2);
      GSMTEST.sendSMS(PHONE_NUMBER1, MESSAGE2);
      Serial.println("Message sent.");
      delay(21600000);
  }
}
