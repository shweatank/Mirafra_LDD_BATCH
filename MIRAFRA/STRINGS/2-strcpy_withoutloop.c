#include<stdio.h>
int i=0;
int main()
{
	char s[100];char d[100];
	printf("enter source and dest:\n");
	scanf("%s",s);
	scanf("%s",d);
	mystrcpy(s,d);
	printf("source: %s\n",s);
	printf("dest: %s\n",d);
}
int mystrcpy(char s[],char d[])
{
//	int i;
	printf("i entered\n");
	if(s[i]=='\0')
		return;
	else
	{
		d[i]=s[i];
		i++;
		mystrcpy(s,d);
	}
}
