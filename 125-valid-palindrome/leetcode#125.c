#include <stdio.h>
#include <string.h>
#include <stdbool.h>
char s[]= " ";

char normalize_char(char c)
{
    if(c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    else if((c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9'))
    {
        return c;
    }

    return '\0';

}
int main()
{
    int len=strlen(s);
    int j=len-1;
    int left=0;
    int right=len-1;
    char temp_left;
    char temp_right;
    bool result=true;
while(left<right)
{
    for(int i=left;i<len;i++)
    {
        temp_left=normalize_char(s[i]);
        if(temp_left!='\0')
        {
            break;
        }
        else
        {
            left++;
        }
    }        
    for(int j=right;j>left;j--)
    {
        temp_right=normalize_char(s[j]);
        if(temp_right!='\0')
        {
            break;
        }
        else
        {
            right--;
        }
    }
    if(left>=right)
    {
        result=true;
        break;
    }
    if(temp_right!=temp_left)
    {
        result=false;
        break;
    }
        left++;
        right--;

}
printf("result=%d",result);
    return 0;
}