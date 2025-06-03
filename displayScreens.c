/**
* Created by Beck Chamberlain on 20/11/2024.
*
* Display functions that are used by the menus and screens.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menusFunctions.h"
#include "splashScreenImagePt1.h"
#include "displayScreens.h"
#include "fish.h"
#include "splashScreenImagePt2.h"
#include "splashScreenImagePt3.h"
#include "splashScreenImagePt4.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define LINE_BUFFER 22
//DISPLAY SPLASH SCREEN
/**
 * Displays an image made of pixels by taking a 3D array of 8 Bit values and converting them into hex.
 * This makes them compatible with the displayColour function.
 * Displays the pixel using displayLine because the displayPixel function because the colour can be chosen.
 *
 * @param image An array of values making up an image in 8-bit RGB..
 */
void displayPtOfSplashScreen(const uint8_t image[64][128][3]) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {

            uint8_t red = image[y][x][0];   // Gets the red part
            uint8_t green = image[y][x][1]; // Gets the green part
            uint8_t blue = image[y][x][2];  // Gets the blue part

            // Format into a hex string
            char colourInHex[8]; // Char will hold color in a #RRGGBB format
            snprintf(colourInHex, 8, "#%02X%02X%02X", red, green, blue);

            // Display the pixel at (x, y) with the selected colour.
            displayColour(colourInHex, "50cae0"); // Default but functionally useless background colour.
            displayLine(x, y, x, y); // Displays pixel with the given colour.
        }
    }
}

/**
 * Displays all the parts of the splash screen one after with 2500 milliseconds between each frame, to create a moving image affect.
 * This splash screen resembles two fish in a circular swimming pattern. This resembles a loading circle.
 */
void displaySplashScreen() {
    displayPtOfSplashScreen(splashScreenPt1);
    msleep(2500L);
    displayPtOfSplashScreen(splashScreenPt2);
    msleep(2500L);
    displayPtOfSplashScreen(splashScreenPt3);
    msleep(2500L);
    displayPtOfSplashScreen(splashScreenPt4);
    msleep(2500L);

}

//BASIC DISPLAY
/**
 * A generic function called at the start of display functions to ensure they are working on a 'blank canvas' as such.
 * It clears the display and sets the background colour as a white blue and the foreground colour as white. Which is the
 * default colours for this program.
 */
void basicDisplay() {
    displayColour("white", "50cae0");
    displayClear();
}

//MAIN SCREEN
/**
 * Display function for the main screen. The systems time and date will be displayed. This is accurate to the second.
 * This works in conjunction with displayMainScreen to make the main screen display.
 *
 * @param previousSecond The last second that was saved to this variable.
 */
void updateTimeDisplay(int *previousSecond) {
    // When the seconds value changes this displays the updated time and date
    const int second = clockSecond();
    if (second != *previousSecond) {
        *previousSecond = second;
        char time[22]; //Holds the time and date in a char format.
        snprintf(time, 22, "%02i/%02i/%04i  %02i:%02i:%02i", clockDay(), clockMonth(), clockYear(), clockHour(),
                 clockMinute(), clockSecond());
        int xCoOrdinates = (SCREEN_WIDTH - CHAR_WIDTH * 20) / 2; // Center x-coordinate
        displayText(xCoOrdinates, SCREEN_HEIGHT - CHAR_HEIGHT * 1.5, time, 1);
    }
}

/**
 * Display function for the main screen. The programs name will be displayed and information about the operating mode will be displayed.
 * The information that will be displayed: what the current mode is, when the next feed will be, and how many auto feeds have been done.
 * This works in conjunction with updateTimeDisplay to make the main screen display.
 *
 * @param operatingMode The operating mode where the information displayed will come from.
 */
