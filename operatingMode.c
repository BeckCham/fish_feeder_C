/**
* Created by Beck Chamberlain on 21/11/2024.
*
* Basic initialise functions, used when the program starts if there is no 'fakeEEPROM.txt' to ensure errors don't occur due to the operating mode
* or the operating modes feedTimes array not being properly initialised. Also used when blank time is needed.
*/
#include "operatingMode.h"

void initialiseTime(timeStruct *time, const int hour, const int minute, const int rotations) {
    time->hour = hour;
    time->minute = minute;
    time->rotations = rotations;
}

void initialiseOperatingMode(operatingModeStruct *operatingMode, const int mode, const int nextFeed,
                             const int numberOfFeedsInADay, timeStruct feedTimes[9], const int autoFeedsDone) {
    operatingMode->mode = mode;
    operatingMode->nextFeed = nextFeed;
    operatingMode->numberOfFeedsInADay = numberOfFeedsInADay;
    operatingMode->autoFeedsDone = autoFeedsDone;
    for (int i = 0; i < numberOfFeedsInADay; i++) {
        operatingMode->feedTimes[i] = feedTimes[i];
    }
}
