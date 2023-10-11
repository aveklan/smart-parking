# Smart Parking

## Avesani Davide 2060112

### IoT Individual Project

### 1. Introduction

Finding parking spots in traditional parking lots can be slow and annoying, often causing you to waste time and making traffic worse. Traditional parking areas can be time-consuming and frustrating, often resulting in wasted time and increased traffic congestion. Smart parking tunnel solution uses sensors to detect available spaces and communicates that information to a web app. In this way, drivers can quickly and easily locate available parking spots. With the ability to access the data through a web app, drivers can plan their parking before they arrive, saving time and reducing stress. The use of IoT technology in this project provides a more efficient and user-friendly solution to a common problem.

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
Below a picture showing the prototype of the web app console avaiable at [This link](https://dev.djofecpup54jx.amplifyapp.com/)
![ESP32v3 board with Display](/images/website.PNG)

### 3 Components

**ESP32v3 board with Display**
![ESP32v3 board with Display](/images/ESP32.jpg)
WiFi LoRa 32 is a classic IoT dev-board designed & produced by Heltec Automation(TM), it’s a highly integrated product based on ESP32 + SX127x, it has Wi-Fi, BLE, LoRa functions, also Li-Po battery management system. It is equipped wit an onboard 0.96-inch 128\*64 dot matrix Blue OLED display.  
The display is used to show the parking map with the free and the occupied parking spots.

**Infrared Emitters and Detectors**
![Infrared Emitters and Detectors](/images/Infrared%20Emitters%20and%20Detectors.jpg)

- Specifications:  
   **Operating Voltage** 3.3V ~ 5V  
   **Temperature Measurement Range** -40°C to 85°C  
  [**Datasheet**](https://www.sparkfun.com/datasheets/Components/LTE-302.pdf)

These components are used to detect if a car is parked in a parking spot.
They work in pairs, an emitter with a yellow dot on top and a receiver with a red dot.
The pair is positioned facing each other, at the ends of each parking spot, so that the LED beam emitted by the emitter points directly the receiver.
If a car is then parked inside a parking spot, the led beam will be interrupted and the car will be therefore detected.

**10mm LEDs**
![LEDs](/images/LED.webp)

- Specifications  
  **Forward Voltage** 1.8-2.2VDC  
  **Forward Current** 20mA  
  **Viewing angle** 40 degree  
  **Output** 100-150 MCD

A pair of LEDs, one green and one red, are placed in correspondance of each parking spot.  
The red LED will be switched when a parked car is detected, the green one in the opposite case.

### 4. Network architecture

![Network architecture](/images/network_architecture.jpg)
The above pictore shows an high level explanation on the network architecture which is used to connect the ESP32v2 board to the web portal.  
The network architecture is implemented using AWS, which is used to receive the information, process it and make it available through the web application.  
Therefore, a Python transparent bridge it is used to allow the communication between the board and the AWS architecture. Following a step by step explanation on how the connectivity works.

---

The ESP32 board communicates with the Mosquitto MQTT broker via the MQTT protocol over WiFi IPV4, both devices residing on the same network.  
The MQTT broker interfaces with AWS IoT Core using the Python bridge, maintaining communication through MQTT, and sharing a common topic (smartpark). The bridge forwards the data to AWS IoT Core under the DATA topic.  
AWS IoT Core treats the incoming data as if it's from an ESP32 board, allowing rule-based actions to be defined. All data received on this topic is then stored in a DynamoDB table  
Subsequently, a Lambda function, secured by AWS IAM, retrieves data from DynamoDB. An API Gateway is employed to create a RESTful GET API linked to the Lambda function, providing a web-app-accessible endpoint.
The final deployment is then hosted by AWS Amplify.

### 5. Technical details

As we introduced before, the parked car is detected by using a pair of infrared sensors.  
The emitter is driven up to 50mA. The detect is a NPN transistor that is biased by incoming IR light.  
In a real case scenario, a parking spot, will have easy access to power supply that we can use for powering both the LEDs and the ESP32 board.

#### Avoiding false measurements

Infrared sensors must be positioned so that they only detect the parked car and not other elements that could trigger them (birds, small animals, leaves, etc.). A good solution might be to position them just below the height where the bodywork on average ends and the windows begin.This must however be decided on the basis of the location and specific characteristics of the parking area and parking space.  
Another solution to avoid false measurements could be to take two measurements in a row a few seconds apart. In this way if something was passing through the sensors in the exact moment of the measurement, it can be detected as a false measurement.  
Another solution, but that requires more effort and expenses, is to use other sensors such as weight or proximity sensors to ensure the avoidance of false measurements.

### 6. Hands on tutorial

This section provides a hands on tutorial on how to set up and run the application.

1. Download and install [RiotOs](https://www.riot-os.org/)
2. Create a new RiotOs application and copy the code provided under the code folder.
3. Purchase all the componens listed above and connect them as in the following pucture:
   ![Network architecture](/images/Untitled%20Sketch_bb.jpg)
4.

### 7. Possible future implementations

The proposed prototype could be be improved in several ways and could include many other features.  
The one I would like to highlight in this section is the one that allows parking spots users to remotly reserve the car spot.  
To implement this improvement, there must be some mechanisms that allows only the users the reserved a parking spot to park on. For example an automatic parking barrier could be placed in corrispondance of parking spots. This barrier will activate when someone reserve the parking spot via web app and it can be deactivated only by the user that reserved it.

## Video demonstration

- [Link to the video](https://youtu.be/ZhnvmG-MbWw)

## Hackster.io article

- [Project blog entry on Hackster](https://www.hackster.io/aveklan/smart-parking-ea58aa)
