

// Include the libraries:
#include "RTClib.h" //
#include "TM1637Display.h" //
#include <Encoder.h> // from http://www.pjrc.com/teensy/td_libs_Encoder.html
Encoder knob(3, 2); //encoder connected to pins 2 and 3 (and ground)
#define buton 6 // pin button encoder

// Define the connections pins of the screens :
#define CLK 7 
#define DIO1 8
#define DIO2 9
#define DIO3 10

// Create rtc and display object:
RTC_DS3231 rtc;

TM1637Display display1(CLK, DIO1);
TM1637Display display2(CLK, DIO2);
TM1637Display display3(CLK, DIO3);

// These variables are for the push button routine
int buttonstate = 0; //flag to see if the button has been pressed, used internal on the subroutine only
int pushlengthset = 2500; // value for a long push in mS
int pushlength = pushlengthset; // set default pushlength
int pushstart = 0;// sets default push value for the button going low
int pushstop = 0;// sets the default value for when the button goes back high

int knobval; // value for the rotation of the knob
boolean buttonflag = false; // default value for the button flag
int sethourstemp;
int setminstemp;
int setmonthtemp;
int setdaytemp;
int setyeartemp;


int greenAM = 12;
int greenPM = 13;

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

  pinMode(greenAM, OUTPUT);
  pinMode(greenPM, OUTPUT);

  pinMode(buton,INPUT);//push button on encoder connected 
  digitalWrite(buton,HIGH); //Pull at 1 high
}

void loop() {
  // Get current date and time:
  DateTime now = rtc.now();

  // Create time format to display:
  int displaytime = (now.hour() * 100) + now.minute();
  int displaydatemonth = (now.month() * 100) + now.day();
  int displayyear = now.year();
  
  if(now.hour()>=13){
      digitalWrite(greenAM,0);
      digitalWrite(greenPM,1);
  }
  else if(now.hour()==12)
  {
      digitalWrite(greenAM,0);
      digitalWrite(greenPM,1);
  }
  else{
      digitalWrite(greenAM,1);
      digitalWrite(greenPM,0);
  }
 
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
  
  delay(500);
   pushlength = pushlengthset;
    pushlength = getpushlength ();
    delay (10);
    
   //This runs the setclock routine if the knob is pushed for a long time
       if (pushlength < pushlengthset) {
        // player.play(2);  
       //  delay(500);
         DateTime now = rtc.now();
         sethourstemp=now.hour(),DEC;
         setminstemp=now.minute(),DEC;     
         setmonthtemp=now.month(),DEC; 
         setdaytemp=now.day(),DEC;
         setyeartemp=now.year(),DEC;      
         setclock();
         pushlength = pushlengthset;
       };
  
}
// subroutine to return the length of the button push.
int getpushlength () {
  buttonstate = digitalRead(buton);  
       if(buttonstate == LOW && buttonflag==false) {     
              pushstart = millis();
              buttonflag = true;
             // player.play(2); 
          };
          
       if (buttonstate == HIGH && buttonflag==true) {
         pushstop = millis ();
         pushlength = pushstop - pushstart;
         buttonflag = false;
       };
       return pushlength;
}
int sethours () {
    display1.clear();
    int displaytime2 = (sethourstemp * 100) + setminstemp ;
    display1.showNumberDecEx(displaytime2, 0b01000000, true);
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return sethourstemp;
    }
    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    sethourstemp=sethourstemp + knobval;
    if (sethourstemp < 0) {
      sethourstemp = 23;
    }
    if (sethourstemp > 23) {
      sethourstemp = 0;
    }
    sethours();
}


int setmins () {
    display1.clear();
    int displaytime2 = (sethourstemp * 100) + setminstemp ;
    display1.showNumberDecEx(displaytime2, 0b01000000, true);
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setminstemp;
    }
    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setminstemp=setminstemp + knobval;
    if (setminstemp < 0) {
      setminstemp = 59;
    }
    if (setminstemp > 59) {
      setminstemp = 0;
    }
    setmins();
}

int setmonth () {
    display2.clear();
    int displaydatemonth2 = setmonthtemp + setminstemp ;
    display2.showNumberDecEx(displaydatemonth2, true);
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setmonthtemp;
    }
    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setmonthtemp=setmonthtemp + knobval;
    if (setmonthtemp < 0) {
      setmonthtemp = 12;
    }
    if (setmonthtemp > 12) {
      setmonthtemp = 1;
    }
    setmonth();
}

int setday() {
   display2.clear();
   int displaydatemonth2 = setmonthtemp + setminstemp ;
   display2.showNumberDecEx(displaydatemonth2, true);
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setdaytemp;
    }
    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setdaytemp=setdaytemp + knobval;
    if (setdaytemp < 0) {
      setdaytemp = 31;
    }
    if (setdaytemp > 31) {
      setdaytemp = 1;
    }
    setday();
}
int setyear() {
    display3.clear();
    int displayyear2 = setyeartemp ;
    display3.showNumberDecEx(displayyear2, true);
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return setyeartemp;
    }
    knob.write(0);
    delay (50);
    knobval=knob.read();
    if (knobval < -1) {
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    setyeartemp=setyeartemp + knobval;
    if (setyeartemp < 21) {
      setyeartemp = 30;
    }
    if (setyeartemp > 30) {
      setyeartemp = 23;
    }
    setyear();
}

//sets the clock
void setclock (){
   sethours ();
   delay(500);
   setmins ();
   delay(500);
   setmonth ();
   delay(500);
   setday();
   delay(500);
   setyear(); 
   rtc.adjust(DateTime(setyeartemp,setmonthtemp,setdaytemp,sethourstemp,setminstemp,0));
   delay (500); 
  
}
