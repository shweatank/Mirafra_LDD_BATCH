#include<stdio.h>
void printbin(int n)
{
	int i;
	for(i=0;i<sizeof(n)*8;i++)
	{
		if(n&(0x1<<(sizeof(n)*8)-1))
			printf("1");
		else
			printf("0");
		n=n<<1;
	}
}

int main()
{
	int n;
	scanf("%d",&n);
	printbin(n);
}

