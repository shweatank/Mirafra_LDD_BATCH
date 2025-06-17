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
stu* insertnode(stu *H, int);
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();int p;
	traverse(h);
	printf("enter position to insert:\n");
	scanf("%d",&p);
	h=insertnode(h,p);
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
stu* insertnode(stu *H, int p)
{
	stu *T=H;int i=1;
	while(i<p-1)
	{
		T=T->next;
		i++;
	}
	stu *N=createnode();
	N->next=T->next;
	T->next=N;
	return H;
}
