/*
#include<stdio.h>
struct Student
{
	char name[20];
	int age;
	float score;
};
int main()
{
	struct Student student1 = { "李顺",19,686 };
	struct Student* p = &student1;
	printf("学生信息:\n");
	printf("%s %d %.1f\n", p->name, p->age, p->score);
	p->score = 866;
	printf("修改后的成绩:%.1f", student1.score);
	return 0;
}
*/


#include<stdio.h>
struct Student
{
	char name[20];
	float score;
};
void order(struct Student* x, int n);
int main()
{
	struct Student stu[3] = { {"李顺",520},{"张",666 },{"马",185 } };
	struct Student* p = stu;
	order(p, 3);
	printf("学生分数按从大到小的顺序:\n");
	printf("姓名\t\t分数\n");
	for (int i = 0;i < 3;i++)
	{
		printf("%s\t\t%.1f\n", stu[i].name,stu[i].score);
	}
	return 0;
}
void order(struct Student* x, int n)
{
	int i,j, k;
	struct Student temp;
	for (i = 0;i < n - 1;i++)
	{
		k = i;
		for (j = i + 1;j < n;j++)
		{
			if ((x + j)->score > (x + k)->score)
				temp = *(x + j), * (x + j) = *(x + k), * (x + k) = temp;
		}
	}
}