void displayMainScreen(operatingModeStruct *operatingMode) {
    basicDisplay();
    char operatingModeType[LINE_BUFFER]; //Will hold the current mode information in a char format.
    char nextFeedTime[LINE_BUFFER]; //Will hold the next feed time in a char format.
    char autoFeedsDoneNumber[LINE_BUFFER]; //Will hold how many feeds have been done automatically in a char format
    char *title = "Fish Feeder";
    int xCoOrdinates = (SCREEN_WIDTH - (11 * CHAR_WIDTH)) / 2; // Center x-coordinate for the title.
    displayText(xCoOrdinates, 5, title, 1); //Display the title.
    displayLine(0,CHAR_HEIGHT * 2 + 1,SCREEN_WIDTH,CHAR_HEIGHT * 2 + 1); //Displays a line under title.
    int nextFeedTimeIndex = operatingMode->nextFeed; //A local int holding index of the next feed time.
    if (operatingMode->mode == 0) {
        //If the operating mode is on automatic mode.
        strcpy(operatingModeType, "Current mode: Auto");
        snprintf(nextFeedTime, LINE_BUFFER, "Next feed time: %02d:%02d",
                 operatingMode->feedTimes[nextFeedTimeIndex].hour, operatingMode->feedTimes[nextFeedTimeIndex].minute);
    } else {
        //If the operating mode is on paused mode.
        strcpy(operatingModeType, "Current mode: Paused");
        snprintf(nextFeedTime, LINE_BUFFER, "Next feed time: N/A");
    }
    //Can only show 3 characters for the auto feeds so a measure is taken when increasing number of automatic feeds so it resets the counter at 999.
    snprintf(autoFeedsDoneNumber, LINE_BUFFER, "Auto feeds done: %d", operatingMode->autoFeedsDone);
    //Displays all the information.
    displayText(1,CHAR_HEIGHT * 2.5 + 1, operatingModeType, 1);
    displayText(1,CHAR_HEIGHT * 3.5 + 1, autoFeedsDoneNumber, 1);
    displayText(1,CHAR_HEIGHT * 4.5 + 1, nextFeedTime, 1);

    // display graphic lines which will be around the time/date
    displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
    displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT * 2 - 1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT * 2 - 1);
}

//FUNCTIONS THAT DISPLAY MULTIPLE OPTIONS THAT CAN BE SCROLLED THROUGH
/**
 * A generic function that displays a menus options. If the menu has more than 4 options then it allows the user to
 * "Scroll" through the options. As well as this it displays a line and text at the top of the display. The current selection is always highlighted.
 *
 * @param optionsAmount How many options the menu has.
 * @param options The options the menu has.
 * @param currentSelection Which option is currently selected.
 * @param topText The menu's title.
 */
void displayOptions(const int optionsAmount, char options[optionsAmount][LINE_BUFFER], const int currentSelection,
                    char *topText) {
    int startIndex;
    basicDisplay();
    displayText(0, 1, topText, 1); //Displays top text.
    displayLine(0,CHAR_HEIGHT + 2, SCREEN_WIDTH,CHAR_HEIGHT + 2); //Displays line at the top of screen.
    if (currentSelection >= 4) {
        //If the currently selected option is the 5th option or more.
        startIndex = currentSelection - 3;
        //Selection option will be the bottom option with its 3 previous options above it.
    } else {
        //If the currently selected option is the 4th option or less.
        startIndex = 0; //The first four options will be shown.
    }
    double multiplicationFactor = 2; //The multiplication factor that affects where the first option is shown.
    for (int i = startIndex; (optionsAmount >= 4 ? i < (startIndex + 4) : i < optionsAmount); i++) {
        //Displays 4 options from the start index or if there aren't 4 options, displays all the options from the start index.
        (currentSelection == i) ? displayColour("50cae0", "white") : displayColour("white", "50cae0");
        displayText(0,CHAR_HEIGHT * multiplicationFactor, options[i], 1);
        multiplicationFactor += 1.5; //Leaves a bit of space between options
    }
}

/**
 * Gets all the times in the operating modes current schedule and puts them into a char array in the format '##:##'
 * This array will then be passed into a function and will be displayed appropriately.
 *
 * @param currentSelection Which time is currently selected.
 * @param operatingMode The operating mode the schedule information will be taken from.
 */
void displayEditCurrentSchedule(const int currentSelection, operatingModeStruct *operatingMode) {
    char selectionOptions[operatingMode->numberOfFeedsInADay][LINE_BUFFER];
    for (int i = 0; i < operatingMode->numberOfFeedsInADay; i++) {
        snprintf(selectionOptions[i], 6, "%02d:%02d", operatingMode->feedTimes[i].hour,
                 operatingMode->feedTimes[i].minute);
    }
    strcpy(selectionOptions[operatingMode->numberOfFeedsInADay], "Exit");
    displayOptions(operatingMode->numberOfFeedsInADay + 1, selectionOptions, currentSelection, "Choose time to edit:");
}

/**
 * Gets all the options for the configuration menu and puts them into a char array.
 * This array will then be passed into a function and will be displayed appropriately.
 *
 * @param currentSelection THe options that is currently selected.
 */
void displayConfigurationMenu(const int currentSelection) {
    char selectionOptions[4][LINE_BUFFER] = {"Set The Clock", "Config Feed Schedule", "Select Operating Mode", "Exit"};
    displayOptions(4, selectionOptions, currentSelection, "Config menu:");
}

/**
 * Gets all the options for the set the clock menu and puts them into a char array.
 * This array will then be passed into a function and will be displayed appropriately.
 *
 * @param currentSelection The option that is currently selected.
 */
void displaySetTheClockMenu(const int currentSelection) {
    char selectionOptions[3][LINE_BUFFER] = {"Set the date", "Set the time", "Exit"};
    displayOptions(3, selectionOptions, currentSelection, "Set the clock:");
}

