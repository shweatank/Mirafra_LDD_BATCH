/*
 * Name: Ashritha Reddy
 * Date:
 * File_name:
 * Sample_Input:
 * Sample_Output:
 *
 * */

#include<stdio.h>
int power(int a,int b)
{
	if(b==0)
		return 1;
	else
		return a*power(a,--b);

}
int main()
{
	int a,b;
	scanf("%d%d",&a,&b);
	printf("%d",power(a,b));
	return 0;
}

