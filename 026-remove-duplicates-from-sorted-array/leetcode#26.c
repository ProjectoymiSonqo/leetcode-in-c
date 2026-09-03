#include <stdio.h>
int nums[] = {0,0,1,1,1,2,2,3,3,4};
int previous;
int k=1;
int main(){
    previous=nums[0];

    printf("previous=%d\n",previous);
    for(int i=1;i<sizeof(nums)/4;i++)
    {
        if(nums[i]!=previous)
        {
            previous=nums[i];
            nums[k]=nums[i];
            k++;
            printf("k=%d\n",k);
            printf("previous=%d\n",previous);
            
        }
    }
        for (int a = 0; a < sizeof(nums)/4; a++) {
        printf("%d ", nums[a]); // 逐一印出陣列值
    }

    return 0;
}