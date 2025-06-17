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
stu* insertposition(stu*,int p);
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();
	traverse(h);
	printf("enter which position u want to insert\n");
	int p;
	scanf("%d",&p);
	h=insertposition(h,p);
	traverse(h);
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
stu* insertposition(stu* h,int p)
{
	stu *n=NULL;stu *t=h;
	if(p==1)
	{
		n=createnode();
		n->next=h;
		h=n;
		return h;
	}
	else
	{
		int i=1;
		while(i<p-1)
		{
			t=t->next;
			i++;
		}
		n=createnode();
		n->next=t->next;
		t->next=n;
		return h;
	}
}
