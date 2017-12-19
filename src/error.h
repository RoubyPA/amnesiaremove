#ifndef ERROR_H
#define ERROR_H

#define error(a) _error(a,__FILE__,__func__,__LINE__)

void _error (char *msg, const char *src_file, const char *function, int line);

#endif /* ERROR_H */
