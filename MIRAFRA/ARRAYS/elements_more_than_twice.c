#include<stdio.h>
#define MAX 100
int main()
{
	int arr[8]={1,2,3,2,1,4,2,3};
	int n=8,i;int seen[MAX]={0};
	for(i=0;i<n;i++)
	{
		seen[arr[i]]++;
	}
	for(i=0;i<n;i++)
	{
		if(seen[arr[i]]==2)
			printf("%d is repeated twice\n",arr[i]);
	}
}
