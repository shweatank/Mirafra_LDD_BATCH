#include<stdio.h>
#include<stdlib.h>
#define SIZE 5
int s[SIZE];
int push(int,int*);
int pop(int *);
void print(int s[],int *top);
int main()
{
	int n,num;
	int top=-1;
	while(1)
	{
		printf("1.push 2.pop 3.print\n");
		scanf("%d",&n);
		//printf("enter number\n");
		//scanf("%d",&num);
		switch(n)
		{
		case 1:printf("enter number\n");
               		 scanf("%d",&num);
	       	         push(num,&top);
			break;
		case 2: pop(&top);
			break;
		case 3: print(s,&top);
			break;
		case 4: exit(0);
		}
	}
}
int push(int num,int *top)
{
	if((*top)==SIZE-1)
		printf("full\n");
	else
	{
	(*top)++;
	s[*top]=num;
	printf("pushed");
	}
}
int pop(int *top)
{
	if((*top)==-1)
		printf("no elements\n");
	else
	{
		(*top)--;
		printf("poped\n");
	}
}
void print(int s[],int *top)
{
	if((*top)==-1)
		printf("nothing to print\n");
	int i;
	for(i=0;i<=(*top);i++)
	{
		printf("%d ",s[i]);
	}
}
