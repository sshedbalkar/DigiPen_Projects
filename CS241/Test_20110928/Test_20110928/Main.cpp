#include <stdio.h>
#include <conio.h>


int main(void)
{

	//
	getch();
}
int *[] f1(const int (*a)[4], const int (*b)[4])
{
	int *c[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

	int i = 0;
	int j = 0;
	int k = 0;
	
	for(i = 0; i < 4; i++) 
		for( j = 0; j < 4; j++)
			for( k = 0; k < 4; k++) 
				c[i][j] +=  (a[i][k]) * (b[k][j]);

	return c;
}