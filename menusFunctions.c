/**
* Created by Beck Chamberlain on 04/12/2024.
*
* Functions used by the menus that perform various tasks and decrease code repetition.
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menusFunctions.h"
#include "displayScreens.h"
#include "fish.h"
#include "operatingMode.h"
#define LINE_BUFFER 22
#define MIN_YEAR 1970
#define MINUTES_IN_DAY 1440
//FEEDS CONFIGURATION
/**
 * This function compares the current time to the times in the feed schedule and finds which feed is next.
 * Since the current time will be between two of the time schedules it finds which of the two it is between and the later time
 * will be the next time schedule.
 *
 * Note that the current time cannot also be the next feed time.
 * Eg if the current time is 13:00 and there are the times: {10:00,13:00,16:00}
 * then the next feed time will be 16:00.
 *
 * Once it finds the next feed the index of the next feed in the schedule is set.
 *
 * @param operatingMode The operating mode that the feed schedule will be taken from.
 */
void findNextFeed(operatingModeStruct *operatingMode) {
    operatingMode->nextFeed = 0;
    //If the for loops if condition isn't met then the next feed will be the first feed of the day.
    int currentTimeInMinutes = (clockHour() * 60 + clockMinute());
    //Goes through the feeds in the schedule starting at the second feed.
    for (int i = 1; i < operatingMode->numberOfFeedsInADay; i++) {
        //Gets the time in minutes of the feed that is previous to the comparison time.
        int lastTimeInMinutes = (operatingMode->feedTimes[i - 1].hour * 60) + operatingMode->feedTimes[i - 1].minute;
        //Gets the time in minutes of the comparison time.
        int currentComparisonTimeInMinutes = (operatingMode->feedTimes[i].hour * 60) + operatingMode->feedTimes[i].
                                             minute;
        //If the current time between the previously examined time and comparison time.
        if (currentTimeInMinutes > lastTimeInMinutes && currentTimeInMinutes <= currentComparisonTimeInMinutes) {
            operatingMode->nextFeed = i; //Sets as the index of the current comparison time.
        }
    }
}

/**
 * This is a custom function for qsort which takes two times and compares them which determines what order they will be in the schedule.
 * They will either be compared by their hour or minutes depending on which is most relevant.
 *
 * @param timeParam1 A pointer to the first time to be compared.
 * @param timeParam2 A pointer to the second time to be compared.
 * @return A negative value if the first time is smaller than the second time or a positive value if the first time is bigger than the second time.
 */
int compareTimes(const void *timeParam1, const void *timeParam2) {
    const timeStruct *time1 = (timeStruct *) timeParam1;
    const timeStruct *time2 = (timeStruct *) timeParam2;

    //If the hours aren't equal compares by hour.
    if (time1->hour != time2->hour) {
        return time1->hour - time2->hour;
    }
    // If the hours are equal compares by the minutes.
    return time1->minute - time2->minute;
}

/**
 * This function calls qsort on the current feed schedule and then calls the findNextFeed function.
 * This is for use after the feed schedule has changed.
 *
 * @param operatingMode The operating mode that the time schedule will be taken from.
 */
void sortScheduleTimes(operatingModeStruct *operatingMode) {
    qsort(operatingMode->feedTimes, operatingMode->numberOfFeedsInADay, sizeof(timeStruct), compareTimes);
    findNextFeed(operatingMode);
}

//FISH FEEDING FUNCTIONS
/**
 * This function rotates the fish feeder a given amount of times and if relevant increments the autoFeedsDone variable.
 *
 * @param numberOfRotations The number of times the fish feeder should make a whole 360 rotation.
 * @param ifCalledFromAuto If the function is being called because of the feed schedule.
 * @param operatingMode  The operating mode that will have its autoFeedsDone value incremented if applicable.
 */
void rotateFishFeeder(const int numberOfRotations, const bool ifCalledFromAuto, operatingModeStruct *operatingMode) {
    //if it was called fom the checkIfItsTimeToFeedFish function then it increments operatingModes number of auto feeds.
    if (ifCalledFromAuto) {
        //Increases how many automatic feeds have been done by 1.
        incrementNumber(&operatingMode->autoFeedsDone, 999, 1);
    }
    for (int i = 0; i < numberOfRotations; i++) {
        // Step the motor through a complete rotation.
        for (int j = 0; j < 360; j++) {
            motorStep(); // Step the motor
            msleep(40L); //Pauses by 40ms to reduce speed.
        }
    }
    foodFill(50); //Refills the fish feeder after a feed.
}

