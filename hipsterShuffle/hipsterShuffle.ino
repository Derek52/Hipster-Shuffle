#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); //RX, TX
DFRobotDFPlayerMini myDfPlayer;

const int volumePin = A0;//analog input for volume knob
const int musicPlayingPin = 2;
const int pauseButtonPin = 5;
const int nextButtonPin = 8;

boolean paused = false;

int debounceDelay = 110;//the amount of time we wait to make sure 1 button press doesn't register as multiple.

//the dfrobot player uses volume values from 0-30. Our analogRead can read from 0-1023.
//1023 divided by 30 = 34. Meaning diving our analogRead by 34, will give us a number from 0-30.
//e.g. analogRead value could be  522, which gives us a volume of 15, because 522/34 = 15.34(but decimals are ignored)
int volumeScaler = 34;
unsigned long volumeDelay = 200;
unsigned long lastVolumeTime = 0;

void setup() {
  pinMode(musicPlayingPin, INPUT);
  pinMode(pauseButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  if (!myDfPlayer.begin(mySoftwareSerial)) {
    while(true);
  }

  myDfPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  setVolume();
  myDfPlayer.enableLoop();
  myDfPlayer.randomAll();


  //some stuff I was using for debugging.
  //Serial.println(myDfPlayer.readState());
  //Serial.println(myDfPlayer.readFileCounts());//this line isn't working for some reason, and I really wish it was.
  //Serial.println(myDfPlayer.readVolume());
  //Serial.println(myDfPlayer.readState());
}

void loop() {
  //the analogRead method in setVolume(), is apparently a resource heavy task
  //Checking the value every loop, caused the music to slowdown/stutter, very noticably
  //so, we only check the volumePin after our volumeDelay has passed
  //this causes a slight delay when changing the volume, but I don't mind it.
  //the other option would be to use regular buttons for volume, but I didn't want to do that for my device.
  if (millis() - lastVolumeTime > volumeDelay) {
    setVolume();
    lastVolumeTime = millis();
  }
  
  int pauseButtonReading = digitalRead(pauseButtonPin);
  int nextButtonReading = digitalRead(nextButtonPin);

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
      delay(debounceDelay);
  }

  //the DfPlayer Busy pin, is LOW while a song is playing, and HIGH when a song is not playing
  //if our music is not paused, and the musicPlayingPin is HIGH, we need to start a new song.
  if (!paused) {
    if (digitalRead(musicPlayingPin) == HIGH) {
      myDfPlayer.next();
      delay(debounceDelay);//without this delay, my device would play ~.2 seconds of one song, and then skip to another song.
    }
  }

  //Serial.println(myDfPlayer.readVolume());

}

void setVolume() {
  int volumePinReading = analogRead(volumePin);
  myDfPlayer.volume(volumePinReading/volumeScaler);
}


//this is a function used to communicate with the dfplayer module on a lower level
//I haven't gotten it to work for me yet.
//void execute_CMD(byte CMD, byte Par1, byte Par2)
//// Excecute the command and parameters
//{
//// Calculate the checksum (2 bytes)
//word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
//// Build the command line
//byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
//Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
////Send the command line to the module
//for (byte k=0; k<10; k++)
//{
//mySerial.write( Command_line[k]);
//}
//}
