
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// ========================================
// LeetCode 128. Longest Consecutive Sequence
// Implement your solution here
// ========================================

int longestConsecutive(int* nums, int numsSize)
{
    // Your solution here
    struct node{
        int val;
        bool visited;
        struct node *next;
    };
    int bucket=10007;
    struct node **table=calloc(bucket,sizeof(struct node*));
    for(int i=0;i<numsSize;i++)
    {
        bool duplicate=false;
        int index=nums[i]%bucket;
        if (index < 0)
            index += bucket;
        struct node *current=table[index];
        if(current==NULL)
        {
            struct node *new_node=malloc(sizeof(struct node));
            new_node->val=nums[i];
            new_node->next=NULL;
            new_node->visited=false;
            table[index]=new_node;
        }
        else
        {
            while(current!=NULL)
            {
                if(nums[i]==current->val)
                {
                    duplicate=true;
                    break;
                }
                else
                {
                    current=current->next;
                }
            }
            if(duplicate==false)
            {
                struct node *new_node=malloc(sizeof(struct node));
                new_node->val=nums[i];
                new_node->next=table[index];
                new_node->visited=false;
                table[index]=new_node;
            }
        }
    }
    int max_length=0;
    for(int i=0;i<numsSize;i++)
    {
        int index=nums[i]%bucket;
        if (index < 0)
            index += bucket;
        int length=1;
        bool checked=false;
        struct node *current=table[index];
        while(current!=NULL)
        {
            if(current->val==nums[i])
            {
                if(current->visited==false)
                {
                    current->visited=true;
                    break;
                }
                else
                {
                    checked=true;
                    break;
                }
            }
            else
            {
                current=current->next;
            }
        }
        if(checked==false)
        {
            int target_right;
            int target_left;                  //做調整不然會撞到下限    
            int right_index;
            int left_index;
            bool consecutive_right;
            bool consecutive_left;             
            if(nums[i]==INT_MAX)
            {
                consecutive_right=false;
            }
            else
            {
                target_right=nums[i]+1; 
                right_index=target_right%bucket;  
                if(right_index<0)
                    right_index+=bucket;               
                consecutive_right=true;
            }
            if(nums[i]==INT_MIN)
            {
                consecutive_left=false;
            }
            else
            {
                target_left=nums[i]-1;
                left_index=target_left%bucket;
                if (left_index < 0)
                    left_index += bucket;
                consecutive_left=true;
            }       
        while(consecutive_right==true)
        {
            if(table[right_index]!=NULL)
            {
                current=table[right_index];
                while(current!=NULL)
                {
                    if(current->val==target_right)
                    {
                        length++;
                        current->visited=true;
                        if(target_right==INT_MAX)
                        {
                            consecutive_right=false;
                        }
                        else
                        {
                            right_index=(right_index+1)%bucket;
                            target_right++;
                        }
                        break;
                    }
                    else
                    {
                        if(current->next==NULL)
                        {
                            consecutive_right=false;
                        }
                        current=current->next;
                    }
                }
            }
            else
            {   
                consecutive_right=false;
            }
        }
        while(consecutive_left==true)
        {
            if(table[left_index]!=NULL)
            {
                current=table[left_index];
                while(current!=NULL)
                {
                    if(current->val==target_left)
                    {
                        length++;
                        current->visited=true;
                        if(target_left==INT_MIN)
                        {
                            consecutive_left=false;
                        }
                        else
                        {
                            
                            left_index--;
                            if (left_index < 0)
                                left_index += bucket;
                            target_left--;
                        }
                        break;
                    }
                    else
                    {
                        if(current->next==NULL)
                        {
                            consecutive_left=false;
                        }
                        current=current->next;
                    }
                }
            }
            else
            {
                consecutive_left=false;
            }
        }
        if(length>max_length)
        {
            max_length=length;
        }
        }
    }
    for(int i=0;i<bucket;i++)
    {
        struct node *current=table[i];
        while(current!=NULL)
        {
            struct node *next=current->next;
            free(current);
            current=next;
        }
        
    }
    free(table);
    return max_length;
}

// ========================================
// Test Environment
// ========================================

void runTest(int id, int* nums, int numsSize, int expected)
{
    int result = longestConsecutive(nums, numsSize);

    printf("Test %02d | Expected: %-3d | Result: %-3d | %s\n",
           id,
           expected,
           result,
           result == expected ? "PASS" : "FAIL");
}

int main(void)
{
    printf("===== LeetCode 128 Test =====\n\n");

    // Test 01: Basic example
    int a1[] = {100, 4, 200, 1, 3, 2};
    runTest(1, a1, 6, 4);

    // Test 02: Duplicates
    int a2[] = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    runTest(2, a2, 10, 9);

    // Test 03: Empty array
    runTest(3, NULL, 0, 0);

    // Test 04: Single element
    int a4[] = {5};
    runTest(4, a4, 1, 1);

    // Test 05: All identical
    int a5[] = {2, 2, 2, 2};
    runTest(5, a5, 4, 1);

    // Test 06: Negative values
    int a6[] = {-3, -2, -1, 0, 1};
    runTest(6, a6, 5, 5);

    // Test 07: Unsorted with gaps
    int a7[] = {10, 5, 12, 3, 55, 11, 4};
    runTest(7, a7, 7, 3);

    // Test 08: Reverse order
    int a8[] = {5, 4, 3, 2, 1};
    runTest(8, a8, 5, 5);

    // Test 09: Multiple sequences
    int a9[] = {1, 2, 3, 10, 11, 20, 21, 22, 23};
    runTest(9, a9, 9, 4);

    // Test 10: No consecutive numbers
    int a10[] = {10, 20, 30, 40};
    runTest(10, a10, 4, 1);

    // Test 11: Large positive values
    int a11[] = {1000000, 999999, 1000001, 42};
    runTest(11, a11, 4, 3);

    // Test 12: Integer boundaries
    int a12[] = {INT_MAX, INT_MAX - 1, INT_MIN, INT_MIN + 1};
    runTest(12, a12, 4, 2);

    // Test 13: Duplicates mixed with negatives
    int a13[] = {-1, -1, 0, 1, 1, 2, -2};
    runTest(13, a13, 7, 5);

    // Test 14: One missing number
    int a14[] = {1, 2, 3, 5, 6, 7, 8};
    runTest(14, a14, 7, 4);

    // Test 15: Sequence across zero
    int a15[] = {-2, 2, 0, -1, 1};
    runTest(15, a15, 5, 5);

    printf("\n===== Test Finished =====\n");

    return 0;
}
