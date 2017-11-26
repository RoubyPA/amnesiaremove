#ifndef ARM_H
#define ARM_H

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PROGRAM "Amnesia remove"
#define VERSION "Beta 0.1"

#define DONE 0
#define ERROR -1

#define MAX_FILES 32
#define MAX_PATH_SIZE 256

#define DEFAULT_ITERATIONS 1

/* TODO move on error.h */
#define error(a) _error(a,__FILE__,__func__,__LINE__)

/* rm configuration */
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

/* rm options */
#define rm_short_opt "fiIrRdaVpPtnzhv"

static struct option const rm_long_opt[] = {
  {"help",        no_argument, NULL, 'h'},
  {"verbose",     no_argument, NULL, 'v'},
  {"force",       no_argument, NULL, 'f'},
  {"recursive",   no_argument, NULL, 'r'},
  {"dir",         no_argument, NULL, 'd'},
  {"amnesia",     no_argument, NULL, 'a'},
  {"random",      no_argument, NULL, 'p'},
  {"zero",        no_argument, NULL, 'z'},
  {"amnesia-verbose", no_argument, NULL, 'V'},
  {"random-path", required_argument, NULL, 'P'},
  {"interactive", optional_argument, NULL, 't'},
  {"iterations",  required_argument, NULL, 'n'},
  {NULL, 0, NULL, 0}
};

#endif /* ARM_H */
