
#include<stdio.h>
void printbinary(int num)
{
	int i;
	for(i=0;i<sizeof(num)*8;i++)
	{
		if(num&(0x1<<sizeof(num)*8-1))
			printf("1");
		else
			printf("0");
		num=num<<1;
	}
}

int main()
{
	int num1,num2,p1,p2;
	scanf("%d%d%d%d",&num1,&num2,&p1,&p2);
	int mask=((1<<(p2-p1+1))-1)<<p1;
	printf("\nmask\n");
	printbinary(mask);
	printf("\nnum1 clearing tht positions\n");
	num1=num1&~mask;
	printbinary(num1);
	printf("\nnum2 extracting tht position bits\n");
	num2=num2&mask;
	printbinary(num2);
	printf("\nnum1 after replacing\n");
	num1=num1|num2;
	printf("\n%d\n",num1);
	printbinary(num1);
}

