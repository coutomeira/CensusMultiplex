/*------------------------------------------------------------------------------------------------
|         _       _     ___                                            _ _   _      _            |
|     _ _| |_ _ _(_)___/ __| __ __ _ _ _  _ _  ___ _ _       _ __ _  _| | |_(_)_ __| |_____ __   |
|   / _` |  _| '_| / -_)__ \/ _/ _` | ' \| ' \/ -_) '_|     | '  \ || | |  _| | '_ \ / -_) \ /   |
|   \__, |\__|_| |_\___|___/\__\__,_|_||_|_||_\___|_|       |_|_|_\_,_|_|\__|_| .__/_\___/_\_\   |
|   |___/                                                                     |_|                |
|                                                                                                |
|------------------------------------------------------------------------------------------------|
|   Pedro Ribeiro ( pribeiro@dcc.fc.up.pt )    &    Andre Couto Meira ( up2014048772fc.up.pt )   |
-------------------------------------------------------------------------------------------------*/
/*
 * Reduction Utilities
 *
 * Last Update: 04/2020
 *
 */


#ifndef _TIMER_
#define _TIMER_


#include "Common.h"

#include <sys/time.h>


class Timer {
private:

    static struct timeval cstart[MAX_TIMERS]; // Start times
    static struct timeval cend[MAX_TIMERS]; // End times

public:

    static void start(int n); // Start the clock of a timer
    static void stop(int n); // Stop the clock of a timer
    static double elapsed(int n); // Elapsed time of a timer
};

#endif
