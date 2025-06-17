#include<stdio.h>
int set(int n, int p)
{
        int i,pow=1;
        for(i=0;i<p;i++)
        {
                pow=pow*2;
        }
        if((n/pow)%2==1)
                n=n-pow;
        return n;
}
int main()
{
        int n=10;
        int i=1;
        int k=set(n,i);
        printf("%d",k);
}
