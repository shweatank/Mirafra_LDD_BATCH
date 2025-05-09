#include<stdio.h>
int main()
{
	char s[100];
	scanf("%99[^\n]s",s);
	int st=0,end=0,c1=0,i,j;
	for(i=0;;i++)
	{
		if(s[i]==' ' || s[i]=='\0')
		{
			c1=0;
			for(j=st;j<i;j++)
			{
				if(s[j]=='a' || s[j]=='e' || s[j]=='i' || s[j]=='o' || s[j]=='u')
				{	
					c1=1;
					break;
				}
			}
			if(c1==0){
				for(j=st;j<i;j++){
					printf("%c",s[j]);
				}
			printf(" ");
			}
			st=i+1;
		}
		if(s[i]=='\0')
			break;
	}
}
