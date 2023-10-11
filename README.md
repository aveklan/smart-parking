# Smart Parking

## Avesani Davide 2060112

[<img src="/images/linkedin.png" width="64" height="64">](https://www.linkedin.com/in/davide-avesani-394835223/)

### IoT Individual Project

### 1. Introduction

Finding parking spots in traditional parking areas can be time-consuming and frustrating, often resulting in wasted time and increased traffic congestion. Smart parking tunnel solution uses sensors to detect available spaces and communicates that information to a web app. In this way, drivers can quickly and easily locate available parking spots. Moreover, with the ability to access the data through the web app, drivers can plan their parking before they arrive, saving time and reducing stress.

### 2. Prototype design

The prototype is designed to perform to main tasks:

- Detect if a car is parked inside a parking spot;
- Allow users to reach this information

The first task is carried out using a number of laser sensors, positioned in pairs facing eacnh other. If a car is parked in between the pairs, the laser beam is interrupted and the car is detected.  
The second, instead, is done either by collecting real-time data and sending it to the web app and by activating some LEDs, positioned over each parking spot, to inform users inside the parking area about the status of the parking spot.  
Finally, the ESP32 display will be used to simulate a screen which shows a parking map with the free and the occupied parking spots. This screen can be in a second time replicated and positioned in strategic areas to inform the users regarding the status of the parking.  
The following picture shows an high level schema about the idea described above.

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

### 5. Considertations and evaluation

As we introduced before, the parked car is detected by using a pair of infrared sensors.
The emitter is driven up to 50mA. The detect is a NPN transistor that is biased by incoming IR light.
The overall system energy consuption will highly depends on the adopted sensors and lights. However, in a real case scenario, each parking spot should have easy access to power supply that can be used for powering the LEDs, the the sensors and the ESP32 board.

#### Avoiding false measurements

Infrared sensors must be positioned so that they only detect the parked car and not other elements that could trigger them (birds, small animals, leaves, etc.). A good solution might be to position them just below the height where the bodywork on average ends and the windows begin. This must however be decided on the basis of the location and specific characteristics of the parking area and parking space.  
A second solution to avoid false measurements is be to take two measurements in a row a few seconds apart. In this way if something was passing through the sensors in the exact moment of the measurement, it can be detected as a false measurement.  
A final solution solution, but that requires more effort and expenses, is to use other sensors such as weight or proximity sensors to ensure the avoidance of false measurements.

### 6. Hands on tutorial

This section provides a hands on tutorial on how to set up and run the application.

1. Download and install [RiotOs](https://www.riot-os.org/)
2. Create a new RiotOs application and copy the code provided under the code folder.
3. Create and set up your AWS account
4. Purchase all the componens listed above and connect them as in the following pucture:

   ![Network architecture](/images/Untitled%20Sketch_bb.jpg)

5. Download, install and configure Mosquitto.
   - Edit mosquitto.conf configuration file using this settings:  
      ° allow_anonymous true  
      ° listener 1883
6. AWS IoT core
   - [ Connect a new thing to AWS IoT core](https://eu-west-3.console.aws.amazon.com/iot/home?region=eu-west-3#/home)
   - Access your thing under "Things" and go to the "Certificates" section. Attach the policy located in the SDK folder you downloaded earlier. Modify the active version by adding specific lines in the JSON document.
   - In the message routing section, create a new rule named "smartparking" and set it up to select data from 'smartparking/data' and route it to DynamoDBv2.
     Create a new DynamoDB table called "smartparking" with a "timestamp" partition key.
     Associate this table with the rule and create a new IAM role.  
     Complete the rule creation.
7. AWS Lambda function
   - Create a new Python Lambda function named "getSmartParkingData" and paste the Python script provided under the code folder.
   - Save your changes and deploy the function.
   - Configure the Lambda function to access the DynamoDB table by attaching the appropriate policy.
8. AWS Apy Gateway
   - Set up a new REST API named "smartParkingInfo"
   - Create a "GET" method under "Options" in the resource section and provide the ARN of your Lambda function as requested.
   - Enable CORS for the API and deploy the API
9. AWS amplify
   - Create a new app named "smartBinApp" in AWS Amplify.Rename the environment to "dev" and upload the web application folder of your project.

#### How to run the code

Once all the previous steps are performed and AWS is setted up, use this command to flash the ESP32 and run the code:

- _sudo BOARD=esp32s3-devkit BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make all flash term_

### 7. Possible future implementations

The proposed prototype could be be improved in several ways and could include many other features.  
The one I would like to highlight in this section is the one that allows parking spots users to remotly reserve the car spot.  
To implement this improvement, there must be some mechanisms that permits only the users which reserved a parking spot to park on it. For example an automatic parking barrier could be placed in corrispondance of parking spots. This barrier will be activated when someone reserve the parking spot via web app and it wil be deactivated by the user that reserved it, always through the web app.

## Video demonstration

- [Link to the video](https://youtu.be/ZhnvmG-MbWw)

## Hackster.io article

- [Project blog entry on Hackster](https://www.hackster.io/aveklan/smart-parking-ea58aa)
