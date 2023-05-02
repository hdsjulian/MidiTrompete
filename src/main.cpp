#include <Arduino.h>
#include <BLEMidi.h>
#include "OneButton.h"
#define BUTTON_VORNE_OBEN D8
#define BUTTON_VORNE_UNTEN D7
#define BUTTON_HINTEN_OBEN D10
#define BUTTON_HINTEN_UNTEN D9
const int buttonVorneOben = BUTTON_VORNE_OBEN;
const int buttonVorneUnten = BUTTON_VORNE_UNTEN;
const int buttonHintenOben = BUTTON_HINTEN_OBEN;
const int buttonHintenUnten = BUTTON_HINTEN_UNTEN;
OneButton ButtonVorneOben(buttonVorneOben, true);
OneButton ButtonVorneUnten(buttonVorneUnten, true);
OneButton ButtonHintenOben(buttonHintenOben, true);
OneButton ButtonHintenUnten(buttonHintenUnten, true);

const uint8_t midi_channel = 1;
const uint8_t controller_number = 0;

void click_vo() {
  BLEMidiServer.controlChange(midi_channel, 16, 100);
  delay(10);
  BLEMidiServer.controlChange(midi_channel, 16, 0);
  //Serial.println("VO");
}
void longclick_vo() {
  BLEMidiServer.controlChange(midi_channel, 20, 100);
  delay(10);
    BLEMidiServer.controlChange(midi_channel, 20, 0;
    //Serial.println("VO");
}
void click_vu() {
  BLEMidiServer.controlChange(midi_channel, 17, 100);
  delay(10);
  //Serial.println("VU");
  BLEMidiServer.controlChange(midi_channel, 17, 0);
}
void click_ho() {
  BLEMidiServer.controlChange(midi_channel, 18, 100);
  delay(10);
  BLEMidiServer.controlChange(midi_channel, 18, 0);
//Serial.println("HO");
}
void click_hu() {
  BLEMidiServer.controlChange(midi_channel, 19, 100);
  delay(10);
  BLEMidiServer.controlChange(midi_channel, 19, 0);
//Serial.println("HU");
}


void setup() {
  //Serial.begin(115200);
  //Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("Basic MIDI device");
  //Serial.println("Waiting for connections...");
  BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library
  ButtonVorneOben.attachClick(click_vo);
  ButtonVorneUnten.attachClick(click_vu);
  ButtonHintenOben.attachClick(click_ho);
  ButtonHintenUnten.attachClick(click_hu);
  ButtonVorneOben.attachLongPressStart(longclick_vo);
}

void loop() {
  if(BLEMidiServer.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full velocity (127)
    ButtonVorneOben.tick();
    ButtonVorneUnten.tick();
    ButtonHintenOben.tick();
    ButtonHintenUnten.tick();
    delay(10);
  }
}