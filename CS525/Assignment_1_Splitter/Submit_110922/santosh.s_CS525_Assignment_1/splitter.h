#ifndef SPLITTER_H
#define SPLITTER_H

#include <stddef.h>

/**
*	@enum enumeration of return values
*
*	@code E_BAD_SOURCE input file cannot be opened
*	@code E_BAD_DESTINATION output file cannot be opened
*	@code E_NO_MEMORY allocating memory has failed
*	@code E_NO_ACTION no action provided by the user
*	@code E_SMALL_SIZE **
*/
enum 
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};

/**
*	@function SplitFile Splits a large file into chunks of desired size
*
*	@param filename pointer to the name of the file to be split
*	@param output pointer to the name suffix of the chunk files
*	@param size size of each chunk
*/
int SplitFile(char*  filename,  char* output, size_t size);

/**
*	@function JoinFiles joins various chunks into one file
*
*	@param filenames is a pointer to an array of file names to be joined
*	@param output is a pointer to the output file name
*/
int JoinFiles(char** filenames, char* output);

#endif 
