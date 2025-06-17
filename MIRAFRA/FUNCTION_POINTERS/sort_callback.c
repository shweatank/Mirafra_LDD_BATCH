#include<stdio.h>
#include<stdlib.h>

int comp(int,int);
int sort(int (*fp)(int,int),int[],int);
int main()
{
	int a[]={1,3,2,5,4};int i;
	sort(comp,a,5);
	for(i=0;i<5;i++)
	{
		printf("%d ",a[i]);
	}
}
int sort(int (*fp)(int,int),int a[],int n)
{
	int i,temp,j;
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(fp(a[i],a[i+1])>0)
			{
				temp=a[i];
				a[i]=a[i+1];
				a[i+1]=temp;
			}
		}
	}
}
int comp(int a,int b)
{
	return a-b;
}

