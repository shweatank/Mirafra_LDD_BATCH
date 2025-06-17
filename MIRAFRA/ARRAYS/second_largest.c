#include<stdio.h>
int main()
{
	int a[]={1,2,3,4,5,6};
	int n=6,i,j,high=a[i],high2=0;
	for(i=0;i<n;i++)
	{
		if(a[i]>high)
			high=a[i];
	}
	printf("1st highest %d\n",high);
	high2=-1;
	for(j=0;j<n;j++)
	{
	if(a[j]!=high && a[j]>high2)
	{
		high2=a[j];
	}	
	}
	printf("%d is 2nd highest\n",high2);
}
