#include<stdio.h>
int main()
{
	int a;
	int p1,p2,c;
	scanf("%d%d%d%d",&a,&p1,&p2,&c);
	printbin(a);
	int i,j,k,l,n1,n2;
	for(i=p1,j=p2;i<j;i=i+c)
	{
		for(k=i,l=i+c;k<l;k++,l--)
		{
			n1=a>>k&1;
			n2=a>>l&1;
			if(n1!=n2)
			{
				a=a^(1<<k);
				a=a^(1<<l);
			}
		}
	}
	printf("\n");
	printbin(a);
}

void printbin(char num)
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


