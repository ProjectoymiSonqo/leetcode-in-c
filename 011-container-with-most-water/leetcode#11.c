#include <stdio.h>
int height[] = {1,8,6,2,5,4,8,3,7};
int max_area=0;
int main(){
    int left=0;
    int len= sizeof(height)/4;
    int right=len-1;
    int current=0;
    while(right>left)
    {
        if(height[left]<=height[right])
        {
            current=height[left]*(right-left);
        }
        else
        {
            current=height[right]*(right-left);
        }
        if(current>max_area)
        {
            max_area=current;
        }
        if(height[left]<height[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    printf("area=%d\n",max_area);

    return 0;
}