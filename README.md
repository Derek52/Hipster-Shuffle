My mp3 player, that served me for several years, died this week, and that saddened me dearly. So, I threw this together with stuff from my electronics box.

In this project, there are 2 arduino projects, that are nearly identical. Both, randomly play mp3's off of a micro SD card using a DFPlayer Mini module.

They both have buttons for skipping songs, and pausing songs. The difference is that 1 project, has code that reads 2 pins, to connect buttons for volume up and down.
The other project does an analogRead and uses a potentiometer for volume. This is my preferred version. The potentiometer sticking out of the device is real inconvenient in my pocket.
But, it looks so cool.

I've included some photos of both setups on a breadboard, mostly because I thought putting everything on one tiny breadboard, looked like beautiful organized chaos.

There is also a schematic for the project that uses the volume potentiometer. I'll make a schematic for the other version, at some point in time(probably).

To compile this project, you will need the DFPlayer's library which you can find here https://github.com/DFRobot/DFRobotDFPlayerMini