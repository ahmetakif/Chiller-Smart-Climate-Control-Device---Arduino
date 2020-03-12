///////////////////////////////////////////////////
//######## CHILLER-SMART-CLIMATE-CONTROL-DEVICE #########
//#
//# Author: Ahmet Akif Kaya
//# Date: 08/25/19
//# Description:
//# This project is developed by @ahmetakifkaya
//# for Uzerman Muhendislik to be used as a 
//# controller for the chiller devices in 
//# Suleymen Demirel Culture Centre in KBB

//# This code is specifically written for the Chiller-Smart-Climate-Control-Device project by Ahmet Akif Kaya
//# and all rights of this program is owned by Ahmet Akif Kaya under APACHE open source license. 
//# You are automatically accepting the terms of the APACHE license by using this code in any way.
//# The terms and conditions about this project's APACHE license can be found in the file named
//# "LICENSE" in the root directory of this repository.

//# Copyright 2020 Ahmet Akif Kaya
//# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
//# You may obtain a copy of the License at
//# http://www.apache.org/licenses/LICENSE-2.0
//# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an 
//# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific 
//# language governing permissions and limitations under the License.
// 
//    ///////////////    ////////////////
//    //           //    //            //
//    //  ///////////    ////////  //////
//    //  //                   //  //
//    //  ////                 //  //
//    //    //                 //  //
//    //  ////                 //  //
//    //  //                   //  //
//    //  ///////////    ////////  ///////
//    //           //    //             //
//    ///////////////    /////////////////
//
//   ////////////////////////////////////////
//   ////////////////////////////////////////
//
////////////////////////////////////////////////////
// https://github.com/ahmetakif
// http://ahmetakifkaya.com
// https://www.uzerman.com
////////////////////////////////////////////////////

#include <Nextion.h>
#include <stdio.h>
#include <DS1302.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A2  
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

const int role = 9; //9 or 10 is OK
boolean runn = 0;
float istsic = 22.0;
float paysic = 1.5;
boolean mode = 0; //0 for heating 1 for cooling
String ErrorMes;
String tex;

boolean readmode = 0;

int minn = 10;
int hr = 16;
int date = 2;
int mon = 11;
int yr = 2019;

int CurrentPage = 0;


//page 0
NexDSButton bt0 = NexDSButton(0, 1, "bt0");
NexButton b0 = NexButton(0, 4, "b0");
NexButton b1 = NexButton(0, 15, "b1");
//page 1
NexDSButton bt1 = NexDSButton(1, 5, "bt0");
NexButton b4 = NexButton(1, 3, "b4");
NexButton b5 = NexButton(1, 15, "b5");
//page 3
NexButton b201 = NexButton(3, 7, "b201");
NexButton b202 = NexButton(3, 10, "b202");
NexButton b203 = NexButton(3, 8, "b203");
NexButton b204 = NexButton(3, 9, "b204");
NexButton b205 = NexButton(3, 12, "b205");
NexButton b206 = NexButton(3, 14, "b206");
NexButton b207 = NexButton(3, 11, "b207");
NexButton b208 = NexButton(3, 15, "b208");
NexButton b209 = NexButton(3, 13, "b209");
NexButton b210 = NexButton(3, 16, "b210");
//page 4
NexButton b301 = NexButton(4, 8, "b301");
NexButton b302 = NexButton(4, 9, "b302");
//pages
NexPage page0 = NexPage(0, 0, "page0");  // Page added as a touch event
NexPage page1 = NexPage(1, 0, "page1");  // Page added as a touch event
NexPage page3 = NexPage(3, 0, "page3");  // Page added as a touch event
NexPage page4 = NexPage(4, 0, "page4");  // Page added as a touch event

NexTouch *nex_listen_list[] = 
{
  &bt0,  // Dual state button added
  &bt1,  // Dual state button added
  &b0,  // Button added
  &b1,  // Button added
  &b4,  // Button added
  &b5,  // Button added
  &b201,  // Button added
  &b202,  // Button added
  &b203,  // Button added
  &b204,  // Button added
  &b205,  // Button added
  &b206,  // Button added
  &b207,  // Button added
  &b208,  // Button added
  &b209,  // Button added
  &b210,  // Button added
  &b301,  // Button added
  &b302,  // Button added
  &page0,  // Page added as a touch event
  &page1,  // Page added as a touch event
  &page3,  // Page added as a touch event
  &page4,  // Page added as a touch event
  NULL  // String terminated
};  // End of touch event list