/**
 * This function compares the current time to the time of the next feed in the schedule and if they are equal then the fish will be fed.
 * This is called every half second.
 *
 * @param operatingMode The operating mode the feed schedule and mode will be taken from.
 * @param previousMinute The previous minute that was compared.
 */
void checkIfItsTimeToFeedFish(operatingModeStruct *operatingMode, int *previousMinute) {
    if (*previousMinute != clockMinute() && operatingMode->mode == 0) {
        //If the current mode is auto and the minute value has changed.
        const int nextFeedIndex = operatingMode->nextFeed;
        //If the current time and next feed time are equal.
        if (operatingMode->feedTimes[nextFeedIndex].hour == clockHour() && operatingMode->feedTimes[nextFeedIndex].
            minute == clockMinute()) {
            //Feed the fish for as many rotations specified for that time.
            rotateFishFeeder(operatingMode->feedTimes[nextFeedIndex].rotations,true, operatingMode);
            //Sets next feed to the index of the next feed in the schedule.
            incrementNumber(&operatingMode->nextFeed, operatingMode->numberOfFeedsInADay - 1, 0);
        }
        *previousMinute = clockMinute(); //Sets previous minute to the current minute.
    }
}

//COMMON MENU FUNCTIONS
/**
 * This function increments a variable by one or if it has reached its max value loop around and set it to the minimum value.
 * This is a called very often due to how users must give inputs.
 *
 * @param number The variable to be incremented.
 * @param maxValue The max value number can be.
 * @param minValue The minimum value number can be.
 */
void incrementNumber(int *number, const int maxValue, const int minValue) {
    if (*number == maxValue) {
        *number = minValue;
    } else {
        *number += 1;
    }
}

/**
 * This function performs all the necessary actions at the end of the while loops for menus/screens.
 * This is used in every screen and menu while loop.
 *
 * @param result Holds the result from the buttonPress.
 * @param timeCounter A counter to check for inactivity.
 */
void loopEnd(char *result, double *timeCounter) {
    free(result); //Frees the result of the buttonPress so it can be checked again.
    msleep(500L); //Sleeps for a half-second.
    *timeCounter += 0.5; //Adds a half second to show that it has passed.
}

/**
 * Resets the given variables appropriately.
 * This is used in every menu and screen.
 *
 * @param timeCounter A counter to check for inactivity.
 * @param previousMinute A means of tracking if a minute has passed on the clock.
 */
void resetVariables(double *timeCounter, int *previousMinute) {
    *timeCounter = 0; //Resets the time counter.
    *previousMinute = -1; //Ensures that checkIfItsTimeToFeedFish will check the time.
}

/**
 * A generic reset configuration that can be used for multiple menu's and screens. It resets the variables and displays the menu/screen.
 * This is for use after another screen has been displayed.
 *
 * @param displayFunction The appropriate display function for the menu/screen that is calling the function.
 * @param currentSelection The current selection for the displayFunction to take as a parameter.
 * @param timeCounter A counter to check for inactivity.
 * @param previousMinute A means of tracking if a minute has passed on the clock.
 */
void resetGenericConfiguration(void (*displayFunction)(int), int currentSelection, double *timeCounter,
                               int *previousMinute) {
    displayFunction(currentSelection); // Displays the appropriate menu
    resetVariables(timeCounter, previousMinute);
}

/**
 * A generic configuration that can be used for multiple menu's and screens.
 * It resets the time counter and displays the menu/screen.
 * This is for use after the currentSelection has been changed.
 *
 * @param displayFunction The appropriate display function for the menu/screen that is calling the function.
 * @param currentSelection The current selection for the displayFunction to take as a parameter.
 * @param timeCounter A counter to check for inactivity.
 */
void interactionGenericConfiguration(void (*displayFunction)(int), int currentSelection, double *timeCounter) {
    displayFunction(currentSelection); // Displays the appropriate menu
    *timeCounter = 0;
}

//MAIN SCREEN FUNCTION
/**
 * This function resets variables and displays the main screen.
 *
 * @param operatingMode The operating mode that the main screen will display the information of.
 * @param timeCounter A counter to check for inactivity.
 * @param previousMinute A means of tracking if a minute has passed on the clock.
 */
