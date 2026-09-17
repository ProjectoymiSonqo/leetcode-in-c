#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char a[] = "11";
char b[] = "111";//return1010
int main(){
    int len_a=strlen(a);
    int len_b=strlen(b);
    int max_len = (len_a>len_b)?len_a:len_b;
//    printf("%d\n",max_len);
    int c_int[max_len+1];
    int i=len_a-1;
    int j=len_b-1;
    int k=0;
    int carry=0;
    while(j>=0||i>=0)
    {
//         printf("a[%d]=%c,b[%d]=%c\n",i,a[i],j,b[j]);
        // printf("c[k]=%d\n",c[k]);
        if(i<0||j<0)
        {
            if(i<0)
            {
                c_int[k]=(b[j]-'0'+carry)%2;
                if(b[j]-'0'+carry>=2)
                {
                    carry=1;
                }
                else
                {
                    carry=0;
                }
            }
            else
            {
                c_int[k]=(a[i]-'0'+carry)%2;
                if(a[i]-'0'+carry>=2)
                {
                    carry=1;
                }
                else
                {
                    carry=0;
                }
            }
        }
        else
        {
            c_int[k]=(a[i]-'0'+b[j]-'0'+carry)%2;
            if(a[i]-'0'+b[j]-'0'+carry>=2)
            {
                carry=1;
            }
            else
            {
                carry=0;
            }
        }
        printf("c_int[%d]=%d\n",k,c_int[k]);
        k++;
        j--;
        i--;
    }
    if(carry==1)
    {
        c_int[k]=1;
        printf("c_int[%d]=%d\n",k,c_int[k]);
        k++;
    }

    char *c=(char *)malloc((k+1)*sizeof(char));
    int count=k-1;
    for(int i=0; i<k;i++)
    {
        
        c[i]=c_int[count]+'0';
        count--;
        printf("%c",c[i]);

    }
    c[k]='\0';
    return 0;

}