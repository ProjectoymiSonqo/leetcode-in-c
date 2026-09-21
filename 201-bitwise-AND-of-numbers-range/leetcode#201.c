#include <stdio.h>
#include <stdbool.h>

int left = 416;
int right = 436;
int result=0;
int left_1_position;
int right_1_position;
bool right_1_position_find=false;
int count=31;
int result_box[31]={0};
int main()
{
    while(count>=0)
    {
        if(right_1_position_find==false)
        {
            if((right>>count)&1u)
            {
                right_1_position=count;
                printf("%d,",right_1_position);
                right_1_position_find=true;
            }
        }
        if(right_1_position_find==true)
        {
            if((left>>count)&1u)
            {
                left_1_position=count;
                printf("%d\n",left_1_position);
                break;
            }
        }
        
        count--;
    }
    if(left_1_position==right_1_position)
    {
        for(int i=left_1_position;i>=0;i--)
        {
            if(((left>>i)&1u)==((right>>i)&1u))
            {
                if(((left>>i)&1u)==1u && ((right>>i)&1u)==1u)
                {
                    result_box[i]=1;
                    result|=(1u<<i);
                    printf("i=%d\n",i);
                }
            }
            else
            {
                break;
            }
        }
        //result|=(left_1_position<<1);
    }
    else
    {
        result=0;
    }
    printf("%d",result);
    return 0;
}