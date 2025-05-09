#include<stdio.h>
int main()
{
	int i,j,temp;
	int a[5]={1,3,2,4,5};
	for(i=0;i<5;i++)
	{
		for(j=0;j<5-1;j++)
		{
			if(a[i]<a[j])
			{
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
			}
		}
	}
	for(i=0;i<5;i++)
		printf("%d ",a[i]);
}

