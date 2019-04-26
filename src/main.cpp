#include <Adafruit_NeoPixel.h>
#include <string.h>
#include <neopixelHelper.h>
#include <Wire.h>
#include "RTClib.h"



RTC_DS3231 rtc;

//unstable codekdölsakdölaksüpodkaölosd

int displayTime = 2; 

String incomming;

long r, g, b;
volatile bool received = false;
bool timeoutTimer = false;
bool longTimeout = false;
bool offtime = false;
DateTime timestamp;
volatile bool activity = false;
volatile int lastActivityTimestamp;
long nextUpdate;
long timeoutAmount = 12000;
volatile int timeoutCount = 2;


void activitySensorInterrupt()
{
   if(activity == true)
    {
      activity = false;
    }
    else activity = true;
}



void serialEvent()
{
   
    incomming = Serial.readString();
    if (incomming.startsWith("#"))
    {
       if (incomming.length() == 7 ){ 
         // Get rid of '#' and convert it to integer
         long number = (long) strtol( &incomming[1], NULL, 16);

         // Split them up into r, g, b values
        r = number >> 16;
        g = number >> 8 & 0xFF;
        b = number & 0xFF;
        setStripColor(r, g, b);
        strip.show();
       }
       else if(incomming.length() == 3 || incomming.length() == 4)
        {
         long number = (long) strtol( &incomming[1], NULL, 10);
         r = setStripBrightness(r, number);
         g = setStripBrightness(g, number);
         b = setStripBrightness(b, number);

         
        }
         received = true;

      
        Serial.println(incomming);

        //incomming = "";
    
    
     }
}

void setup ()
{
 strip.setBrightness(maxBrightness);
 strip.begin();
 strip.show();  //Clear Strip
 pinMode(activityInterupt, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(activityInterupt), activitySensorInterrupt, LOW);

Serial.begin(9600);
}


void loop()
{
  Serial.print(rtc.now().hour());
  Serial.print("   ");
  Serial.print(rtc.now().minute());
for(int hour = 2; 23; hour++)
  {
    for(int minute = 30; 60; minute++)
    {
       if((rtc.now().hour() > hour ))
  {
    if(rtc.now().minute() > minute)
    Serial.println("shut down");
    activity = false;
    break;
  } 
    }
  }
 Serial.println(activity);
 if(activity == false) 
     {
       Serial.println("uff");
       setStripColor(0,0,0);
       strip.show();
     }

 if(activity == true)
        if(received == true)
        {
           setStripColor(r, g, b);
           strip.show();         
         }
        else if(received == false) 
        { 
           CylonBounce(00, 0xff, 00, 4,50,50);
           
        }
   


}