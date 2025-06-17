#include<stdio.h>
#define M 100
int main()
{
	int n=11,i,j=0,temp;
	int a[]={1,2,3,4,5,6,7,8,9,10,11};
	int s[M]={0};
	for(i=0;i<n;i++)
	{
			if(a[i]%2==0)
			{
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
				j++;
			}
	}
	for(i=0;i<n;i++)printf("%d ",a[i]);

}
