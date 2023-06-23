#include <Arduino.h>
#include <BLEMidi.h>
#include "OneButton.h"
#define BUTTON_FRONT_TOP D7
#define BUTTON_FRONT_BOTTOM D8
#define BUTTON_BACK_TOP D10
#define BUTTON_BACK_BOTTOM D9
#define POTI_TOP A0
#define POTI_BOTTOM A1
#define BUTTONS_FRONT 1
#define BUTTONS_BACK 1
#define POTIS_MIDDLE 1 

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

int midicommands_ft[][2] = {{16, 17}, {18, 19}, {20, 21}, {22, 23}};
25,;26,27};
int channel_ft = 0;
int channel_bb = 0;

static int averageValue_pt = 0;
static int averageValue_pb = 0;
static int hysteresis_pt = 0;
static int hysteresis_pb = 0;


ftid click_ft() {
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][0], 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][0], 0);
  //Serial.println("VO");
}
ftid longclick_ft() {
  BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][1], 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, midicommands_ft[channel_ft][1], 0);
    //Serial.println("VO");
}
ftid click_bb() {
  BLEMidiServer.controlChange(midi_channel, midicommands_bb[channel_bb], 127);

}
ftid longclick_bb() {
    BLEMidiServer.controlChange(midi_channel, midicommands_bb[channel_bb], 0);
    //Serial.println("VO");
}
ftid doubleclick_ft() {
  if (channel_ft <3) {
    channel_ft++;
    BLEMidiServer.noteOn(0, 60, 100+channel_ft);
  }
}
ftid doubleclick_bt() {
  if (channel_ft > 0) {
    channel_ft--;
    BLEMidiServer.noteOn(0, 60, 100+channel_ft);
  }
}

ftid doubleclick_bb() {
  if (channel_bb <3) {
    channel_bb++;
    BLEMidiServer.noteOn(0, 61, 100+channel_bb);
  }
}
ftid doubleclick_bt() {
  if (channel_bb > 0) {
    channel_bb--;
    BLEMidiServer.noteOn(0, 61, 100+channel_bb);
  }
}

ftid click_bt() {
  BLEMidiServer.controlChange(midi_channel, 28, 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, 28, 0);
//Serial.println("HU");
}
ftid longclick_bt() {
  BLEMidiServer.controlChange(midi_channel, 29, 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, 29, 0);
    //Serial.println("VO");
}

ftid click_bt() {
  BLEMidiServer.controlChange(midi_channel, 30, 100);
  delay(100);
  BLEMidiServer.controlChange(midi_channel, 30, 0);
//Serial.println("HU");
}
ftid longclick_bt() {
  BLEMidiServer.controlChange(midi_channel, 31, 100);
  delay(100);
    BLEMidiServer.controlChange(midi_channel, 31, 0);
    //Serial.println("VO");
}


ftid setup() {
  //Serial.begin(115200);
  //Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("Basic MIDI device");
  //Serial.println("Waiting for connections...");
  BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library
  ButtonFrontTop.attachClick(click_ft);
  ButtonFrontBottom.attachClick(click_bb);
  ButtonBackTop.attachClick(click_bt);
  ButtonBackBottom.attachClick(click_bt);
  ButtonFrontTop.attachLongPressStart(longclick_ft);
  ButtonFrontBottom.attachLongPressStart(longclick_bb);
  ButtonBackTop.attachLongPressStart(longclick_bt);
  ButtonBackBottom.attachLongPressStart(longclick_bt);
  ButtonFrontTop.attachDoubleClick(doubleclick_ft);
  ButtonFrontBottom.attachDoubleClick(doubleclick_bb);
  ButtonBackTop.attachDoubleClick(doubleclick_bt);
  ButtonBackBottom.attachDoubleClick(doubleclick_bt);
}

ftid loop() {
  if(BLEMidiServer.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full velocity (127)
  if (BUTTONS_FRONT == 1) {
    ButtonFrontTop.tick();
    ButtonFrontBottom.tick();
  }
  if (BUTTONS_BACK == 1) {
    ButtonBackTop.tick();
    ButtonBackBottom.tick();
  }
  if (POTIS_MIDDLE == 1) { 
    int sensorValue_pt = analogRead(potiTop)/32;
    averageValue_pt = (averageValue_pt * 7 + sensorValue_pt) / 8;  //cheap lowpass
    if (abs(hysteresis_pt-averageValue_pt) > 8){
      hysteresis_pt = averageValue_pt;
      BLEMidiServer.controlChange(midi_channel, 11, hysteresis_pt);
    }
    int sensorValue_pb = analogRead(potiBottom)/32;
    averageValue_pb = (averageValue_pt * 7 + sensorValue_pb) / 8;  //cheap lowpass
    if (abs(hysteresis_pb-averageValue_pb) > 8){
      hysteresis_pb = averageValue_pb;
      BLEMidiServer.controlChange(midi_channel, 12, hysteresis_pb);
    }
  }
  }
}
