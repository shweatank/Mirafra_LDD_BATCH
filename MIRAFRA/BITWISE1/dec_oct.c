#include<stdio.h>
int dec_oct(int n)
{
	int oct[32];int rem,i=0,j;
	while(n!=0)
	{
		rem=n%8;
		oct[i++]=rem;
		n=n/8;
	}
	for(j=i-1;j>=0;j--)
		printf("%d",oct[j]);
}
int main()
{
	int n;
	scanf("%d",&n);
	dec_oct(n);
}

