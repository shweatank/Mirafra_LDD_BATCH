#include<stdio.h>
#include<string.h>
void rev(char str[],int l)
{
	int i;
        for(i=0;i<l/2;i++)
        {
                char temp=str[i];
                str[i]=str[l-i-1];
                str[l-i-1]=temp;
        }

}
void wordrev(char str[],int len)
{
	int i,start=0,end=0;char temp;
	for(i=0;i<=len;i++)
	{
		end=i-1;
		if(str[i]==' '|| str[i]=='\0')
		{
			while(start<end)
			{
				temp=str[start];
		//		printf("%c",temp);
				str[start]=str[end];
				str[end]=temp;
				start++;
				end--;
			}			
			start=i+1;
		}
	}
}
int main()
{
	char str[100];
	scanf("%99[^\n]s",str);
	int l=strlen(str);
	rev(str,l);
//	printf("%s\n",str);
	wordrev(str,l);
	printf("%s\n",str);

}

