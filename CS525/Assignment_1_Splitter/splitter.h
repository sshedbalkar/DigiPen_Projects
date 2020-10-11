#ifndef SPLITTER_H
#define SPLITTER_H

#include <stddef.h>

enum 
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};

int SplitFile(char*  filename,  char* output, size_t size);
int JoinFiles(char** filenames, char* output);

#endif 

