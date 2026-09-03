#include <stdio.h>
int nums[] = {0,0,1,1,1,1,2,3,3};

int main(){

int previous;
int k=1; 
int count=1;   
previous=nums[0];
    for(int i=1;i<sizeof(nums)/4;i++)
    {
        if(nums[i]!=previous)
        {
            previous=nums[i];
            count=1;
            nums[k]=nums[i];
             k++;
        }
        else if(count<2)
        {
            nums[k]=nums[i];
            k++;
            count++;
            
        }
        else{
            count++;

        }
    printf("k=%d\n",k);

    }
    for (int a = 0; a < sizeof(nums)/4; a++) {
        printf("%d ", nums[a]); // 逐一印出陣列值
    }
    return 0;
}