void resetMainScreen(operatingModeStruct *operatingMode, double *timeCounter, int *previousMinute) {
    displayMainScreen(operatingMode); //Displays the main screen
    resetVariables(timeCounter, previousMinute);
}

//FUNCTIONS USED BY CONFIGURE FEED SCHEDULE
/**
 * A configuration that can be used for the get time screen, it resets the time counter and displays the screen.
 * This is for use after the current selection has changed or the value of a digit.
 *
 * @param currentSelection Indicates the digit that is currently selected.
 * @param timeCounter A counter to check for inactivity.
 * @param digits An array holding the values of the time digits.
 * @param bottomText Text that will be displayed at the bottom of the screen. This will be a warning to the user.
 */
void configureGetTimeScreen(int currentSelection, double *timeCounter, int digits[4], char *bottomText) {
    displayScheduleGetTime(currentSelection, digits, bottomText);
    *timeCounter = 0;
}

/**
 * This function takes a time and compares it to other times in the feeding schedule.
 * If it finds a conflict eg another time that is within 5 minutes before or after the time given then it returns true.
 *
 * @param totalMinutes The time to be compared in the form of its total Minutes.
 * @param operatingMode The current operating mode that holds the times that are being compared to the given time.
 *
 * @return If there is a time conflict with a pre-existing time in the schedule.
 */
bool findIfTimeConflict(const operatingModeStruct *operatingMode, const int totalMinutes) {
    //Goes through every feedTime in the current feeding schedule.
    for (int i = 0; i < operatingMode->numberOfFeedsInADay; i++) {
        timeStruct time = operatingMode->feedTimes[i];
        int timeDifference = abs((time.hour * 60 + time.minute) - totalMinutes);
        //Ensures that still cant be added within 5 minutes of each other even in edge cases.
        if (timeDifference > MINUTES_IN_DAY / 2) {
            timeDifference = MINUTES_IN_DAY - timeDifference;
        }
        if (timeDifference <= 5) {
            //If new time is 5 minutes before or after a preexisting one.
            return true;
        }
    }
    return false;
}

/**
 * The get rotations screen, through this the user can select how many times the fish feeder will rotate for a timed feed.
 *
 * @param operatingMode The current operating mode that will have the rotation added to it.
 * @param position The position in the array in the operating mode that is having the number of rotations changed.
 */
void scheduleGetRotationsScreen(operatingModeStruct *operatingMode, const int position) {
    bool getRotationAmount = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 1; //Can be between 1-9
    displayScheduleGetRotations(currentSelection);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the numbers 1-9 and a long press confirms the number.
     */
    while (getRotationAmount) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode);
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayScheduleGetRotations, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through the possible values for current selection.
         * The value of current selection will change thus the display function needs to be called.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 9, 1);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayScheduleGetRotations, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection, once this is done the rotations value in the Time struct selected will be changed to the current selection.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            operatingMode->feedTimes[position].rotations = currentSelection;
            getRotationAmount = false; //Exits the loop.
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The get schedule time screen, through this the user can select a time when they want the fish to be fed.
 * After completion of this function the user will need to select how many rotations the fish feeder should make at the given time.
 *
 * @param position The position in the feedTimes array that is to be set.
 * @param operatingMode The current operating mode that will have a time changed.
 */
