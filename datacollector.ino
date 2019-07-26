#include <FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 6
#include <SD.h>
#include <SPI.h>
#include <TM1637Display.h>
String datastring = "Age,Gender,Location,Interest,Skill Level /n";
const int buttonlogger =25;
const int button1 = 2;    // add 1 to number
const int button2 = 3;    // add 5 to number
const int button3 = 4;    // subtract 1 to number
const int button4 = 5;    // subtract 5 to number
const int button5 = 6;    // Male gender
const int button6 = 7;    // Female Gender
const int button7 = 8;    // Non-Binary Gender
const int button12 = 13;  // button switcher for location
const int button13 = 14;  // classes
const int button14 = 15;  // retail
const int button15 = 16;  // makerspace
const int button16 = 17;  // prototyping
const int button17 = 18;   //kits
const int led1 = 19;      // Washtenaw led function#2
const int led2 = 20;      // Southeast Michigan led function#2
const int led3 = 21;      // Other michigan led function#2
const int led4 = 22;      // out of state led function#2

const int CLK = 23;
const int DIO = 24;
const int chipSelect = 25;
int skilllevellights;
int locationstore = 0;
int number = 0;
String location = "";
String genders = "";
String reason = "";
String skilllevels = "";
String numberstring = "";

CRGB leds[NUM_LEDS];
TM1637Display display(CLK, DIO);
void adder() {
  display.showNumberDec(number);
    if (digitalRead(button1) == HIGH) {
      number += 1;
    }
  if (digitalRead(button2) == HIGH) {
    number += 5;
  }
  if (digitalRead(button3) == HIGH) {
    number -= 1;
  }
  if (digitalRead(button4) == HIGH) {
    number -= 5;
  }
}

void gender() {
  if (digitalRead(button5) == HIGH) {
    genders = "Male";
  }
  else if (digitalRead(button6) == HIGH) {
    genders = "Female";
  }
  else {
    genders = "NonBinary";
 }
}


void location() {
  if (digitalRead(button12) == HIGH) {
    locationstore += 1;
  }

  // may need to separate into 2 functions
    if (locationstore % 4 == 1) {
      digitalWrite(led1, HIGH);
      digitalWrite(led4, LOW);
    }
    if (locationstore % 4 == 2) {
      digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
    }
    if (locationstore % 4 == 3) {
      digitalWrite(led3, HIGH);
      digitalWrite(led2, LOW);
    }
    if (locationstore % 4 == 0) {
      digitalWrite(led4, HIGH);
      digitalWrite(led3, LOW);
    }
  }


void locationreader(){ 
  if (digitalRead(led1) == HIGH) {
    location = "Washtenaw";
  }
  else if (digitalRead(led2) == HIGH) {
    location = "Southeast Michigan";
  }
  else if (digitalRead(led3) == HIGH) {
    location = "Other Michigan";
  }
  else if (digitalRead(led4) == HIGH) {
    location = "Out of State";
  }}
void reasonchecks() {
  if (digitalRead(button13) == HIGH) {
    reason = "classes";
  }
  if (digitalRead(button14) == HIGH) {
    reason = "retail";
  }
  if (digitalRead(button15) == HIGH) {
    reason = "MakerSpace";
  }
  if (digitalRead(button16) == HIGH) {
    reason = "Kits";
  }
}

void skilllevel() {
  int val = analogRead(2);
  int numLedsToLight = map(val, 0, 1023, 0, NUM_LEDS);

  // First, clear the existing led values
  FastLED.clear();
  for (int led = 0; led < numLedsToLight; led++) {
    leds[led] = CRGB::Blue;
  }
  FastLED.show();
  skilllevellights = map(val, 0, 1023, 0, 8);
}

void reasons(){
  if (digitalRead(button13)==HIGH){
    reason= "1";
    }
    else if (digitalRead(button14)==HIGH){
    reason= "2";
    }
    else if (digitalRead(button15)==HIGH){
    reason= "3";
    }
    else if (digitalRead(button16)==HIGH){
    reason= "4";}
    else if (digitalRead(button17)==HIGH){
    reason= "5";
    }
  }
  
void mapper() {
  if (buttonlogger == HIGH) {
    numberstring = String(number);
    datastring += numberstring;
    datastring += ",";
    datastring += genders;
    datastring += ",";
    datastring += location;
    datastring += ",";
    datastring += reason;
    datastring += ",";
    skilllevels = String(skilllevellights);
    datastring += skilllevels;
    datastring += ",";
    datastring += "\n";
  }
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(datastring);
    dataFile.close();
    // print to the serial port too:
    Serial.println(datastring);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  location = "";
  genders = "";
  reason = "";
  skilllevels = "";
  numberstring = "";
}

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(13,INPUT);
  pinMode(14,INPUT);
  pinMode(15,INPUT);
  pinMode(16,INPUT);
  pinMode(17,INPUT);
  
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(27,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
  display.setBrightness(0x0a);  // set the diplay to maximum brightness
}

void loop() {
  adder();
  gender();
  location();
  locationreader();
  skilllevel();
  reasons();
  mapper();
}
