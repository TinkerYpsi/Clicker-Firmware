#include <FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 9
#include <SD.h>
#include <SPI.h>
#include <TM1637Display.h>
#include "RTClib.h"

// Real-Time Clock
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool rtc_active = false;

// OK Button
const int ACCEPT_BUTTON = 47;

// Age
const int BUTTON_ADD_1 = 3;    // add 1 to number
const int BUTTON_MINUS_1 = 5;    // subtract 1 to number
const int BUTTON_ADD_10 = 4;    // add 10 to number
const int BUTTON_MINUS_10 = 6;    // subtract 10 to number

const int AGE_BUTTON_COUNT = 4;
const int AGE_BUTTON[AGE_BUTTON_COUNT] =
  {BUTTON_ADD_1, BUTTON_ADD_10, BUTTON_MINUS_1, BUTTON_MINUS_10};

const int SEVEN_SEG_DIO = 30;
const int SEVEN_SEG_CLK = 31;
TM1637Display display(SEVEN_SEG_CLK, SEVEN_SEG_DIO);

// Gender
const int GEN_COUNT = 3;
const int GEN_SW[GEN_COUNT - 1] = {34, 35};
const int GEN_LED[GEN_COUNT] = {42, 43, 44};
// Male and female are indicated by low signals on pins indicated above.
// Nonbinary is indicated by a high signal on both pins

// Location
const int LOC_BUTTON = 13;  // button switcher for location
const int LOC_COUNT = 4;
const int LOC_LED[LOC_COUNT] = {46, 48, 45, 41};

// Interest
const int INTEREST_COUNT = 5;
const int INTEREST_SW[INTEREST_COUNT] = {14, 15, 16, 17, 18};
const int INTEREST_LED[INTEREST_COUNT] = {40, 37, 36, 39, 38};

//Skill
const int SKILL_POT = A0;
CRGB leds[NUM_LEDS];

// Data storage
const int SD_CHIP_SELECT = 53;

void acceptButtonTest() {
  Serial.println(!digitalRead(ACCEPT_BUTTON));
}

void ageButtonTest() {
  for(int i = 0; i < AGE_BUTTON_COUNT; i++) {
    if(!digitalRead(AGE_BUTTON[i])) {
      Serial.println(AGE_BUTTON[i]);
    }
  }
}


void genderTest() {
  if(!digitalRead(GEN_SW[0])) {
    Serial.println("M");
  }
  else if(!digitalRead(GEN_SW[1])) {
    Serial.println("F");
  }
  else {
    Serial.println("NB");
  }
}

void ledTest() {
  const int WAIT = 50;
  for(int i = 0; i < GEN_COUNT; i++) {
    digitalWrite(GEN_LED[i], HIGH);
    delay(WAIT);
    digitalWrite(GEN_LED[i], LOW);
  }

  for(int i = 0; i < LOC_COUNT; i++) {
    digitalWrite(LOC_LED[i], HIGH);
    delay(WAIT);
    digitalWrite(LOC_LED[i], LOW);
  }

  for(int i = 0; i < INTEREST_COUNT; i++) {
    digitalWrite(INTEREST_LED[i], HIGH);
    delay(WAIT);
    digitalWrite(INTEREST_LED[i], LOW);
  }
}

void skillTest() {
  Serial.println(analogRead(SKILL_POT));
  delay(100);
}


int updateAge() {

  static int age = 0;
  display.showNumberDec(age);

  static bool was_pressed[AGE_BUTTON_COUNT] = {false};
  bool pressed[AGE_BUTTON_COUNT];

  for(int i = 0; i < AGE_BUTTON_COUNT; i++) {
    pressed[i] = !digitalRead(AGE_BUTTON[i]);
  }

  for(int i = 0; i < AGE_BUTTON_COUNT; i++) {
    if(pressed[i] && !was_pressed[i]) {
      switch (i) {
        case 0:
          age += 1;
          break;
        case 1:
          age += 10;
          break;
        case 2:
          age -= 1;
          break;
        case 3:
          age -= 10;
          break;
      }
    }
  }

  if(age < 1) {
    age = 1;
  }
  else if(age > 100) {
    age = 1;
  }

  for(int i = 0; i < AGE_BUTTON_COUNT; i++) {
    was_pressed[i] = pressed[i];
  }

  display.showNumberDec(age);
  return age;
}


int updateGender()  {
  int gender = 0;

  if(!digitalRead(GEN_SW[0])) {
    gender = 0;
  }
  else if(!digitalRead(GEN_SW[1])) {
    gender = 2;
  }
  else {
    gender = 1;
  }

  // Clear the LEDs and set the new one
  for(int i = 0; i < GEN_COUNT; i++) {
    digitalWrite(GEN_LED[i], LOW);
  }
  digitalWrite(GEN_LED[gender], HIGH);

  return gender;
}


