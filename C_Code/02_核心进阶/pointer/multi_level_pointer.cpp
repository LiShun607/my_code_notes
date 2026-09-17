#include<stdio.h>
int main()
{
	int a = 10, * p = &a, ** pp = &p;
	printf("%d,%d,%d\n", a, *p, **pp);
	**pp = 20;
	printf("%d\n", a);
	return 0;
}