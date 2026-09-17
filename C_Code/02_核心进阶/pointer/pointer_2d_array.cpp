#include<stdio.h>
int main()
{
	int arr[2][3] = { {1,2,3}, {4,5,6} }, (*p)[3] = arr;
	int i, j;
	for (i = 0;i < 6;i++)
	{
		printf("%d\n", (*p)[i]);
	}
	printf("\n");
	for (j = 0;j < 2; j++)
	{
		for (i = 0;i < 3; i++)
		{
			printf("%d ", *(*(p + j) + i));
		}
		printf("\n");
	}
	printf("\n");
	for (j = 0;j < 2; j++)
	{
		for (i = 0;i < 3; i++)
		{
			printf("%d ", p[j][i]);
		}
		printf("\n");
	}
	return 0;
}