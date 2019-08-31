/*
    demo for  LILYGO TTGO T-Display ESP32 1.14 Inch LCD Development Board For Arduino
    Correct display driver installed
    https://github.com/Xinyuan-LilyGO/TTGO-T-Display
 */
#include <Arduino.h>

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <SPI.h>

#include "TFTDriver.h"
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"
#include <Ticker.h>

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4  // Display backlight control pin
#define ADC_EN          14
#define ADC_PIN         34
#define BUTTON_1        35
#define BUTTON_2        0
#define BUTTONS_MAP     {BUTTON_1,BUTTON_2}

TFTDriver tft = TFTDriver(135, 240); // Invoke custom library
SemaphoreHandle_t _lock = NULL;

int16_t count0 = 0;
int16_t count1 = 0;

void Task0( void * pvParameters )
{
    String core = String(xPortGetCoreID());
    while(true){
        String counter = String(count0++);
        
        String message = "Core: " + core + " - "  + counter; 
        xSemaphoreTake(_lock,portMAX_DELAY); //aquire lock before using tft
            //Serial.println(message);
            tft.fillRect(0,0,tft.width(),19,TFT_BLACK);
            tft.setTextColor(TFT_GREEN);
            tft.drawString(message,0,0);
        xSemaphoreGive(_lock);  //release lock
        delay(850);
    }
}


void Task1( void * pvParameters )
{
    String core = String(xPortGetCoreID());
    while(true){
        String counter = String(count1++);
        
        String message = "Core: " + core + " - "  + counter; 
        xSemaphoreTake(_lock,portMAX_DELAY); //aquire lock before using tft
            //Serial.println(message);
            tft.fillRect(0,30,tft.width(),49,TFT_BLACK);
            tft.setTextColor(TFT_RED);
            tft.drawString(message,0,30);
        xSemaphoreGive(_lock);  //release lock

        delay(1000);
    }
}


void setup()
{
    vSemaphoreCreateBinary(_lock);  /* Create binary semaphore */


    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting Task Testing");
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextDatum(TL_DATUM);
    //tft.drawString("Starting",  tft.width() / 2, tft.height() / 2);
    //turn on backlight
    if (TFT_BL > 0) {
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);
    }

xTaskCreatePinnedToCore(
                    Task0,   /* Function to implement the task */
                    "task0", /* Name of the task */
                    1000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    1,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    0);  /* Core where the task should run */
 
xTaskCreatePinnedToCore(
                    Task1,   /* Function to implement the task */
                    "task1", /* Name of the task */
                    1000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    1,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    1);  /* Core where the task should run */
 


}




void loop()
{
    delay(1000);

}