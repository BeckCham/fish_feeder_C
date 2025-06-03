/**
 * Created by Neal Snooke on 12/07/2024.
 * Version 0.82
 * This file provides functions for the fish feeder hardware simulator
 * The signatures match (some of) those used in the arduino library to access:
 *   DS3201 real time clock chip (virtuabotixRTC.h)
 *   128x64 OLED displays (GOFi2cOLED.h)
 *   plus functions to check the button GPIO input and send pulse sequences to the
 *   ULN2003 stepper motor driver
 *   Additionally a function is provided to simulate the user filling the feeder.
 *
 * The utility functions section includes functions to set up the simulator
 * and control debug output (both in the GUI simulator debug area and on the console)
 */

// display functions for the 128x64 OLED display
void displayClear();
void displayColour(char* fg, char* bg); // set foreground and background colour. Only "BLACK","WHITE",""
void displayText(int x, int y, char* text, int size); // pixel coordinates; size can be 1 or 2 only
void displayPixel(int x, int y); // set an individual pixel;
void displayLine(int x, int y, int x1, int y1); // draw a line between any two coordinates on the display
void displayClearArea(int x, int y, int w, int h); // clear part of the display to the background colour

// real time clock (RTC) functions
// set the clock.
void clockSet(int sec, int min, int hour, int day, int month, int year);
// retrieve a specific element of the time/date
int clockSecond();
int clockMinute();
int clockHour();
int clockDay();
int clockMonth();
int clockYear();
int clockDayOfWeek(); // Sunday = 0, Monday = 1, etc
// to maintain a clock between executions of the emulator save the value returned by this function
// when given 0. restore the clock by calling the function with the previously saved value.
// the clock will have continued to keep time.
// note the result value will not change unless the clock is set to a different time with clockSet()
long long clockWarmStart(long long offset); // fix nns 29/11/2024 change to long long type

// mechanical feeder functions
void motorStep(); // rotates the feeder container one step (1 degree)
void foodFill(int foodLevel); // set food level. range 1 - 60%

// button function
// returns one of "SHORT_PRESS" "LONG_PRESS" "NO_PRESS"
char* buttonState(); // note caller must dispose of char* result

//------------------
// utility functions
//------------------

// info area function to display information in the GUI debug info area (if enabled)
void infoMessage(char *text); // display an information message

// this function is where the main users code goes and must be implemented to use the simulator
void userProcessing();

// jni functions for the JavaFX fish feeder simulation GUI
// must be called once only and in sequence since jniSetup spawns a
// new thread for user processing allowing the GUI to run in the main thread.
int jniSetup(); // setup the JavaFX GUI and then run userProcessing() once GUI is initialised
int javaFx(); // start the JavaFX GUI - must be called after jniSetup returns when GUI quits

// delay for a specified number of milliseconds
int msleep(long msec);

// it is possible to output various levels of debug info from the Fish GUI Emulator Java and C code
// the following constants are used to select what to output to the console log.
// the debug level is a single integer, with information selected by bitwise OR of the following constants
__attribute__((unused)) extern const int THREAD_NAME; // append the thread name to all log messages
extern const int THREAD_ID; // append the thread id to all log messages
extern const int METHOD_ENTRY; // method entry and exit messages
extern const int JNI_MESSAGES; // Java Native Interface messages
extern const int JFX_MESSAGES; // JavaFX processing messages
extern const int GENERAL;
extern const int STACK_INFO; // stack trace information
extern const int GUI_INFO_DEBUG; // show the GUI debug window

extern int  log_level; // global that stores the current log level setting

// add a log entry for a given log level. l is one of the constants specified above
void logAdd(int level, char* message);

// select a log level by bitwise OR of the above constants
void logAddInfo(int level);
// stop logging a specified level. l is one of the constants specified above
void logRemoveInfo(int level);


