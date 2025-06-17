#define MAX 100
#include<stdio.h>
int main()
{
	int arr[8]={1,2,3,1,2,3};
	int n=8,i;int seen[MAX]={0};
	for(i=0;i<n;i++)
	{
		seen[arr[i]]++;
		if(seen[arr[i]]==2)
		{
			printf("%d is repeat",arr[i]);
			return 0;
		}
	}
}
