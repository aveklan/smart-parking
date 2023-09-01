# smart parking

## Avesani Davide 2060112

### IoT Individual Project

### 1. What is the problem and why do you need IoT?

The problem that this IoT project aims to cope with is the difficulty of finding available parking spaces in crowded areas. Traditional parking areas can be time-consuming and frustrating, often resulting in wasted time and increased traffic congestion. Smart parking tunnel solution uses sensors to detect available spaces and communicates that information to a web app. In this way, drivers can quickly and easily locate available parking spots. With the ability to access the data through a web app, drivers can plan their parking before they arrive, saving time and reducing stress. The use of IoT technology in this project provides a more efficient and user-friendly solution to a common problem.

### 2. What data are collected and by which sensors?

Each parking spot will comprehend a laser sensor, responsible of detecting whether the car spot is free or occupied. This sensor, along with the ESP32 board, will detect the data regarding the state of the parking spot and the overall duration of the parking.
The data collected can be used in future to generate statistical analysis.

### 3. What are the connected components, the protocols to connect them and the overall IoT architecture?

The project components are the lasers sensors, green red LEDs and ESP32 board among with its display. All the components will be connected to the ESP32 board, that will manage its functioning and interaction as described below.

One red and led will be positioned above each parking spot. The ESP32 board will activate the red led, depending when the laser sensor indicates that the parking space is occupied.

The ESP32 display will be used to show a parking map with the free and the occupied parking spots, updated regularly.

The collected data will be sent to an AWS web server, and it will be accessible through a web portal that, using these information, will display real time updates regarding the parking area status.

![Overall project Architecture](/Diagrams/Parking%20Diagram.JPG)

#### 3.1 Components

**ESP32v3 board with Display**
![ESP32v3 board with Display](/images/ESP32.jpg)

**Infrared Emitters and Detectors**
![Infrared Emitters and Detectors](/images/Infrared%20Emitters%20and%20Detectors.jpg)

**LEDs**
![LEDs](/images/LED.webp)

### 5. Network architecture

![Network architecture](/images/network_architecture.jpg)

### 6. How do you measure the performance of the system?

The system should be able to correctly identify weather the parking spot is occupied. It should also be able to avoid false laser sensor updates that may be caused by people standing in front of the sensor or other environment variables such as animals or trash.

## Video demonstration

- [Link to the video](https://youtu.be/ZhnvmG-MbWw)

## Hackster.io article

- [Project blog entry on Hackster](#)
