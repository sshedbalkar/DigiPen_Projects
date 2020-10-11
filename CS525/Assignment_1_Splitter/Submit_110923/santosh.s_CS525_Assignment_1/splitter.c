#include <stdio.h>
#include <stdlib.h>

#include "splitter.h"

#define BUFFER_SIZE 4096

/*!
	\fn int SplitFile(char* filename, char* output, size_t size)
	\brief SplitFile Splits a large file into chunks of desired size

	\param filename The pointer to the name of the file to be split
	\param output The pointer to the name suffix of the chunk files
	\param size The size of each chunk
*/
int SplitFile(char* filename, char* output, size_t size)
{
	char current_file_name[256];
	char *prefix;	/**< stores a valid output name prefix */
	int file_count=0;
	FILE *fSource, *fDest = 0;
	
	char buffer[BUFFER_SIZE];
	int bytes_read = 0;
	int total_bytes_read = 0;
	
	size_t bytes_left_to_read = size;	/**< keeps track of the bytes still left to read to finish writing the current file chunk */

	size_t bytes_to_read;	/**< It is the amount of bytes needed to be read in one fread() function call */
	size_t item_size = sizeof(char), item_count, items_written;
	
	printf("split %s into chunks of %d named\n",filename,size);

	/*!
		\brief if filename is null or the file cannot be opened then show error and quit
	*/
	if(!filename || !(fSource = fopen(filename, "rb")))
	{
		perror(filename);
		printf("File %s cannot be opened for reading\n", filename);
		return E_BAD_SOURCE;
	}
	else
	{
		prefix = (output && *output != '\0') ? output : filename;
		
		/*!
			\brief Read from the source file as long as there are valid bytes read
		*/
		do
		{
			bytes_to_read = (bytes_left_to_read > BUFFER_SIZE) ? BUFFER_SIZE : bytes_left_to_read;
			
			/*!
				\brief Read data from the source file 'bytes_to_read' bytes at a time
			*/
			bytes_read = fread(buffer, item_size, bytes_to_read/item_size, fSource);
			if(bytes_read > 0)
			{
				bytes_left_to_read -= bytes_read;	/**< update the bytes_left_to_read variable */
				
				/*!
					\brief if a destination file is not present then create one
				*/
				if(!fDest)
				{
					sprintf(current_file_name,"%s%04lu", prefix, (long unsigned int)file_count++);
					
					if(!(fDest = fopen(current_file_name, "wb")))
					{
						perror(current_file_name);
						printf("File %s cannot be opened for writing\n", current_file_name);
						return E_BAD_DESTINATION;
					}
					printf("Created file: %s\n", current_file_name);
				}
				
				/*!
					\brief write into the destination file 'bytes_read' bytes at a time from the buffer
				*/
				items_written = fwrite(buffer, item_size, (item_count = bytes_read/item_size), fDest);
				
				/*!
					\brief if the total bytes written is not equal to the total bytes read then an error has occurred while writing
				*/
				if(items_written != item_count)
				{
					perror(current_file_name);
					printf("Error encountered while writing to File: %s\n", current_file_name);
					return E_BAD_DESTINATION;
				}
				
				/*!
					\brief If the destination file size becomes equal to the max chunk size then close the file and update variables for the next iteration
				*/
				if(bytes_left_to_read < 1)
				{
					printf("	Closing file: %s\n", current_file_name);
					fclose(fDest);
					fDest = 0;
					bytes_left_to_read = size;
				}
			}
			total_bytes_read += bytes_read;
		} while (bytes_read > 0);
		
		/*!
			\brief If a destination file is already opened then close it
		*/
		if(fDest)
		{
			fclose(fDest);
		}
		printf("total bytes read: %d\n", total_bytes_read);
		
		/*!
			\brief Check if the exit from the loop is because of an error
		*/
		if(!feof(fSource))
		{
			printf("There was an error at byte %i while reading from file %s\n", total_bytes_read, filename);
			return E_BAD_SOURCE;
		}
		else
		{
			/*!
				\brief Source file has been read successfully. Now close it
			*/
			fclose(fSource);
			printf("Files created successfully");
		}
	}
	
	return 0;
}

/*!
	\fn int JoinFiles(char** filenames, char* output)
	\brief JoinFiles joins various chunks into one file

	\param filenames This is a pointer to an array of file names to be joined
	\param output This is a pointer to the output file name
*/
int JoinFiles(char** filenames, char* output)
{
	FILE *fSource, *fDest = 0;
	int total_bytes_read = 0;
	
	char buffer[BUFFER_SIZE];
	int bytes_read = 0;
	size_t item_size = sizeof(char), item_count, items_written;
	
	char *current_file_name = (output && *output != '\0') ? output : "_combined_";	/**< current_file_name is a pointer to a valid file name for the output file */
	
	/*!
		\brief Do the following as long as there are valid file names in the array
	*/
	while (*filenames)
	{
		/*!
			\brief Try to open the current file chunk
		*/
		if(!(fSource = fopen(*filenames, "rb")))
		{
			perror(*filenames);
			printf("File %s cannot be opened for reading\n", *filenames);
			return E_BAD_SOURCE;
		}
		else
		{
			printf("Opened file: %s\n", *filenames);
			
			/*!
				\brief Do the following as long as there are bytes to read from ths current file chunk
			*/
			do
			{
				bytes_read = fread(buffer, item_size, BUFFER_SIZE/item_size, fSource);	/**< Read from the file chunk 'BUFFER_SIZE' bytes at a time */
				
				/*!
					\brief If the total number of bytes read is a valid number then write them to the output file
				*/
				if(bytes_read > 0)
				{
					/*!
						\brief If there is no output file present then create one
					*/
					if(!fDest)
					{
						if(!(fDest = fopen(current_file_name, "wb")))
						{
							perror(current_file_name);
							printf("File %s cannot be opened for writing\n", current_file_name);
							return E_BAD_DESTINATION;
						}
						printf("Created file: %s\n", current_file_name);
					}
					
					/*!
						\brief Write the bytes read to the output file
					*/
					items_written = fwrite(buffer, item_size, (item_count = bytes_read/item_size), fDest);
					
					/*!
						\brief If the total bytes written is not equal to the bytes read then there was a problem while writing
					*/
					if(items_written != item_count)
					{
						perror(current_file_name);
						printf("Error encountered while writing to File: %s\n", current_file_name);
						return E_BAD_DESTINATION;
					}
				}
				total_bytes_read += bytes_read;
			} while (bytes_read > 0);
			
			/*!
				\brief Check if the exit from the loop is because of an error
			*/
			if(!feof(fSource))
			{
				printf("There was an error at byte %i while reading from file %s\n", total_bytes_read, *filenames);
				return E_BAD_SOURCE;
			}
			else
			{
				/*!
					\brief File joining was successful, close the file chunk and update the pointer to the next file chunk name
				*/
				fclose(fSource);
				++filenames;
			}
		}
	}
	printf("total bytes read: %d\n", total_bytes_read);
	
	/*!
		\brief Close the output file
	*/
	if(fDest)
	{
		fclose(fDest);
	}
	printf("into %s\n",output);
	return 0;
}
