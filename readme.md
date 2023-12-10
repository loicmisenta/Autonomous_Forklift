---
title: Autonomous forklift
author: Loïc Misenta, Hadrien Saigot Forest, Inés Araujo Cañas, Athena Papageorgiou Koufidou, Kalil Sama Bouzigues, Dylan Vairoli
---

# Introduction
This project was developed for the CS-356 Making Intelligent Things course at EPFL. The team members are Loïc Misenta, Hadrien Saigot Forest, Inés Araujo Cañas, Athena Papageorgiou Koufidou, Kalil Sama Bouzigues and Dylan Vairoli.

# Overview
The autonomous forklift is a project aimed at automating the movement of packets within a facility. When placing a packet in front of the forklift, it uses a camera to read and process a QR code on the packet. The QR code contains information about the packet's intended location in the facility. The forklift then picks it up and moves in a to the designated location and places the packet on the ground. Upon completion, the forklift returns to its starting position, ready to repeat the process with another packet. Manual controls are also available with a gamepad or a web application.


<!-- add video -->
<video src="report/images/AutonomousForkLiftMIT.mp4" controls="controls" style="max-height: 550px;">
</video>

# Materials


### Microcontrollers
- [ESP32-CAM-MB, USB-to-serial interface for ESP32-CAM](https://www.berrybase.de/en/esp32-cam-mb-usb-zu-seriell-schnittstelle-fuer-esp32-cam)
- [ESP32-CAM Development Board incl. OV2640 camera module](https://www.dfrobot.com/product-1879.html)
- [ESP8266](https://en.wikipedia.org/wiki/ESP8266)
- [Arduino Uno](https://en.wikipedia.org/wiki/Arduino_Uno)

### Motors
- [5 NEMA17-04 stepper motors](https://www.3dware.ch/NEMA-17-Stepper-motor-1.8-degree,-1.2A-En.htm)
- [5 stepper drivers](https://www.fruugoschweiz.com/5pcs-a4988-stepstick-stepper-motor-driver-module-with-heat-sink-for-3d-printer-reprap-suitable/p-117051283-245844502?language=en&ac=ProductCasterAPI&asc=pmax)
- [Arduino Uno CNC shield](https://www.bastelgarage.ch/cnc-shield-a4988-arduino-fur-3d-drucker-und-frasmaschinen-grbl)

### Electronics
- PCB board
- [2 end switches]("https://www.spplotter.com.br/micro-switch-fim-de-curso-omron-ss-5gl)
- jumper cables

### Power supply
- Conrad Energy Lipo battery 11.1V 5000mAh
- 5V to 10V battery

### Construction
- [Machifit 15pcs 400mm Optical Axis Guide Bearing Housings Linear Rail Shaft Support Screws Set](https://www.fruugoschweiz.com/15pcsset-400mm-optical-guide-bearing-axis-screws-linear-guide-rail-shaft-support-set/p-163505313-347691854?language=en&ac=ProductCasterAPI&asc=pmax)
- 3d printer filament
- MDF
- screws

# Tools
For this project, we used a Prusa 3D printer and a laser cutter, both available at [The SPOT](https://www.epfl.ch/education/educational-initiatives/discovery-learning-program-2/prototyping/page-111858-en-html/) at EPFL. The 3D printer was used to print the wheels and structures for cable and electronic components management, while the laser cutter was used to create the base platform, boxes and forks. Other tools needed for this project include a soldering iron, a multimeter, a screwdriver and other basic tools.
The software you are going to need is the Arduino IDE and a text editor of your choice.
<!-- List of required tools would go here, e.g. laser cutter, 3d printer etc., perhaps also a note about were to obtain them or where to find access to them -->

# Safety
It is important to be aware of potential risks associated with the tools and processes involved. The use of tools such as 3D printers, laser cutters, soldering irons, and other equipment can pose certain hazards. When operating these tools, there is a risk of burns, electric shocks, or injuries from moving parts. To mitigate these risks, it is crucial to follow safety guidelines provided by the tool manufacturers and wear appropriate personal protective equipment (PPE) such as safety glasses, gloves, and closed-toe shoes. Ensure that the work area is well-ventilated and free from clutter, providing a safe environment for operating tools. Take precautions when handling hot or sharp objects, and avoid touching heated surfaces directly. Additionally, exercise caution when working with electrical components or circuits, ensuring proper insulation, grounding, and adherence to safe soldering practices. Regularly inspect and maintain the tools and equipment to ensure they are in good working condition. 


# Construction

<figure>
  <img src="report/images/forklift.png" alt="my alt text"/>
  <figcaption>Initial idea of the forlift, made with Blender</figcaption>
</figure>

<figure>
  <img src="report/images/forklift_preview.png" alt="my alt text"/>
  <figcaption>Final forklift design.</figcaption>
</figure>


We decided to go with MDF as the primary material for our forklift. It is quite cheap and easily modifiable, while we were prototyping we could quickly drill some new holes and so on.

## Base

<figure>
  <img src="report/images/base_preview.png" alt="my alt text"/>
  <figcaption>Base of the forklift body.</figcaption>
</figure>

We took the base from the [Arduino Mecanum Wheels Robot](https://howtomechatronics.com/projects/arduino-mecanum-wheels-robot/) project by [Dejan](https://howtomechatronics.com/author/howtom12_wp/) that we slightly adapted to use comb joints and have some more holes so that we can glue parts together and pass cables through it.

You can find the `.dxf` file for this at [`Modeling/Body/base.dxf`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/base.dxf)

## Side supports

<figure>
  <img src="report/images/side_supports_preview.png" alt="my alt text"/>
  <figcaption>Side supports of the forklift.</figcaption>
</figure>

We designed some side supports to attach the two side shafts on which the forks are drilled. They are quite massive but it was giving a nice raw look to the forklift.

You can find the `.dxf` file for this at [`Modeling/Body/side_supports.dxf`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/side_supports.dxf)

## Cap

<figure>
  <img src="report/images/cap_preview.png" alt="my alt text"/>
  <figcaption>Cap of the forklift.</figcaption>
</figure>

Also using this comb joints technique, we added a cap that can easily be removed on top of the base platform. It gives a polished and refined aspect to our vehicle by hiding all the cables.

You can find the `.dxf` file for this at [`Modeling/Body/cap.dxf`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/cap.dxf).

## Forks

<figure>
  <img src="report/images/forks_preview.png" alt="my alt text"/>
  <figcaption>Forks of the forklift.</figcaption>
</figure>

Obviously, we also had to create the forks that are used to take the boxes. Our first try was a failure because it was too thin and couldn't lift anything without breaking. This final model works better but it still needs brackets to support at least two kilograms. We 3D-printed them because we couldn't get any metal one, but 10x10mm metal brackets are enough.

You can find the `.dxf` file for this at [`Modeling/Body/forks.dxf`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/forks.dxf). You can also find the `.stl`file for the brackets at [`Modeling/Body/bracket.stl`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/Bracket.stl).

## Fixation panels

<figure>
  <img src="report/images/fixation_panels_preview.png" alt="my alt text"/>
  <figcaption>Fixation panels of the forklift.</figcaption>
</figure>

Finally, we added some MDF fixation panels to attach our components such as the vertical stepper motor and the three shafts.

You can find the `.dxf` file for this at [`Modeling/Body/fixation_panels.dxf`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/fixation_panels.dxf).

## Fixations


<figure>
  <img src="report/images/fixations_preview.png" alt="my alt text"/>
  <figcaption>Fixations of the forklift.</figcaption>
</figure>

Along the way, we encountered small issues for which we had to design some small 3D-printed pieces
- To hold the two side shafts
- To hold the central screw shaft
- To attach the forks to the block bearings
- To keep the vertical end switch at the right position
- To avoid that the side shafts get out of the fixations when lifting up. Because our block bearings didn't slide perfectly smoothly along our shafts, we needed some caps to force them to stay inside the fixations
- To hold the ESP32-CAM against the forks as well as the horizontal end switch
  
You can find the `.stl` files for these pieces at [`Modeling/Fixations/`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Modeling/Fixations).

## Cable management
To keep our circuit clean and avoid short circuits because of floating components, we designed structures to cleanly manage our cables. 

- One that is used to wrap the cables around
- One to screw our PCB board to the base
- One to attach the ESP8266 to the base

However all of these pieces are very specific to our usecase so it may not suit perfectly your components.
  
You can find the `.stl` files for these pieces at [`Modeling/Cable Management/`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Modeling/Cable%20Management).

## Counterweight box

<figure>
  <img src="report/images/counterweight_box_preview.png" alt="my alt text"/>
  <figcaption>Counterweight box of the forklift.</figcaption>
</figure>

Any forklift needs some counterweight at the back of it to balance with the weight it is carrying. To do so, we opted for a 3D-printed box with round edges so that nothing gets stuck in them. You can then fill it with whatever you want, rocks or sand for example.

You can find the `.stl` files for this at [`Modeling/Body/counterweight_box.stl`](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Modeling/Body/counterweight_box.stl).

## Mecanum wheels

<figure>
  <img src="report/images/mecanum_wheels_diagram.png" alt="my alt text"/>
  <figcaption>Mecanum wheels  combinations diagram (<a href="https://www.researchgate.net/figure/Fig-5-Top-view-of-turning-principle-of-Mecanum-wheel_fig2_307868549">source</a>).</figcaption>
</figure>

Mecanum wheels allow you to move sideways, turn in place as well as go diagonally with only 4 independent wheels. By turning the wheels with a specific combination of directions, you can achieve the desired effect.

We used the [Arduino Mecanum Wheels Robot](https://howtomechatronics.com/projects/arduino-mecanum-wheels-robot/) project as a basis for our wheels. You can either download the `.stl` files from his tutorial or you can find them under [Modeling/Wheels](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Modeling/Wheels). Pay attention to print two right wheels and two left wheels. For more details about the mounting of these wheels, you can check [Dejan's tutorial](https://howtomechatronics.com/projects/arduino-mecanum-wheels-robot/).


> Note that we had to slightly modify the outer part of the wheels because we did not have any M3 bolts that were 45mm long, only 40mm.

# Motors and electronics

## A4988 stepper driver
We used A4988 stepper drivers to control our steppers. However we need to limit the output current they produce to 1.5A. To do so we need to adjust the small screw on top of them. Follow the _A4988 Current Limiting_ section of [Stepper Motors and Arduino – The Ultimate Guide by Dejan](https://howtomechatronics.com/tutorials/arduino/stepper-motors-and-arduino-the-ultimate-guide/).

## CNC Shield and Arduino UNO

<figure>
  <img src="report/images/circuits/cnc_arduino/cnc_shield_arduino_uno.png" alt="my alt text"/>
  <figcaption>CNC shield and drivers on top of an arduino UNO (<a href="https://grabcad.com/library/arduino-uno-cnc-shield-1">source</a>).</figcaption>
</figure>

<figure>
  <img src="report/images/circuits/cnc_arduino/cnc_shield_a.png" alt="my alt text"/>
  <figcaption>CNC shield closeup (<a href="https://forum.arduino.cc/t/arduino-uno-cnc-shield-controlling-4-stepper-motors/913742/3">source</a>).</figcaption>
</figure>

The CNC shield is very simple to use, you just need to plug it on top of the Arduino UNO and install the drivers on top of it.

However be careful with a few points:

- The `Enable` pin of the drivers should be at the position marked as `EN` on the shield.
- As we use the 4 steppers independently, we need to indicate that the `A` (red) driver is controlled on its own. This is done by connecting a jumper at the green and orange spots in the image above. ([source](https://www.zyltech.com/arduino-cnc-shield-instructions/)).
- For smoother movement, we decided to use 16x microstepping. This is not mandatory but it works best with the mecanum wheels in our opinion. To do so, you need to connect jumper wires on all the `M0`, `M1` and `M2` pins. You can find more information about other microstepping resolutions [here](https://www.zyltech.com/arduino-cnc-shield-instructions/).
- When plugging in the motors, they might turn in the incorrect direction or you might have plugged steppers in different locations as us. You can test this by uploading the `steppers_test.ino` sketch to the Arduino UNO. It should start turning the steppers clockwise in this order: back left, front left, back right, front right. If they turn counter-clockwise instead, you need to turn their cables by 180 degrees. If they turn in an incorrect order, try to switch the positions where they're plugged in the shield accordingly. 
- You can find the mapping between the Arduino UNO pins number and the actual controls of the shield in the `arduino_main.ino` sketch.

## ESP8266
Unfortunately the Arduino UNO we used does not have WIFI capabilities. As we needed to transfer instructions from the computer using WIFI to our boards, we used an ESP8266. We can consider it as the brain of our circuit, it receives the instructions from the computer online and dispatch it either to the Arduino UNO to move the wheels or to itself to control the vertical stepper. As we just mentioned, it is also responsible to control the vertical stepper motor because the Arduino UNO had all of its pins busy with the shield.

## ESP32-CAM
To scan QR codes we needed a camera, the ESP32-cam was perfect for that as it also has WIFI capabilities. When reading a QR code, it sends its payload (which is simply a number) to the computer which then send the correct instruction to the ESP8266 based on the number it received.

## NEMA 17 stepper motors
We use 5 NEMA17 stepper motors; 4 for the wheels and one for the forks. They are strong motors capable of lifting up our boxes. You can find more information about how stepper motors work [here](https://howtomechatronics.com/how-it-works/electrical-engineering/stepper-motor/)

## Conrad Energy Lipo battery 11.1V 5000mAh
As a power supply for our steppers and the Arduino UNO, we used a 12V Lipo battery.

## 5V to 10V battery
You will also need any kind of 5V to 10V power source for the ESP8266 because it can't take the 12V of the Lipo battery.


## End switches

<figure>
  <img src="report/images/end_switch.png" alt="my alt text"/>
  <figcaption>Microswitch Omron SS 5GL (<a href="https://www.spplotter.com.br/micro-switch-fim-de-curso-omron-ss-5gl">source</a>).</figcaption>
</figure>

We used two end switches in our project. The first one is used to know when the forks are at the bottom and we should stop, the other one is used to know when a box is entirely placed on the forks.

# Circuits

## CNC shield
- Connect the stepper cables to the blue area in the above schema.
- Add jumpers between the two green pins and between the two orange pins to control the A slot as well.
- Add jumpers between the `M0`, `M1` and `M2` (red) pins to enable 16x microstepping.
- Connect a 12V power source to the purple area above and to the Arduino UNO.

## ESP8266
- Connect a 5V to 10V power source to the `VIN` and `GND` pins of the ESP8266.

## ESP32
- Connect the `5V` and the `GND` pins of the ESP32 to the `5V` and `GND` pins (pink) of the CNC shield. 

## ESP8266 – Arduino UNO serial communication
- Connect the `D4` and the `GND` pins of the ESP8266 to the `RX` and `GND` pins (yellow) of the CNC shield.

## End switches
- Connect one of the extremity pins of the two end switches to the `3V` pin of the ESP8266.
- Connect the middle pin of the vertical end switch to the `D5` pin of the ESP8266.
- Connect the middle pin of the horizontal end switch to the `D1` pin of the ESP8266.
- Connect the `GND` pin of the ESP8266 to the middle pins of the end switches through a 10k$\Omega$ resistor.

# Software

<figure>
  <img src="report/images/software_diagram.png" alt="my alt text"/>
  <figcaption>Software diagram.</figcaption>
</figure>

## Arduino UNO
The Arduino UNO is only responsible to move the wheels of the forklift. It receives instructions from the ESP8266 about where to go using serial communication. We used the `AccelStepper` library to control our steppers.

You can find Arduino-related code at [Code/arduino-main](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Code/arduino-main).

## ESP8266
The ESP8266 is the brain of our forklift. It receives instruction from the computer using WIFI and dispatch them correctly to the Arduino UNO if needed, to itself otherwise. It keeps track of the positions of our grid and the current state of the forklift. It also controls the vertical stepper motor.

You can find ESP8266-related code at [Code/esp8266-main](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Code/esp8266-main).

## Web clients
To send instructions to the ESP8266's server, we need a web client running on the computer. 

### Web application client
The first one can send intructions by clicking on buttons in a custom UI. You can tell the forklift to go in a particular direction or to pickup a box and to put it at a specific position. It can be found at [Code/webapp](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/Code/web-clients/webapp).

<figure>
  <img src="report/images/webapp.png" alt="my alt text"/>
  <figcaption>Web interface to control the forklift.</figcaption>
</figure>

### Gamepad client
The second one controls the forklift with a gamepad connected to the computer. Simply open [Code/web-clients/gamepad_client.html](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Code/web-clients/gamepad_client.html), if a gamepad is detected it will be displayed. Enter the correct IP for your ESP8266 and make sure that it is connected to the same WIFI as your computer. You can change WIFI credentials in [Code/esp8266-main/esp8266-main.ino](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/Code/esp8266-main/esp8266-main.ino). 


<figure>
  <img src="report/images/gamepad_app.png" alt="my alt text"/>
  <figcaption>Gamepad web controller</figcaption>
</figure>

### QR code recognition
We can also send instructions to the ESP8266 by scanning a QR code. The QR code should contain a number between 1 and 6. The ESP8266 will then move the forklift to the corresponding position in the facility. You can find the code for this at [QR_Code](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/tree/main/QR_Code).

The QR code handling is a big part of the project. The way it works is the following. The ESP32-
CAM scan the QR Code present on the box, send the id number of the box to the computer 
which has a .csv file with all the data that corresponds to the box, then send the placement of 
the box to the ESP8266 that can after move appropriately given the information that was in the 
.csv file. The ESP8266 is also responsible for telling the ESP32-CAM to start scanning QR code.

<figure>
  <img src="report/images/Capture.png" alt=""/>
  <figcaption>QR code recognition</figcaption>
</figure>

### Implementation
There is 3 files : [QR_CODE_CSV_2.ino](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/QR_Code/QR_CODE_CSV_2/QR_CODE_CSV_2.ino), [data.csv](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/QR_Code/QR_CODE_CSV_2/data.csv), [server.js](https://gitlab.epfl.ch/papageor/cs-356-autonomous-forklift/-/blob/main/QR_Code/QR_CODE_CSV_2/server.js).

#### QR_CODE_CSV_2.ino
In the Arduino file there is multiple sections.  
The first section is the configuration of the server of the ESP32, the connection to the server 
of the computer, the initialisation of camera and the handling of HTTP request coming from the 
ESP8266. 
The second one is the handling of the QR code scan. In this section the scanning will begin 
only if the ESP8266 want a scan. If so it will search for a QR code and if it finds one, it send the 
id found by an HTTP request to the server running on the computer. If there is no error a flash 
will be done to have a visual confirmation that the id was correctly sent. 
The third part is where the server, the client, the camera are all setup correctly to handle all 
situation

#### data.csv
This file contains a table of 3 columns : id, type and description. The id is used to know what 
box it is and where it is going, the type and the description are for the display on the webpage 
to know what is inside. 

#### server.js
This is where the local server on the computer is setup. When it received an http request with 
the id, it will scan each row of the csv file to get all the information needed to send to ESP8266.

### Problems encountered
The main problem that I have encountered is the scanning of the QR Code. I spent a lot of time 
finding and understanding the libraries for scanning. There were many of them so I have to tried 
many to find the best. In this project we wanted to scan quick, also the main question was : 
“What do I put in my QR Code ?”. At first I tried to put a JSON text but the camera detect it very 
slowly so I tried to find some compression algorithms like “shoco” which compress little string 
but not efficient. Finally I found a way to have only a little information on the QR, an id, and all 
the rest on a csv file on the computer. But to do this I have to learn JavaScript and how to create 
a server, with Node in my case.  
Another problem was the voltage of the ESP32-CAM. This one randomly give an infinite loop of 
error misunderstanding. At first I was thinking it was my code, so I spent few days to recode all. 
After a deep search I found that this ESP32-CAM needs 5V and not 3V when it used a lot of 
resources, like QR Code scanning. But I was using my ESP8266 to send, and decode code so 
it was relies with the 3V of the ESP8266. 
I wrote these line for you to know where I spent my time and to help others students in the 
future to not spending weeks and weeks to debug.

## Navigation
There are several navigation options that can be used to guide the vehicle to the correct path. Since the forklift will be moving in a known facility, we decided to use a form of graph-based navigation, i.e. to divide the facility into a grid of possible positions or nodes, with edges connecting the nodes representing possible paths that the forklift can take. The forklift can use this graph to find the shortest path to a destination node based on its current position and the instructions associated with each node. This approach is simple and easy to implement, as it does not require complex sensors or computer vision algorithms and efficient, as the graph can be precomputed and stored, making the navigation process faster. 

This form of navigation proved to be sufficient for our use case, as the forklift was able to reach its destination with accuracy and without any issues. However, it is not the most robust solution, as it does not account for obstacles or other dynamic changes in the environment. For example, if a box is placed in the middle of the path, the forklift will not be able to detect it and will continue to move forward, potentially colliding with the box. To address this issue, we could use a combination of graph-based navigation and computer vision, where the forklift can use the graph to find the shortest path to the destination and use computer vision to detect obstacles and avoid collisions. This approach would be more robust, as it would allow the forklift to adapt to changes in the environment and avoid obstacles. However, it would also be more complex to implement, as it would require more sensors and computer vision algorithms.

Another possible implementation was using a map where each position are represented as 2D vectors, the x-component is the horizontal distance to travel for the forklift to reach the box in that position and the y-component is the vertical distance to travel from the starting point to where the box stands in that given position. This way we can make any large number of boxes and the map will be made accordingly using a function with a for-loop. The thing is that, in real life the wheels slip a little§ bit on the floor, and even if each box are spaced by a certain equal distance, the vertical distance to each box are not exactly multiple of the vertical step. In that way, we had to recalibrate those steps with some arbtrary coefficient and measure every vertical distance to each position. 


# Conclusion
This project was a great opportunity to learn about the different aspects of robotics, from the mechanical design and construction of the forklift to the electronics and software implementation. We were able to apply the concepts learned in class to a real-world application, which allowed us to gain a better understanding of the material. We also learned how to use new tools and technologies, such as Fusion 360, Arduino, and ESP8266, which will be useful for future projects. Overall, we are satisfied with the results of the project, as we were able to successfully implement the forklift and achieve our goal of automating the movement of packets within a facility.
