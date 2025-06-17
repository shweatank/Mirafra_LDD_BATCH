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
	printf("ennter num: ");
	scanf("%d",&N->id);
	N->next=NULL;
	return N;
}
stu* createlist()
{
	stu *H=NULL,*L=NULL,*N=NULL;
	char ch;
	printf("enter y/n ");
	scanf("%c",&ch);
	while(ch=='y')
	{
		N=createnode();
		if(H==NULL)
			H=N;
		else
			L->next=N;
		L=N;
		printf("enter y/n : ");
		scanf(" %c",&ch);
	}
	return H;
}
void traverse(stu* T)
{
	while(T)
	{
		printf("%d ",T->id);
		T=T->next;
	}
}

stu* rev_n_nodes(int n, stu *H)
{
	stu *T=H;int c=0,i;
	stu *p=NULL,*q=NULL,*r=NULL;
	//first count wheather that many are present or not
	while(c<n && T)
	{
		c++;
		T=T->next;
	}

                p=H;
                q=p->next;
                r=q->next;
		printf("%d nodes are present \n",c);
		for(i=1;i<n&&q;i++)
		{
               		q->next=p;
               	 	p=q;
               	 	q=r;
               	 	if(r)
                        r=r->next;
		}
		H->next=q;
		return p;
}
int main()
{
	stu *H=NULL;int num;
	H=createlist();
	traverse(H);
	printf("Enter num of nodes to rev\n");
	scanf("%d",&num);
	H=rev_n_nodes(num,H);
	traverse(H);

}
