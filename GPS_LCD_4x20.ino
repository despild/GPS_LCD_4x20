
#include <SoftwareSerial.h>
#include <string.h>
#define GPSrxPin 0
#define GPStxPin 1
#define LCDtxPin 2

SoftwareSerial LCD = SoftwareSerial(7,LCDtxPin);


String readLine ="";
boolean stringComplete = false;

String time="hhmmss";
String la="ddmm.mmmm";
String ns="a";
String lo="dddmm.mmmm";
String ew="a";
String al="x.x";
String sat="xx";



void setup(){
  pinMode(GPSrxPin,INPUT);
  pinMode(GPStxPin,OUTPUT);
  pinMode(LCDtxPin,OUTPUT);
  Serial.begin(9600);
  LCD.begin(9600);
  LCD.write("$B,0\r");
  LCD.write("$D,1\r");
  backlightOn();
  clearLCD();
  readLine.reserve(200);

  goTo(1,1);

}



void loop(){
  if (stringComplete) {
//    Serial.println(readLine);
    if(splitString(readLine,',',0)=="$GPGGA"){
      time =splitString(readLine,',',1);
      la =splitString(readLine,',',2);
      ns =splitString(readLine,',',3);
      lo =splitString(readLine,',',4);
      ew =splitString(readLine,',',5);
      al =splitString(readLine,',',9);
    }
    if(splitString(readLine,',',0)=="$GPGSV"){
      sat = splitString(readLine,',',3);
    }
    readLine ="";
    stringComplete=false;

  }

  goTo(1,1);
//  LCD.print("$T,                    \r");
  LCD.print("$T,T:");
  LCD.print(time);
  LCD.print("\r");

  goTo(1,15);
  LCD.print("$T,Sat:");
  LCD.print(sat);
  LCD.print("\r");  

  goTo(2,1);
//  LCD.print("$T,                    \r");
  LCD.print("$T,La:");
  LCD.print(la);
  LCD.print(',');
  LCD.print(ns);
  LCD.print("\r");      
  goTo(3,1);
//  LCD.print("$T,                    \r");
  LCD.print("$T,Lo:");
  LCD.print(lo);
  LCD.print(',');
  LCD.print(ew);
  LCD.print("\r");      
  goTo(4,1);
//  LCD.print("$T,                    \r");
  LCD.print("$T,Al:");
  LCD.print(al);
  LCD.print("m\r");  
  
  delay(50);

}

void serialEvent(){

  while (Serial.available()) {

    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    readLine += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}


String splitString(String s, char parser,int index){
  String rs='\0';
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;

  while(index>=parserCnt){
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);

    if(index == parserCnt){
      if(rToIndex == 0 || rToIndex == -1){
        return '\0';
      }
      return s.substring(rFromIndex,rToIndex);
    }
    else{
      parserCnt++;
    }

  }
  return rs;
}



void goTo(int row, int col){
  LCD.print("$G,");
  LCD.print(row);
  LCD.print(",");
  LCD.print(col);
  LCD.print(",\r");

}

void clearLCD(){
  LCD.print("$C,\r");

}

void backlightOn(){
  LCD.print("$L,1\r");

}

void backlightOff(){
  LCD.print("$L,0\r");
}







