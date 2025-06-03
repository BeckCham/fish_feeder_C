/**
* Created by Beck Chamberlain on 04/12/2024.
*
* This file provides functions for the menus.
*
* The feeds configuration section includes functions to sort the feeds and find when the next feed is.
* The fish feeding functions section includes a function to check if it's time to feed the fish and a function that rotates the fish feeder aka feeds the fish.
* The common menu functions section includes functions that were made to reduce code repetition.
* The main screen function is a function to reduce repetition in the main screen.
* The configure feed schedule menu functions includes functions that change the current feed schedule.
* The set the clock menu functions includes functions that change the current time or date.
*/
#ifndef MENU_HELPERS_HEADER
#define MENU_HELPERS_HEADER
#include <stdbool.h>
#include "operatingMode.h"
//Feeds configuration
void findNextFeed(operatingModeStruct *operatingMode); //Finds when the next feed is using the schedule and current time.
int compareTimes(const void *timeParam1, const void *timeParam2); //Custom function for the qsort method.
void sortScheduleTimes(operatingModeStruct *operatingMode); //Sorts the schedule times from smallest to largest.
//Fish feeding functions
void rotateFishFeeder(const int numberOfRotations,const bool ifCalledFromAuto,operatingModeStruct *operatingMode); //Rotates the fish feeder for a given amount of times.
void checkIfItsTimeToFeedFish(operatingModeStruct *operatingMode, int *previousMinute); //If it's time to feed the fish then does appropriately.
//Common menu functions
void incrementNumber(int *number, const int maxValue, const int minValue); //Increments the number given in a cycle like fashion using the max and min values.
void loopEnd(char *result,double *timeCounter); //Performs the necessary statements for the end of the menu while loops.
void resetVariables(double *timeCounter, int *previousMinute); //Resets the time counter and previous minute.
void resetGenericConfiguration(void (*displayFunction)(int), int currentSelection, double *timeCounter, int *previousMinute);
void interactionGenericConfiguration(void (*displayFunction)(int), int currentSelection, double *timeCounter);
//Functions used by main screen
void resetMainScreen(operatingModeStruct *operatingMode,double *timeCounter,int *previousMinute);
//Functions used by configure feed schedule menu
void configureGetTimeScreen(int currentSelection,double *timeCounter,int digits[4],char *bottomText);
bool findIfTimeConflict(const operatingModeStruct *operatingMode,int totalMinutes);
void scheduleGetRotationsScreen(operatingModeStruct *operatingMode,const int position);
void scheduleGetTimeScreen(operatingModeStruct *operatingMode,const int position);
void createNewScheduleScreen(operatingModeStruct *operatingMode);
void editCurrentSchedule(operatingModeStruct *operatingMode);
//Functions used by set the clock menu
void configureSetTheDateScreen(const int currentSelection,double *timeCounter,char *bottomText,int digits[8]);
void configureSetTheTimeScreen(const int currentSelection,double *timeCounter,char *bottomText,int digits[6]);
void setTheTime(operatingModeStruct *operatingMode);
void setTheDate(operatingModeStruct *operatingMode);

#endif //MENU_HELPERS_HEADER
