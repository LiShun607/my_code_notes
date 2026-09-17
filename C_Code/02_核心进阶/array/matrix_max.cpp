#include<stdio.h>
int main()
{
	int a[3][4], j, i;
	for (j = 0;j < 3;j++)
	{
		for (i = 0;i < 4;i++)
		{
			printf("请输入第%d行第%d列个整数:",j+1,i+1);
			scanf_s("%d", &a[j][i]);
		}
	}
	int max = a[0][0], max_xbj = 0,max_xbi=0;
	for (j = 0;j < 3;j++)
	{
		for (i = 0;i < 4;i++)
		{
			if (a[j][i] > max)
			{
				max = a[j][i];
				max_xbj = j;
				max_xbi = i;
			}
		}
	}
	printf("max:%d(下标:%d %d)", max, max_xbj, max_xbi);
	return 0;
}