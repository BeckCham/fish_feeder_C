/**
* Created by Beck Chamberlain on 20/11/2024.
*
* This file provides displays for the menus and screens.
*
* The display splash screen section includes functions that together display the splash screen.
* The basic display function section includes a function that is the generic display set up.
* The main screen section includes functions that together display the main screen.
* The functions that display multiple options section includes functions that use the displayOptions function.
* The functions that display numbers section includes functions that use the setDigitsDisplay.
* The blank screen function section includes a function that is the generic blank display set up.
*
*/
#ifndef DISPLAY_SCREENS_HEADER
#define DISPLAY_SCREENS_HEADER
#include <stdint.h>
#include "operatingMode.h"

//DISPLAY SPLASH SCREEN
void displayPtOfSplashScreen(const uint8_t image[64][128][3]); //A helper function for displaying the splash screen
void displaySplashScreen(); //Displays the changing splash screen

//BASIC DISPLAY FUNCTION
void basicDisplay(); //Generic set up for a display.

//MAIN SCREEN
void displayMainScreen(operatingModeStruct *operatingMode); //Displays the main screen
void updateTimeDisplay(int *previousSecond); //Updates the time display on the main screen

//FUNCTIONS THAT DISPLAY MULTIPLE OPTIONS THAT CAN BE SCROLLED THROUGH
void displayOptions(const int optionsAmount, char options[optionsAmount][22], const int currentSelection,
                    char *topText); //Generic function for displaying options.
void displayEditCurrentSchedule(const int currentSelection, operatingModeStruct *operatingMode);

//Displays the current schedule times as a menu screen.
void displayConfigurationMenu(const int currentSelection); // Displays the configuration menu screen.
void displaySetTheClockMenu(const int currentSelection); //Displays the setting the clock menu screen.
void displayOperatingModeMenu(const int currentSelection); //Displays the operating mode menu screen.
void displayConfigFeedScheduleMenu(const int currentSelection); //Displays the configure feed schedule menu screen.

//FUNCTIONS THAT DISPLAY NUMBERS THAT CAN BE CHANGED
void displayBoarder(const int x, const int y, const int x1, const int y1); //Helper function that displays a boarder.
void setDigitsDisplay(const int charAmount, char displayedChars[charAmount][3], const int currentSelection,
                      char *bottomText, char *topText); //Generic function for displaying numbers.

//Displays a time in the '##:##:##' format.
void displaySetTheTimeScreen(const int currentSelection, int digits[6], char *bottomText);
//Displays a date in the '##/##/##' format.
void displaySetTheDateScreen(const int currentSelection, int digits[8], char *bottomText);
//Displays a time in the '##:##' format.
void displayScheduleGetTime(const int currentSelection, int digits[4], char *bottomText);

//Displays the number of rotations the user wants.
void displayScheduleGetRotations(const int currentSelection);
//Displays the number of feeds the user wants in their new schedule.
void displayScheduleGetFeedsAmount(const int currentSelection);


//THE BLANK SCREEN FUNCTION
void blankScreen(operatingModeStruct *operatingMode); //Clears the display.
#endif //DISPLAY_SCREENS_HEADER
