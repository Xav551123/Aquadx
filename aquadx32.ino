/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESPWiFi.h>
#include "WiFi.h" // for WiFi shield
#include <BlynkSimpleEsp32.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char pass[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
int ledPin1 = 0;
   int ledPin2 = 0;
   float q = 2.5;
   float u = 15.0;
   float q2= 2;
   float u2 = 15.0;
   unsigned long previousMillis=0 ;
   float interval = 3000.0;//3000.0
   float ledtime1 = 1.0;
   int swich = 2;
   int ledtime2;
   float intervaldx = interval;
   float dled = 0.0;
   int countblynk=0;
   const int led2Pin = 16;
 const int led2Pin2 = 17;
float test = 10.0;
float x=0.0;
int swtime;


// Paramètre du channel 0 du PWM
const int freq = 50000; // 100000 Hz
const int ledChannel = 1;
const int resolution = 10; // Résolution de 8 bits

void setup()
{Serial.begin(9600);
Serial.println("setup");
WiFi.mode(WIFI_STA);
  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(2, freq, resolution);
  //analogWriteFreq(100);
 // pinMode(15, OUTPUT);
   //pinMode(16, OUTPUT);
   ledcAttachPin(led2Pin, 1);
   ledcAttachPin(led2Pin2, 2);
  // Debug console
    // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("myAQUA");
  // No authentication by default
 // ArduinoOTA.setPassword("ED1993992F2EAAAA93C4CD62A6649D");

    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 Blynk.begin(auth, ssid, pass);
   timeClient.begin();
   delay(1000);
 //intitialisation horloge
 timeClient.update();
float secondes = (timeClient.getSeconds());  
x = 60*timeClient.getHours()+ timeClient.getMinutes()+ secondes/60;
}

void loop()
{
   if(timeClient.getHours()==0){
    swich = 2;
    delay(65*1000);
   }
   ArduinoOTA.handle();
  //Serial.println("salu");
  updateblynk();
 Blynk.run();
  if(swich == 1){
    //Serial.print("phone");
  ledcWrite(1,ledPin1);
  ledcWrite(2,ledPin2);
 }
  else if(swich ==0){
  ledcWrite(1,ledtime1);
  ledcWrite(2,ledtime2);
    //Serial.println("timer");
  }
  else if(swich == 2){
     swtime = timeClient.getHours();
    if(swtime>=11 & swtime<21){
     ledcWrite(1,1024);
     ledcWrite(2,1024);
  }
  else{
    ledcWrite(1,0);
    ledcWrite(2,0);
  }
  }
  if( millis() - previousMillis >= (interval)) {
   // timeClient.update();
   // float secondes = (timeClient.getSeconds());
   // float x = 60*timeClient.getHours()+ timeClient.getMinutes()+ secondes/60;

if(x>1440){
   timeClient.update();
   //secondes = (timeClient.getSeconds());
   x = 60*timeClient.getHours()+ timeClient.getMinutes();//+ secondes/60;

}
x=x+interval/60000; //hardware timer


    float previousledtime = ledtime1;   
    previousMillis = millis();
     
 // Serial.println(secondes);
 // Serial.println(minutesomme);
  
  ledtime1 = 1300*exp(-1.0/2.0*pow((x/60-u)/q,2)); //pome est declaré comme float pome = 1; avant le  void setup
  if(ledtime1>1024){
    ledtime1=1024;
  }
  dled = 65.0*((-q*x)/15.0+4.0*q*u)*exp(-1.0/2.0*pow(((-u+x/60)/q),2))/(12*pow(q,3));
  intervaldx = abs(interval/dled);
  //Serial.print("dx");
  //Serial.println(intervaldx);

  //intervalmultip = constrain((ledtime1-previousledtime),0.1,100);
  if(ledtime1 < 50){
    ledtime1 = 0;
  }
  
  //Serial.println(ledtime1);
 // ledtime2 = 1024*exp(-1.0/2.0*pow((x/60-u2)/q2,2));
  // if(ledtime2 < 50){
    ledtime2 = 0;
 // }
  //Serial.println(ledtime2);
    }
 
}


BLYNK_WRITE(V1)
{
   ledPin1 = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

BLYNK_WRITE(V2)
{
   ledPin2 = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

BLYNK_WRITE(V3)
{
  swich = param.asInt();
  //Serial.println(swich);
}

BLYNK_WRITE(V6)
{
  u = param.asInt();
  u2 = u;
  //Serial.println(swich);
}

BLYNK_READ(V4) //Blynk app has something on V5
{
  
  Blynk.virtualWrite(V4, ledtime1); //sending to Blynk
}

BLYNK_READ(V8) //Blynk app has something on V5
{
  
  Blynk.virtualWrite(V8, ledtime2); //sending to Blynk
}


BLYNK_READ(V5) //Blynk app has something on V5
{
  
  Blynk.virtualWrite(V5, dled); //sending to Blynk
}

void updateblynk()
{
int blynkmillis = millis();

if(blynkmillis-millis() > 1000){
  
  //countblynk = countblynk + 1;
  //Serial.println(countblynk);
  blynkmillis = millis();
  Blynk.virtualWrite(V5, dled); //sending to Blynk
  Blynk.virtualWrite(V4, ledtime1);
  Blynk.virtualWrite(V8, ledtime2);
  //sending to Blynk
  Blynk.virtualWrite(V7, swtime);
  return;
}
}
