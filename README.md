# EENG 348: Digital Systems

This repository contains the completed assignments for EENG 348: Digital Systems. Below is an overview of each lab and the final project, detailing the work implemented and the concepts covered.

## Lab 0: Getting Started

- **Setup Arduino Software**: Installed the Arduino IDE and set up the Arduino Uno.
- **Write and Run a Program**: Created a simple sketch to print "Hello, world!" to the serial monitor using the Serial library.
- **Precautions**: Ensured proper handling of the microcontroller boards to avoid static damage and incorrect connections.

## Lab 1: Assembly Language Programming

- **Part 1: Getting Started with Assembly**: 
  - Wrote an assembly program to add two 8-bit integers and return a 16-bit result.
- **Part 2: Procedures**:
  - Implemented `sumval` and `diffval` functions in assembly, respecting calling conventions.
  - Combined their results in the main assembly program.
- **Part 3: Recursion**:
  - Implemented a recursive function to compute the nth Fibonacci number using assembly.
  - Displayed the result on the serial monitor.

## Lab 2: Digital I/O

- **Part 1: Digital Input by Polling**:
  - Built a circuit with a push-button switch and configured it as an input with a passive pull-up.
  - Implemented a polling-based approach to read the switch state and debounced the switch.
- **Part 2: Digital Output: Dot-Matrix LED Display**:
  - Wired up a 5x7 LED matrix using shift registers.
  - Implemented a program to cycle through digits 0 to 9 on the display.
- **Part 3: Counter and Interrupts**:
  - Combined the previous circuit with interrupts to read the switch state.
  - Used interrupts to increment a count displayed on the LED matrix.

## Lab 3: Analog I/O and Digital I/O Protocols

- **Part 1: Analog Voltage Input: Photocell**:
  - Created a voltage divider with a photocell and read the analog value using `analogRead()`.
  - Converted the analog voltage to an integer value and displayed it on the serial monitor while controlling an RGB LED's brightness.
- **Part 2: Time-encoded Analog Value: Ultrasonic Range Finder**:
  - Used the range finder to measure distance and converted it to an integer value.
  - Used this value to change the color of the RGB LED.
- **Part 3: SPI Display**:
  - Controlled an OLED display using a rotary encoder to adjust the velocity of a bouncing ball animation.
  - Implemented communication with the display using SPI protocols.

## Lab 4: Implementing Concurrency

- **Part 1: Concurrency**:
  - Implemented process management on the Arduino platform, creating and scheduling processes.
  - Developed a system to maintain and switch between multiple processes using timer interrupts.
- **Part 2: Locks**:
  - Implemented lock mechanisms to manage resource access between processes.
  - Ensured processes could acquire and release locks, blocking when necessary and yielding control appropriately.

## Final Project: Braitenberg Vehicles

Implemented autonomous robots inspired by Braitenberg vehicles, exhibiting different behaviors based on light input.

- **Love**: Moves towards light by wiring sensors to motors to steer towards the light source.
- **Fear**: Moves away from light by reversing the sensor-motor wiring.
- **Aggression**: Drives faster towards stronger light sources.
- **Coward**: Moves away more quickly from stronger light sources.

Additional features:
- **Behavior Switching**: Used a button to switch between different behaviors.
- **Advanced Interactions**: Incorporated additional components like OLED displays and buzzers to enhance the robots' behaviors.
