using namespace std;
#include <TM1637Display.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
string datastring = "Age,Gender,Location,Interest,Skill Level /n" ;
const int buttonlogger;
const int button1=1; // add 1 to number
const int button2 =2;//add 5 to number
const int button3=3;//subtract 1 to number
const int button4=4;//subtract 5 to number
const int button5=5;//Male gender
const int button6=6;//Female Gender
const int button7=7;// Non-Binary Gender
const int button8=8;//Washtenaw location
const int button9=9;//Southeast Michigan location
const int button10=10;//Other Michigan
const int button11=11; // Out of State
const int button12=12;//button switcher for location
const int button13=13;//classes
const int button14=14;//retail
const int button15=15;//makerspace
const int button16=16;//kits
const int led1=17;//Washtenaw led function#2
const int led2=18;//Southeast Michigan led function#2
const int led3 = 19;  // Other michigan led function#2
const int led4 = 20;  // out of state led function#2
const int CLK = 21;   
const int DIO=22;
int skillevellights;
int locationstore=0;
int number = 0;
string location="";
string genders="";
string reason="";
string skilllevels="";
string numberstring="";
TM1637Display display(CLK, DIO);

void adder() {
display.showNumberDec(number);
if 
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
    genders += "Male";
  }
  if (digitalRead(button6) == HIGH) {
    genders += "Female";
  }
  if (digitalRead(button7) == HIGH) {
    genders += "NonBinary";
  }
}

void locations(){
    if (digitalRead(button8)==HIGH){
        location= "Washtenaw";
    }
    if (digitalRead(button9)==HIGH){
        location="Southeast Michigan";
    }
    if (digitalRead(button10)==HIGH){
        location="Other Michigan";
    }
    if (digitalRead(button11)==HIGH){
        location="Out of State";
    }
}

void locationstest(){
if (digitalRead(button12)==HIGH){
    locationstore+=1;
    }

    // may need to separate into 2 functions
while (true){
    if (locationstore%4==1){
        digitalWrite(led1,HIGH);
        digitalWrite(led4, LOW);
    }
if (locationstore % 4 == 2) {
  digitalWrite(led2, HIGH);
  digitalWrite(led1, LOW);
}
if (locationstore % 4 == 3) {
  digitalWrite(led3,HIGH);
  digitalWrite(led2, LOW);
}
if (locationstore % 4 == 1) {
  digitalWrite(led4, HIGH);
  digitalWrite(led3, LOW);
}
}
if (digitalRead(led1) == HIGH) {
  location = "Washtenaw";
}
if (digitalRead(led2) == HIGH) {
  location = "Southeast Michigan";
}
if (digitalRead(led3) == HIGH) {
  location = "Other Michigan";
}
if (digitalRead(led4) == HIGH) {
  location = "Out of State";
}
}
void reasonchecks(){
  if (digitalRead(button13) == HIGH) {
    reason = "classes";
  }
  if (digitalRead(button14 )== HIGH) {
    reason = "retail";
    }
    if (digitalRead(button15) == HIGH) {
      reason = "MakerSpace";
    }
    if (digitalRead(button16) == HIGH) {
      reason = "Kits";
    }
}

void skilllevel(){
  int val = analogRead(2);
  int numLedsToLight = map(val, 0, 1023, 0, NUM_LEDS);

  // First, clear the existing led values
  FastLED.clear();
  for (int led = 0; led < numLedsToLight; led++) {
    leds[led] = CRGB::Blue;
  }
  FastLED.show();
  skilllevellights = map (val, 0, 1023, 0, 8);
}

void mapper(){
    if (buttonlogger==HIGH)
    {
      numberstring = String(number);
      datastring += numberstring;
      datastring += ",";
      datastring += genders;
      datastring += ",";
      datastring += location;
      datastring += ",";
      datastring += reason;
      datastring += ",";
      skilllevels = String (skillevellights);
      datastring += skilllevels;
      datastring += ",";
      datastring += "\n"
    }
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
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
  display.setBrightness(0x0a);  // set the diplay to maximum brightness
}

void loop(){

    adder();
    gender();
    locations();
    skilllevel();
    mapper();
}