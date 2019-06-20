const int other=0;
const int education=1;
const int retail =2;
const int makerspace=3;
const int project=4;
const int confirm=5;

const int otherled = 6;
const int educationled=7;
const int retailled=8;
const int makerspaceled=9;
const int projectled=10;
const int confirmled =11;

boolean otherstate =LOW;
boolean educationstate= LOW;
boolean retailstate=LOW;
boolean makerspacestate=LOW;
boolean projectstate=LOW;
boolean confirmstate=LOW;

boolean prevotherstate =LOW;
boolean preveducationstate= LOW;
boolean prevretailstate=LOW;
boolean prevmakerspacestate=LOW;
boolean prevprojectstate=LOW;
boolean prevconfirmstate=LOW;

boolean onother =LOW;
boolean oneducation =LOW;
boolean onretail =LOW;
boolean onmakerspace =LOW;
boolean onproject =LOW;
boolean onconfirm =LOW;

String datastring ="";
#include <SPI.h>
#include <SD.h>

const int chipSelect = 12;

boolean debounce(const int x, boolean last){
  boolean current = digitalRead(x);
  if(last!=current){delay(5);}
  current=digitalRead(x);
  return current;
}

void turn_on (const int x,const int y, boolean w, boolean k, boolean u) {
  w = debounce(x, k); 
  if (k==LOW&& w==HIGH){
    u= !u;
  }
  k= w;

  digitalWrite(y,u);}



void mapfunction(int u, int v, int w, int x, int y, int z,String t)
{
  if (digitalRead(z)==HIGH){
        if (digitalRead(u)==HIGH){t +="1,";}
          else{t += "0,";}
          if (digitalRead(v)==HIGH){t +="1,";}
          else{t += "0,";}
          if (digitalRead(w)==HIGH){t +="1,";}
          else{t += "0,";}
          if (digitalRead(x)==HIGH){t +="1,";}
          else{t += "0,";}
          if (digitalRead(y)==HIGH){t +="1,";}
          else{t += "0,";}
//File datafile = SD.open ("datalog.txt", FILE_WRITE);
//    if (datafile){datafile.println(datastring);}
//    else {
//      Serial.println("can not open file");
//    }
Serial.println(datastring);     
}
}
void setup() {
  // put your setup code here, to run once:
pinMode(otherled, OUTPUT);
pinMode(educationled, OUTPUT);
pinMode(retailled, OUTPUT);
pinMode(makerspaceled, OUTPUT);
pinMode(projectled, OUTPUT);
pinMode(confirm, INPUT);
pinMode(other, INPUT);
pinMode(education, INPUT);
pinMode(retail, INPUT);
pinMode(makerspace, INPUT);
pinMode(project, INPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

//  // see if the card is present and can be initialized:
//  if (!SD.begin(chipSelect)) {
//    Serial.println("Card failed, or not present");
//    // don't do anything more:
//    while (1);
//  }
//  Serial.println("card initialized.");
//  datastring= "other, education, retail, makerspace, project";
}



void loop() {
turn_on(other, otherled, otherstate, prevotherstate, onother);
turn_on(education, educationled, educationstate, preveducationstate, oneducation);
turn_on(retail, retailled,retailstate, prevretailstate, onretail);
turn_on(makerspace, makerspaceled, makerspacestate, prevmakerspacestate,onmakerspace);
turn_on(project, projectled,projectstate, prevprojectstate, onproject);

mapfunction(otherled, educationled, retailled, makerspaceled, projectled, confirm, datastring);
}
