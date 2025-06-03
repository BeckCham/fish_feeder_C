/**
* Created by Beck Chamberlain on 04/12/2024.
*
* Functions that display and manage the menus.
*/
#include <stdbool.h>
#include <string.h>
#include "fish.h"
#include "menus.h"
#include "displayScreens.h"
#include "menusFunctions.h"

/**
 * The select operating mode menu, the user can choose to change the operating mode from the options 'Paused',
 * 'Auto', 'Skip next feed',and 'Feed now'
 *
 * The user is also able to quit this menu and return to the configuration menu.
 *
 * @param operatingMode The current operating mode that will have its mode changed.
 */
void selectOperatingModeMenu(operatingModeStruct *operatingMode) {
    bool operatingModeMenu = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //0 is Paused, 1 is auto, 2,is feed now , 3 is skip next feed, 4 is exit
    displayOperatingModeMenu(currentSelection);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the selection options and a long press confirms the current option.
     */
    while (operatingModeMenu) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayOperatingModeMenu, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through 5 options one by one. The current selection will be highlighted
         * thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 4, 0);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayOperatingModeMenu, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection. Different actions will then be taken depending on the current
         * selection.The current selection will be reset to 0 and the select operating mode menu will be displayed as well as the time counter being reset.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            if (currentSelection == 0) {
                //Sets the current mode to paused, this means no feeds will take place until the mode is changed.
                operatingMode->mode = 1;
            } else if (currentSelection == 1) {
                //Sets the current mode to auto, this means the fish will be fed according to the feed schedule.
                operatingMode->mode = 0;
                findNextFeed(operatingMode); //Ensures the next feed time is accurate to the feed schedule.
            } else if (currentSelection == 2) {
                //Feeds the fish manually, this will not affect the current mode.
                rotateFishFeeder(1,false, operatingMode);
            } else if (currentSelection == 3) {
                //Skips the next feed and goes to the next feed in the current schedule.
                if (operatingMode->numberOfFeedsInADay > 0) {
                    //Ensures there actually a feed schedule to get the next feed from.
                    incrementNumber(&operatingMode->nextFeed, operatingMode->numberOfFeedsInADay - 1, 0);
                }
            } else {
                operatingModeMenu = false; //Exits the loop.
            }
            currentSelection = 0;
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayOperatingModeMenu, currentSelection, &timeCounter);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The configure feed schedule menu, the user can choose to create a new feed schedule or edit the current one.
 * The user is also able to quit this menu and return to the configuration menu.
 *
 * @param operatingMode The current operating mode that will have its feed schedule changed.
 */
void configureFeedScheduleMenu(operatingModeStruct *operatingMode) {
    bool configFeedSchedule = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; // 0 is new schedule,1 is edit schedule, 2 is exit.
    displayConfigFeedScheduleMenu(currentSelection);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the selection options and a long press confirms the current option.
     */
    while (configFeedSchedule) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayConfigFeedScheduleMenu, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through 3 options one by one. The current selection will be highlighted
         * thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 2, 0);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayConfigFeedScheduleMenu, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection. Different actions will then be taken depending on the current
         * selection. The configure feed schedule menu will be reset and displayed after the selected action is complete.
         * The current selection will also be reset to 0.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            if (currentSelection == 0) {
                //Enters a screen where the user is able to create a new schedule.
                createNewScheduleScreen(operatingMode);
            } else if (currentSelection == 1) {
                if (operatingMode->numberOfFeedsInADay != 0) {
                    //Checks that a schedule exists to edit.
                    //Enters a screen where the user is able to edit the current schedule.
                    editCurrentSchedule(operatingMode);
                }
            } else {
                configFeedSchedule = false; //Exits the loop
            }
            currentSelection = 0;
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayConfigFeedScheduleMenu, currentSelection, &timeCounter, &previousMinute);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The set the clock menu, you can choose to change the time or change the date.
 * The user is also able to quit this menu and return to the configuration menu.
 *
 * @param operatingMode The operating mode is passed to allow for constant checks for if it's time to feed the fish.
 */
