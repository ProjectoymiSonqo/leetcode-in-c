#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
int x= -15020;
bool sign;
int new_x;
// typedef struct Node
// {
//     int digit;
//     struct Node *next;
// } Node;
int reverse(int x){
    long long new_x=0;
    
    while(x!=0)
    {
        int digit = x % 10;
        new_x=new_x*10+digit;
    if (new_x > INT_MAX || new_x < INT_MIN)

        return 0;        
        x=x/10;
    }

    return (int)new_x;

}
int main()
{
    if(x<0)
    {
        sign=false;
    }
    else
    {
        sign=true;
    }
    x=abs(x);
    // struct Node *head = NULL;
    // struct Node *tail = NULL;
    new_x=0;
    while(x!=0)
    {
        int digit = x % 10;

        // Node *newNode = malloc(sizeof(Node));
        // newNode->digit = digit;
        // newNode->next = NULL;
        new_x=new_x*10+digit;
        // if (head == NULL)
        // {
        //     head = newNode;
        //     tail = newNode;
        // }
        // else
        // {
        //     tail->next = newNode;
        //     tail = newNode;
        // }
        
        x=x/10;
    }
    if(sign==false)
    {
        new_x=-new_x;
    }
    printf("%d",new_x);

    return 0;
}
