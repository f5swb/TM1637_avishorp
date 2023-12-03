/* Arduino example code to display a 24 hour time format clock on a 
  TM1637 4 digit 7 segment display with a DS32321 RTC. 
  More info: www.www.makerguides.com */

// Include the libraries:
#include "RTClib.h"
#include "TM1637Display.h"

// Define the connections pins:
#define CLK 7
#define DIO1 8
#define DIO2 9
#define DIO3 10

// Create rtc and display object:
RTC_DS3231 rtc;

TM1637Display display1(CLK, DIO1);
TM1637Display display2(CLK, DIO2);
TM1637Display display3(CLK, DIO3);


void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Wait for console opening:
  delay(3000);

  // Check if RTC is connected correctly:
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // Check if the RTC lost power and if so, set the time:
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // The following line sets the RTC to the date & time this sketch was compiled:
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
   // rtc.adjust(DateTime(2023, 12, 3, 15, 48, 0));
  }
  
  // rtc.adjust(DateTime(2023, 12, 3, 15, 51, 0));
  // Set the display brightness (0-7):
  display1.setBrightness(5);
  display2.setBrightness(5);
  display3.setBrightness(5);

  // Clear the display:
  display1.clear();
  display2.clear();
  display3.clear();
}

void loop() {
  // Get current date and time:
  DateTime now = rtc.now();

  // Create time format to display:
  int displaytime = (now.hour() * 100) + now.minute();
  int displaydatemonth = (now.month() * 100) + now.day();
  int displayyear = now.year();
  
  // Print displaytime to the Serial Monitor:
  Serial.print("TIME = ");Serial.println(displaytime);
  Serial.print("MONTH/DAY = ");Serial.println(displaydatemonth);
  Serial.print("YEAR = ");Serial.println(displayyear);

  // Display the current time in 24 hour format with leading zeros enabled and a center colon:
  display1.showNumberDecEx(displaytime, 0b01000000, true);
  display2.showNumberDecEx(displaydatemonth, true);
  display3.showNumberDecEx(displayyear, true);
  // Remove the following lines of code if you want a static instead of a blinking center colon:
  delay(1000);

  display1.showNumberDec(displaytime, true); // Prints displaytime without center colon.

  delay(1000);
}