void setTheClockMenu(operatingModeStruct *operatingMode) {
    bool selectTimeChangeType = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //0 is set the date, 1 is set the time, 2 is exit.
    displaySetTheClockMenu(currentSelection);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the selection options and a long press confirms the current option.
     */
    while (selectTimeChangeType) {
        char *result = buttonState(); //Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displaySetTheClockMenu, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through 3 options one by one. The current selection will be highlighted
         * thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 2, 0);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displaySetTheClockMenu, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection. Different actions will then be taken depending on the current
         * selection.The set the clock menu will be reset and displayed after the selected action is complete.
         * The current selection will also be reset to 0.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            if (currentSelection == 0) {
                setTheDate(operatingMode); //Enters a screen where the user can change the systems current date.
            } else if (currentSelection == 1) {
                setTheTime(operatingMode); //Enters a screen where the user can change the systems current time.
            } else {
                selectTimeChangeType = false; //Exits the loop.
            }
            currentSelection = 0;
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displaySetTheClockMenu, currentSelection, &timeCounter, &previousMinute);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The configuration menu, further menus that can edit the system, operating mode or the feed schedule are displayed.
 * The user is also able to quit this menu and return to the main screen.
 *
 * @param operatingMode The operating mode that will be edited and used for checks.
 */
void configurationMenu(operatingModeStruct *operatingMode) {
    bool selectConfigMenu = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int currentSelection = 0; //O is set the clock, 1 is config feed schedule, 2 is select operating mode, 3 is exit.
    displayConfigurationMenu(currentSelection);
    /*
     * Loop that checks for button presses,
     * a short press cycles through the selection options and a long press confirms the current option.
     */
    while (selectConfigMenu) {
        char *result = buttonState(); // Get the button state.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed.
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayConfigurationMenu, currentSelection, &timeCounter, &previousMinute);
        }
        /*
         * Short pressing the button allows the user to cycle through 4 options one by one. The current selection will be highlighted
         * thus the display function needs to be called every time the selection changes.
         */
        if (strcmp(result, "SHORT_PRESS") == 0) {
            incrementNumber(&currentSelection, 3, 0);
            //Applies the relevant changes after the current selection changes.
            interactionGenericConfiguration(displayConfigurationMenu, currentSelection, &timeCounter);
        }
        /*
         * Long pressing the button confirms the current selection. Different actions will then be taken depending on the current
         * selection. The configuration menu will be reset and displayed after the selected action is complete.
         * The current selection will also be reset to 0.
         */
        if (strcmp(result, "LONG_PRESS") == 0) {
            if (currentSelection == 0) {
                setTheClockMenu(operatingMode); //Enters a menu where you can set the clock to a different time or date.
            } else if (currentSelection == 1) {
                //Enters a menu where you can configure the operating modes feed schedule.
                configureFeedScheduleMenu(operatingMode);
            } else if (currentSelection == 2) {
                selectOperatingModeMenu(operatingMode); //Enters a menu where you can select the current operating mode.
            } else {
                selectConfigMenu = false; //Exits the loop.
            }
            currentSelection = 0;
            //Resets the menu after returning to the screen.
            resetGenericConfiguration(displayConfigurationMenu, currentSelection, &timeCounter, &previousMinute);
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}

/**
 * The main menu, information about the current operating mode and the system is displayed.
 * The user is also able to either quit the program or enter the configuration menu.
 *
 * @param operatingMode The operating mode that will be edited and used for checks.
 */
void mainScreen(operatingModeStruct *operatingMode) {
    bool runningMainScreen = true; //Keeps the menu active until user exits it.
    double timeCounter = 0; //Tracks how many seconds have gone by since the last user interaction.
    int previousMinute = -1; //Allows detection when minutes value has changed.
    int previousSecond = -1; //Allows detection when seconds value has changed.
    int nextFeed = operatingMode->nextFeed;
    displayMainScreen(operatingMode);
    /*
     * Loop that checks for button presses,
     * a short press enters the configuration menu and a long press exits the loop.
     * When the loop is exited the program will end.
     */
    while (runningMainScreen) {
        char *result = buttonState(); // Gets the button state.
        updateTimeDisplay(&previousSecond); //Updates the time display.
        //Checks if the minute has changed and if it's time to feed fish.
        checkIfItsTimeToFeedFish(operatingMode, &previousMinute);
        if (nextFeed != operatingMode->nextFeed) {
            //If the next feed has changed re-display menu.
            displayMainScreen(operatingMode);
            nextFeed = operatingMode->nextFeed; //updates the next feed
        }
        //Checks if user has been inactive for 60 seconds.
        if (timeCounter >= 60) {
            blankScreen(operatingMode); //Clears the screen until the button is pressed.
            resetMainScreen(operatingMode, &timeCounter, &previousMinute); //Resets the screen after returning.
        }
        if (strcmp(result, "LONG_PRESS") == 0) {
            runningMainScreen = false; //Exits the loop.
        }
        if (strcmp(result, "SHORT_PRESS") == 0) {
            configurationMenu(operatingMode); //Runs and displays the configuration menu.
            resetMainScreen(operatingMode, &timeCounter, &previousSecond); //Resets the screen after returning.
        }
        loopEnd(result, &timeCounter); //Ensures 0.5 seconds pass and updates/resets variables accordingly.
    }
}
