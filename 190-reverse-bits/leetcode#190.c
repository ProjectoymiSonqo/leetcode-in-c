#include <stdio.h>

//u_int8_t n = 0b10110110;
int n=43261596;
//int inverse=~n;
int main()
{
//    printf("%d",n<<1);
    int count=31;
    unsigned int get;
    unsigned int new_n=0;
    unsigned int value = (unsigned int)n;
    while(count>=0)
    {
        get=value&1u;
        if(get==1)
        {
            new_n=new_n|(1u<<count);
        }
        else
        {
            new_n=new_n&~(1u<<count);
        }
        value=value>>1;
        count--;
    printf("%d\n",new_n);
    }
    return 0;
}