# Project_elective_6th_sem
____

College: [International Institute of Information Technology Bangalore](https://www.iiitb.ac.in/)

Mentor professor: [Professor Madhav Rao](https://www.iiitb.ac.in/faculty/madhav-rao)

Student: Sai Manish Sasanapuri - 3rd year ECE Integrated Mtech(Btech + Mtech) 

____
## Project Overview
### Precision agriculture for small and medium size farmlands - using an IoT approach 
#### Parameters currently measured :
1. Soil Temperature
2. Soil moisture content
3. Air temperature
4. Air Moisture content/ Humidity

___
#### Components used : 
1. Arduino Microcontroller
2. DHT 22
3. Capacitive Soil Moisture Sensor
4. Soil Temperature sensor ( DS18B20)
5. DSRTC 3231 module
6. HC-05 Bluetooth module
7. Resistors

#### Configure of system :
1. Parent device subsystem
2. Child device subsystem
3. Repeater subsystem

#### Overview of Project Implementation :
The primary objective in this project elective is child subsystem collects sensors
data and transmit using a wireless communication device. Presently I am using
the HC-05 Bluetooth module here but we can use Lora devices which is low power
long-range communication device.
How it works? Firstly we will connect all the sensors to Arduino to collect
field data. We will be using the RTC timer to wake up Arduino by sending an
interrupt to Arduino for every specified interval of time. I have tried two ways of
storing the data, one is to store the data in the local system as a CSV file or can
upload the data to the cloud. In this project, I have used the Thingspeak platform
as a cloud platform to store the data and plot the received data.
Here python is middleware to save or upload collected sensor data by arduino, using
the serial library in python to read the real-time data of all the sensors from the
COM port and wrote a python script to write them in a CSV file or upload data to
the Thingspeak platform.
Assuming that we don’t need a precision of 1 or 2 then we can save a lot of
bits. If our message packet should have only 32bits, without precision we can pack
2 sets of data(soil temp and atmosphere temp) + 8bits(one data + 2bits can be
used for something else) or those 8 bits can used for some encoding scheme. With
precision, we can pack only one set of data (soil temp and atmosphere temp).
An efficient way of sending data by reducing bits. If we know the range of
sensor value, we can manipulate them. For example, if we know the temperature
range varies in between (-7 to 7 from a certain value) then we can transmit just 4
bits(offset) in place of 6 bits at the receiver side we add this certain value to that
offset. By doing this we save 2 bits for each sensor value(without precision). By
this method, if the payload size is 32bit we can pack 4 sets of data(soil temp +
atmosphere temp). The mean value for which we will be adding the offset is send
at the start of the program so that both the receiver and transmitter are in sync
with the values.
The Arduino sends the packed data as we are using python as middleware it reads
the packed data received through COM port first, and then decode or unpack them
and get actual values (by adding mean + offset). The decoded data is processed
and then either saved in a CSV file or uploaded to the cloud.

___
#### Code :
All my code implementations are present in Code folder with readme file for every code folder.
___

#### Future Developements
1. We can collect water level data.
2. We use camera for Photo analysis of Nutrient contents in crop
3. We can collect Soil pH level data
4. Can make it semi automated or automated system. In Automated it should
compute the sensor values and do the action accordingly. In semi automated
the user should check the values and if possible using previous data we can
suggest what steps to follow.
5. For further development of the project we can use data-mining and ml algorithms to suggest best crop on soil, weather conditions, and place at which the
crop is present.
