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
void loop(stu*);
int main()
{
	stu *h=NULL, *n=NULL, *l=NULL;
	h=createlist();
	traverse(h);
	loop(h);

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
void loop(stu *h)
{
	stu *p=NULL,*q=NULL,*r=NULL;
	p=q=h;
	while(p)
	{
		p=p->next;
		if(p)
			p=p->next;
		q=q->next;
		if(p==q)
			printf("loop exist\n");
	}
	if(p==NULL) printf("no loop\n");
	if(p==q)
	{
		printf("loop overcome\n");
		p=h;
		while(p!=q)
		{
			r=q;
			q=q->next;
			p=p->next;
		}
		r->next=NULL;
	}
}

