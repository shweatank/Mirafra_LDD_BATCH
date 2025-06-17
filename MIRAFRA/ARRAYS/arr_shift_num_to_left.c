#include<stdio.h>
int main()
{
	int a[]={1,2,3,4,9,0,8,32,0,2,1,0,9,0,2,9};
	int n=16,i,i1=0,c=0,c1=0;
	for(i=0;i<n;i++)
	{
		if(a[i]!=0){
			a[i1++]=a[i];
			c++;}
		else 
			c1++;
	}
	for(i=c;i<n;i++)
	{
		a[i]=0;
	}
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
}
