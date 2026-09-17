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
	printf("请输入第%d个数:", n + 1);
	scanf_s("%d", &p1->data);
	while (p1->data != -1)
	{
		if (n == 0)
		{
			head = p1;
		}
		else
		{
			p2->next = p1;
		}
		p2 = p1;
		n++;
		p1 = (struct Node*)malloc(sizeof(struct Node));
		printf("请输入第%d个数:", n + 1);
		scanf_s("%d", &p1->data);
	}
	p2->next = NULL;
	free(p1);
	return(head);
}
struct Node* deletall(struct Node* head, int x)
{
	struct Node* newhead = (struct Node*)malloc(sizeof(struct Node));
	struct Node* cru = head, * preul = newhead;
	newhead->next = head;
	int m = 0;
	while (cru != NULL)
	{
		while (cru!=NULL&&cru->data != x)
		{
			preul = cru;
			cru = cru->next;
		}
		if (cru!=NULL)
		{
			preul->next = cru->next;
			cru = cru->next;
			m++;
		}
		if (m==0)
		{
			printf("无法找到需要删除的数!!!\n");
			free(newhead);
			return(head);
		}
	}
	head=newhead->next;
	free(newhead);
	/*
	while (cru != NULL && cru->data != x)

	{
		preul = cru;
		cru = cru->next;
	}
	if (cru == NULL)
	{
		printf("无法找到需要删除的数!!!\n");
		free(newhead);
		return(head);
	}
	else
	{
		preul->next = cru->next;
		head = newhead->next;
	}
	*/
	//free(newhead), free(cru);
	return(head);
}
int main()
{
	struct Node* head = NULL, * p;
	int n;
	head = creat();
	printf("请输入要删去的数:");
	scanf_s("%d", &n);
	head = deletall(head, n);
	p = head;
	printf("删去%d后:\n", n);
	while (p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	return 0;
}