#include<stdio.h>
int main()
{
	int a[]={1,2,3,4,5};
	int n=5;
	int c=2,sum=0,i,j;
	for(i=0;i<n;i++)
	{
		for(j=1;j<=c;j++)
		{
			sum=a[i]+a[j];
			if(sum<=a[i]+a[j])
				sum=a[i]+a[j];
			else
				sum=sum;
		}
	}
	printf("sum %d",sum);	
}
