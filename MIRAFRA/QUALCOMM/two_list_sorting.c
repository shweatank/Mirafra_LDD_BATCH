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
stu* sortboth(stu *P)
{
	

}

int main()
{
	stu *P=createlist();
	traverse(P);
	stu *Q=createlist();
	traverse(Q);
	printf("\n");
	stu *P=sortlist(P);
	stu *Q=sortlist(Q);
	traverse(R);
}
