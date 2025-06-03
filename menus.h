/**
* Created by Beck Chamberlain on 04/12/2024.
*
* This file provides functions that display and manage the menus.
* The user will 'start' in mainScreen then will be able to navigate through the other menus from there.
*/
#ifndef MENUS_HEADER
#define MENUS_HEADER
#include "operatingMode.h"
void selectOperatingModeMenu(operatingModeStruct *operatingMode); //Menu for changing the current operating mode.
void configureFeedScheduleMenu(operatingModeStruct *operatingMode); //Menu for configuring the feed schedule.
void setTheClockMenu(operatingModeStruct *operatingMode); //Menu to choose if you want to change the date or change the time.
void configurationMenu(operatingModeStruct *operatingMode); //The configuration menu which lets you choose between accessing the above menus.
void mainScreen(operatingModeStruct *operatingMode); //The main menu.
#endif //MENUS_HEADER
