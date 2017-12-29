#include "amnesia.h"
#include "arm.h"
#include "error.h"
#include "random.h"
#include "remove.h"

void
version ()
{
  printf (VERSION);
  printf ("  Build: %s %s\n", __TIME__, __DATE__);
}

void
usage (char *prog)
{
  printf ("Usage: %s [OPTION]... [FILE]...\n", prog);
  printf ("Remove (unlink) the file(s).\n\n");
  printf ("  -f, --force\t igniore nonexistent files and arguments, \n"
	  "never prompt\n");
  printf ("  -i\t\t prompt before every removal\n");
  printf ("  -I\t\t prompt once before removing more than three files, or\n"
	  "\t\t when removing recursively\n");
  /* TODO interactive long opt */
  printf ("  -r, -R, --recursive\t remove directories and their contents \n"
	  "recursively\n");
  printf ("  -d, --dir\t remove empty directories\n");
  printf ("  -v, --verbose\t explain what is being done\n");
  printf ("  -h, --help\t display this help and exit\n");
  printf ("\n");
  /* Amnesia options */
  printf ("Amnesia options to overwrite file(s):\n");
  printf ("  -a, --amnesia\t active the amnesia mode, this mode overwrite the\n"
	  "\t\t file(s) before unlink\n");
  printf ("  -p, --random\t overwrite file(s) with random data (default)\n");
  /* printf ("  -P, --random-source=FILE set the random source\n") */;
  printf ("  -n, --iterations=N\t overwrite N times, (default 1)\n");
  printf ("  -z, --zero\t add a final overwrite with zeros\n");
  printf ("\n");
  printf ("\"Amnesia remove\" bugs report: "
	  "<http://github.com/RoubyPA/amnesiaremove>\n\n");
  version ();
  exit (EXIT_SUCCESS);
}

void
init_rm_option ()
{
  conf.verbose = false;
  conf.force = false;
  conf.recursive = false;
  conf.dir = false;
  conf.interactive = false;
  conf.amnesia.actived = false;
  conf.amnesia.random = true;
  conf.amnesia.zero = false;
  conf.amnesia.verbose = false;
  conf.amnesia.iterations = DEFAULT_ITERATIONS;
  strcpy (conf.amnesia.random_path, "");
}

/* Main function */
int
main (int argc, char **argv)
{
  int long_index;
  int index;
  char optchar;
  char **files;

  init_rm_option ();
  
  if (argc < 2)
    usage (argv[0]);
  
  while ((optchar = getopt_long(argc, argv, rm_short_opt,
				rm_long_opt, &long_index)) != -1)
    {
      switch (optchar)
	{
	case 'h':
	  usage (argv[0]);
	  break;
	case 'v':
	  conf.verbose = true;
	case 'f':
	  conf.force = true;
	  break;
	case 'i':
	  conf.interactive = 1;
	  break;
	case 'I':
	  conf.interactive = 2;
	  break;
	case 'r':
	  conf.recursive = true;
	  break;
	case 'R':
	  conf.recursive = true;
	  break;
	case 'd':
	  conf.dir = true;
	  break;
	case 't':
	  /* TODO interactive mode select */
	  break;
	case 'a':
	  conf.amnesia.actived = true;
	  break;
	case 'V':
	  conf.amnesia.verbose = true;
	case 'p':
	  conf.amnesia.random = true;
	  break;
	case 'P':
	  strcpy (conf.amnesia.random_path, optarg);
	  break;
	case 'z':
	  conf.amnesia.zero = true;
	  break;
	case 'n':
	  if (atoi(optarg) == 0)
	    error ("Invalid argument to -n (--iteration) options");
	  conf.amnesia.iterations = atoi(optarg);
	  break;
	}
    }
    
  /* HACK not optimized */
  for (index = 1 ; index < argc ; index++)
    {
      if (strncmp("-- ", argv[index], 3) == 0)
        break;    /* is argument like "-- -foo" */
      else if (strncmp("--", argv[index], 2) == 0)
        continue; /* is long option */
      else if (strncmp ("-", argv[index], 1) == 0)
        continue; /* is short option */
      else
        break;    /* is argument */
    }

  files = malloc (argc-index);
  if (files == NULL)
    error ("malloc");

  for (int i = 0 ; argv[index+i] != NULL ; i++)
    {
      files[i] = malloc (strlen(argv[index+i])+1);
      if (files[i] == NULL)
	error ("malloc");
      
      strcpy (files[i], argv[index+i]);
      files[i+1] = NULL; /* last element == NULL */
    }
  
  for (int i = 0 ; files[i] != NULL ; i++)
    { 
      if (conf.interactive == 2)
	if (!interactive(files[i]))
	  break;
      
      if (conf.recursive)
	recursive_remove (files[i]);
      else if (conf.dir)
      	_rmdir (files[i]);
      else
	remove_file (files[i]);
    }

  free (*files);
  
  exit (EXIT_SUCCESS);
}
