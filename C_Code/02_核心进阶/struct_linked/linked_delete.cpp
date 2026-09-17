#include<stdio.h>
#include<stdlib.h>
struct Node
{
	int data;
	struct Node* next;
};
struct Node* creat(void)
{
	struct Node* head = NULL, * p1, * p2;
	int n = 0;
	p1 = p2 = (struct Node*)malloc(sizeof(struct Node));
	printf("请输入第1个数:");
	scanf_s("%d", &p1->data);
	while (p1->data != -1)
	{
		if (n == 0)
		{
			head = p1;
		}
		else
			p2->next = p1;
		p2 = p1;
		p1 = (struct Node*)malloc(sizeof(struct Node));
		n++;
		printf("请输入第%d个数:", n + 1);
		scanf_s("%d", &p1->data);
	}
	p2->next = NULL, free(p1);
	return(head);
}
struct Node* deletnode(struct Node* head, int x)
{
	struct Node* preul = NULL, * cru = head;
	while (cru != NULL && cru->data != x)
	{
		preul = cru;
		cru = cru->next;
	}
	if (cru == NULL)
	{
		printf("不存在需要删去的数!!!\n");
		return(head);
	}
	if (preul == NULL)
	{
		head = cru->next;
	}
	else
	{
		preul->next = cru->next;
	}
	free(cru);
	return(head);
}
int main()
{
	struct Node* head;
	int n;
	head = creat();
	printf("请输入要删除的数:");
	scanf_s("%d", &n);
	head = deletnode(head, n);
	struct Node* p = head;
	printf("删除%d后:\n", n);
	while (p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	return 0;
}