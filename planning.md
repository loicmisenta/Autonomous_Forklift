# Current status
- The ESP32 can read QR Code information (that is encoded) and send the information (that is decoded) on the webserver
- The platform is able to move in any direction using the mecanum wheels.
- The fork assembly is attached to the base platform.
- Some boxes to test the fork assembly have been created.

# Planning
## Weeks 10 and 11

At the end of week 11, we should have a working prototype of the forklift that can move around the facility and place boxes to the requested location. The forklift will be able to navigate the facility using a grid system and communicate with a server to receive instructions. The forklift will be able to detect the location of the boxes using a camera and QR codes.

### Navigation: (Athena)
- Implement the algorithm for navigating the grid.
- Write code to improve the accuracy of the navigation algorithm by testing and fine-tuning values.

### Communication: (Dylan and Loic)
- Establish communication between ESP32 and Arduino Uno by writing code for both devices to send and receive data and figuring out the electronics needed (Arduino UNO doesn't have wifi).
- Run a server to test the communication between the devices by sending different types of data.
- Develop a plan for adding the camera, including designing the circuit and writing the code to transmit data from ESP32 to ESP8266 to Arduino.

### Boxes and Facility:(Hadrien)
- Redesign the boxes to be compatible with the new forks by analyzing the dimensions and determining what changes are necessary.
- Create a design for the facility in which the forklift will move by considering the size of the space, obstacles, and the location of the boxes. (we need a floor!!!)

### Forks (Ines and Hadrien and Kalil):
- Ensure that the fork assembly can move vertically to the requested height by designing and implementing a motor control system.
- Add end-switches to the forks to recalibrate the motor at the bottom and detect when the forks are at the bottom of a box by designing and implementing a circuit and code to interface with the switches.

### Camera and QR code (Loic)
- Create a database based on ID  and location of the boxes. The QR code will will contain th eID of the box
- Make the webserver able to send the location of the box to the forklift

### Documentation (all)
- Create documentation for all components of the system, including the code, hardware, and design files.
- Ensure that the documentation is clear, concise, and easy to understand.

### Testing (all)
- Conduct comprehensive testing of the entire system, including navigation, communication, and fork operation.
- Identify and fix any issues discovered during testing.

## Weeks 12 and 14

The schedule for the last weeks will depend on the progress made during the first two weeks. If the forklift is not fully functional by the end of week 11, we will continue working on the tasks from the previous weeks. If the forklift is fully functional, we will work on the following extensions:

- The forklift should be able to stack boxes on top of each other.
- The forklift should be able to pick up boxes from the self and bring them back to the starting point.