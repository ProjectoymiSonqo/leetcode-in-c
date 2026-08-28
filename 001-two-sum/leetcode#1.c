#include <stdlib.h>
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int *ans = malloc(sizeof(int) * 2);
    for(int i=0;i<numsSize;i++){
        for(int j=i+1;j<numsSize;j++){
            if(nums[i]+nums[j]==target)
            {
                ans[0]=i;
                ans[1]=j;
                *returnSize=2;
                return ans;

            }


        }
    }
    free(ans);
    return NULL;
}