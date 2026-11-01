#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// ========================================
// LeetCode 202. Happy Number
// 在這裡實作你的演算法
// ========================================

int calculate(int n)
{
    int result=0;
    while(n/10!=0)
    {
        result=result+n%10*(n%10);
        n=n/10;
    }
    result=result+n%10*(n%10);
    return result;
}

bool isHappy(int n)
{
    // Your solution here
    int new_value=0;
    new_value=calculate(n);
    printf("result=%d\r\n",new_value);
    struct ListNode{
        int value;
        struct ListNode *next;
    };
    struct ListNode **table;
    table=calloc(17,sizeof(struct ListNode *));
    int index;
    bool result=false;
    bool end=false;
    while(end==false)
    {
        if(new_value==1)
        {
            result=true;
            end=true;
            break;
        }
        else
        {
            index=new_value%17;
            struct ListNode *current=table[index];
            while(current!=NULL)
            {
                if(current->value==new_value)
                {
                    result=false;
                    end=true;
                    break;
                }
                else
                {
                    current=current->next;
                }
            }
            if(end==true)
            {
                break;
            }
            struct ListNode *new_node=malloc(sizeof(struct ListNode));
            new_node->value=new_value;
            new_node->next=table[index];
            table[index]=new_node;            
        }
        new_value=calculate(new_value);
    }
    for(int i=0; i<17; i++)
    {
        if(table[i]!=NULL)
        {
            struct ListNode *current=table[i]->next;
            struct ListNode *prev=table[i];
            while(prev!=NULL)
            {
                free(prev);
                prev=current;
                if(current!=NULL)
                {
                    current=current->next;
                }
            }
        }
        // 逐一走訪每個 Bucket
        // 釋放其中的所有 Node
    }
    free(table);
    return result;

}

// ========================================
// Test Environment
// ========================================

void runTest(int n, bool expected)
{
    bool result = isHappy(n);

    printf("Input: %-10d | Expected: %-5s | Result: %-5s | %s\n",
           n,
           expected ? "true" : "false",
           result ? "true" : "false",
           result == expected ? "PASS" : "FAIL");
}

int main(void)
{
    printf("===== Happy Number Test =====\n\n");

    runTest(19, true);
    runTest(2, false);
    runTest(1, true);
    runTest(7, true);
    runTest(10, true);
    runTest(4, false);
    runTest(20, false);
    runTest(100, true);

    printf("\n===== Test Finished =====\n");

    return 0;
}