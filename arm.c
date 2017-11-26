#include "arm.h"

void
_error (char *msg, const char *src_file, const char *function, int line)
{
  fprintf (stderr, "Error: [%s] %s:%d => %s: (%d)%s\n",
	   src_file, function, line, msg, errno, strerror(errno));
  exit (EXIT_FAILURE);
}

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
  /*   printf ("  -i\t\t prompt before every removal\n"); */
  /*   printf ("  -I\t\t prompt once before removing more than three files, or\n" */
  /*           "\t\t when removing recursively\n"); */
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
	  "<http://github.com/RoubyPA/amnesiaremove>\n"); /* TODO create repo ! */
  version ();
  exit (EXIT_SUCCESS);
}

/* Get a new random byte */
static char
get_random_char ()
{
  static float tmp;
  srand (time(NULL) + rand ()); /* change seed */
  tmp = (float)rand () / (float)RAND_MAX; /* random number between 0 and 1 */ 

  return ((char)(tmp * CHAR_MAX));
}

long
file_len (FILE *file)
{
  assert (file != NULL);

  long len;

  if (fseek (file, 0, SEEK_END) == -1)
    error ("fseek -> SEEK_END");

  len = ftell (file);
  if (len == -1)
    error ("ftell");

  if (fseek (file, 0, SEEK_SET) == -1)
    error ("fseek -> SEEK_SET");

  return len;
}

/* Overwrite a file */
static int
overwrite (FILE *file, long len)
{
  assert (file != NULL);

  unsigned char data = 0;
  
  /* overwrite with random data */
  for (int i = 0 ; i < conf.amnesia.iterations ; i++)
    {
      for (int l = 0 ; l < len ; l++)
        {
          data = get_random_char ();
          fputc (data, file);
        }

      if (conf.amnesia.verbose == true)
	printf ("*");
      
      if (fseek (file, 0, SEEK_SET) == -1)
	error ("fseek -> SEEK_SET");
    }
  
  /* overwrite with 0 */
  if (conf.amnesia.zero == true)
    {
      data = 0;
      if (fseek (file, 0, SEEK_SET) == -1)
	error ("fseek -> SEEK_SET");

      if (conf.amnesia.verbose)
	printf ("* ");
      
      for (int l = 0 ; l < len ; l++)
	fputc (data, file);
    }
  
  return DONE;
}

static void
_rmdir (char *rep)
{
  assert (rep != NULL);

  if (rmdir (rep) == -1)
    error ("rmdir");
  
  if (conf.verbose)
    printf ("removed directory \'%s\'\n", rep);
}

/* Remove a file */
static int
remove_file (char *file_path)
{
  assert (file_path != NULL);

  FILE *file = NULL;
  long fsize = 0;

  file = fopen (file_path, "r+");
  if (file == NULL)
    error ("fopen");

  /* overwrite */
  if (conf.amnesia.actived)
    {
      fsize = file_len (file);
      if (fsize != 0)
	overwrite (file, fsize);
    }
  
  if (unlink (file_path) == -1)
    error ("unlink");
  
  if (conf.verbose)
    printf ("removed \'%s\'\n", file_path);
  
  return DONE;
}

/* Remove folder and all content files */
void
recursive_remove (char *rep)
{
  assert (rep != NULL);

  char *path = malloc (256);
  if (path == NULL)
    error ("malloc");
  
  DIR *dir;
  struct dirent *dir_info;

  dir = opendir (rep);
  if (dir == NULL)
    error ("opendir");

  while ((dir_info = readdir (dir)) != NULL)
    {
      if (strcmp (dir_info->d_name, ".") == 0 ||
          strncmp (dir_info->d_name, "..", 2) == 0)
        continue;

      strcpy (path, rep);
      /* HACK dirty... */
      if (rindex (path, '/') != (path+strlen (path)-1))
	strcat (path, "/");
      strcat (path, dir_info->d_name);

      if (dir_info->d_type == DT_DIR)
	recursive_remove (path);
      else if (dir_info->d_type == DT_REG)
	remove_file (path);
      /* TODO other file type */
      /* else if (dir_info->d_type == DT_LNK) */
      /* 	remove_file (dir_info); */
      else /* TODO delete */
	error ("format not implemented yet");
    }

  _rmdir (rep);
  
  free (path);
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
      if (conf.recursive)
	recursive_remove (files[i]);
      else if (conf.dir)
      	_rmdir (files[i]);
      else
	remove_file (files[i]);
    }

  exit (EXIT_SUCCESS);
}
