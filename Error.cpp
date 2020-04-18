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
 * Error Utilities
 *
 * Last Update: 04/2020
 *
 */


#include "Error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


void Error::msg(const char *format, ...) {
  va_list p;
  if (format == NULL)
    msg("%s", strerror (errno));
  else {
      fprintf (stderr, ERROR_HEADER);
      va_start (p, format);
      vfprintf (stderr, format, p);
      va_end (p);
      fprintf (stderr, "\n");
    }

  exit(EXIT_FAILURE);
}
