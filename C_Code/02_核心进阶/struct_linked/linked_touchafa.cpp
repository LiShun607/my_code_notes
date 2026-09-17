#include<stdio.h>
#include<stdlib.h>
struct Node
{
	int data;
	struct Node* next;
};
int main()
{
	struct Node* head = NULL, *newnode=(struct Node*)malloc(sizeof(struct Node));
	printf("请输入一个整数(输入0结束):");
	scanf_s("%d", &newnode->data);
	while (newnode->data != 0)
	{
		newnode->next = head;
		head = newnode;
		newnode = (struct Node*)malloc(sizeof(struct Node));
		printf("请输入一个整数(输入0结束):");
		scanf_s("%d", &newnode->data);
	}
	free(newnode);
	struct Node* p = head;
	while (p != NULL)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	return 0;
}