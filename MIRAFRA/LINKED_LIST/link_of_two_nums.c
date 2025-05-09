#include<stdio.h>
#include<stdlib.h>

typedef struct student
{
	int a;
	struct student *next;
}stu;
stu* createlist();
stu* createnode();
stu* traverse();
int rev(int,int,stu*);
int main()
{
	int a,b;
	stu*H=createlist();
	traverse(H);
	printf("\nenter nums to rev\n");
	scanf("%d%d",&a,&b);
	rev(a,b,H);
	traverse(H);
}

stu* createlist()
{
	stu* h=NULL,*l=NULL,*n=NULL;
	char ch='y';
	while(ch=='y')
	{
		n=createnode();
		if(h==NULL)
			h=n;
		else
			l->next=n;
		l=n;
	printf("enter character y or n\n");	
		scanf(" %c",&ch);
	}
	return h;
}

stu* createnode()
{
	stu* N=malloc(sizeof(stu));
	printf("enter num:");
	scanf("%d",&N->a);
	N->next=NULL;
	return N;
}
stu* traverse(stu* T)
{
	while(T)
	{
		printf("%d ",T->a);
		T=T->next;
	}
}
int rev(int a,int b,stu* T)
{
	if(a==b)return;
	stu *P=T, *Q=T, *N=NULL,*PP=NULL,*PQ=NULL;
	int i=1;
	while(i<a && P)
	{
		PP=P;
		P=P->next;
		i++;
	}
	printf("p: %d",P->a);
	i=1;
	while(i<b && Q)
	{
		PQ=Q;
		Q=Q->next;
		i++;
	}
	printf("q: %d",Q->a);
	N=P->next;
	P->next=Q->next;
	Q->next=N;
	if(PP)
		PP->next=Q;
	if(PQ)
		PQ->next=P;
}
