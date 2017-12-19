#ifndef REMOVE_H
#define REMOVE_H

#include "amnesia.h"

int overwrite (FILE *file, long len);
void _rmdir (char *rep);
int remove_file (char *file_path);
void recursive_remove (char *rep);

#endif /* REMOVE_H */
