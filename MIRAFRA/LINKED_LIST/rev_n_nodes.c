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
/*stu* rev(stu* h,int n)
{
	stu *p=NULL,*q=NULL,*r=NULL;
	p=h;
	q=p->next;
	r=q->next;
	while(q)
	{
		q->next=p;
		p=q;
		q=r;
		if(r)
			r=r->next;
	}
	h->next=NULL;
	h=p;
	return h;
}*/
stu* rev_n_nodes(int n, stu *H,stu **next_group_start)
{
	stu *T=H;int c=0,i;
	stu *p=NULL,*q=NULL,*r=NULL;
	//first count wheather that many are present or not
	while(c<n && T)
	{
		c++;
		T=T->next;
	}
    	if (c<n)
       	{
       	 	*next_group_start = H;
        	return NULL;  // less than k, don't reverse
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
		*next_group_start=q;
		H->next=NULL;
		return p;
}
stu* rev_in_groups(int num, stu* H)
{
	stu *next_group_start=NULL;
	stu *new_head=rev_n_nodes(num,H,&next_group_start);
	if(!new_head)return H;
	H->next=rev_in_groups(num,next_group_start);
	return new_head;

}
int main()
{
	stu *H=NULL;int num;
	H=createlist();
	traverse(H);
	printf("Enter num of nodes to rev\n");
	scanf("%d",&num);
	H=rev_in_groups(num,H);
	traverse(H);

}
