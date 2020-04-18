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
 *
 * Last Update: 04/2020
 *
 */


#ifndef _COMMON_
#define _COMMON_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <list>
#include <map>
#include <iterator>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

#define MAX_BUF 1024 // Maximum string buffer size

#define VERSION      "0.01"
#define PROGRAM_NAME "Census a Multiplex Network"

#define SEPARATOR    "--------------------------------------------"

// Limits
#define MIN_SG_SIZE  2
#define MAX_SG_SIZE 20
#define MAX_LAYERS 50
#define MAX_RES 500
#define MAX_TIMERS 5

#define BAR_SIZE 20
#define WIDTH 44

#define ERROR_HEADER "Error: "

#define INVALID_FILE   "__NULL__"  // Invalid file name string

#define DEFAULT_OUTPUT "output"    // Default name for results file
#define DEFAULT_EXTENSION ".html"
#define DEFAULT_FOLDER "HTML"

using namespace std; // Could be avoided if wanted

typedef map<string, int> mapStringInt;

typedef struct {
    char *s;
    int freq;
} Results_struct;

#endif
