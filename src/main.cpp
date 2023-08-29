#include <Arduino.h>
#include <BLEMidi.h>
#include "OneButton.h"
#define BUTTON_FRONT_TOP D7
#define BUTTON_FRONT_BOTTOM D8
#define BUTTON_BACK_TOP D10
#define BUTTON_BACK_BOTTOM D9
#define POTI_TOP A1
#define POTI_BOTTOM A0
#define BUTTONS_FRONT 1
#define BUTTONS_BACK 1
#define POTIS 1

const int buttonFrontTop = BUTTON_FRONT_TOP;
const int buttonFrontBottom = BUTTON_FRONT_BOTTOM;
const int buttonBackTop = BUTTON_BACK_TOP;
const int buttonBackBottom = BUTTON_BACK_BOTTOM;
OneButton ButtonFrontTop(buttonFrontTop, true);
OneButton ButtonFrontBottom(buttonFrontBottom, true);
OneButton ButtonBackTop(buttonBackTop, true);
OneButton ButtonBackBottom(buttonBackBottom, true);
const int potiTop = POTI_TOP;
const int potiBottom = POTI_BOTTOM;

const uint8_t midi_channel = 1;
const uint8_t controller_number = 0;

int midicommands_ft[][2] = {{16, 17}, {18, 19}, {20, 21}, {22, 23}};

int midicommands_fb[] = {24, 25, 26, 27};
int channel_ft = 0;
int channel_fb = 0;

static int averageValue_top = 0;
static int averageValue_bottom = 0;
static int hysteresis_top = 0;
static int hysteresis_bottom = 0;

void click_ft() {
  //Serial.println("frontclicked");
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][0], 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][0], 0);
  //Serial.println("VO");
}
void longclick_ft() {
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][1], 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][1], 0);
    //Serial.println("VO");
}
void click_fb() {
  BLEMidiServer.controlChange(midi_channel, midicommands_fb[channel_fb], 127);

}
void longclick_fb() {
    BLEMidiServer.controlChange(midi_channel, midicommands_fb[channel_fb], 0);
    //Serial.println("VO");
}
void doubleclick_ft() {
  if (channel_ft <3) {
    channel_ft++;
    //BLEMidiServer.noteOn(0, 60, 100+channel_ft);
  }
}
void doubleclick_bt() {
  if (channel_ft > 0) {
    channel_ft--;
    //BLEMidiServer.noteOn(0, 60, 100+channel_ft);
  }
}

void doubleclick_fb() {
  if (channel_fb <3) {
    channel_fb++;
    //BLEMidiServer.noteOn(0, 61, 100+channel_fb);
  }
}
void doubleclick_bb() {
  if (channel_fb > 0) {
    channel_fb--;
    //BLEMidiServer.noteOn(0, 61, 100+channel_fb);
  }
}

void click_bt() {
  BLEMidiServer.controlChange(midi_channel, 28, 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, 28, 0);
//Serial.println("HO");
}
void longclick_bt() {
  BLEMidiServer.controlChange(midi_channel, 29, 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, 29, 0);
    //Serial.println("VO");
}

void click_bb() {
  BLEMidiServer.controlChange(midi_channel, 30, 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, 30, 0);
//Serial.println("HU");
}
void longclick_bb() {
  BLEMidiServer.controlChange(midi_channel, 31, 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, 31, 0);
    //Serial.println("VO");
}


void setup() {
  //Serial.begin(115200);
  //Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("Trumpet Controller");
  //Serial.println("Waiting for connections...");
  BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library
  ButtonFrontTop.attachClick(click_ft);
  ButtonFrontBottom.attachClick(click_fb);
  ButtonBackTop.attachClick(click_bt);
  ButtonBackBottom.attachClick(click_bb);
  ButtonFrontTop.attachLongPressStart(longclick_ft);
  ButtonFrontBottom.attachLongPressStart(longclick_fb);
  ButtonBackTop.attachLongPressStart(longclick_bt);
  ButtonBackBottom.attachLongPressStart(longclick_bb);
  ButtonFrontTop.attachDoubleClick(doubleclick_ft);
  ButtonFrontBottom.attachDoubleClick(doubleclick_fb);
  ButtonBackTop.attachDoubleClick(doubleclick_bt);
  ButtonBackBottom.attachDoubleClick(doubleclick_bb);
}

void loop() {
  if(BLEMidiServer.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full velocity (127)
  if (BUTTONS_FRONT == 1) {
    ButtonFrontTop.tick();
    ButtonFrontBottom.tick();
  }
  if (BUTTONS_BACK == 1) {
    ButtonBackTop.tick();
    ButtonBackBottom.tick();
  }
  
  if (POTIS == 1) { 
    int sensorValue_top = analogRead(potiTop)/8;
    averageValue_top = (averageValue_top * 7 + sensorValue_top) / 8;  // billig Tiefpass um ausreißer wegzufiltern
    if (abs(hysteresis_top-averageValue_top) > 8){
      hysteresis_top = averageValue_top;
      //Serial.print("Top ");
      //Serial.println(hysteresis_top);
      BLEMidiServer.controlChange(midi_channel, 13, 127-hysteresis_top);
    }
    int sensorValue_bottom = analogRead(potiBottom)/8;
    averageValue_bottom = (averageValue_bottom * 7 + sensorValue_bottom) / 8;  // billig Tiefpass um ausreißer wegzufiltern
    if (abs(hysteresis_bottom-averageValue_bottom) > 8){
      hysteresis_bottom = averageValue_bottom;
      //Serial.println(hysteresis_bottom);
      BLEMidiServer.controlChange(midi_channel, 12, 127-hysteresis_bottom);
    }
  }
  delay(10);
  }
}