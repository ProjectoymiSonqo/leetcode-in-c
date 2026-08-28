#include <stdio.h>
#include <string.h>
#include <stdbool.h>
char s[]="sasbsas";
int checkpoint;
int max_len;
int best_start;
int best_end;

bool is_palindrome(char *s,int left,int right)
{
    bool result=false;

    while(s[left]==s[right])
    {
        if((right-left)<=1)
        {
            result=true;
            break;
        }
        left++;
        right--;
    }
    return result;
}
int main(){
    int len=strlen(s);
int left;
int right=len-1;


    for(left=0;left<len;left++)
    {
        for(right=len-1;right;right--)
        {
            if(s[left]!=s[right])
            {
                continue;
            }
            printf("index:[%d],[%d]\n",left,right);
            if(is_palindrome(s,left,right))
            {
                printf("find new palindrome\n");
                int curr_len=right-left+1;
                if(curr_len>max_len)
                {
                    max_len=curr_len;
                    best_start=left;
                    best_end=right;
                }
                printf("max_len is %d\n",max_len);
                printf("best_start is %d\n",best_start);
                printf("best_end is %d\n",best_end);

            }
            break;
        }

    }

    return 0;
}