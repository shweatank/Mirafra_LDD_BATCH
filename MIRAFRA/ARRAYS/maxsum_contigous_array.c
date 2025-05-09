#include<stdio.h>
int main()
{
	int a[]={11,-12,15,-3,8,-9,1,8,10,-2};
	int n=10,sum=0,maxsum=0,i,j,k;
	for(i=0;i<n;i++)
	{
		for(j=i;j<n;j++)
		{
			sum=0;
			for(k=i;k<j;k++)
				sum=sum+a[k];
		}
		if(sum>maxsum)
			maxsum=sum;
		else maxsum=maxsum;
	}
	printf("Max sum is %d",maxsum);

}

















