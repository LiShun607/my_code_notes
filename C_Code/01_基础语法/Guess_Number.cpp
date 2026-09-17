#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int t, g, count = 0;
	char ch;
	srand(time(NULL));
	t = rand() % 100 + 1;
	do
	{
		srand(time(NULL));
		t = rand() % 100 + 1;
		count = 0;
		do
		{
			printf("请输入你猜的数字(1-100):");
			scanf_s("%d", &g);
			count++;
			if (g < 1 || g>100)
			{
				printf("无效输入，请重新输入\n");
				count--;
				continue;
			}
			if (count > 10)
			{
				printf("次数超过10次，游戏失败,正确数字是%d\n", t);
				break;
			}
			if (g > t)
			{
				printf("大了\n");
			}
			if (g < t)
			{
				printf("小了\n");
			}
		} while (g != t);
		if (g == t)
		{
			printf("恭喜你猜对了!!!\n");
			printf("你一共猜了%d次\n", count);
	    }
		printf("是否继续游戏（输入y继续游戏，输入n不继续）:");
		scanf_s(" %c", &ch);
	} while (ch=='y');
	return 0;
}