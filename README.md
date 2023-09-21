# smart parking

## Avesani Davide 2060112

### IoT Individual Project

### 1. Introduction

The problem that this IoT project aims to cope with is the difficulty of finding available parking spaces in crowded areas. Traditional parking areas can be time-consuming and frustrating, often resulting in wasted time and increased traffic congestion. Smart parking tunnel solution uses sensors to detect available spaces and communicates that information to a web app. In this way, drivers can quickly and easily locate available parking spots. With the ability to access the data through a web app, drivers can plan their parking before they arrive, saving time and reducing stress. The use of IoT technology in this project provides a more efficient and user-friendly solution to a common problem.

### 2. Prototype design

The prototype is designed to perform these to main tasks:

- Detect whther a car is parked inside a parking spot;
- Allow system user to reach this information

The first task is carried out using a number of laser sensor, positioned in pairs facing eacnh other. If a car is parked in between the pairs, the laser beam is interrupted and the car is detected.  
The second instead is done either by collecting real-time data and sending it to the web app and by activating some LEDs, positioned over each parking spot, to inform users inside the tunnel about the status of the parking space.  
Also, the ESP32 display will be used to simulate a screen which shows a parking map with the free and the occupied parking spots.  
The following picture shows a schema about the high level architecture described.

![Overall project Architecture](/Diagrams/Parking%20Diagram.JPG)

All components will be connected to the ESP32 board, which will manage their operation and interaction. The board will also be responsible for sending the data received from the sensors to the AWS web server, to make it accessible through the web portal.

#### 3 Components

**ESP32v3 board with Display**
![ESP32v3 board with Display](/images/ESP32.jpg)
The ESP32v2 ins equipped wit an onboard 0.96-inch 128\*64 dot matrix Blue OLED display.  
The display is used to show the parking map with the free and the occupied parking spots.

**Infrared Emitters and Detectors**
![Infrared Emitters and Detectors](/images/Infrared%20Emitters%20and%20Detectors.jpg)
These components are used to detect if a car is parked in a parking spot.
They work in pairs, an emitter with a yellow dot on top and a receiver with a red dot.
The pair is positioned facing each other, at the ends of each parking spot, so that the LED beam emitted by the emitter points directly the receiver.
If a car is then parked inside a parking spot, the led beam will be interrupted and the car will be therefore detected.

**LEDs**
![LEDs](/images/LED.webp)
A pair of LEDs, one green and one red, are placed in correspondance of each parking spot.  
The red LED will be switched when a parked car is detected, the green one in the opposite case.

### 4. Network architecture

![Network architecture](/images/network_architecture.jpg)
The above pictore shows an high level explanation on the network architecture which is used to connect the ESP32v2 board to the web portal.  
The network architecture is implemented using AWS, which is used to receive the information, process it and make it available through the web application.  
Therefore, a Python transparent bridge it is used to allow the communication between the board and the AWS architecture.

### 6. How do you measure the performance of the system?

The system should be able to correctly identify weather the parking spot is occupied. It should also be able to avoid false laser sensor updates that may be caused by people standing in front of the sensor or other environment variables such as animals or trash.

## Video demonstration

- [Link to the video](https://youtu.be/ZhnvmG-MbWw)

## Hackster.io article

- [Project blog entry on Hackster](#)
