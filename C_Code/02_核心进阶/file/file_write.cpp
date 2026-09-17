
/*
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
struct Student
{
	char name[20];
	float score;
};
int main()
{
	struct Student stu[3] = { {"lishun",666},{"zhang",520},{"liudagen",564} };
	FILE* fp;
	int i;
	if ((fp = fopen("students.txt", "w")) == NULL)
	{
		printf("don't open the file\n");
		return 1;
	}
	for (i = 0;i < 3;i++)
	{
		fprintf(fp, "%s\t%.1f\n", stu[i].name, stu[i].score);
		printf("%s\t%.1f\n", stu[i].name, stu[i].score);
	}
	fclose(fp);
	printf("写入成功\n");
	return 0;
}
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
struct Student
{
	char name[20];
	float score;
};
int main()
{
	struct Student stu[3];
	int i = 0;
	FILE* fp;
	if ((fp = fopen("students.txt", "r")) == NULL)
	{
		printf("文件打开错误!!!\n");
		return 1;
	}
	while (!feof(fp)&&i<3)
	{
		if ((fscanf(fp, "%s %f", stu[i].name, &stu[i].score)) != 2)
		{
			printf("第%d个学生读取错误!!!\n", i + 1);
		}
		if (feof(fp) != 0)
			break;
		printf("%s %.1f\n", stu[i].name, stu[i].score);
		i++;
	}
	fclose(fp);
	printf("读取完毕!!!");
	return 0;
}