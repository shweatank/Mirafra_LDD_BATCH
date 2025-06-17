#include<stdio.h>
#define M 100
int main()
{
	int n=11,k=0,i,j,temp;
	int a[]={1,2,3,4,5,6,7,8,9,10,11};
	int s[M]={0};
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-1;j++)
		{
			if(a[j]%2==0)
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	for(i=0;i<n;i++)printf("%d ",a[i]);

}
