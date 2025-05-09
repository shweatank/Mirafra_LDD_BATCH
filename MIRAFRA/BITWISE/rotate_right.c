/*
 * Name: Ashritha Reddy
 * Date:
 * File_name:
 * Sample_Input:
 * Sample_Output:
 *
 * */

#include<stdio.h>
void printbin(int);
int main()
{
	int num;
	int p;
	scanf("%d%d",&num,&p);
	printbin(num);
	printf("\n");
	int total_bits=sizeof(num)*8;
	int temp=(num>>p);
	int temp1=(num<<total_bits-p);
	num=temp|temp1;
	printf("%x \n",num);
	printbin(num);
	return 0;
}
void printbin(int num)
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


