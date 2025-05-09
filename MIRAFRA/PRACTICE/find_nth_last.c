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
stu* lastnth(stu*,int );
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();
	traverse(h);
	printf("\nenter nth pos\n");
	int num;
	scanf("%d",&num);
	lastnth(h,num);
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
stu* lastnth(stu* h,int n)
{
	int i=0;
	stu*q=h,*p=h;
	while(i<n)
	{
		q=q->next;	
		i++;
	}
	while(q)
	{
		q=q->next;
		p=p->next;	
	}
	printf("\n%d is nth node from last\n",p->id);
}
