#ifndef ARM_H
#define ARM_H

/* arm options */
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
