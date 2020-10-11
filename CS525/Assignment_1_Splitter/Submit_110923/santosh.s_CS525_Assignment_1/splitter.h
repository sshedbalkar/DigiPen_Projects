#ifndef SPLITTER_H
#define SPLITTER_H

#include <stddef.h>

/*!
	\enum RETURN_CODES
	\brief enumeration of return values

	\param E_BAD_SOURCE input file cannot be opened
	\param E_BAD_DESTINATION output file cannot be opened
	\param E_NO_MEMORY allocating memory has failed
	\param E_NO_ACTION no action provided by the user
	\param E_SMALL_SIZE **
*/
enum RETURN_CODES
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};

#ifdef __cplusplus
extern "C"
{
#endif
	/*!
		\fn int SplitFile(char* filename, char* output, size_t size)
		\brief SplitFile Splits a large file into chunks of desired size

		\param filename The pointer to the name of the file to be split
		\param output The pointer to the name suffix of the chunk files
		\param size The size of each chunk
	*/
	int SplitFile(char*  filename,  char* output, size_t size);

	/*!
		\fn int JoinFiles(char** filenames, char* output)
		\brief JoinFiles joins various chunks into one file

		\param filenames This is a pointer to an array of file names to be joined
		\param output This is a pointer to the output file name
	*/
	int JoinFiles(char** filenames, char* output);

#ifdef __cplusplus
}
#endif

#endif 
