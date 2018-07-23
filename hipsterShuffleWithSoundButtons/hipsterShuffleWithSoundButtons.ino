#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); //RX, TX
DFRobotDFPlayerMini myDfPlayer;

const int musicPlayingPin = 2;
const int volumeDownButtonPin = 3;
const int volumeUpButtonPin = 4;
const int pauseButtonPin = 5;
const int nextButtonPin = 8;

boolean paused = false;

int debounceDelay = 120;//The device will wait this amount of milliseconds, to make sure 1 button press, doesn't register as multiple.

void setup() {
  pinMode(musicPlayingPin, INPUT);
  pinMode(volumeDownButtonPin, INPUT);
  pinMode(volumeUpButtonPin, INPUT);
  pinMode(pauseButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  if (!myDfPlayer.begin(mySoftwareSerial)) {
    while(true);
  }
  myDfPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDfPlayer.volume(20);
  myDfPlayer.enableLoop();
  myDfPlayer.randomAll();

  //some stuff I was using for debugging.
  //Serial.println(myDfPlayer.readState());
  //Serial.println(myDfPlayer.readFileCounts());//this line isn't working for some reason, and I really wish it was.
  //Serial.println(myDfPlayer.readVolume());
  //Serial.println(myDfPlayer.readState());
}

void loop() {
  int pauseButtonReading = digitalRead(pauseButtonPin);
  int nextButtonReading = digitalRead(nextButtonPin);
  int volumeDownReading = digitalRead(volumeDownButtonPin);
  int volumeUpReading = digitalRead(volumeUpButtonPin);

  if (pauseButtonReading == HIGH) {
      if (paused) {
        myDfPlayer.start();
      } else {
        myDfPlayer.pause();
      }
      paused = !paused;
      delay(debounceDelay);
  }

  if (nextButtonReading == HIGH) {
      myDfPlayer.next();
      paused = true;
      delay(debounceDelay);
  }

  if (volumeDownReading == HIGH) {
      myDfPlayer.volumeDown();
      delay(debounceDelay);
  }

  if (volumeUpReading == HIGH) {
      myDfPlayer.volumeUp();
      delay(debounceDelay);
  }

  //the DfPlayer Busy pin, is LOW while a song is playing, and HIGH when a song is not playing
  //if our music is not paused, and the musicPlayingPin is HIGH, we need to start a new song.
  if (!paused) {
    if (digitalRead(musicPlayingPin) == HIGH) {
      myDfPlayer.next();
      delay(debounceDelay);
    }
  }
}
