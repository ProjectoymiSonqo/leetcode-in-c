#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

// ========================================
// LeetCode #142
// 在這裡實作你的解法
// ========================================

struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *fast=head;
    struct ListNode *slow=head;
    int cnt=0;
    bool flag=false;
    bool find=false;
    while(fast!=NULL&&fast->next!=NULL)
    {
        fast=fast->next->next;
        slow=slow->next;
        cnt++;
        if(slow==fast)
        {
            flag=true;
            break;
        }
    }
    if(flag==false)
        return NULL;
    struct ListNode *first=head;
    struct ListNode *second=head;
    struct ListNode *result=NULL;
    while(cnt!=0)
    {
        first=first->next;
        cnt--;
    }
    while(find!=true)
    {
        if(first==second)
        {
            find=true;
            result=first;
            break;
        }
        first=first->next;
        second=second->next;
    }
    return result;

}

// ========================================
// Test Environment
// ========================================

struct ListNode *createNode(int val) {
    struct ListNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    node->val = val;
    node->next = NULL;

    return node;
}

void runTest(int testId, int values[], int size, int pos) {

    struct ListNode **nodes = malloc(size * sizeof(*nodes));

    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        nodes[i] = createNode(values[i]);
    }

    for (int i = 0; i < size - 1; i++) {
        nodes[i]->next = nodes[i + 1];
    }

    // 建立 Cycle
    if (pos >= 0) {
        nodes[size - 1]->next = nodes[pos];
    }

    struct ListNode *expected = (pos == -1) ? NULL : nodes[pos];
    struct ListNode *result = detectCycle(nodes[0]);

    printf("Test %d: ", testId);

    if (result == expected) {
        printf("PASS\n");
    } else {
        printf("FAIL (expected node index: %d)\n", pos);
    }

    // 釋放記憶體前先斷開 Cycle
    nodes[size - 1]->next = NULL;

    for (int i = 0; i < size; i++) {
        free(nodes[i]);
    }

    free(nodes);
}

// ========================================
// Main
// ========================================

int main(void) {

    int test1[] = {3, 2, 0, -4};
    runTest(1, test1, 4, 1);

    int test2[] = {1, 2};
    runTest(2, test2, 2, 0);

    int test3[] = {1};
    runTest(3, test3, 1, -1);

    int test4[] = {1, 2, 3};
    runTest(4, test4, 3, 0);

    int test5[] = {1};
    runTest(5, test5, 1, 0);

    int test6[] = {1, 2, 3, 4, 5};
    runTest(6, test6, 5, -1);

    int test7[] = {1, 2, 3, 4, 5};
    runTest(7, test7, 5, 4);

    int test8[] = {1, 2, 3, 4, 5};
    runTest(8, test8, 5, 2);

    return 0;
}