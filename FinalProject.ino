

/*
Modified by Jacky Britton
April 14th 2022
 *
 */

#include <HTTPClient.h>
#include <M5StickC.h>
#include <Arduino.h> 
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>  
#include <ArduinoHttpClient.h>
#include "secrets.h"

#define WIFI_SSID "Ositos"                    // Enter your SSID here
#define WIFI_PASS "10SiebenUndZwanzig2017"    // Enter your WiFi password here

const int NetworkTimeout = 30*1000;
const int NetworkDelay = 2000; 
double vbat = 0.0;
int discharge, charge;
double temp = 0.0;
double bat_p = 0.0;
double bat_p2 = 0.0;
int status = WL_IDLE_STATUS;
int Population=0;
int National_Debt=0;
int GDP=0;

WiFiClientSecure client;
HttpClient http(client,"www.usdebtclock.org/index.html", 443);

uint8_t bat_3[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xdb, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

uint8_t bat_2[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xdb, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

uint8_t bat_1[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

  
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  M5.begin();
  M5.Lcd.setRotation(3);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.printf("Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.setCursor(20, 40);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print("Please Wait...");
    delay(500);
  }
  M5.Lcd.setCursor(15, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.println("Connected");
  IPAddress ip = WiFi.localIP();
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.println(ip);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Collecting Data...");
  
}

void loop() {
  
  check_country("US");
  delay(2000); 
 
  check_country("World");
  delay(2000); 
  
}


void draw_country_screen(String sCountry){
  M5.Lcd.fillScreen(BLACK);
  battery_status();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.print(sCountry + ":");

  // population
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 35);
  M5.Lcd.print("Population: " + Population);

  // National Debt
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.print("Unemployed: " + National_Debt);

  // GDP
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 65);
  M5.Lcd.print("Debt: " + GDP);
  
}

void check_country(String sCountry) {
  int err =0;
  int readcounter = 0;
  int read_value_step = 0;
  String s1 = "";
  String s2 = "";
  
  if (sCountry == "World") { err = http.get("/world-debt-clock.html"); }
  else {err = http.get("www.usdebtclock.org/" + sCountry +"/"); }
  if (err == 0)
  {
    Serial.println("Requesting ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
    
      unsigned long timeoutStart = millis();
      char c;
      while ( (http.connected() || http.available()) &&
             (!http.endOfBodyReached()) &&
             ((millis() - timeoutStart) < NetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              s2 = s2 + c;
              if (readcounter < 300) {
                readcounter++;
              } else {
                readcounter = 0;
                String tempString = "";
                tempString.concat(s1);
                tempString.concat(s2);
                
                // check Population 
                if (read_value_step == 0) {                               
                  int place = tempString.indexOf("Population:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 1;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("#aaa") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    
                    Serial.print("Population: ");
                    Population = s1;
                    Serail.println(Population);
                  }
                  
                }
                // check National_Debt              
                if (read_value_step == 1) {
                  int place = tempString.indexOf("National Debt:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 2;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
             
                    Serial.print("National Debt: ");
                    Serial.println(s1);
                    National_Debt = s1;
                  }
                }                
                // check GDP               
                if (read_value_step == 2) {
                  int place = tempString.indexOf("GDP:");
                  if ((place != -1) && (place < 350)) {                   
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                     
                    Serial.print("GDP: ");
                    Serial.println(s1);
                    GDP = s1;
                    draw_country_screen(sCountry);
                    http.stop();
                    return;
                  }
                }                
      
                s1 = s2;
                s2 = ""; 
              }              
              
              timeoutStart = millis();
          }
          else
          {
              delay(NetworkDelay);
          }
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    //Serial.print("Connect failed: ");
    //Serial.println(err);
  }
  http.stop();
  
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  //Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");
}
void battery_status()
{
  vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  discharge = M5.Axp.GetIdischargeData() / 2;
  if (vbat >= 4)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_3);
  }
  else if (vbat >= 3.7)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_2);
  }
  else if (vbat < 3.7)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_1);
  }
  else
  {}
  // M5.Lcd.setTextColor(TFT_YELLOW);
  // M5.Lcd.setCursor(140, 12);
  // M5.Lcd.setTextSize(1);
  // M5.Lcd.println(discharge);
}
