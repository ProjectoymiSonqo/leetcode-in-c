#include <stdio.h>
int nums[]={3,2,2,3};
int val=3;
int count=0;
int len;

int main(){
len=sizeof(nums)/4;
    for(int i=0;i<sizeof(nums)/4;i++)
    {
        if(nums[i]==val)
        {
            printf("index=%d\n",i);
            count++;
            printf("count=%d\n",count);
            for(int j=len-1;j>0;j--)
            {
                printf("j=%d\n",j);
                printf("i=%d\n",i);
                if(nums[j]!=val&&nums[j]>=0)
                {
                    nums[i]=nums[j];
                    nums[j]=-1;
                    break;

                }
            }

        }

    }

    for(int i=0;i<len-count;i++)
    {

        if(nums[i]==-1)
        {
            while(1)
            {
                if(nums[len-1]!=-1)
                {
                    nums[i]=nums[len-1];
                    nums[len-1]=-1;
                    break;
                }
                else{len--;}
            }


        }
    }
        for (int a = 0; a < sizeof(nums)/4; a++) {
        printf("%d ", nums[a]); // 逐一印出陣列值
    }


    return 0;

}