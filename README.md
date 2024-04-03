<h2 align="center">Embedded Systems Development Learning</h2>

This repository contains all the code I have written while learning about embedded systems development. I have used the Raspberry Pi 3 Model B+ as my embedded system development board, and I have used the C++ programming language to write the code for the Raspberry Pi.

## How to run
To run the code in this repository, you need to have a Raspberry Pi 3 Model B+ and a breadboard with a Red LED, Yellow LED, and a push button.

1. Clone the repository to your Raspberry Pi.
2. You will need to have wires, a breadboard, a Red LED, Yellow LED, and a push button connected to the Raspberry Pi GPIO pins. The GPIO pins that wil be open at startup are defined in the export_gpio.sh file. Edit the file [export_gpio.sh](#export_gpiosh) and [main.cpp](#maincpp) to match the GPIO pins you have connected to the Raspberry Pi.
3. Once the above is completed, run the following command to open the GPIOs, compile the code and run the program:
```bash
make init   # will open the GPIOs defined in the export_gpio.sh file, need to be done at startup only.
make run    # will compile the code and run the program
```
4. Run the following command to get more options:
```bash
make help   # will display the help menu
```

## Code Structure

#### export_gpio.sh
This file contain all the General Purposes Input/Output (GPIO) pins to be opened at startup of the raspberry pi.

#### gpio.h
This file contain the GPIO class that is used to control the GPIO pins on the raspberry pi.

#### gpio.cpp
This file contain the implementation of the GPIO class.

#### main.cpp
This file contain the main program for my study of semaphore, mutexes, polls and threads. This program contain a simple light show with a Red and Yellow LED when a physical button is pressed on the raspberry pi breadboard.