void scheduleGetTimeScreen(operatingModeStruct *operatingMode, const int position) {
    bool getNewScheduleTime = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //Tracks what digit is currently being chosen.
    int digits[4] = {0, 0, 0, 0}; //Tracks the value of the digits that make up the time.
    char bottomText[LINE_BUFFER] = ""; //Holds the message that will be shown at the bottom of the screen.
    displayScheduleGetTime(currentSelection, digits, bottomText);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the possible number values for the current digit and a long press confirms the digit.
     */
    while (getNewScheduleTime) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after returning to the screen.
            displayScheduleGetTime(currentSelection, digits, bottomText);
            resetVariables(&timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through the possible values for the digit currently selected.
         * The current digit will change thus the display function needs to be called.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            if (currentSelection == 0) {
                incrementNumber(&digits[0], 2, 0); //First digit of hours must be between 0-2
            } else if (currentSelection == 1) {
                incrementNumber(&digits[1], 9, 0); //Second digit of hours must be between 0-9
            } else if (currentSelection == 3) {
                incrementNumber(&digits[2], 5, 0); //First digit of minutes must be between 0-5
            } else if (currentSelection == 4) {
                incrementNumber(&digits[3], 9, 0); //Second digit of minutes must be between 0-9
            }
            //Re-Configures the menu after the value of the current digit changes.
            configureGetTimeScreen(currentSelection, &timeCounter, digits, bottomText);
        }

        /**
         * Long pressing the button confirms the current digit. Then the next digit will be selected if all digits haven't been
         * confirmed. Once the digits are all confirmed a check will be done to ensure the time is valid.
         * If the time is not valid a warning message will be shown and the user is allowed to give the time again.
         * If the time is valid then it is set as the time in the position given as a parameter.
         *
         * The current digit will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            char hours[3];
            char minutes[3];
            switch (currentSelection) {
                case 1: //Once the digits making up the hours have been confirmed.
                    currentSelection += 2; //Skips ':' character.
                    strcpy(bottomText, ""); //Erases error message
                    break;
                case 4: //Once the digits making up the minutes have been confirmed.
                    //Gets the minutes and hours as an int. Allowing the TimeStruct variables to be set to them and to make validation checks easier.
                    snprintf(hours, 3, "%d%d", digits[0], digits[1]);
                    snprintf(minutes, 3, "%d%d", digits[2], digits[3]);
                    int intHours = atoi(hours);
                    int intMinutes = atoi(minutes);
                    int totalMinutes = intHours * 60 + intMinutes;
                    //If intHours is no higher than 24 and there's not another feed scheduled within 5 minutes of the time given then the feed time is set.
                    //If it's not a valid time then an appropriate error message is displayed, the user is allowed to re-enter a time, and the time values are reset.
                    if (intHours >= 24) {
                        strcpy(bottomText, "Max hour: 23");
                        digits[0] = digits[1] = digits[2] = digits[3] = 0;
                        currentSelection = 0;
                    } else if (findIfTimeConflict(operatingMode, totalMinutes)) {
                        strcpy(bottomText, "Time conflict, retry");
                        digits[0] = digits[1] = digits[2] = digits[3] = 0;
                        currentSelection = 0;
                    } else {
                        operatingMode->feedTimes[position].hour = intHours;
                        operatingMode->feedTimes[position].minute = intMinutes;
                        getNewScheduleTime = false;; //Exits the loop.
                    }
                    break;
                default: //When a digit has been confirmed, and it is none of the above cases.
                    currentSelection += 1; //Goes to the next digit.
            }
            //Re-Configures the menu after current digit changes.
            configureGetTimeScreen(currentSelection, &timeCounter, digits, bottomText);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The create new schedule screen, through this the user can select how many times they want the fish to be fed a day
 * After completion of this function the user will return to the configure feed schedule menu.
 *
 * @param operatingMode The operating mode that the feed schedule will be added to.
 */
void createNewScheduleScreen(operatingModeStruct *operatingMode) {
    bool createNewSchedule = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 1; //Can be between 1-9
    displayScheduleGetFeedsAmount(currentSelection);
    /*
     * Loop that checks for button presses, a short press allows the user to decide how many schedules to add
     * and a long press confirms the current number selected.
     */
    while (createNewSchedule) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayScheduleGetFeedsAmount, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through the numbers 1-9.
         * The current selection will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 9, 1);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayScheduleGetFeedsAmount, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection. The schedules number of feeds in a day is set to the current selection,
         * then a time and number of rotations is got for every feed in the day. The new schedule times are then sorted.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            timeStruct time;
            initialiseTime(&time, 0, 0, 0);
            for (int i = 0; i < 9; i++) {
                operatingMode->feedTimes[i] = time;
            }
            operatingMode->numberOfFeedsInADay = 0;
            for (int i = 0; i < currentSelection; i++) {
                scheduleGetTimeScreen(operatingMode, i);
                scheduleGetRotationsScreen(operatingMode, i);
                operatingMode->numberOfFeedsInADay = operatingMode->numberOfFeedsInADay + 1;
            }
            //Sorts the new schedule times from smallest to largest and adjusts the next time accordingly.
            sortScheduleTimes(operatingMode);
            createNewSchedule = false; //Exits the loop.
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}


/**
 * The edit current schedule screen, through this the user can select one of the currently existing times in the schedule and change it.
 * After completion of this function the user will return to the configure feed schedule menu.
 *
 * @param operatingMode The operating mode for the feed schedule that is being changed.
 */
