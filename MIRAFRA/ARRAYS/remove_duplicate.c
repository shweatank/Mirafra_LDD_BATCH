#include<stdio.h>
int main()
{
	int a[]={2,1,3,2,2,3,4,1};
	int i,j,k;
	int n=8;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;)
		{
			if(a[i]==a[j])
			{
				for(k=j;k<n-1;k++)
					a[k]=a[k+1];
				n--;
			}
			else
			{
				j++;
			}
		}

	}
	for(i=0;i<n;i++)
	{
		printf("%d ",a[i]);
	}
}
