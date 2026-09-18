#include <stdio.h>

int n=2147483645;
int count_1=0;
int count=31;
int main()
{
    // while(count>=0)
    // {
    //     if((n&1u)==1u)
    //     {
    //         count_1++;
    //     }
    //     n=n>>1u;
    //     count--;
    // }
    while (n != 0) 
    {

    n = n & (n - 1);

    count_1++;

    }
    printf("%d",count_1);

    return 0;

}