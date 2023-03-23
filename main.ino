#include <WiFi.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h> 
#include <HTTPClient.h>

#define DHTPIN 15
#define DHTTYPE DHT11

byte smile[8]={B00000,B10001,B00000,B00000,B10001,B01110,B00000,B00000};

long previousMillis = 0;
LiquidCrystal_I2C lcd(0x27,16,2);

const char* ssid = "NTUT真他媽牛逼"; //your wifi name
const char* password = "66666666"; //your wifi password
unsigned long time_now = 0; 
DHT dht(DHTPIN, DHTTYPE);

String url = "https://api.thingspeak.com/update?api_key=OEX4H2UM2CYJDC9S"; 
void UploadData(int h,int t){ 
  HTTPClient http;  
  String url1= url+"&field1="+h+"&field2="+t;
  http.begin(url1); //Write a Channel

  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK)Serial.println("網路上傳失敗"); 
}

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0,smile);

  WiFi.mode(WIFI_STA); //To set the ESP32 as an access point, set the Wi-Fi mode to access point:
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");

    lcd.setCursor(0,0);
    lcd.print("Wifi Connection");
    lcd.setCursor(0,1); 
    lcd.print("Failure"); 
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  dht.begin();

  time_now = millis(); //run time
  
}

void loop(){
  unsigned long currentMillis = millis();
  float h = dht.readHumidity();   //Humidity
  float t = dht.readTemperature(); //Temperature

  
  
  if (WiFi.status() == WL_CONNECTED) { //if wifi connected,smile and update
    lcd.setCursor(0,0); //(col/row)
    lcd.print("Humidity:" + String(h)+" ");
    lcd.setCursor(0,1);
    lcd.print("Temperature:"+ String(t)); 
    lcd.setCursor(15,0); 
    lcd.write(byte(0)); //smile
    
    if(millis() > time_now + 15000) {
      time_now = millis();
      UploadData(h,t);
      }
    }
}