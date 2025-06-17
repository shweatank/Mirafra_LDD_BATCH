#include<stdio.h>
int main()
{
	int a[]={1,2,3,1,2,3};
	int n=6,c=0,i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(a[i]==a[j])c++;
		}
		printf("%d appeared %d times\n",a[i],c);
		c=0;
	}
}
