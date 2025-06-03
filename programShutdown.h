/**
* Created by Beck Chamberlain on 07/12/2024.
*
* This file provides a function that saves the operating mode information to a file.
*/
#ifndef PROGRAM_SHUTDOWN_HEADER
#define PROGRAM_SHUTDOWN_HEADER
#include "operatingMode.h"
void saveToEEPROM(const char *filename, const operatingModeStruct *operatingMode); //Saves info to file.
#endif //PROGRAM_SHUTDOWN_HEADER
