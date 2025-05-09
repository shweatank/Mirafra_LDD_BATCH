#include<stdio.h>
int main()
{
	int n,i;
	scanf("%d",&n);
	int a[n];
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	firstrepeat(n,a);
}

int firstrepeat(int n,int a[])
{
	int i,j,c=0;
	for(i=0;i<n;i++)
	{
//		printf("i is %d\n",i);
		for(j=i+1;j>=0;j--)
		{
//			printf("j is %d\n",j);		
			if(i==j)
				continue;
			if(a[i]==a[j])
			{
				printf("%d",a[j]);
				c++;
				break;
			}
		}
		if(c==1)
			break;
				
	}
}
