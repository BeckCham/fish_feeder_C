/**
* Created by Beck Chamberlain on 20/11/2024.
*
* The main entry point function's:
* main() , the program entry point and userProcessing(), the entry point for the fish feeder main logic.
*/
#include <stdio.h>
#include <stdlib.h>
#include "displayScreens.h"
#include "fish.h"
#include "operatingMode.h"
#include "menus.h"
#include "programShutdown.h"
#include "programStartup.h"


/**
 * This function is the entry point for the fish feeder C program main logic.
 * It is called by jniSetup() from main, once the GUI thread has been initialised.
 *
 * The splash screen is displayed, it attempts to read the 'fakeEEPROM.txt', then the main screen is displayed.
 *
 * When the main screen is exited the current operating modes information is saved to 'fakeEEPROM.txt'.
 */
void userProcessing() {
    displaySplashScreen();
    //Creates an instance of operatingModeStruct which will be used throughout the code.
    operatingModeStruct operatingMode;
    loadFromEEPROM("fakeEEPROM.txt", &operatingMode); //Reads the save file.
    mainScreen(&operatingMode); //Enters the fish feeder main screen.

    saveToEEPROM("fakeEEPROM.txt", &operatingMode); //Saves operating mode information.
}


/**
 * main entry point for the Fish Feeder Emulator program
 *
 * @return "EXIT_SUCCESS"
 */
int main() {
    // turn on all console logging for the Java and fish feeder JNI C code. Shouldn't need this
    //logAddInfo(GENERAL | METHOD_ENTRY | JNI_MESSAGES | JFX_MESSAGES | THREAD_NAME | THREAD_ID | STACK_INFO);
    //logAddInfo( GENERAL | JFX_MESSAGES | THREAD_NAME | THREAD_ID );
    //logRemoveInfo(METHOD_ENTRY); // don't show these debug messages in the gui

    // add the GUI debug window showing messages received, If needed uncomment it
    //logAddInfo( GUI_INFO_DEBUG);

    // add a log entry for entry to this method
    logAdd(METHOD_ENTRY, "main(). test of Fish Feed Hardware Emulator using a JavaFX GUI and jni");

    // start the JVM and set up the JNI environment
    // this will result in the userProcessing() function being called to run the C part of the program
    if (jniSetup() != 0) return 1;

    // hand over this thread to Java FX to create the fish hardware emulator GUI.
    // FX will terminate the application when the main window is closed
    // or the C application calls the FishFeederEmulator.exit() function.
    javaFx();

    return (EXIT_SUCCESS);
}
