#include <stdio.h>
#include <stdbool.h>
int nums1[]={0,1,1,2,2,0,0,0};
int m=5;
int nums2[]={0,3,3};
int n=3;

int main()
{
    int i=m-1;
    int j=n-1;
    for(int k=m+n-1;k>0;k--)
    {
        if(m==0)
        break;
        else if(n==0)
        break;
        if(nums1[i]<nums2[j])
        {
           nums1[k]=nums2[j];
           if(j==0)
           {

            break;
           }
           else
           {
             j--;
           }
        }
         else 
         {
             nums1[k]=nums1[i];
             if(i==0)
             {
                for(int a=0;a<k;a++)
                {
                    nums1[a]=nums2[a];
                }
                break;
             }
             else
             {
                i--;
             }
         }
         printf("k=%d,nums1[k]=%d\n",k,nums1[k]);
    }
    if(m==0&&n!=0)
    {
        for(int a=0;a<m+n;a++)
        {
        if(nums1[a]==0&&nums2[a]!=0)
        {
          nums1[a]=nums2[a];
        }
         }
    }


    for (int a = 0; a < sizeof(nums1)/4; a++) {
        printf("%d ", nums1[a]); // 逐一印出陣列值
    }
    
    
    return 0;
}