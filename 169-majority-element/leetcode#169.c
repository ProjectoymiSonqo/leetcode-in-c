#include <stdio.h>
#include <stdbool.h>
int nums[] = {6,53,53,96,45,79,53,58,53,90,40,53,53,1,53,53,89,53,33,27,53,53,84,42,53,53,87,51,66,53,28,53,53,53,50,39,36,48,19,74,38,53,42,53,99,53,80,53,53,53,53,96,78,52,24,53,53,53,53,64,10,53,53,53,53,82,53,53,53,22,53,53,67,53,53,53,53,53,67,53,19,99,53,53,21,53,69,53,53,53,52,53,96,53,53,51,81,62,4,6};
int count=1;
int count_max=1;
int check_value[sizeof(nums)/4];
int max_value;
int j=0;
int main(){
check_value[0]=nums[0];
while(count_max<sizeof(nums)/4/2+1)
{
    for(int i=1;i<sizeof(nums)/4;i++)
    {
        if(nums[i]==check_value[j])
        {
            count++;
            if(count>count_max)
            {
//                max_value=check_value;
                count_max=count;
                printf("countmax=%d\n",count_max);
                max_value=check_value[j];
                printf("value is %d\n",check_value[j]);
            }
        }
        else{
bool exist = false;

for(int k = 0; k < j+1; k++)

{

    if(nums[i] == check_value[k])

    {

        exist = true;

        break;

    }

}

if(!exist)

{

    check_value[j+1] = nums[i];

}

        }



    }
    count=0;
    j++;
                    
}

printf("value is %d\n",max_value);
        for (int a = 0; a < sizeof(check_value)/4; a++) {
        printf("%d ", check_value[a]); // 逐一印出陣列值
    }

    return 0;
}