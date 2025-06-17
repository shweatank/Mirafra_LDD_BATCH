#include<stdio.h>
#define MAX 100
int main()
{
	int arr[8]={1,1,1,1,2,2,2,3};
	int n=8,max=0,k,i,ele;int seen[MAX]={0};
	for(i=0;i<n;i++)
	{
		k=++seen[arr[i]];
		if(max<k)
		{
			max=k;
			ele=arr[i];
		}
		else
		{
			max=max;
		}
	}
	printf("max freq: %d ele is %d\n",max,ele);
}
