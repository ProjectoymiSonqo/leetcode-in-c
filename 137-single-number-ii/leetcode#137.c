#include <stdio.h>
#include <math.h>

int nums[] = {-2,-2,1,1,4,1,4,4,-4,-2};
int count[32]={0};
u_int32_t result=0;
int main(){
    int numsSize=sizeof(nums)/4;
    for(int i=0;i<numsSize;i++)
    {
        // for(int j=0;j<32;j++)
        // {
        //     if((nums[i]&1u)==1u)
        //     {
        //         count[j]++;
        //     }
        //     nums[i]=nums[i]>>1u;
        // }
        u_int32_t x = (u_int32_t)nums[i];
        printf("%u,%d\n",(u_int32_t)nums[0],nums[0]);
        for (int j = 0; j < 32; j++)
        {
            if ((x & 1u) == 1u)
            {
                count[j]++;
            }
            x >>= 1;
        }
    }
    for(int i=0;i<32;i++)
    {
        result=result+(count[i]%3)*pow(2,i);
    //         if (count[i] % 3)

    // {

    //     result |= (1u << i);

    // }
    }
    printf("result=%d\n",(int)result);


    return 0;
}