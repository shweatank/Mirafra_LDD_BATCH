#include<stdio.h>
#include<stdlib.h>
int main()
{
	int n=5,i;
	int *a=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		a[i]=i;
		printf("%d ",a[i]);
	}
}
