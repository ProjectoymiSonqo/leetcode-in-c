
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ==========================================
// LeetCode 739 - Daily Temperatures
// Implement your solution here
// ==========================================

int* dailyTemperatures(
    int* temperatures,
    int temperaturesSize,
    int* returnSize
) {
    // TODO: Implement your algorithm

    *returnSize = temperaturesSize;

    int* result = calloc(temperaturesSize, sizeof(int));
    int stack[temperaturesSize];
    int day[temperaturesSize];
    int top=-1;
    for(int i=0;i<temperaturesSize-1;i++)
    {
        int current=temperatures[i];
        int next=temperatures[i+1];
        if(next>current)
        {
            result[i]=1;
            while(top!=-1)
            {
                if(stack[top]<next)
                {
                    result[day[top]]=i+1-day[top];
                    top--;
                }
                else
                    break;
                
            }
        }
        else
        {
            top++;
            stack[top]=temperatures[i];
            day[top]=i;
        }
    }
    return result;
}

// ==========================================
// Test Framework
// ==========================================

static int passed = 0;
static int failed = 0;

void runTest(
    const char* name,
    int* input,
    int size,
    int* expected
) {
    int returnSize = 0;

    int* actual = dailyTemperatures(
        input, size, &returnSize
    );

    bool success = (returnSize == size && actual != NULL);

    if (success) {
        for (int i = 0; i < size; i++) {
            if (actual[i] != expected[i]) {
                success = false;
                break;
            }
        }
    }

    printf("[%s] %s\n",
           success ? "PASS" : "FAIL",
           name);

    if (success) {
        passed++;
    } else {
        failed++;

        printf("  Input:    ");
        for (int i = 0; i < size; i++)
            printf("%d ", input[i]);

        printf("\n  Expected: ");
        for (int i = 0; i < size; i++)
            printf("%d ", expected[i]);

        printf("\n  Actual:   ");
        if (actual == NULL) {
            printf("NULL");
        } else {
            for (int i = 0; i < returnSize; i++)
                printf("%d ", actual[i]);
        }

        printf("\n");
    }

    free(actual);
}

// ==========================================
// Main
// ==========================================

int main(void) {

    printf("=== LeetCode 739 Test Suite ===\n\n");

    // Test 1: Official example
    {
        int input[] = {73,74,75,71,69,72,76,73};
        int expected[] = {1,1,4,2,1,1,0,0};
        runTest("Official Example",
                input, 8, expected);
    }

    // Test 2: Increasing temperatures
    {
        int input[] = {30,40,50,60};
        int expected[] = {1,1,1,0};
        runTest("Increasing",
                input, 4, expected);
    }

    // Test 3: Decreasing temperatures
    {
        int input[] = {90,80,70,60,50};
        int expected[] = {0,0,0,0,0};
        runTest("Decreasing",
                input, 5, expected);
    }

    // Test 4: All equal
    {
        int input[] = {50,50,50,50};
        int expected[] = {0,0,0,0};
        runTest("All Equal",
                input, 4, expected);
    }

    // Test 5: Single element
    {
        int input[] = {42};
        int expected[] = {0};
        runTest("Single Element",
                input, 1, expected);
    }

    // Test 6: Alternating temperatures
    {
        int input[] = {30,60,40,70,50,80};
        int expected[] = {1,2,1,2,1,0};
        runTest("Alternating",
                input, 6, expected);
    }

    // Test 7: Long waiting period
    {
        int input[] = {80,70,60,50,40,90};
        int expected[] = {5,4,3,2,1,0};
        runTest("Long Wait",
                input, 6, expected);
    }

    // Test 8: Repeated values
    {
        int input[] = {30,30,40,40,50};
        int expected[] = {2,1,2,1,0};
        runTest("Repeated Values",
                input, 5, expected);
    }

    // Test 9: No warmer day
    {
        int input[] = {100,90,90,80};
        int expected[] = {0,0,0,0};
        runTest("No Warmer Day",
                input, 4, expected);
    }

    // Test 10: Mixed temperatures
    {
        int input[] = {70,71,69,72,68,73,67,74};
        int expected[] = {1,2,1,2,1,2,1,0};
        runTest("Mixed",
                input, 8, expected);
    }

    printf("\n=========================\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("=========================\n");

    return failed == 0 ? 0 : 1;
}
