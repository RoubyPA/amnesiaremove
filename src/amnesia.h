#ifndef AMNESIA_H
#define AMNESIA_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <assert.h>

#define PROGRAM "Amnesia remove"
#define VERSION "Beta 0.1"

#define DONE 0
#define ERROR -1

#define MAX_FILES 32
#define MAX_PATH_SIZE 256

#define DEFAULT_ITERATIONS 1

/* arm configuration */
struct amnesia_opt
{
  bool actived; /* active overwrite file */
  bool random;  /* overwrite with random data */
  bool zero;    /* add last overwrite with 0 */
  bool verbose; /* amnesia verbose mode */
  int  iterations; /* number of overwrite pass */
  char random_path[MAX_PATH_SIZE]; /* path to random source */
};
  
struct rm_opt
{
  bool verbose;
  bool force;
  bool recursive;
  bool dir;
  short interactive;
  struct amnesia_opt amnesia; /* amnesia options */
};

/* Global rm_opt structure */
struct rm_opt conf;

#endif /* AMNESIA_H */
