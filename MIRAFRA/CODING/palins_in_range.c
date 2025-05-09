#include<stdio.h>
int main()
{
	int a,b,i,j,rev=0;
	scanf("%d%d",&a,&b);
	for(i=a;i<=b;i++)
	{
		rev=0;
		int num=i;
		//printf("\n%d ",i);
		for(j=i;j!=0;j=j/10)
		{
			rev=(rev*10)+j%10;
		}
		//printf(" %d\n",rev);
		if(rev==i)
			printf("%d ",num);
	}
}
