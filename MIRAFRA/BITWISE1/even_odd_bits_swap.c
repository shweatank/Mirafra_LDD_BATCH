#include<stdio.h>
int main()
{
	int num;
	scanf("%d",&num);
	num=((num&0x5555)<<1) |((num&0xAAAA)>>1);
	printf("%d\n",num);
}