void editCurrentSchedule(operatingModeStruct *operatingMode) {
    bool editCurrentScheduleMenu = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //Holds the index of the current selection.
    displayEditCurrentSchedule(currentSelection, operatingMode);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the pre-existing schedule times and a long press confirms the current selection.
     */
    while (editCurrentScheduleMenu) {
        char *result = buttonState(); // Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed.
            displayEditCurrentSchedule(currentSelection, operatingMode);
            resetVariables(&timeCounter, &previousMinute); //Re-Configures the variables after returning to the screen.
        }
        /*
         * Short pressing the button allows the user to cycle through schedule times.
         * The current selection will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, operatingMode->numberOfFeedsInADay, 0);
            displayEditCurrentSchedule(currentSelection, operatingMode);
            timeCounter = 0; //Resets because the user has interacted with the program.
        }
        /*
         * Long pressing the button confirms the current selection, if the user selects a pre-existing time then they are
         * prompted to select a new time to replace the pre-existing one and to choose the number of rotations that will take place for it.
         * Following this the schedule times will be sorted to ensure that the next feed time is still accurate.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            if (currentSelection != operatingMode->numberOfFeedsInADay) {
                //If the user hasn't selected exit.
                timeStruct time;
                initialiseTime(&time, 0, 0, 0);
                operatingMode->feedTimes[currentSelection] = time;
                scheduleGetTimeScreen(operatingMode, currentSelection);
                scheduleGetRotationsScreen(operatingMode, currentSelection);
            }
            sortScheduleTimes(operatingMode);
            editCurrentScheduleMenu = false; //Exits the loop.
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}


//SET THE CLOCK FUNCTIONS
/**
 * A configuration that can be used for the set the time screen, it resets the time counter and displays the screen.
 * This is for use after the current selection has changed or the value of a digit.
 *
 * @param currentSelection indicates the digit that is currently selected.
 * @param timeCounter A counter to check for inactivity.
 * @param bottomText Text that will be displayed at the bottom of the screen. This will be a warning to the user.
 * @param digits An array holding the values of the time digits.
 */
void configureSetTheTimeScreen(const int currentSelection, double *timeCounter, char *bottomText, int digits[6]) {
    displaySetTheTimeScreen(currentSelection, digits, bottomText); //Displays the time screen
    *timeCounter = 0; //Resets the time counter
}

/**
 * A configuration that can be used for the set the date screen, it resets the time counter and displays the screen.
 * This is for use after the current selection has changed or the value of a digit.
 *
 * @param currentSelection indicates the digit that is currently selected.
 * @param timeCounter A counter to check for inactivity.
 * @param bottomText Text that will be displayed at the bottom of the screen. This will be a warning to the user.
 * @param digits An array holding the values of the date digits.
 */
void configureSetTheDateScreen(const int currentSelection, double *timeCounter, char *bottomText, int digits[8]) {
    displaySetTheDateScreen(currentSelection, digits, bottomText); //Displays the date screen
    *timeCounter = 0; //Resets the time counter
}

/**
 *The set the time screen, through this the user can select what the time should be changed to.
 * After completion of this function the user will return to the set the clock menu.
 *
 * @param operatingMode The operating mode that hold the current feed schedule.
 */
void setTheTime(operatingModeStruct *operatingMode) {
    bool setTheTimeScreen = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //Tracks what digit is currently being chosen.
    int digits[6] = {0, 0, 0, 0, 0, 0}; //Tracks the value of the digits that make up the time.
    char bottomText[LINE_BUFFER] = ""; //Holds the text that will be displayed at the bottom of the screen.
    displaySetTheTimeScreen(currentSelection, digits, bottomText);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the possible number values for the current digit and a long press confirms the digit.
     */
    while (setTheTimeScreen) {
        char *result = buttonState();// Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after the screen has changed.
            displaySetTheTimeScreen(currentSelection, digits, bottomText);
            resetVariables(&timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through the possible values for the digit currently selected.
         * The current digit will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            if (currentSelection == 0) {
                incrementNumber(&digits[0], 2, 0); //First digit of hours must be between 0-2
            } else if (currentSelection == 1) {
                incrementNumber(&digits[1], 9, 0); //Second digit of hours must be between 0-9
            } else if (currentSelection == 3) {
                incrementNumber(&digits[2], 5, 0); //First digit of minutes must be between 0-5
            } else if (currentSelection == 4) {
                incrementNumber(&digits[3], 9, 0); //Second digit of minutes must be between 0-9
            } else if (currentSelection == 6) {
                incrementNumber(&digits[4], 5, 0); //First digit of seconds must be between 0-5
            } else if (currentSelection == 7) {
                incrementNumber(&digits[5], 9, 0); //Second digit of seconds must be between 0-9
            }
            //Re-Configures the menu after the value of the current selection changes.
            configureSetTheTimeScreen(currentSelection, &timeCounter, bottomText, digits);
        }
        /*
         * Long pressing the button confirms the current digit. Then the next digit will be selected if all digits haven't been
         * confirmed. Depending on the digit confirmed sometimes a check will be done to ensure the time is valid.
         * If the time is not valid a warning message will be shown and the user is allowed to give the time again.
         * If the last digit is confirmed then the clock is set and the time of the next feed is recalculated.
         *
         * The current digit will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            char hours[3];
            char minutes[3];
            char seconds[3];
            switch (currentSelection) {
                case 1: //Once the digits making up the hour have been confirmed.
                    if (digits[0] == 2 && digits[1] > 3) {
                        //If the hour given is over 24 let user re-input and give a warning message.
                        currentSelection = 0;
                        strcpy(bottomText, "Max hour: 23");
                    } else {
                        currentSelection += 2; //Skips ':' character.
                        strcpy(bottomText, ""); //Erases any error messages.
                    }
                    break;
                case 4: //Once the digits making up the minute have been confirmed.
                    currentSelection += 2; //Skips ':' character.
                    break;
                case 7: //Once the digits making up the seconds have been confirmed.
                    //Gets the seconds, minutes, and hours as an int. Allowing them to be given to the clockSet function.
                    snprintf(seconds, 3, "%d%d", digits[4], digits[5]);
                    int intSecond = atoi(seconds);
                    snprintf(minutes, 3, "%d%d", digits[2], digits[3]);
                    int intMinute = atoi(minutes);
                    snprintf(hours, 3, "%d%d", digits[0], digits[1]);
                    int intHour = atoi(hours);

                    clockSet(intSecond, intMinute, intHour, clockDay(), clockMonth(), clockYear());
                    findNextFeed(operatingMode);
                    setTheTimeScreen = false; //Exits the loop.
                    break;
                default: //When a digit has been confirmed, and it is none of the above cases.
                    currentSelection += 1; //Goes to the next digit.
            }
            //Re-Configures the menu after the current selection changes.
            configureSetTheTimeScreen(currentSelection, &timeCounter, bottomText, digits);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The set the date screen, through this the user can select what the date should be changed to.
 * After completion of this function the user will return to the set the clock menu.
 *
 * @param operatingMode The operating mode that hold the current feed schedule.
 */
void setTheDate(operatingModeStruct *operatingMode) {
    bool setTheDateMenu = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //Tracks what digit is currently being chosen.
    int digits[8] = {0, 1, 0, 1, 1, 9, 7, 0}; //Tracks the value of the digits that make up the date.
    char bottomText[LINE_BUFFER] = ""; //Holds the text that will be displayed at the bottom of the screen.
    displaySetTheDateScreen(currentSelection, digits, bottomText);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the possible number values for the current digit and a long press confirms the digit.
     */
    while (setTheDateMenu) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed.
            //Resets the menu after the screen has changed.
            displaySetTheDateScreen(currentSelection, digits, bottomText);
            resetVariables(&timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through the possible values for the digit currently selected.
         * The current digit will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            if (currentSelection == 0) {
                incrementNumber(&digits[0], 3, 0); //First digit of day must be between 0-3
            } else if (currentSelection == 1) {
                incrementNumber(&digits[1], 9, 0); //Second digit of day must be between 0-9
            } else if (currentSelection == 3) {
                incrementNumber(&digits[2], 1, 0); //First digit of month must be between 0-1
            } else if (currentSelection == 4) {
                incrementNumber(&digits[3], 9, 0); //Second digit of month must be between 0-9
            } else if (currentSelection == 6) {
                incrementNumber(&digits[4], 9, 0); //First digit of year must be between 0-9
            } else if (currentSelection == 7) {
                incrementNumber(&digits[5], 9, 0); //Second digit of year must be between 0-9
            } else if (currentSelection == 8) {
                incrementNumber(&digits[6], 9, 0); //Third digit of year must be between 0-9
            } else if (currentSelection == 9) {
                incrementNumber(&digits[7], 9, 0); //Fourth digit of year must be between 0-9
            }
            //Re-Configures the menu after the current selection changes.
            configureSetTheDateScreen(currentSelection, &timeCounter, bottomText, digits);
        }
        /*
         * Long pressing the button confirms the current digit.
         * Then the next digit will be selected if all digits haven't been confirmed.
         * Depending on the digit confirmed sometimes a check will be done to ensure the date is valid.
         * If the date is not valid a warning message will be shown and the user is allowed to give the date again.
         * If the last digit is confirmed and the date is valid then the clock is set.
         *
         * The current digit will be highlighted thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            char year[5];
            char month[3];
            char day[3];
            switch (currentSelection) {
                case 1: //Once the digits making up the day have been confirmed.
                    //Ensures day is between the values 1-31, Otherwise an appropriate warning is given and the user must re-enter day.
                    if ((digits[0] == 0 && digits[1] > 0) || digits[0] == 1 || digits[0] == 2 || (
                            digits[0] == 3 && digits[1] < 2)) {
                        currentSelection += 2; //Skips '/' character.
                        strcpy(bottomText, ""); //Erases any error messages.
                    } else if (digits[0] == 0 && digits[1] == 0) {
                        //If the day is 0
                        currentSelection = 0;
                        strcpy(bottomText, "Min day: 01");
                    } else {
                        // if the day is over 31
                        currentSelection = 0;
                        strcpy(bottomText, "Max day: 31");
                    }
                    break;
                case 4: //Once the digits making up the month have been confirmed.
                    //Ensures month is between the values 1-12, Otherwise an appropriate warning is given and the user must re-enter month.
                    if ((digits[2] == 0 && digits[3] > 0) || (digits[2] == 1 && digits[3] < 3)) {
                        currentSelection += 2; //Skips '/' character.
                        strcpy(bottomText, ""); //Erases any error messages.
                    } else if (digits[2] == 0 && digits[3] == 0) {
                        currentSelection = 3;
                        strcpy(bottomText, "Min month: 01");
                    } else {
                        currentSelection = 3;
                        strcpy(bottomText, "Max month: 12");
                    }
                    break;
                case 9: //Once the digits making up the year have been confirmed.
                    //Gets the day, month, and years as an int. Allowing them to be given to the clockSet function.
                    snprintf(year, 5, "%d%d%d%d", digits[4], digits[5], digits[6], digits[7]);
                    int intYear = atoi(year);
                    snprintf(month, 3, "%d%d", digits[2], digits[3]);
                    int intMonth = atoi(month);
                    snprintf(day, 3, "%d%d", digits[0], digits[1]);
                    int intDay = atoi(day);
                    //Sets bottom text as empty - if it stays at this value then we know no issues have been found.
                    strcpy(bottomText, "");
                    //Ensures year is between the values 1970-9999, Otherwise an appropriate warning is given and the user must re-enter year.
                    if (intYear < MIN_YEAR) {
                        currentSelection = 6;
                        strcpy(bottomText, "Min year: 1970");
                    }
                    //Ensures that the day given is valid for the month given, taking into account if it is a leap year.
                    //If the day isn't valid an appropriate warning is given and the user must re-enter the date.
                    if (intMonth == 1 || intMonth == 3 || intMonth == 5 || intMonth == 7 || intMonth == 8 || intMonth ==
                        10 || intMonth == 12) {
                        if (intDay > 31) {
                            strcpy(bottomText, "Date entered invalid");
                            currentSelection = 0;
                        }
                    } else if (intMonth == 2) {
                        if ((intDay > 29) || (intDay == 29 && !(
                                                  intYear % 4 == 0 && (intYear % 100 != 0 || intYear % 400 == 0)))) {
                            strcpy(bottomText, "Date entered invalid");
                            currentSelection = 0;
                        }
                    } else if (intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11) {
                        if (intDay > 30) {
                            strcpy(bottomText, "Date entered invalid");
                            currentSelection = 0;
                        }
                    }
                    if (strlen(bottomText) == 0) {
                        //If no warnings were given and the date is valid then set the clock.
                        clockSet(clockSecond(), clockMinute(), clockHour(), intDay, intMonth, intYear);
                        setTheDateMenu = false; //Exits the loop.
                    }
                    break;
                default: //When a digit has been confirmed, and it is none of the above cases.
                    currentSelection += 1; //Goes to the next digit.
            }
            configureSetTheDateScreen(currentSelection, &timeCounter, bottomText, digits);
            //Re-Configures the menu after the current selection changes.
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}
