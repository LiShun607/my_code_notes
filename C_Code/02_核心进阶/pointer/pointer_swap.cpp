#include<stdio.h>
void swap(int* a, int* b);
int main()
{
	int x, y, * m, * n;
	printf("输入两个整数（空格隔开）:");
	scanf_s("%d %d", &x, &y);
	m = &x, n = &y;
	swap(m, n);
	printf("%d %d", x, y);
	return 0;
}
void swap(int* a, int* b)
{
	int temp;
	temp = *a, * a = *b, * b = temp;
}