/**
 * Gets all the options for the operating mode menu and puts them into a char array.
 * This array will then be passed into a function and will be displayed appropriately.
 *
 * @param currentSelection The option that is currently selected.
 */
void displayOperatingModeMenu(const int currentSelection) {
    char selectionOptions[5][LINE_BUFFER] = {"Paused", "Auto", "Feed Now", "Skip Next feed", "Exit"};
    displayOptions(5, selectionOptions, currentSelection, "Operating Mode:");
}

/**
 * Gets all the options for the configure feed schedule menu and puts them into a char array.
 * This array will then be passed into a function and will be displayed appropriately.
 *
 * @param currentSelection The option that is currently selected.
 */
void displayConfigFeedScheduleMenu(const int currentSelection) {
    char selectionOptions[3][LINE_BUFFER] = {"New Schedule", "Edit Schedule", "Exit"};
    displayOptions(3, selectionOptions, currentSelection, "Config Feed Schedule:");
}

// FUNCTIONS THAT DISPLAY NUMBERS THAT CAN BE CHANGED

/**
 * Displays a boarder by drawing four lines between the four parameters.
 *
 * @param x The left most of the boarder .
 * @param y The top most of the boarder.
 * @param x1 the right most of the boarder
 * @param y1 The bottom most of the boarder.
 */
void displayBoarder(int x, int y, int x1, int y1) {
    displayLine(x, y, x1, y); //Draws the top of the boarder.
    displayLine(x, y1, x1, y1); //Draws the bottom of the boarder.
    displayLine(x, y, x, y1); //Draws the left of the boarder.
    displayLine(x1, y, x1, y1); //Draws the right of the boarder.
}

/**
 * A generic function that displays the digit/digits given with arrows above and below them,
 * After two digits the next character won't have arrows above it to allow space for a ':' or a '/'.
 * It displays two lines at the top and the bottom, as well as text at the top and bottom of the display.
 * The current selection is always highlighted.
 *
 * @param charAmount How many characters should be displayed.
 * @param displayedChars An array holding the characters to be displayed.
 * @param currentSelection Holds the index of the character that is selected.
 * @param bottomText The text that to be displayed at the bottom of the screen, this will be a warning.
 * @param topText The text to be displayed at the top of the screen, this will be a title.
 */
void setDigitsDisplay(const int charAmount, char displayedChars[charAmount][3], const int currentSelection,
                      char *bottomText, char *topText) {
    basicDisplay();
    displayText(0, 1, topText, 1); //Displays Top text.
    displayText(0, (SCREEN_HEIGHT - CHAR_HEIGHT), bottomText, 1); // Displays Bottom text.

    //Displays lines at top and bottom of the screen.
    displayLine(0,CHAR_HEIGHT + 1, SCREEN_WIDTH,CHAR_HEIGHT + 1);
    displayLine(0, (SCREEN_HEIGHT - CHAR_HEIGHT) - 3, SCREEN_WIDTH,SCREEN_HEIGHT - CHAR_HEIGHT - 3);

    int middleXCoordinate = (SCREEN_WIDTH - CHAR_WIDTH * 2 * charAmount) / 2; //adjusts accordingly.
    int middleYCoordinate = (SCREEN_HEIGHT - CHAR_HEIGHT * 2) / 2;
    int middleXCoordinateForBoarder = middleXCoordinate;
    for (int i = 0; i < charAmount; i++) {
        if (currentSelection == i) {
            displayColour("50cae0", "white");
        }
        displayText(middleXCoordinate, middleYCoordinate, displayedChars[i], 2);
        displayColour("white", "50cae0");
        if (!(i == 2 || i == 5)) {
            displayText(middleXCoordinate, (middleYCoordinate - 3) - CHAR_HEIGHT, "/\\", 1);
            displayText(middleXCoordinate, middleYCoordinate + CHAR_HEIGHT * 2 + 3, "\\/", 1);
        }
        middleXCoordinate += CHAR_WIDTH * 2;
    }
    displayBoarder(middleXCoordinateForBoarder - 1, middleYCoordinate - 1,
                   middleXCoordinateForBoarder + CHAR_WIDTH * 2 * charAmount - 1,
                   middleYCoordinate + CHAR_HEIGHT * 2 - 1);
}

/**
 * Takes an array of digits holding the hours,minutes,and seconds then makes it into a char array in a '##:##:##' format.
 * The char array can then be passed into a function to be displayed.
 *
 * @param currentSelection Which digit is currently selected.
 * @param digits All the digits making up the time.
 * @param bottomText The text to be displayed at the bottom of the screen. If this isn't empty it will be an error message.
 */
