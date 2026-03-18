// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String command;

// pre-calculate the number of seconds for 3 cases: 1 minute, 10 minutes, 1 hour
int minsec = 60; // number of seconds in a minute
int nap10minsec = 10 * minsec; // number of seconds in a 10 minute period
int hoursec = 60 * minsec; // number of seconds in an hour

void report_time() {

  DateTime now = rtc.now();
  char buf2[] = "YYYY-MM-DD hh:mm:ss";

  Serial.println(now.toString(buf2));
  //Serial.println();
}

void adjust_time(int addsec) {
  // first version will add 5 seconds if the command add was sent
  Serial.println("Adjust time +" + String(addsec) + " seconds");
  DateTime now = rtc.now();
  char buf2[] = "YYYY-MM-DD hh:mm:ss";

  DateTime newtime = now + addsec;
  rtc.adjust(newtime);
  //Serial.println();
}

void show_valid_commands() {
  Serial.println("Valid commands:");
  Serial.println();
  Serial.println("  f - adds  5 seconds to the time");
  Serial.println("  m - adds  1 minute  to the time");
  Serial.println("  n - adds 10 minutes to the time (i.e. a nap)");
  Serial.println("  h - adds  1 hour    to the time (60 minutes)");
  Serial.println("  b - subtracts 1 hour from the time");
  Serial.println();
  Serial.println("Any other text will cause the valid commands to be shown.");
}

void setup () {
  Serial.begin(57600);

// #ifndef ESP8266
//   while (!Serial); // wait for serial port to connect. Needed for native USB
// #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  Serial.println("RTC has been found.");
  // The following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  delay(5);
  Serial.println("RTC has been programmed with an initial time.");
  Serial.println("To adjust the time, send one of these commands");
  show_valid_commands();

}

void loop() {
  // print the current time and listen for commands
  // to exit this loop, remove power to the device (MCU board has a switch on the left side)
  report_time();
  delay(2000);

  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equals("f")) {
      Serial.println("Received command: f to add 5 seconds to the time");
      adjust_time(5);  // adds 5 seconds
    }
    else if (command.equals("m")) {
      Serial.println("Received command: m to add 1 minute to the time");
      adjust_time(minsec);
    }
    else if (command.equals("n")) {
      Serial.println("Received command: n to add 10 minute Nap to the time");
      adjust_time(nap10minsec);
    }
    else if (command.equals("h")) {
      Serial.println("Received command: h to add 1 hour to the time");
      adjust_time(hoursec);
    }
    else if (command.equals("b")) {
      Serial.println("Received command: b to subtract 1 hour from the time");
      adjust_time(-1*hoursec);
    }
    else {
      Serial.println("Received unknown command: " + String(command) + " (ignored)");
      show_valid_commands();
    }
    Serial.flush();
  }
}
