#include<stdio.h>
int dec_hex(int n)
{
	char hex[20];int rem,i=0,j;
	while(n!=0)
	{
		rem=n%16;
		hex[i++]=(rem<10) ? (rem+'0') : (rem-10+'A');
		n=n/16;
	}
	for(j=i-1;j>=0;j--)
		printf("%c",hex[j]);
}
int main()
{
	int n;
	scanf("%d",&n);
	dec_hex(n);
}
