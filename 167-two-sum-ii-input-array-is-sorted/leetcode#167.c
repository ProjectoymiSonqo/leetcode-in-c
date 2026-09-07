#include <stdio.h>
#include <stdlib.h>

int numbers[] ={3,5,6,8,10}; 
int target = 18;
int sum;
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int sum;
    int left=0;
    int right=numbersSize-1;  
    while(right-left>0)
    {
        sum=numbers[left]+numbers[right];
        if(sum==target)
        {
            int *result=malloc(2 * sizeof(int));
            result[0]=left+1;
            result[1]=right+1;
            *returnSize = 2;

            return result;
        }
        else if(sum<target)
        {
            left++;
        }
        else if(sum>target)
        {
            right--;
        }
    }
    *returnSize = 0;
    return NULL; 
}
int main()
{
    int len=sizeof(numbers)/4;
    int left=0;
    int right=len-1;  
    while(right-left>0)
    {
        sum=numbers[left]+numbers[right];
        if(sum==target)
        {
            int *result=malloc(2 * sizeof(int));
            result[0]=left+1;
            result[1]=right+1;
            printf("returnsize[%d],[%d]=%d,%d",0,1,result[0],result[1]);
            break;
        }
        else if(sum<target)
        {
            left++;
        }
        else if(sum>target)
        {
            right--;
        }
    }
    // for (int i=left; i<right;i++)
    // {
    //     for (int j=i+1;j<right;j++)
    //     {
    //         sum = numbers[i]+numbers[j];
    //         if(sum==target)
    //         {
    //          printf("i=%d,j=%d\n",i,j);
    //          int *result = malloc(2 * sizeof(int));
    //          result[0]=i+1;
    //          result[1]=j+1;
    //          printf("returnsize[%d],[%d]=%d,%d",0,1,result[0],result[1]);
    //          break;
    //         }
    //         if(sum<target)
    //         {
    //             left++;
    //         }
    //         else if(sum>target)
    //         {
    //             right--;
    //         }
            
    //     }

    // }

    
    return 0;
}