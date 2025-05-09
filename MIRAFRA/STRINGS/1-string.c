#include<stdio.h>
#include<string.h>
int main()
{
	char s[100];
	int i=0,n=0;
	scanf("%99[^\n]s",s);
	int l=strlen(s);
	int a[100];
	for(i=0;i<=l;i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			a[n++]=s[i]-48;
		}
	}
	//for(i=0;i<n;i++)
	//	printf("%d ",a[i]);
	sort(n,a);
	//for(i=0;i<n;i++)
        //       printf("%d ",a[i]);
	sortstring(s,n,a);		
}
void sortstring(char s[],int n,int a[])
{
	int i;
	int l=strlen(s);
	printf("len is %d\n",l);
	int j,k,num,index;
	for(i=0;i<n;i++)
	{
		//printf("MY I VALUE IS : %d\n",i);
		for(j=0;s[j]!='\0';j++)
		{
			//printf("s[j]==a[i] %c %d\n", s[j],a[i]);
			if(s[j]==(a[i]+48))
			{
				num=j;
			//	printf("num %d is found at %d =%d\n",a[i],j,num);
				break;
			}
			else
				continue;	
		}
		for(l=num;l>=0;l--)
		{
			if(s[l]==' '||l==0)
			{
				index=l;
			//printf("index space came at %d\n",index);
			break;
			}
		}
		if(index==0)
			k=index;
		else if(index!=0)
			k=index+1;
		
		for(;s[k]!=' '||s[k]!='\0' ;k++)
		{
			
			printf("%c",s[k]);
			if(s[k+1]==' '|| s[k+1]=='\0')
				break;
			else
				continue;
		}
		printf(" ");
	}
}
int sort(int n,int a[])
{
	int i,j,temp;
	for(i=0;i<n;i++)
	{
	for(j=0;j<n-1;j++)
	{
		if(a[i]<a[j])
		{
			temp=a[i];
			a[i]=a[j];
			a[j]=temp;
		}
	}
	}
}
