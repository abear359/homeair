# Overview
Collect and print pollution data (small particulate matter) using Arduino, ESP-01 (ESP8266), SDS011, DHT22 and an OLED 128x32 screen.

## What Is Arduino
Arduino is a microcontroller that is great at small tasks. Please refer to the website arduino.cc

## What is an ESP-01
ESP-01 is a wifi module for Arduino, It was first released as an add on wifi module. Then the manufacturer released documentation for it and source code, since then you are able to find many more versions of ESP-01 all with the base architecure of ESP8266. The most modern version of ESP-01 is known as the node mcu. 

## What Is SDS011
SDS011 is an inexpensive particle matter sensor (lazer based) that senses pm 2.5 and PM 10 particles. It is sold by inovafitness based in China.

## What Is DHT22
The DHT22 is a very inexpensive temperature and humidity sensor.

## What Is OLED Screen
OLED screen is a 128 by 32 pixel organic light emmitting diode screen. It's very bright, low power and inexpensive.

## What Is Special About This Project
Nothing. Maybe this code might be useful for someone.

## Notes / What I learned
If you are making an air monitor, the SDS011 performs well. It measures PM 10 and PM 2.5 well (a good review can be found on https://aqicn.org/). I recommend using a NodeMCU instead of an arduino. A NodeMCU can do all the work of displaying data and updating a server. Alternatively, If you don't need a graphical display and only want to update data to a server, then you can easily use only an ESP-01 and an SDS011, they will work well together and be very affordable. 
