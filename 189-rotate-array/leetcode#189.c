#include <stdio.h>
int nums[] ={1};
int k = 0;
int len=sizeof(nums)/4;
int j=0;
int main()
{
    int box[k];
    
    for(int i=len-k;i<len;i++)
    {
        box[j]=nums[i];
        // printf("%d, ",j);
        // printf("%d\n",box[j]);
        if(j==k-1)
        {
            break;
        }
        j++;
        
    }
    for(int l=len-1;l>=0;l--)
    {
        
        if(l-k<0)
        {
            // printf("%d, ",l);
            // printf("%d\n",box[l]);
            nums[l]=box[l];
        }
        else
        {
            nums[l]=nums[l-k];
        }
        printf("%d\n",nums[l]);
    }


    return 0;
}