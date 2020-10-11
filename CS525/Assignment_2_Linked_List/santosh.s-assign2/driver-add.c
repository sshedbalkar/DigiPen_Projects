#include "spvector.h"
#include <stdio.h>  /* printf, NULL   */
#include <stdlib.h>
#include <string.h>
#include <time.h>



int main( ) {
    ElementNode_handle v1 = NULL, v2 = NULL, result = NULL;
    int i = 0;
    int size = 1000000;

    for ( i=size; i>=0; --i ) {
        insert_element( &v1, i, size-i );
        insert_element( &v2, i, size-i );
    }

    result = add( v1,v2 );
    /*printf_elements(result,"%8d\n", 10); */
    printf("finished\n");

    return 0;
}
