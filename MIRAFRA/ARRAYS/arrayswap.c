#include<stdio.h>
int main()
{
	int n,i;
	scanf("%d",&n);
	int a[n];
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	func(n,a);
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
}
int func(int n,int *a)
{
	int i;
//	printf("%p",a);
	int *p=a+(n-1);
//	printf("%p\n",p);
//	printf("%d\n",*p);
	while(a<p)
	{
		swap(a,p);
		a++;
		p--;
	}	
}
int swap(int *a,int *b)
{
	int temp=*a;
	*a=*b;
	*b=temp;

}
