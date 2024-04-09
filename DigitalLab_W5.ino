#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform waveform1;  //xy=288,318
AudioOutputI2S i2s1;           //xy=476,319
AudioConnection patchCord1(waveform1, 0, i2s1, 0);
AudioConnection patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=391,209
// GUItool: end automatically generated code

int ledPinArray[4] = { 29, 30, 31, 32 };
int switchPinArray[4] = { 33, 34, 35, 36 };
int potPinArray[4] = { A10, A11, A12, A13 };
int potValArray[4] = { 0, 0, 0, 0 };
int mappedPotArray[4] = { 0, 0, 0, 0 };
int waveforms[4] = { WAVEFORM_SINE, WAVEFORM_SQUARE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH };
int mappedPotWaveType[4] = { 0, 0, 0, 0 };
int mappedPotWave[4] = { 0, 0, 0, 0 };
int mappedNoteFreq[4] = { 0, 0, 0, 0 };
int noteFrequency[4] = { 0, 0, 0, 0 };


int ratePot = 0;
int waveFormPot = 0;
// variables

void setup() {
  AudioMemory(12);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPinArray[i], OUTPUT);
    pinMode(potPinArray[i], INPUT);
    pinMode(switchPinArray[i], INPUT);
    waveform1.begin(waveforms[i]);
  }
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveform1.amplitude(0.2);
  waveform1.frequency(262);
}


void potVal() {  //mapped pot values for both regular and snap modes
  for (int i = 0; i < 4; i++) {
    potValArray[i] = analogRead(potPinArray[i]);
    mappedPotWave[i] = map(potValArray[i], 0, 1023, 261, 523);
    waveFormPot = analogRead(A15);
    mappedPotWaveType[i] = map(waveFormPot, 0, 1023, 0, 3);
    waveform1.begin(mappedPotWaveType[i]);
  }
}

void potSnaps() {
  for (int i = 0; i < 4; i++) {
    potValArray[i] = analogRead(potPinArray[i]);
    mappedNoteFreq[i] = map(potValArray[i], 0, 1023, 0, 12);
  }
}

void loop() {
  ratePot = analogRead(A14);
  waveFormPot = analogRead(A15);
  potVal();
  potSnaps();
  if (digitalRead(switchPinArray[1]) == LOW) {     //if octave switch is low play regular octave
    if (digitalRead(switchPinArray[0]) == HIGH) {  //snap mode on if switch pin 0 is HIGH
      for (int i = 0; i < 4; i++) {
        noteFrequency[i] = 261.6 * pow(2, mappedNoteFreq[i] / 12.0);
        digitalWrite(ledPinArray[i], HIGH);
        waveform1.begin(mappedPotWaveType[i]);
        waveform1.frequency(noteFrequency[i]);
        delay(ratePot);
        digitalWrite(ledPinArray[i], LOW);
        delay(ratePot);
      }
    } else {
      for (int i = 0; i < 4; i++) {  //snap mode off
        digitalWrite(ledPinArray[i], HIGH);
        waveform1.begin(mappedPotWaveType[i]);
        waveform1.frequency(mappedPotWave[i]);
        delay(ratePot);
        digitalWrite(ledPinArray[i], LOW);
        delay(ratePot);
      }
    }
  }
  if (digitalRead(switchPinArray[1]) == HIGH) {  //octave on, same code
    if (digitalRead(switchPinArray[0]) == HIGH) {
      for (int i = 0; i < 4; i++) {
        noteFrequency[i] = 261.6 * pow(2, mappedNoteFreq[i] / 12.0);
        digitalWrite(ledPinArray[i], HIGH);
        waveform1.begin(mappedPotWaveType[i]);
        waveform1.frequency(noteFrequency[i] * 2);
        delay(ratePot);
        digitalWrite(ledPinArray[i], LOW);
        delay(ratePot);
      }
    } else {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPinArray[i], HIGH);
        waveform1.begin(mappedPotWaveType[i]);
        waveform1.frequency(mappedPotWave[i] * 2);
        delay(ratePot);
        digitalWrite(ledPinArray[i], LOW);
        delay(ratePot);
      }
    }
  }
}