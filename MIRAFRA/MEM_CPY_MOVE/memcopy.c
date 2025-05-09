#include<stdio.h>
#include<string.h>
void* mymemcpy(void* des, void* str,size_t n)
{
	int i=0;
	char *sour= (char *)str;
	char *dest=(char *)des;
	for(i=0;i<n;i++)
	{
		dest[i]=sour[i];
	}
	return des;
}
int main()
{
        char str[]="hello ashritha";
        char des[100];
        mymemcpy(des,str,strlen(str)+1);
	printf("%s\n",des);
	memmove(str+5,str,strlen(str)+1);
	printf("%s\n",str);
	mymemcpy(str+3,str,strlen(str)+1);//undefined behaviour
	 printf("%s\n",str);
	int a[]={10,20,30,40,50};
	int n=sizeof(a)/sizeof(a[0]);
	int a1[n];
	mymemcpy(a1,a,sizeof(a));
	int i=0;
	for(i=0;i<n;i++)
	printf("%d ",a1[i]);
        
}

