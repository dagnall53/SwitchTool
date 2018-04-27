# SwitchTool
Rocnet MQTT switch setting tool for Rocrail 
## A simple tool for setting the switch positions on Servo operated points (turnouts)
Number of points (~100) readable is limited by MQTT message limits, you will need to modify PubSubClient to adjust the maximum size of message acceptable. 

The code uses the new Rocrail swprops command which sends all the information about each switch sequentially as separate MQTT messages. 
This means that the overall message length does not vary with th enuber of swithes, BUT the number of MWTT messages will be the same as the number of switches. Because the ESP can be slow parsing the data, this will mean that there can be quite a lot of "messages in flight". Mosquitto places alimit of 100 messages in flight and this will restrict how many switches the tool can detect/work with.

Try to press the "refresh" button only once, as the code can get confused and double (or more) the number of "switches seen".

Once you have a liist of switches, press right to scroll through the slecet / left, right screens. 
The code reads data from the switches by ID and updates on and off positions. You can adjust the switch positions by pressing up and down, and the revised positions will be sent to the servos. 

With non Rocnet switches, the code cannot adjust the left and right positions, but will switch the servo to the left or right.  

Proof of principle code only!, not well written, but lots of comments...

## Hardware
Designed for "wemos battery OLED board"

## Video
Short video here:https://youtu.be/kwU2oWAyCBs
