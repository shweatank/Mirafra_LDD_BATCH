#include<stdio.h>
int main()
{
	int n=6,i,j,temp;
	int a[]={1,0,1,0,1,0};
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-1;j++)
		{
			if(a[j]==0)
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	for(i=0;i<n;i++)printf("%d ",a[i]);
}
