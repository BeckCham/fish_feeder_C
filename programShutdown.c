/**
* Created by Beck Chamberlain on 07/12/2024.
*
* This function mimics saving to EEPROM.
*/
#include <stdio.h>
#include "fish.h"
#include "programShutdown.h"
/**
 * This function saves the operating mode in a file.
 * This allows the program to be set up in the same way when it starts up again.
 *
 * @param filename The name of the file that is to be saved to.
 * @param operatingMode The operating mode that the information is got from.
 */
void saveToEEPROM(const char *filename, const operatingModeStruct *operatingMode) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        //If the file can't be written to
        printf("Error opening file\n");
    } else {
        fprintf(file, "%lld %d %d %d %d", clockWarmStart(0), operatingMode->mode, operatingMode->numberOfFeedsInADay,
                operatingMode->autoFeedsDone, operatingMode->nextFeed);
        for (int i = 0; i < operatingMode->numberOfFeedsInADay; i++) {
            fprintf(file, " %d %d %d", operatingMode->feedTimes[i].rotations, operatingMode->feedTimes[i].hour,
                    operatingMode->feedTimes[i].minute);
        }
        fclose(file); //Close the file.
    }
}
