/**
* Created by Beck Chamberlain on 07/12/2024.
*
* This file provides functions that start up the program with information from a file.
*/

#ifndef PROGRAM_STARTUP_HEADER
#define PROGRAM_STARTUP_HEADER
void initialiseProgram(const long warmStart); //Sets the time up and fills the fish feeder.
void loadFromEEPROM(const char *filename,operatingModeStruct *operatingMode); //Sets the operating mode up.
#endif //PROGRAM_STARTUP_HEADER
