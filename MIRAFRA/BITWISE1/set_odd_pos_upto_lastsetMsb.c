#include<stdio.h>
int findlastset(int x)
{
	int i,p=0;
	for(i=0;i<sizeof(x)*8;i++)
	{
		if(x&(0x1<<(sizeof(x)*8-1)))
		{
		//	printf("1");
			p=i;
			break;
		}
		else 
		//	printf("0");
		x=x<<1;
	}
	return p;
}
int setoddpos(int n,int p)
{
	int i;
	for(i=0;i<p;i++)
	{
		if(i%2!=0)
		n=n|(0x1<<i);
	}
	return n;
}

int main()
{
	int num;
	scanf("%d",&num);
	int last_set_msb=31-findlastset(num);
	printf("last set msb is at position:%d\n",last_set_msb);
	num=setoddpos(num,last_set_msb);
	printf("%d",num);
}

