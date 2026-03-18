#include "SPI.h"
#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_ILI9341.h>     // Hardware-specific library
#include <SdFat.h>                // SD card & FAT filesystem library (Adafruit's version)
#include <Adafruit_FT6206.h>

#include <SensirionI2CSen5x.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>

#include "TouchScreen.h"
#include "wiring_private.h"
#include "RTClib.h"
#include "sdios.h"

#define RTC_TYPE 1 // 0 = Old, 1= New high precision


ArduinoOutStream cout(Serial);

void cardOrSpeed() {
  cout << F("Try another SD card or reduce the SPI bus speed.\n");
  cout << F("Edit SPI_SPEED in this program to change it.\n");
}

// END TESTING

#define   VERSION   "1.2.4"

Adafruit_FT6206 ctp = Adafruit_FT6206();

SensirionI2CSen5x sen5x;
Adafruit_AS7341 as7341;

#if RTC_TYPE == 0
RTC_PCF8523 myRTC;
#elif RTC_TYPE == 1
RTC_DS3231 myRTC;
#endif

#define TFT_DC   14
#define TFT_CS   15
#define TFT_BACKLIGHT 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); //, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
#define SD_CS 9
unsigned char sen_product[48];
unsigned char sen_sn[48];
SdFs sd;
FsFile file;

char filename[20];
unsigned long screenTime = 0;
int screenStatus = 1; // 1 means on, 0 means off
unsigned long lastTime = 0;
unsigned long file_lines = 0;

void(* resetFunc) (void) = 0; 

void newLogFile() {

  DateTime now = myRTC.now();
  char buf2[] = "YYYYMMDDhhmmss";
  String temp = (String) now.toString(buf2) + ".csv";
  temp.toCharArray(filename, 20);
  file.open(filename,  O_RDWR | O_CREAT | O_AT_END);
  if (file) {
      Serial.println("; " + String((char *) sen_product) + " " + String((char *) sen_sn));
      file.println("; Version: " + String( VERSION ));
      file.println("; " + String((char *) sen_product) + " " + String((char *) sen_sn));
      if (RTC_TYPE == 0) {
        file.println("ts,lch0,lch1,lch2,lch3,lch6,lch7,lch8,lch9,lch10,lch11,pm1,pm2.5,pm4,pm10,hum,temp,voc,nox,screen,ff");
      } else if (RTC_TYPE == 1) {
        file.println("ts,lch0,lch1,lch2,lch3,lch6,lch7,lch8,lch9,lch10,lch11,pm1,pm2.5,pm4,pm10,hum,temp,voc,nox,screen,ff,inttemp");
      }
      
      file.close();
  } else {
      Serial.println("Could not write to file!");
      tft.println(sd.card()->errorCode());
      tft.println(sd.card()->errorData());
      while (1) { delay(100); }
  }
}

