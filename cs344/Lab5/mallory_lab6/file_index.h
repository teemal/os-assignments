// R. Jesse Chaney

#include "file_struct.h"

#ifndef _FILE_INDEX_H
# define _FILE_INDEX_H

# define INDEX_ID_LEN ID_LEN
# ifndef TRUE
#  define TRUE 1
# endif // TRUE
# ifndef FALSE
#  define FALSE 0
# endif // FALSE

typedef struct file_index_s {
  char id[INDEX_ID_LEN];     // A "unique" id for each person in the file.

  off_t offset;              // The offset in bytes from the beginning of the
                             // input file where the given id occurs.
} file_index_t;

#endif // _FILE_INDEX_H
