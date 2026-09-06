#include <stdio.h>
#include <string.h>
#include <stdbool.h>
char s[] = "aza";
char t[] = "abzba";
int indext=0;
int indexs=0;
bool result=true;
int count=0;
int main(){
    int lens=strlen(s);
    int lent=strlen(t);
    for(int i=indexs;i<lens;i++)
    {
        for(int j=indext;j<lent;j++)
        {
            if(s[i]==t[j])
            {
                printf("s[%d]=%c,t[%d]=%c\n",i,s[i],j,t[j]);
                count++;
                printf("count=%d\n",count);
                indext=j+1;
                indexs=i+1;
                break;

            }
        }
    }
    if(count<lens)
    {
        result=false;
    }
    printf("count=%d,result=%d\n",count,result);
    return 0;
}