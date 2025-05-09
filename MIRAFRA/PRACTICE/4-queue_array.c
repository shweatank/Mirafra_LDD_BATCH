#include<stdio.h>
#include<stdlib.h>
#define SIZE 5
int q[SIZE];
int f=-1,r=-1;
int enq(int num)
{
	if(r==SIZE-1)
		printf("queue is full\n");
	else if(f==-1 && r==-1)
	{
		r=f=0;
		q[r]=num;
	}
	else
	{
		r++;
		q[r]=num;
	}
}
int deq()
{
	if(r==-1 && f==-1)
		printf("que is empty\n");
	else if( f==r)
		f=r=-1;
	else
		f++;
}
void print()
{
	int i;
	for(i=f;i<=r;i++)
		printf("%d ",q[i]);
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
