#include <stdio.h>
int prices[] ={2,4,1};
int len=sizeof(prices)/4;
int profit=0;
int max_price;
int mini_price;
int max_price_index;
int mini_price_index;
int main(){
    max_price=prices[len-1];
    mini_price=prices[len-1];
    max_price_index=len-1;
    mini_price_index=len-1;
    for(int i=len-1;i>=0;i--)
    {
        if(prices[i]>max_price)
        {
            max_price=prices[i];
            max_price_index=i;
            mini_price=prices[i];
            mini_price_index=i;
            printf("max=%d\n",max_price);
        }
        if(prices[i]<mini_price)
        {
            mini_price=prices[i];
            mini_price_index=i;
            printf("mini=%d\n",mini_price);
        }
        if(max_price_index-mini_price_index>0)
        {
            profit=profit+max_price-mini_price;
            printf("profit=%d\n",profit);
            max_price=mini_price;
            max_price_index=mini_price_index;
        }

    }
    printf("profit=%d\n",profit);


    return 0;  
}