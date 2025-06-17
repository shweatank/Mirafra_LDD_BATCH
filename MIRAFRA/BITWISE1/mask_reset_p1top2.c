#include<stdio.h>
int main()
{
	int num=5;
	int start=0,end=4;
	int mask=~(((1<<(end-start+1))-1)<<start);
	num=num&mask;
	printf("%d",num);
}