int updateLocation() {

  const int MIN_LOC = 0;
  const int MAX_LOC = 3;
  static int location = MIN_LOC;

  // Initialization for the first run
  static bool first_call = true;
  if(first_call) {
    first_call = false;
    for(int i = 0; i < MAX_LOC; i++) {
      digitalWrite(LOC_LED[i], LOW);
    }
    digitalWrite(LOC_LED[0], HIGH);
  }

  // Keeps track of the button state
  static bool was_pressed = false;
  bool pressed = !digitalRead(LOC_BUTTON);

  // Only run on a new press
  if(pressed && !was_pressed) {

    // Clear the LEDs
    for(int i = 0; i < LOC_COUNT; i++) {
      digitalWrite(LOC_LED[i], LOW);
    }

    // Advance the location count or loop to start
    location++;
    if(location > MAX_LOC) {
      location = MIN_LOC;
    }
  }

  digitalWrite(LOC_LED[location], HIGH);
  was_pressed = pressed;
  return location;

}


int updateSkill() {
  int val = analogRead(SKILL_POT);
  int on_led_count = map(val, 0, 1000, 0, NUM_LEDS);

  // First, clear the existing led values
  FastLED.clear();
  for (int led = 0; led < on_led_count; led++) {
    leds[led] = CRGB::Blue;
  }
  FastLED.show();
  return on_led_count;
}


int updateInterest(){
  int interest = B00000;
  for(int i = 0; i < INTEREST_COUNT; i++) {
      if( !digitalRead(INTEREST_SW[i]) ) {
        digitalWrite(INTEREST_LED[i], HIGH);
        interest |= 1 << i;
      }
      else {
        digitalWrite(INTEREST_LED[i], LOW);
      }
  }
  return interest;
}


void saveValues(int age, int gender, int location, int skill, int interest) {

  static bool was_pressed = false;
  static bool was_recording = false;
  bool pressed = !digitalRead(ACCEPT_BUTTON);
  static long press_start = millis();
  const int CONFIRM_TIME = 2000;

  if (pressed && !was_pressed) {
      press_start = millis();
  }

  if(pressed && !was_recording && millis() - press_start > CONFIRM_TIME) {

    String output = "";
    output += getDateTime();
    output += ",";
    output += String(age);
    output += ",";
    output += String(gender);
    output += ",";
    output += String(location);
    output += ",";
    output += String(skill);
    output += ",";

    // Convert from bit encoding
    for(int i = 0; i < INTEREST_COUNT; i++) {
      if(interest & 1 << i) {
        output += "x";
      }
      else {
        output += "";
      }
      output += ",";
    }
    output += "\n";
    Serial.println(output);

    // String output = "Age,Gender,Location,Interest,Skill Level /n";
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(output);
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }

    ledTest();
    was_recording = true;
  }
  else if (!pressed){
    was_recording = false;
  }

  was_pressed = pressed;

}

String getDateTime() {
    if(!rtc_active) {
      return "RTC Inactive";
    }
    else {
      DateTime now = rtc.now();
      String date_time = now.timestamp(DateTime::TIMESTAMP_DATE) + ",";
      date_time += now.timestamp(DateTime::TIMESTAMP_TIME);
      return date_time;
    }
}

void setup() {

  // Set GPIO modes

  // Accept
  pinMode(ACCEPT_BUTTON, INPUT_PULLUP);

  // Skill
  pinMode(SKILL_POT, INPUT);

  // Age
  for(int i = 0; i < AGE_BUTTON_COUNT; i++) {
    pinMode(AGE_BUTTON[i], INPUT_PULLUP);
  }

  // Gender
  for(int i = 0; i < GEN_COUNT - 1; i++) {
      pinMode(GEN_SW[i], INPUT_PULLUP);
  }

  for(int i = 0; i < GEN_COUNT; i++) {
      pinMode(GEN_LED[i], OUTPUT);
      digitalWrite(GEN_LED[i], LOW);
  }

  // Interest
  for(int i = 0; i < INTEREST_COUNT; i++) {
    pinMode(INTEREST_SW[i], INPUT_PULLUP);
    pinMode(INTEREST_LED[i], OUTPUT);
    digitalWrite(INTEREST_LED[i], LOW);
  }

  // Location
  pinMode(LOC_BUTTON, INPUT_PULLUP);
  for(int i = 0; i < LOC_COUNT; i++) {
      pinMode(LOC_LED[i], OUTPUT);
      digitalWrite(LOC_LED[i], LOW);
  }


  ledTest();

  // Initialize the RGB LEDS
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	LEDS.setBrightness(30);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // Start connection to the SD card
  Serial.print("Initializing SD card... ");

  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
  }
  else {
    Serial.println("card initialized.");
  }

  display.setBrightness(0x0a);  // set the diplay to maximum brightness

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    rtc_active = false;
  }
  else {
    Serial.println("Found RTC!");
    rtc_active = true;
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}


void loop() {
  // acceptButtonTest();
  // ageButtonTest();
  // skillTest();
  // genderTest();

  // Update the LEDs on the board and get the values
  int age = updateAge();
  int gender = updateGender();
  int location = updateLocation();
  int skill = updateSkill();
  int interest = updateInterest();

  // If the accept button is pressed, save the values
  saveValues(age, gender, location, skill, interest);
}
