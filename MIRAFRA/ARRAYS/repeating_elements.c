#include<stdio.h>
#define MAX 1000
int main()
{
	int seen[MAX]={0};
	int arr[8]={3,2,1,1,2,3,4,4};
	int n=8,i,j=0;
	for(i=0;i<n;i++)
	{
		seen[arr[i]]++;
	}
	for(i=0;i<n;i++)
		printf("%d repated %d times\n",arr[i],seen[arr[i]]);
}
