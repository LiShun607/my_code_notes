#include<stdio.h>
int main()
{
	int a[5], i, sum = 0;
	for (i = 0;i < 5; i++)
	{
		printf("请输入第%d个整数(总共输入5个):", i + 1);
		scanf_s("%d", &a[i]);
		sum = sum + a[i];
	}
	int max = a[0], min = a[0], max_xb= 0, min_xb= 0;
	for(i=1;i<5;i++)
	{
		if (a[i] > max)
		{
			max = a[i];
			max_xb = i;
		}
		if (a[i] < min)
		{
			min = a[i];
			min_xb = i;
		}
	}
	printf("平均值:%.2f\n", (double)sum / 5);
	printf("min:%d(下标:%d)   max:%d(下标:%d)", min,min_xb, max,max_xb);
	return 0;
}