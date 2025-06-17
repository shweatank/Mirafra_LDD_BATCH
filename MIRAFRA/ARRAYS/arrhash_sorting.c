#include<stdio.h>
#include<stdlib.h>
#define MAX 100
int main()
{
	int c=0,i,n=6,k=0;
	int a[]={20,30,10,40,50,10};int s[MAX]={0};
	for(i=0;i<n;i++)
	{
		s[a[i]]++;
	}
	for(i=0;i<MAX;i++)
	{
		c=s[i];
		while(c&& c!=0){
		printf("%d ",i);
		c--;}
	}
}
