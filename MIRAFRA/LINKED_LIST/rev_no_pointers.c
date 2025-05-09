#include<stdio.h>
typedef struct student
{
	int data;
	int next;
}stu;
void printList(stu s[],int);
int rev(stu s[],int*);
int main()
{
	int n=3,head=0;
	stu s[n];
	s[0].data=10;s[0].next=1;
	s[1].data=20;s[1].next=2;
	s[2].data=30;s[2].next=-1;
	printList(s,head);
	rev(s,&head);
	printf("head after rev:%d\n",head);
	printList(s,head);
}
void printList(stu s[],int head)
{
	int current=head;
	while(current!=-1)
	{
		printf("%d -> ",s[current].data);
		current=s[current].next;
	}
	printf("\n");
}
int rev(stu s[],int *head)
{
	int prev=-1;
	int current=*head,next;
	while(current!=-1)
	{
		next=s[current].next;
		s[current].next=prev;
		prev=current;
		current=next;
	}	
	*head=prev;
	//return head;
}
