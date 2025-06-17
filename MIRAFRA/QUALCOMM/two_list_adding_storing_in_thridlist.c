#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int id;
	struct student *next;
}stu;
stu* createnode()
{
	stu* N=malloc(sizeof(stu));
	printf("enter data:\n");
	scanf("%d",&N->id);
	N->next=NULL;
	return N;
}
stu* createlist()
{
	stu *H=NULL, *L=NULL, *N=NULL;
	char ch;
	printf("enter y/n\n");
	scanf(" %c",&ch);
	while(ch=='y')
	{
		N=createnode();
		if(H==NULL)
			H=N;
		else
			L->next=N;
		L=N;
		printf("enter y/n\n");
		scanf(" %c",&ch);
	}
	return H;
}
void traverse(stu *T)
{
	while(T)
	{
		printf("%d ",T->id);
		T=T->next;
	}
}
stu* addboth(stu *P, stu *Q)
{
	stu *R=NULL,*H=NULL, *L=NULL;
	while(P!=NULL && Q!=NULL)
	{
	R=malloc(sizeof(stu));
	R->next=NULL;
	if(H==NULL) H=R;
	else
		L->next=R;
	L=R;

	R->id=P->id+ Q->id;
	if(P) P=P->next;
	if(Q) Q=Q->next;
	}
	return H;

}
int main()
{
	stu *P=createlist();
	traverse(P);
	stu *Q=createlist();
	traverse(Q);
	printf("\n");
	stu *R=addboth(P,Q);
	traverse(R);
}
