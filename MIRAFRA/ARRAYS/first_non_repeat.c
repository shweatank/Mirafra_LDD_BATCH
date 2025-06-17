#include<stdio.h>
#define MAX 100
int main()
{
	int arr[8]={1,2,3,1,2,3,1,2};
	int n=8,i;int seen[MAX]={0};
	for(i=0;i<n;i++)
	{
		seen[arr[i]]++;
	}
	for(i=0;i<n;i++)
	{
		if(seen[arr[i]]==1)
		{
			printf("%d is first non repeated\n",arr[i]);
			return 0;
		}
	}
}
