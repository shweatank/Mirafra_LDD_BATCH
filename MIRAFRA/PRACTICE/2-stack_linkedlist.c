#include<stdio.h>
#include<stdlib.h>
#define SIZE 5
typedef struct stac
{
	int d;
	struct stac *next;
}stack;
stack *top=NULL;
int push(int);
int pop();
void print();
int main()
{
	int n,num;
	while(1)
	{
		printf("1.push 2.pop 3.print\n");
		scanf("%d",&n);
		switch(n)
		{
		case 1: printf("enter number\n");
               		scanf("%d",&num);
	       	        push(num);
			break;
		case 2: pop();
			break;
		case 3: print();
			break;
		case 4: exit(0);
		}
	}
}
int push(int num)
{
	stack *N=malloc(sizeof(stack));
	N->d=num;
	N->next=top;
	top=N;
	printf("pushed\n");
}
int pop()
{
	stack *T=top;
	if(top==NULL)
	{
		printf("nothing stack empty\n");
	}
	else
	{
	top=top->next;
	free(T);
	}
}
void print()
{
	stack *T=top;
	while(T)
	{
		printf("%d ",T->d);
		T=T->next;
	}
}

