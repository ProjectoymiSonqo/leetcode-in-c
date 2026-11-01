#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ========================================
// LeetCode 219. Contains Duplicate II
// 在這裡實作你的演算法
// ========================================

bool containsNearbyDuplicate(int* nums, int numsSize, int k)
{
    // Your solution here
    bool result=false;
    struct node{
        int val;
        int position;
        struct node *next;
    };
    int bucket_size=10007;
    struct node **table=calloc(bucket_size,sizeof(struct node *));
    for(int i=0;i<numsSize;i++)
    {
        bool update_value=false;
        if(result==true)
        {
            break;
        }
        int index=abs(nums[i]%bucket_size);
        struct node *current=table[index];
        if(current==NULL)
        {
            struct node *new_node=malloc(sizeof(struct node));
            new_node->position=i;
            new_node->val=nums[i];
            new_node->next=NULL;
            table[index]=new_node;
        }
        else
        {
            while(current!=NULL)
            {
                if(current->val==nums[i])
                {
                    if(abs(current->position-i)<=k)//判斷k
                    {
                        result=true;
                        update_value=true;
                        break;
                    }
                    else
                    {
                        current->position=i;
                        update_value=true;
                        break;
                    }
                }
                else
                {
                    current=current->next;
                }
            }
            if(update_value==false)
            {
                struct node *new_node=malloc(sizeof(struct node));
                new_node->position=i;
                new_node->val=nums[i];
                new_node->next=table[index];
                table[index]=new_node;
                update_value=true;
            }

        }

       
    }
    return result;
}

// ========================================
// Test Environment
// ========================================

void runTest(
    int testID,
    int* nums,
    int numsSize,
    int k,
    bool expected
)
{
    bool result = containsNearbyDuplicate(nums, numsSize, k);

    printf("Test %02d | k=%-3d | Expected: %-5s | Result: %-5s | %s\n",
           testID,
           k,
           expected ? "true" : "false",
           result ? "true" : "false",
           result == expected ? "PASS" : "FAIL");
}

int main(void)
{
    printf("===== LeetCode 219 Test =====\n\n");

    // Test 01: Basic true
    int a1[] = {1, 2, 3, 1};
    runTest(1, a1, 4, 3, true);

    // Test 02: Adjacent duplicates
    int a2[] = {1, 0, 1, 1};
    runTest(2, a2, 4, 1, true);

    // Test 03: Duplicates too far apart
    int a3[] = {1, 2, 3, 1, 2, 3};
    runTest(3, a3, 6, 2, false);

    // Test 04: Single element
    int a4[] = {1};
    runTest(4, a4, 1, 1, false);

    // Test 05: No duplicates
    int a5[] = {1, 2, 3, 4, 5};
    runTest(5, a5, 5, 3, false);

    // Test 06: Same value, multiple positions
    int a6[] = {1, 2, 1, 3, 1};
    runTest(6, a6, 5, 2, true);

    // Test 07: Must update latest position
    int a7[] = {1, 2, 3, 1, 4, 1};
    runTest(7, a7, 6, 2, true);

    // Test 08: Negative numbers
    int a8[] = {-1, 2, 3, -1};
    runTest(8, a8, 4, 3, true);

    // Test 09: Negative hash index risk
    int a9[] = {-20, 5, 12, -20};
    runTest(9, a9, 4, 2, false);

    // Test 10: Hash collision, different values
    int a10[] = {3, 20, 37, 54};
    runTest(10, a10, 4, 3, false);

    // Test 11: k = 0
    int a11[] = {1, 1};
    runTest(11, a11, 2, 0, false);

    // Test 12: Large index distance
    int a12[] = {5, 1, 2, 3, 4, 5};
    runTest(12, a12, 6, 5, true);

    printf("\n===== Test Finished =====\n");

    return 0;
}