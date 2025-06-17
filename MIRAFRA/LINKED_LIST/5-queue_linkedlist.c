#include<stdio.h>
#include<stdlib.h>
#define SIZE 5
typedef struct q
{
	int d;
	struct q *next;
}queue;
queue *H=NULL,*L=NULL,*P=NULL,*T=NULL;
int enq(int num)
{
	queue *N=malloc(sizeof(queue));
	N->d=num;
	N->next=NULL;
	if(H==NULL)
		H=N;
	else
		L->next=N;
	L=N;
}
int deq()
{
	if(H==NULL)
		printf("nothing to dequeue\n");
	else
	{
		T=H->next;
		free(H);
		H=T;
	}
}
void print()
{
	queue *P=H;
	while(P)
	{
		printf("%d ",P->d);
		P=P->next;
	}
}
int main()
{
	int n,num;
	while(1)
        {
        printf("enter 1.enqueue 2.dequeue 3.print 4.stop\n");
        scanf("%d",&n);
        switch(n)
        {
                case 1: printf("enter num to enqueue\n");
                                scanf("%d",&num);
                                enq(num);
                                break;
                case 2: deq();
                                break;
                case 3: print();
                                break;
                default:exit(1);
        }
        }

}

