# Fish Feeder Firmware

## Description
A program that mimics a fish feeding device.The hardware being mimicked is based on a low power microcontroller
including a small 126x64 pixel OLED screen, press button, a stepper motor, and a real-time clock chip. The real hardware
would have EEPROM memory which in this program is simulated by a text file.

## Features
- Displays a screen which shows: The current time, next feed time, number of feeds since the automatic feeding schedule
  started, and the feeders current operating mode.
- Allows the user to create a feed schedule where they can specify the times the fish should be fed and also how many
  rotations the feeder should make for each time.
- Allows the user to edit a pre-existing feed schedule by selecting a time in the schedule which the user can then
  change the time and/or the number of rotations.
- Allows the user to change the time or date for the devices clock chip.
- Allows the user to choose the operating mode.
- Allows the user to skip the next feed in the schedule
- Allows the user to initiate one rotation of the feeder mechanism.
- Blanks the display after a minute of inactivity to protect the OLED display from burn-in.
- Saves the feed schedule and clock time into a text file that is reloaded when the program is run, if the file is
  present.

# Instructions for use
One button interface throughout to navigate the program using just short and long presses of the button.

## SHORT PRESSES
Use to move between options or change a selected value.
## LONG PRESSES
Use to select an option or confirm a value.

# Building the program
Only Prerequisite is that user needs a C compiler and uses the standard libraries.

## Standard Libraries
stdbool.h      -Defines boolean type providing a way to use true and false values.
stdint.h       -Provides fixed width integer types which is used to store values for the splashscreen.
stdio.h        -Provides functions for input and output allowing file manipulation and reading/writing to the console.
stdlib.h       -Defines general utility functions for memory allocation including conversions.
string.h       -Provides functions for manipulating C-style strings and memory blocks.

# Files

## displayScreens.c/h
Contains all the functions using JavaFX to control the display on the screen.

## fish.c/h
Contains functions that mimic the hardware.

## main.c
The main entry point for the program.

## menus.c/h
Contains functions that display and manage the menus.

## menusFunctions.c/h
Contains functions that are used by the menus.

## operatingMode.c/h
Defines the structs used throughout the program and their initialisers.

## programShutdown.c/h
Contains a function that mimics saving to EEPROM.

## programStartup.c/h
Contains functions that initialise the program.

## splashScreenImagePt1.c/h
Contains the first image displayed in the splash screen in the form of a 8-bit RGB array.

## splashScreenImagePt2.c/h
Contains the second image displayed in the splash screen in the form of a 8-bit RGB array.

## splashScreenImagePt3.c/h
Contains the third image displayed in the splash screen in the form of a 8-bit RGB array.

## splashScreenImagePt4.c/h
Contains the fourth image displayed in the splash screen in the form of a 8-bit RGB array.
