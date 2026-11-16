
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// ========================================
// LeetCode #155 - Min Stack
// Your Implementation
// ========================================

typedef struct {
    int min[30000];
    int stack[30000];
    int top;
} MinStack;

MinStack* minStackCreate() {
    MinStack *obj=malloc(sizeof(MinStack));
    if(obj==NULL)
        return NULL;

    obj->top=-1;
    return obj;
}

void minStackPush(MinStack* obj, int val) {
    if(obj->top==-1)
    {
        obj->top++;
        obj->min[obj->top]=val;
        obj->stack[obj->top]=val;
    }
    else
    {
        if(obj->min[obj->top]>val)
        {
            obj->top++;
            obj->min[obj->top]=val;
        }
        else
        {
            obj->top++;
            obj->min[obj->top]=obj->min[obj->top-1];
        }
        obj->stack[obj->top]=val;
    }
}

void minStackPop(MinStack* obj) {
    if(obj->top!=-1)
        obj->top--;
    
}

int minStackTop(MinStack* obj) {
    return obj->stack[obj->top];
}

int minStackGetMin(MinStack* obj) {
    return obj->min[obj->top];
}

void minStackFree(MinStack* obj) {
    free(obj);
}

// ========================================
// Test Environment
// ========================================

static int passed = 0;
static int failed = 0;

void check(int testId, const char *name,
           int actual, int expected) {
    if (actual == expected) {
        printf("Test %d - %s: PASS\n", testId, name);
        passed++;
    } else {
        printf("Test %d - %s: FAIL "
               "(expected %d, got %d)\n",
               testId, name, expected, actual);
        failed++;
    }
}

int main(void) {
    MinStack *s;

    // Test 1: LeetCode example
    s = minStackCreate();
    minStackPush(s, -2);
    minStackPush(s, 0);
    minStackPush(s, -3);
    check(1, "getMin", minStackGetMin(s), -3);
    minStackPop(s);
    check(1, "top", minStackTop(s), 0);
    check(1, "getMin after pop",
          minStackGetMin(s), -2);
    minStackFree(s);

    // Test 2: Increasing values
    s = minStackCreate();
    minStackPush(s, 1);
    minStackPush(s, 2);
    minStackPush(s, 3);
    check(2, "top", minStackTop(s), 3);
    check(2, "getMin", minStackGetMin(s), 1);
    minStackPop(s);
    check(2, "getMin after pop",
          minStackGetMin(s), 1);
    minStackFree(s);

    // Test 3: Decreasing values
    s = minStackCreate();
    minStackPush(s, 5);
    minStackPush(s, 3);
    minStackPush(s, 1);
    check(3, "getMin", minStackGetMin(s), 1);
    minStackPop(s);
    check(3, "restore min 3",
          minStackGetMin(s), 3);
    minStackPop(s);
    check(3, "restore min 5",
          minStackGetMin(s), 5);
    minStackFree(s);

    // Test 4: Duplicate minimum
    s = minStackCreate();
    minStackPush(s, 3);
    minStackPush(s, 1);
    minStackPush(s, 1);
    minStackPush(s, 2);
    minStackPop(s);
    check(4, "min after pop 2",
          minStackGetMin(s), 1);
    minStackPop(s);
    check(4, "min after pop first 1",
          minStackGetMin(s), 1);
    minStackPop(s);
    check(4, "min after pop second 1",
          minStackGetMin(s), 3);
    minStackFree(s);

    // Test 5: Negative values
    s = minStackCreate();
    minStackPush(s, -10);
    minStackPush(s, -20);
    minStackPush(s, -5);
    check(5, "getMin", minStackGetMin(s), -20);
    minStackPop(s);
    minStackPop(s);
    check(5, "restore min",
          minStackGetMin(s), -10);
    minStackFree(s);

    // Test 6: Integer boundaries
    s = minStackCreate();
    minStackPush(s, INT_MAX);
    minStackPush(s, INT_MIN);
    check(6, "INT_MIN",
          minStackGetMin(s), INT_MIN);
    minStackPop(s);
    check(6, "INT_MAX",
          minStackGetMin(s), INT_MAX);
    minStackFree(s);

    // Test 7: Reuse after popping all values
    s = minStackCreate();
    minStackPush(s, 10);
    minStackPop(s);
    minStackPush(s, 7);
    check(7, "top after reuse",
          minStackTop(s), 7);
    check(7, "min after reuse",
          minStackGetMin(s), 7);
    minStackFree(s);

    // Test 8: Alternating push and pop
    s = minStackCreate();
    minStackPush(s, 8);
    minStackPush(s, 4);
    minStackPop(s);
    minStackPush(s, 6);
    check(8, "min", minStackGetMin(s), 6);
    minStackPush(s, 2);
    check(8, "new min", minStackGetMin(s), 2);
    minStackPop(s);
    check(8, "restore min",
          minStackGetMin(s), 6);
    minStackFree(s);

    printf("\n========================\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("========================\n");

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
