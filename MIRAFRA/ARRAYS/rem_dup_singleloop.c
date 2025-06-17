#include<stdio.h>
#define MAX 1000
int main()
{
	int seen[MAX]={0};
	int arr[8]={3,2,1,1,2,3,4,4};
	int n=8,i,j=0;
	for(i=0;i<n;i++)
	{
		if(seen[arr[i]]==0)
		{
			seen[arr[i]]=1;
			arr[j++]=arr[i];
		}
	}
	for(i=0;i<j;i++)
		printf("%d ",arr[i]);
}
