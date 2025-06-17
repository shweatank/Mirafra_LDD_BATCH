#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int id;
	struct student *next;
}stu;
stu* createlist();
stu* createnode();
void traverse(stu*);
stu* mid(stu*);
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();
	traverse(h);
	mid(h);
}
stu* createlist()
{
	char ch;
        stu *h=NULL, *n=NULL, *l=NULL;
	printf("enter y/n\n");
	scanf("%c",&ch);
	while(ch=='y')
	{
		n=createnode();
		if(h==NULL)
			h=n;
		else 
			l->next=n;
		l=n;
		scanf(" %c",&ch);
	}
	return h;
}
stu* createnode()
{
	stu *N=(stu*)malloc(sizeof(stu));
	printf("enter num\n");
	scanf("%d",&N->id);
	N->next=NULL;
	return N;
}
void traverse(stu *T)
{
	while(T)
	{
		printf("%d ",T->id);
		T=T->next;
	}
}
stu* mid(stu* h)
{
	stu*p=h,*q=h;
	while(q->next!=NULL)
	{
		p=p->next;
		q=q->next;
		if(q)
			q=q->next;
	}
	printf("\n%d is middle\n",p->id);
}
