#include <stdio.h>
#include <stdbool.h>
char s[]="    x    -520s";
int i=0;
bool sign;
int x=0;
int main()
{
    
    while(s[i]==' ')
    {
        i++;
    }
    if(s[i]=='-')
    {
        sign=false;
        i++;
    }
    
    while(1)
    {
        if((s[i] >= '0' && s[i] <= '9'))
        {
            int digit=s[i]-'0';
            x=x*10+digit;
            printf("x is %d\n",x);
            printf("digit is %d\n",digit);
            i++;
        }
        else
        {
//            i--;
            break;
        }
    }
    if(sign==false)
    {
        x=-x;
    }
    printf("x is %d\n",x);

//    printf("%c\n",s[i]);

    return 0;
}