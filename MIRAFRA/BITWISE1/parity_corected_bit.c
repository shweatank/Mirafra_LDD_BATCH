#include<stdio.h>
#include<stdlib.h>
int countones(int num)
{
	int i,c=0;
	for(i=0;i<7;i++)
	{
		if(num&(0x1<<i))
			c++;
	}
	return c;
}
void getparitybit(unsigned char num, int evenparity)
{
	int parity;
	int ones=countones(num);
	if(evenparity)
		parity=(ones%2==)?1:0;
	else
		parity=(ones
		
}
int main()
{
	unsigned char num;
	int parity;
	printf("Enter number\n");
	scanf("%d",&num);
	printf("chose parity 0-even 1-odd\n");
	scanf("%d",&parity);
	if(parity==0)
		parity=1;
	int num=getparitybit(num,parity);

}

