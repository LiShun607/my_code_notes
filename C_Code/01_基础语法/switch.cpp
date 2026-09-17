#include<stdio.h>
int main()
{
	int a;
	printf("请输入1-7的数字:");
	scanf_s("%d", &a);
	switch (a)
		{
		case 1:printf("星期一\n");break;
		case 2:printf("星期二\n");break;
		case 3:printf("星期三\n");break;
		case 4:printf("星期四\n");break;
		case 5:printf("星期五\n");break;
		case 6:printf("星期六\n");break;
		case 7:printf("星期天\n");break;
		default:printf("输入的数字不在1-7范围内!!!\n");
		}
	return 0;
}