#include <stdio.h>
#include <stdbool.h>
#include <math.h>
int x=-123;
bool result=false;
int i=1;
int main()
{
    int y=x;
    if(x<0)
    {
        result=false;
    }
    if(x==0)
    {
        result=true;
    }
    while(x>0)
    {
        printf("a=%d\n",x/10);
        printf("b=%d\n",x%10);
        x=x/10;
        i++;

    }
    i--;
    printf("has %d\n",i);
    printf("%d\n",y);
    if(i==1)
    {
        result = true;
    }
    else
    {
        int j=0;
        while(i-j>=0)
        {
            printf("i=%d\n",(y/(int)pow(10,i-1))%10);
            printf("j=%d\n",(y/(int)pow(10,j))%10);
            
            if((y/(int)pow(10,i-1))%10==(y/(int)pow(10,j))%10)
            {
             i--;
             j++;
             printf("check\n");
            }
            else
            {
                break;
            }

        }
        if(i-j<=0)
        {
            result=true;
        }
        

    }

    printf("result = %d\n",result);

    return 0;
}