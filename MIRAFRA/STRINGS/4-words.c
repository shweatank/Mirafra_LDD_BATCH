#include<stdio.h>
#include<string.h>
int main()
{
	char s[]="anu is good girl";
	char *word =strtok(s, " ");
	while(word != NULL)
	{
		printf("%s \n",word);
		word=strtok(NULL, " ");
	}
}