void page0PushCallback(void *ptr) {
  CurrentPage = 0;
}
void page1PushCallback(void *ptr) {
  CurrentPage = 1;
}
void page3PushCallback(void *ptr) {
  CurrentPage = 3;
}
void page4PushCallback(void *ptr) {
  CurrentPage = 4;
}
void bt0PushCallback(void *ptr) {
  mode = 1;
  sendmode();
}
void bt1PushCallback(void *ptr) {
  mode = 0;
  sendmode();
}
void b0PushCallback(void *ptr) {
  istsic = istsic + 1;
}
void b1PushCallback(void *ptr) {
  istsic = istsic - 1; 
}
void b4PushCallback(void *ptr) {
  istsic = istsic + 1;
}
void b5PushCallback(void *ptr) {
  istsic = istsic - 1;
}
void b201PushCallback(void *ptr) {
  settingsPrintTime();
  if (hr < 23){
    hr = hr + 1;
  }
  settime();
}
void b202PushCallback(void *ptr) {
  settingsPrintTime();
  if (hr > 0){
    hr = hr - 1;
  }
  settime();
}
void b203PushCallback(void *ptr) {
  settingsPrintTime();
  if (minn < 59){
    minn = minn + 1;
  }
  settime();
}
void b204PushCallback(void *ptr) {
  settingsPrintTime();
  if (minn > 0){
    minn = minn - 1;
  }
  settime();
}
void b205PushCallback(void *ptr) {
  settingsPrintTime();
  if (date < 31){
    date = date + 1;
  }
  settime();
}
void b206PushCallback(void *ptr) {
  settingsPrintTime();
  if (date > 1){
    date = date - 1;
  }
  settime();
}
void b207PushCallback(void *ptr) {
  settingsPrintTime();
  if (mon < 12){
    mon = mon + 1;
  }
  settime();
}
void b208PushCallback(void *ptr) {
  settingsPrintTime();
  if (mon > 1){
    mon = mon - 1;
  }
  settime();
}
void b209PushCallback(void *ptr) {
  settingsPrintTime();
  yr = yr + 1;
  settime();
}
void b210PushCallback(void *ptr) {
  settingsPrintTime();
  if (yr > 0){
    yr = yr - 1;
  }
  settime();
}
void b301PushCallback(void *ptr) {
  paysic = paysic + 0.1;
}
void b302PushCallback(void *ptr) {
  paysic = paysic - 0.1;
}

namespace {
  
const int kCePin   = 6;  // Chip Enable
const int kIoPin   = 7;  // Input/Output
const int kSclkPin = 8;  // Serial Clock
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();
  Serial.print("t5.txt=");
  if (int(t.hr) < 10){
    if (int(t.min) < 10){
      tex = "\"0" + String(t.hr) + ".0" + String(t.min) + "\"";
    }
    else {
      tex = "\"0" + String(t.hr) + "." + String(t.min) + "\"";
    }
  }
  else {
    if (int(t.min) < 10){
      tex = "\"" + String(t.hr) + ".0" + String(t.min) + "\"";
    }
    else{
      tex = "\"" + String(t.hr) + "." + String(t.min) + "\"";
    }
  }
  Serial.print(tex);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void printDate() {
  // Get the current time and date from the chip.
  Time t = rtc.time();
  Serial.print("t3.txt=");
  tex = "\"" + String(t.date) + "/" + String(t.mon) + "/" + String(t.yr) + "\"";
  Serial.print(tex);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
}

void settingsPrintTime(){
  Time t = rtc.time();
  Serial.print("t205.txt=");
  if (int(t.hr) < 10){
    if (int(t.min) < 10){
      tex = "\"0" + String(t.hr) + ".0" + String(t.min) + "\"";
    }
    else {
      tex = "\"0" + String(t.hr) + "." + String(t.min) + "\"";
    }
  }
  else {
    if (int(t.min) < 10){
      tex = "\"" + String(t.hr) + ".0" + String(t.min) + "\"";
    }
    else{
      tex = "\"" + String(t.hr) + "." + String(t.min) + "\"";
    }
  }
  Serial.print(tex);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("t203.txt=");
  tex = "\"" + String(t.date) + "/" + String(t.mon) + "/" + String(t.yr) + "\"";
  Serial.print(tex);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  minn = t.min;
  hr = t.hr;
  date = t.date;
  mon = t.mon;
  yr = t.yr;
}

void settime(){
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(yr, mon, date, hr, minn, 00, Time::kMonday);
  rtc.time(t);
}

void err(){
  Serial.print("t0.txt=");
  tex = "\"" + ErrorMes + "\"";
  Serial.print(tex);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(200);
  Serial.print("t0.pco=");
  Serial.print(63488);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(200);
}

void sendmode(){
  Serial.print("mode.val=");
  Serial.print(int(mode));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);    
}

