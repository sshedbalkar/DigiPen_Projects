#include <stdio.h>
#include "splitter.h"

/*{{{*/
int SplitFile(char* filename, char* output, size_t size) {
  char current_file_name[256];
  int file_count=0,i=0;
  printf("split %s into chunks of %d named\n",filename,size);
  for (;i<5;++i) {
    /* print in to a string. The format is string(%s) 
     * and a 4-digit integer padded with 0's on the left, i.e.
     * 1 is printed as 0001, 123 as 0123, note that 12345 is printed as 12345 */
    sprintf(current_file_name,"%s%04lu\n",output,file_count++);
    /* What if we use this line below?
     * sprintf(current_file_name,"%s%04lu\n",output,++file_count); */
    printf("%s",current_file_name);
  }
  printf("...\n");
  return 0;
}
/*}}}*/

/*{{{*/
int JoinFiles(char** filenames, char* output) {
  printf("join\n");
  while (*filenames) {
    printf("%s\n",*filenames++);
  }
  printf("into %s\n",output);
  return 0;
}
/*}}}*/
