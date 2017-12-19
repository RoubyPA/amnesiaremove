#define _DEFAULT_SOURCE

#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "remove.h"
#include "error.h"
#include "random.h"

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
int
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

/* Remove empty dir */
void
_rmdir (char *rep)
{
  assert (rep != NULL);

  if (rmdir (rep) == -1)
    error ("rmdir");
  
  if (conf.verbose)
    printf ("removed directory \'%s\'\n", rep);
}

/* Remove a file */
int
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