void setup() 
{
  delay(500);
  runn = 0;
  pinMode(role,OUTPUT);
  digitalWrite(role,0);
  Serial.begin(9600);
  rtc.writeProtect(false);
  rtc.halt(false);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  bt0.attachPush(bt0PushCallback);  // Button press
  bt1.attachPush(bt1PushCallback);  // Button press
  b0.attachPush(b0PushCallback);  // Button press
  b1.attachPush(b1PushCallback);  // Button press
  b4.attachPush(b4PushCallback);  // Button press
  b5.attachPush(b5PushCallback);  // Button press
  b201.attachPush(b201PushCallback);  // Button press
  b202.attachPush(b202PushCallback);  // Button press
  b203.attachPush(b203PushCallback);  // Button press
  b204.attachPush(b204PushCallback);  // Button press
  b205.attachPush(b205PushCallback);  // Button press
  b206.attachPush(b206PushCallback);  // Button press
  b207.attachPush(b207PushCallback);  // Button press
  b208.attachPush(b208PushCallback);  // Button press
  b209.attachPush(b209PushCallback);  // Button press
  b210.attachPush(b210PushCallback);  // Button press
  b301.attachPush(b301PushCallback);  // Button press
  b302.attachPush(b302PushCallback);  // Button press
  page0.attachPush(page0PushCallback);  // Page press event
  page1.attachPush(page1PushCallback);  // Page press event
  page3.attachPush(page3PushCallback);  // Page press event
  page4.attachPush(page4PushCallback);  // Page press event
  
  Serial.print("t0.txt=");
  Serial.print("\"Hosgeldiniz\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(200);
  Serial.print("t0.pco=");
  Serial.print(65504);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(200);
}

void loop() 
{
  sendmode(); 
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  // MAIN LOGIC OF CHILLER DEVICE ///////////////////////
  if (mode == 0){
    if (float(event.temperature) < float(abs(istsic - paysic))){
      runn = 1;
    }
    else if (float(event.temperature) > float(istsic)) {
      runn = 0;
    }
    else {
      runn = runn;
    }
  }
  else {
    if (float(event.temperature) > float(abs(istsic + paysic))){
      runn = 1;
    }
    else if (float(event.temperature) < float(istsic)) {
      runn = 0;
    }
    else {
      runn = runn;
    }
  }
  if (runn == 1){
    digitalWrite(role,1);
  }
  else{
    digitalWrite(role,0);
  }
  ///////////////////////////////////////////////////////  
 
  
  if(CurrentPage == 0 or CurrentPage == 1){
    if (isnan(event.temperature)) {
      ErrorMes = "!!Sicaklik bilgisi okunamadi!!";
      err();
    }
    else {
      ErrorMes = " ";
      err();
    }
    Serial.print("x0.val=");
    Serial.print(int(event.temperature*10));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("t6.txt=");
    Serial.print("\"Oda C\"");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("x1.val=");
    Serial.print(int(istsic*10));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      ErrorMes = "!!Nem bilgisi okunamadi!!";
      err();
    }
    else {
      ErrorMes = " ";
      err();
    }
    Serial.print("x2.val=");
    Serial.print(int(event.relative_humidity*10));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.print("t1.txt=");
    Serial.print("\"Nem %\"");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    printDate();
    printTime();
    if(CurrentPage == 0){
      mode = 0;
    }
    else if (CurrentPage == 1){
      mode = 1;
    }
  }
  if(CurrentPage == 3){
    settingsPrintTime();
  }
  if(CurrentPage == 4){
    Serial.print("x300.val=");
    Serial.print(int(paysic*10));
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }
  nexLoop(nex_listen_list);
}
