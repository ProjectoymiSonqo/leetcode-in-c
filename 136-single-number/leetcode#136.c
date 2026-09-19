#include <stdio.h>
int nums[] = {4};
int result=0;
int main(){
    int numsSize=sizeof(nums)/4;

    for(int i=0;i<numsSize;i++)
    {
        result^=nums[i];
    }
    printf("%d",result);
    return 0;

}