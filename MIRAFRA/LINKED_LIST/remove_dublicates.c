#include<stdio.h>
#include<stdlib.h>
typedef struct stu 
{
	int id;
	struct stu *next;
}st;
st * createnode()
{
        st *N=malloc(sizeof(struct stu));
        printf("enter num:\n");
        scanf("%d",&N->id);
        N->next=NULL;
        return N;
}

st * createlist()
{
	st *P=NULL, *L=NULL, *H=NULL; 
	printf("enter y/n:");char ch;
	scanf("%c",&ch);
	while(ch=='y')
	{
		st *N=createnode();
		if(H==NULL)
			H=N;
		else
			L->next=N;
		L=N;
		printf("enter y/n:");
		scanf(" %c",&ch);	
	}
	return H;
}

void traverse(st *p)
{
	while(p)
	{
		printf("%d ",p->id);
		p=p->next;
	}

}
st *remve_dub(st *p)
{
	st* ptr=p;
	while(ptr->next!=NULL)
	{
		st* p1=ptr;
		st* ptr1=ptr->next;
		
		while(ptr1)
		{
			if(ptr->id == ptr1->id)
			{
				p1->next=ptr1->next;
				free(ptr1);
				ptr1=p1->next;
			}
			else
			{
				p1=ptr1;
				ptr1=ptr1->next;
			}
		}
		ptr=ptr->next;
	}
	return p;
}
int main()
{
        st *p=createlist();
        traverse(p);
	p=remve_dub(p);
	traverse(p);
}
