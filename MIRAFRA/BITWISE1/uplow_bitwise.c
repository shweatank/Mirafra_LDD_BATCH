
#include<stdio.h>
int main()
{
	char c='A';
	printf("upper %c to lower case %c\n",c,c|32);
	c='a';
        printf("lower %c to upper case %c\n",c,c&~32);

}
