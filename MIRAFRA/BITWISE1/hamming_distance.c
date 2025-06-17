/*✅ What is Hamming Distance?

    The Hamming distance between two integers is the number of bit positions at which the corresponding bits are different.

🧠 How do we find it?

    Take XOR of the two numbers — this highlights the differing bits.

    Count the set bits (1s) in the XOR result — those are the differing bits.*/
#include<stdio.h>
int main()
{
	int n=2,n1=3;
	int x=n1^n;int c=0;
	while(x!=0)
	{
		c++;
		x=x&(x-1);
	}
	printf("%d",c);
}
