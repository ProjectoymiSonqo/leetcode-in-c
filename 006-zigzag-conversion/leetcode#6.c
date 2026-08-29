#include <stdio.h>
#include <stdbool.h>
#include <string.h>
char s[]="PAHNAPLSIIGYIR";
int numRows=5;


int main()
{
    bool direction=true;
    int len=strlen(s);
    char ns[len+1];
    int j=1;
    int index[strlen(s)];
    for(int i=0;i<strlen(s);i++)
    {
        index[i]=j;
        if(direction==true&&j==numRows)
        {
            direction=false;
        }
        else if(direction==true)
        {
            j++;
        }

        if(direction==false&&j==1)
        {
            j++;
            direction=true;
        }
        else if(direction==false)
        {
            j--;
        }
        printf("direction = %d\n",direction);
        printf("%d\n",index[i]);
        
    }
    int count=0;
    for(int k=1;k<=numRows;k++)
    {
        for(int l=0;l<strlen(s);l++)
        {
            if(index[l]==k)
            {
                ns[count]=s[l];
                count++;

            }

        }
    }
    ns[count]='\0';
    printf("%s",ns);


    return 0;
}