void setup(void) {
  Serial.begin(9600);
  
  //Wire.begin(0x08);
  // Turn on backlight (required on PyPortal)
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(TFT_CS, LOW);
  digitalWrite(SD_CS, HIGH);
  delay(200);

  
  tft.begin();          // Initialize screen
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.setCursor(0, 0);
  tft.println("Starting setup");

  /*************** Touchscreen ***************/
  Serial.print("Touchscreen...");
  tft.print("Touchscreen... ");
  
  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
      tft.setTextColor(ILI9341_RED);
      tft.println("failed!");
      Serial.println("Card init. failed!");
      Serial.flush();
      while (1) { delay(100); }
  } else {
      tft.setTextColor(ILI9341_GREEN);
      tft.println("OK");
      delay(500);
      Serial.println("OK!");
  }
  tft.setTextColor(ILI9341_WHITE);

  /*************** RTC ***************/
  Serial.println("RTC... ");
  tft.print("RTC... ");
  if (! myRTC.begin()) {
      tft.setTextColor(ILI9341_RED);
      tft.print("failed\n");
      delay(500);;
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) { delay(100); }
  } else {
      tft.setTextColor(ILI9341_GREEN);
      tft.println("OK");
      delay(500);
      Serial.println("OK!");
  }
  tft.setTextColor(ILI9341_WHITE);  
  Serial.println("RTC started...");

  /*************** SD CARD ***************/
  
  Serial.print("SD Card...");
  tft.print("SD card... ");

  digitalWrite(TFT_CS, HIGH);
  delay(200);
  if (!sd.begin(SD_CS, SD_SCK_MHZ(10))) { // v1.2.4 new fix from 25 to 10
      tft.setTextColor(ILI9341_RED);
      tft.println("failed!");
      tft.println(int(sd.sdErrorCode()));
      tft.println(int(sd.sdErrorData()));
      Serial.println("Card init. failed!");
      Serial.flush();
      while (1) { delay(100); }
  } else {
      // TESTING

  uint32_t size = sd.card()->sectorCount();
  if (size == 0) {
    cout << F("Can't determine the card size.\n");
    cardOrSpeed();
    return;
  }
  uint32_t sizeMB = 0.000512 * size + 0.5;

  
  cout << F("Card size: ") << sizeMB;
  cout << F(" MB (MB = 1,000,000 bytes)\n");
  cout << endl;
  cout << F("Volume is FAT") << int(sd.vol()->fatType());
  cout << F(", Cluster size (bytes): ") << sd.vol()->bytesPerCluster();
  cout << endl << endl;
  cout << F("Files found (date time size name):\n");
  sd.ls(LS_R | LS_DATE | LS_SIZE);
      // END TESTING
      
            
      tft.setTextColor(ILI9341_GREEN);
      tft.println("OK");
      delay(500);
      Serial.println("OK!");
  }
  tft.setTextColor(ILI9341_WHITE);

  

  /*************** SEN 5X ***************/
  Serial.println("SEN 5x... ");
  tft.print("SEN 5x... ");
  sen5x.begin(Wire);
  uint16_t error;
  char errorMessage[256];
  error = sen5x.deviceReset();
  if (error) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        tft.setTextColor(ILI9341_RED);
        tft.print("failed\n");
        Serial.flush();
        while (1) { delay(100); }
  } else {
      tft.setTextColor(ILI9341_GREEN);
      tft.println("OK");
      delay(500);
      Serial.println("OK!");
  }
  tft.setTextColor(ILI9341_WHITE);


  Serial.println("Setting / cleaning SEN 5x... ");
  tft.print("Setting / cleaning SEN 5x... ");

  sen5x.startFanCleaning();

  float tempOffset = 0.0;
    error = sen5x.setTemperatureOffsetSimple(tempOffset);
    if (error) {
        Serial.print("Error trying to execute setTemperatureOffsetSimple(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        tft.setTextColor(ILI9341_RED);
        tft.print("failed\n");
        Serial.flush();
        while (1) { delay(100); }
    } else {
        Serial.print("Temperature Offset set to ");
        Serial.print(tempOffset);
        Serial.println(" deg. Celsius");

        tft.setTextColor(ILI9341_GREEN);
        tft.println("OK");
        delay(500);
        Serial.println("OK!");
    }
    tft.setTextColor(ILI9341_WHITE);

  Serial.println("Testing SEN 5x... ");
  tft.print("Testing SEN 5x... ");

  sen5x.getProductName(sen_product, 48);
  sen5x.getSerialNumber(sen_sn, 48);

  error = sen5x.startMeasurement();
    if (error) {
        Serial.print("Error trying to execute startMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);

        
        tft.setTextColor(ILI9341_RED);
        tft.print("failed\n");
        Serial.flush();
        while (1) { delay(100); }
    } else {
        tft.setTextColor(ILI9341_GREEN);
        tft.println("OK");
        delay(500);
        Serial.println("OK!");
    }
    tft.setTextColor(ILI9341_WHITE);


  /*************** SPECTRUM ***************/   
  Serial.println("Spectral sensor... ");
  tft.print("Spectral sensor... "); 
    if (!as7341.begin()){
      Serial.println("Could not find AS7341");

        tft.setTextColor(ILI9341_RED);
        tft.print("failed\n");
        Serial.flush();
        while (1) { delay(100); }
    } else {
        
        tft.setTextColor(ILI9341_GREEN);
        tft.println("OK");
        delay(500);
        Serial.println("OK!");
    }
    tft.setTextColor(ILI9341_WHITE);    
    as7341.enableLED(false);
    as7341.setATIME(100);
    as7341.setASTEP(999);
    as7341.setGain(AS7341_GAIN_256X);


  newLogFile();
    
  tft.println("\nAll finished!");
  delay(1000); // Pause 2 seconds before moving on to loop()
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

  screenTime = micros();
  lastTime = 0;
  file_lines = 0;
  screenStatus = 1;
}





void loop() {
  unsigned long newTime = micros();
  
  if (ctp.touched()) {
    screenTime = micros();
    screenStatus = 1;
  }

  if (micros() - screenTime > 30 * 1000 * 1000) {
    screenStatus = 0;
    digitalWrite(TFT_BACKLIGHT, LOW);
  } else {
    digitalWrite(TFT_BACKLIGHT, HIGH);
    delay(100);

  }

  if (newTime - lastTime < 5 * 1000 * 1000)  { 
    return;
  }
  lastTime = newTime;
  float massConcentrationPm1p0;
  float massConcentrationPm2p5;
  float massConcentrationPm4p0;
  float massConcentrationPm10p0;
  float ambientHumidity;
  float ambientTemperature;
  float vocIndex;
  float noxIndex;
  float intTemp;
  uint16_t flicker_freq = 0;

      DateTime now = myRTC.now();
      char buf2[] = "YYYY-MM-DD hh:mm:ss";

      sen5x.readMeasuredValues(
            massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
            massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
            noxIndex);
      uint16_t readings[12];
      float counts[12];
      if (RTC_TYPE == 1) {
        intTemp = myRTC.getTemperature();
      }

      if (!as7341.readAllChannels(readings)){
    //      Serial.println("Error reading all channels!");
          tft.fillRect(0, 0, 240, 15, ILI9341_BLACK);
          tft.setCursor(0, 0);
          tft.print("Error reading all channels!");
          while (1) { delay(100); };
          return;
        }
        flicker_freq = as7341.detectFlickerHz();

  
      for(uint8_t i = 0; i < 12; i++) {
          if(i == 4 || i == 5) continue;
          // we skip the first set of duplicate clear/NIR readings
          // (indices 4 and 5)
          counts[i] = as7341.toBasicCounts(readings[i]);
        }
  
      char output[210];
      if (RTC_TYPE == 0) {
       snprintf(output, sizeof(output), "%s,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d", 
          now.toString(buf2),
          counts[0], counts[1], counts[2], counts[3], counts[6], counts[7], counts[8], counts[9], counts[10], counts[11],
          massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0, massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex, noxIndex,screenStatus, flicker_freq);
      } else if (RTC_TYPE == 1) {
        snprintf(output, sizeof(output), "%s,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.04f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%.2f", 
          now.toString(buf2),
          counts[0], counts[1], counts[2], counts[3], counts[6], counts[7], counts[8], counts[9], counts[10], counts[11],
          massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0, massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex, noxIndex,screenStatus, flicker_freq, intTemp);
        
      }


      ambientTemperature = (ambientTemperature * 9 / 5) + 32;
      String temp = String(ambientTemperature) + " " + (char)247 + "F";
      String hum = String(ambientHumidity) + " %";
      String PM25 = String(massConcentrationPm2p5);
      String voc = String (vocIndex);
      /* printing that data to the Serial port in a meaningful format */ 
    //  Serial.println("************************************************************");
    //  Serial.println(output);

      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(now.toString(buf2));

      tft.setCursor(0, 50);
      tft.print("Temp:  ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(temp);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 80);
      tft.print("Humid: ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(hum);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 110);
      tft.print("PM2.5: ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(PM25); 
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 140);
      tft.print("VOC:   ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(voc); 
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 170);
      tft.print("Light: ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(counts[0] * 10000); 
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 200);
      tft.print("Log:   ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(file_lines); 
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.setCursor(0, 280);
      tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
      tft.print("AI-READI v");
      tft.print(VERSION);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      

      digitalWrite(TFT_CS, HIGH);
      delay(200);

      file.open(filename,  O_RDWR | O_CREAT | O_AT_END);
      if (file) {
        Serial.println(output);
        file.println(output);
        file.close();
        file_lines++;
        if (file_lines % 1000 == 0) {
          newLogFile();
          //file_lines = 0;
        }
        
      } else {
        digitalWrite(TFT_CS, LOW);
        digitalWrite(TFT_BACKLIGHT, HIGH);
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.println("COULD NOT WRITE TO FILE");
        tft.println("Please unplug and plug device.");
        Serial.println("Could not write to file!");
        tft.println(int(sd.sdErrorCode()));
        tft.println(int(sd.sdErrorData()));
        
        while (1) { delay(500); }
        //resetFunc();
      }

}
