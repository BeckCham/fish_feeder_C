/**
* Created by Beck Chamberlain on 21/11/2024.
*
* This file provides two structs that will be used throughout the program and their initialisers.
*/
#ifndef OPERATING_MODE_HEADER
#define OPERATING_MODE_HEADER

/**
 * The time struct, will be used to store a time when the fish should be fed.
 */
typedef struct {
    int hour;
    int minute;
    int rotations; //The number of rotations that should be done when this time is reached.
} timeStruct;

void initialiseTime(timeStruct *time, const int hour, const int minute, const int rotations);

/**
 * The operating mode struct, holds all the information about the current operating mode and stores the last feeding schedule added.
 */
typedef struct {
    int mode; //0 For "Auto" or 1 for "Paused.
    int nextFeed; //Index of next feed in feeds.
    int numberOfFeedsInADay; //The amount of feeds to be done in a day.
    timeStruct feedTimes[9]; //Holds the feed times in a day.
    int autoFeedsDone; //How many automatic feeds have been done when the struct is in auto mode.
} operatingModeStruct;

void initialiseOperatingMode(operatingModeStruct *operatingMode, const int mode, const int nextFeed,
                             const int numberOfFeedsInADay, timeStruct feedTimes[9], const int autoFeedsDone);

#endif //OPERATING_MODE_HEADER
