# smarTunnel

## Avesani Davide 2060112

### IoT Individual Project

### 1. What is the problem and why do you need IoT?

The problem that this IoT project aims to cope with is the difficulty of finding available parking spaces in crowded areas. Traditional parking areas can be time-consuming and frustrating, often resulting in wasted time and increased traffic congestion. By implementing an IoT parking system that uses sensors to detect available spaces and communicates that information to a central server, drivers can quickly and easily locate available parking spots. This system can also help reduce traffic congestion, decrease emissions, and improve overall traffic flow. With the ability to access the data through a web app, drivers can plan their parking before they arrive, saving time and reducing stress. The use of IoT technology in this project provides a more efficient and user-friendly solution to a common problem.

### 2. What data are collected and by which sensors?

Each parking spot will comprehend a laser sensors, responsible of detecting wether the car spot is free or occupied. This sensor, among with the ESP32 board, will detect the data regarding the state of the parking spot and the overall duration of the parking.
The data collected can be used in future to generate statistical analysis.

### 3. What are the connected components, the protocols to connect them and the overall IoT architecture?

The project components are the lasers sensors, green and red leds and ESP32 board among with its display. All the components will be connected to the ESP32 board, that will manage its functioning and interaction as described below.

One red and one green led will be positioned above each parking spot. The ESP32 board will activate the red or the green led, depending on wether the laser sensor indicates that the parking space is free or occupied.

The ESP32 display will be used to show a parking map with the free and the occupied parking spots, updated regularly.

The collected data will be sent to an AWS web server, and it will be accessible trough a web portal that, using these informations, will display real time updates regarding the parking area status.

![Overall project Architecture](/Diagrams/Parking%20Diagram.JPG)

### 4. How do you measure the performance of the system?

The system should be able to correctly identify weather the parking spot is occupied. It should also be able to avoid false laser sensor updates that may be caused by people standing in front of the sensor or other environment variables such as animals or trash.
