#include<stdlib.h>
#include<stdio.h>
int main()
{
	int i,j,rows=3,cols=3;
	int **a=(int **)malloc(rows*sizeof(int*));
	for(i=0;i<rows;i++)
	{
		a[i]=(int*)malloc(cols*sizeof(int*));
	}
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			a[i][j]=i*cols+j;
		}
	}
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
			printf("%d ",a[i][j]);
		printf("\n");
	}
}
