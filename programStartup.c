/**
* Created by Beck Chamberlain on 07/12/2024.
*
* This function mimics reading from EEPROM and initialises the program.
*/
#include <stddef.h>
#include <stdio.h>
#include "fish.h"
#include "operatingMode.h"

/**
 * This function uses the warmStart value to start up the clock to the same time it was when the program last exited.
 * If warmStart is 0 then that information isn't available and the clock will be started at a generic time.
 *
 * The food in the fish feeder is also filled so the program is ready to feed the fish.
 *
 * @param warmStart The value that holds a time the clock can read.
 */
void initialiseProgram(const long warmStart) {
    if (warmStart == 0) {
        // Set the real time clock to 14:00:00 on the 1st Jan 1970.
        clockSet(0, 0, 14, 1, 01, 1970);
    } else {
        clockWarmStart(warmStart);
    }
    foodFill(50); // Fill the food container.
}

/**
 * This function attempts to read from the file given and saves the information from it to the operating mode.
 * This allows the program to be set up the exact same as when it was last closed.
 *
 * @param filename The name of the file that is to be read.
 * @param operatingMode The operating mode that the read information is saved to.
 */
void loadFromEEPROM(const char *filename, operatingModeStruct *operatingMode) {
    FILE *file = fopen(filename, "r");
    timeStruct time;
    initialiseTime(&time, 0, 0, 0);
    //Creates a blank array of feed times.
    timeStruct emptyFeeds[9] = {time, time, time, time, time, time, time, time, time};
    //A generic version of the operating mode with no feeds is initialised so if the file cannot be read then the program will still run smoothly.
    initialiseOperatingMode(operatingMode, 1, -1, 0, emptyFeeds, 0);
    //If there is a file to read from then gets the information from that to set time and what operating mode it is on.
    long long warmStartValue = 0; //Generic value in case the file cannot be read.
    if (file != NULL) {
        //If the file can be read.
        fscanf(file, "%lld %d %d %d %d", &warmStartValue, &operatingMode->mode, &operatingMode->numberOfFeedsInADay,
               &operatingMode->autoFeedsDone, &operatingMode->nextFeed);
        for (int i = 0; i < operatingMode->numberOfFeedsInADay; i++) {
            fscanf(file, " %d %d %d", &time.rotations, &time.hour, &time.minute);
            operatingMode->feedTimes[i] = time;
        }
        fclose(file); //Close the file.
        initialiseProgram(warmStartValue); //Finishes the program startup.
    } else {
        initialiseProgram(warmStartValue); //Finishes the program startup.
    }
}