void displaySetTheTimeScreen(const int currentSelection, int digits[6], char *bottomText) {
    char displayedChars[8][3];
    snprintf(displayedChars[0], 3, "%d", digits[0]);
    snprintf(displayedChars[1], 3, "%d", digits[1]);
    snprintf(displayedChars[2], 3, "%c", ':');
    snprintf(displayedChars[3], 3, "%d", digits[2]);
    snprintf(displayedChars[4], 3, "%d", digits[3]);
    snprintf(displayedChars[5], 3, "%c", ':');
    snprintf(displayedChars[6], 3, "%d", digits[4]);
    snprintf(displayedChars[7], 3, "%d", digits[5]);
    //Displays the Time appropriately.
    setDigitsDisplay(8, displayedChars, currentSelection, bottomText, "Set the time:");
}

/**
 * Takes an array of digits holding the days,month,and year then makes it into a char array in a '##/##/##' format.
 * The char array can then be passed into a function to be displayed.
 *
 * @param currentSelection Which digit is currently selected.
 * @param digits All the digits making up the date.
 * @param bottomText The text to be displayed at the bottom of the screen. If this isn't empty it will be an error message.
 */
void displaySetTheDateScreen(const int currentSelection, int digits[8], char *bottomText) {
    char displayedChars[10][3];
    snprintf(displayedChars[0], 3, "%d", digits[0]);
    snprintf(displayedChars[1], 3, "%d", digits[1]);
    snprintf(displayedChars[2], 3, "%c", '/');
    snprintf(displayedChars[3], 3, "%d", digits[2]);
    snprintf(displayedChars[4], 3, "%d", digits[3]);
    snprintf(displayedChars[5], 3, "%c", '/');
    snprintf(displayedChars[6], 3, "%d", digits[4]);
    snprintf(displayedChars[7], 3, "%d", digits[5]);
    snprintf(displayedChars[8], 3, "%d", digits[6]);
    snprintf(displayedChars[9], 3, "%d", digits[7]);
    //Displays the date appropriately.
    setDigitsDisplay(10, displayedChars, currentSelection, bottomText, "Set the date:");
}

/**
 * Takes an array of digits holding the hours and minutes then makes it into a char array in a '##:##' format.
 * The char array can then be passed into a function to be displayed.
 *
 * @param currentSelection Which digit is currently selected.
 * @param digits All the digits making up the time.
 * @param bottomText The text to be displayed at the bottom of the screen. If this isn't empty it will be an error message.
 */
void displayScheduleGetTime(const int currentSelection, int digits[4], char *bottomText) {
    char displayedChars[5][3];
    snprintf(displayedChars[0], 3, "%d", digits[0]);
    snprintf(displayedChars[1], 3, "%d", digits[1]);
    snprintf(displayedChars[2], 3, "%c", ':');
    snprintf(displayedChars[3], 3, "%d", digits[2]);
    snprintf(displayedChars[4], 3, "%d", digits[3]);
    //Displays the time appropriately.
    setDigitsDisplay(5, displayedChars, currentSelection, bottomText, "Time to feed:");
}

/**
 * Takes an int then converts it into a char array.
 * The char array can then be passed into a function to be displayed.
 *
 * @param currentSelection The digit representing the number of rotations wanted.
 */
void displayScheduleGetRotations(const int currentSelection) {
    char currentCharacter[1][3];
    snprintf(currentCharacter[0], 3, "%d", currentSelection);
    //Displays the number of rotations selection appropriately.
    setDigitsDisplay(1, currentCharacter, 0, "", "Number of rotations:");
}

/**
 * Takes an int then converts it into a char array.
 * The char array can then be passed into a function to be displayed.
 *
 * @param currentSelection The digit representing the number of daily feeds wanted.
 */
void displayScheduleGetFeedsAmount(const int currentSelection) {
    char currentCharacter[1][3];
    snprintf(currentCharacter[0], 3, "%d", currentSelection);
    //Displays the number of daily feeds selection appropriately.
    setDigitsDisplay(1, currentCharacter, 0, "", "Daily feeds:");
}

//THE BLANK SCREEN FUNCTION

/**
 * Clears the display while there is inactivity so to stop burn out of the oled screen.
 * @param operatingMode The operating mode holding the current feed schedule which will be used to check if the fish should be fed.
 */
void blankScreen(operatingModeStruct *operatingMode) {
    bool runningBlankScreen = true; //Keeps the screen blank until user gives an input.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    displayClear(); //Clears the display
    /*
     * Loop that checks for button presses.
     */
    while (runningBlankScreen) {
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        char *result = buttonState(); //Get the button state.
        // Short pressing the button allows the user to return to the previous screen they were on.
        if (strcmp(result, "SHORT_PRESS") == 0) {
            runningBlankScreen = false; // Exits the loop
        }
        msleep(500L); //Ensures 0.5 seconds pass
    }
}
