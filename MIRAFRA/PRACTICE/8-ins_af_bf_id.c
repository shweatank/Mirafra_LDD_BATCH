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
stu* insafbf(stu*,int p,char c);
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();
	traverse(h);
//	printf("enter which position u want to insert\n");
	int p,i;char c;
//	scanf("%d",&p);
	printf("enter u want insert aftr/before a or b and id\n");
	scanf(" %c",&c);
	scanf("%d",&i);
	h=insafbf(h,i,c);
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
stu* insafbf(stu* h,int i,char c)
{
	if(c=='a')
	{
		stu* t=h;
		while(t->id!=i)
		{
			t=t->next;
		}
		stu *n=createnode();
		n->next=t->next;
		t->next=n;
		return h;
	}
	else if(c=='b')
	{
		stu* t=h,*s=NULL;
		while(t->id!=i)
		{
			s=t;
			t=t->next;	
		}
		stu *n=createnode();
		if(t==h)
		{
			n->next=h;
			h=n;return h;
		}
		else
		{
		n->next=s->next;
		s->next=n;
		return h;
		}
	}
	
}
