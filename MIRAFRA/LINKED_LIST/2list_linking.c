#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int id;
	struct student *next;
}stu;
stu* createnode()
{
	stu *N=malloc(sizeof(stu));
	printf("enter number:\n");
	scanf("%d",&N->id);
	N->next=NULL;
	return N;

}
stu* createlist()
{
	stu *H=NULL, *L=NULL, *N=NULL;
	char ch;
	printf("enter y or n");
	scanf(" %c",&ch);
	while(ch=='y')
	{
		N=createnode();
		if(H==NULL)
			H=N;
		else
			L->next=N;
		L=N;
		printf("enter y or n");
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
stu* link_sort(stu* E, stu* O)
{
	stu *T=NULL,*H=NULL;
	if(O->id <= E->id)
	{
		T=H=O;
		O=O->next;
	}
	else 
	{
		T=H=E;
		E=E->next;
	}

	while(O&&E)
	{
		if(O->id <= E->id)
		{
			T->next=O;
			T=O;
			O=O->next;
		}
		else
                {
                        T->next=E;
                        T=E;
                        E=E->next;
                }
	}
	if(O) T->next=O;
	else T->next=E;
	return H;
}
int main()
{
	stu *H=NULL,*H1=NULL;
	H=createlist();
	traverse(H);
	        printf("\n");
	H1=createlist();
	traverse(H1);
	printf("\n");
	H=link_sort(H,H1);
	traverse(H);
	        printf("\n");

}
