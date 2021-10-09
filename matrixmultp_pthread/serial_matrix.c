#include <stdio.h>
#include <stdlib.h>

int main()
{
	int A[5][5] = {{1,2,3,4,5},{5,4,3,2,1}, {1,2,3,4,5}, {5,4,3,2,1}, {1,2,3,4,5}};
	int x[5] = {9,8,7,6,5};

	int y[5]={0};


	int i,j;
	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			y[i] +=A[i][j]*x[j];

	for(i=0; i<5; i++)
		printf("%d\n", y[i]);

	return 0;

}
