#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ===== Linked List Definition ===== */

struct ListNode
{
    int val;
    struct ListNode *next;
};

/* ===== LeetCode 82 ===== */

struct ListNode* deleteDuplicates(struct ListNode* head)
{
    if(head==NULL)
    {
        return head;
    }
    if(head->next==NULL)
    {
        return head;
    }
    if(head->next->next==NULL)
    {
        if(head->val==head->next->val)
        {
            free(head->next);
            free(head);
            head=NULL;
            return head;
        }
        else
        {
            return head;
        }
    }
    struct ListNode *Memorizer=head;
    struct ListNode *Executor=head->next;
    struct ListNode *Explorer=head->next->next;
    bool head_duplicate=false;
    bool in_duplicate=false;
    bool tri_duplicate=false;
    while(Explorer!=NULL)
    {
        if(Memorizer==head)
        {
            if(Memorizer->val==Executor->val)
            {
                head_duplicate=true;
                if(Executor->val==Explorer->val)
                {
                    tri_duplicate=true;
                }
            }
        }
        if(Executor->val==Explorer->val)
        {
            in_duplicate=true;
            free(Executor);
            Executor=Explorer;
            Explorer=Explorer->next;
        }
        else if(in_duplicate==true&&Executor->val!=Explorer->val)
        {
            free(Executor);
            in_duplicate=false;
            Executor=Explorer;
            Explorer=Explorer->next;
        }
        else if(in_duplicate==false&&Executor->val!=Explorer->val)
        {
                Memorizer->next=Executor;
                Memorizer=Memorizer->next;
                Executor=Executor->next;
                Explorer=Explorer->next;            
        }
    }
    Memorizer->next=Executor;
    if(in_duplicate==true)
    {
        free(Executor);
        Memorizer->next=NULL;
    }
    if(tri_duplicate==true)
    {
        Memorizer=head->next;
        free(head);
        head=Memorizer;
    }
    else if(head_duplicate==true)
    {
        Memorizer=head->next->next;
        free(head->next);
        free(head);
        head=Memorizer;
    }


    return head;
}

/* ===== Test Tools ===== */

struct ListNode* createNode(int val)
{
    struct ListNode *node = malloc(sizeof(struct ListNode));

    if (node == NULL)
        return NULL;

    node->val = val;
    node->next = NULL;

    return node;
}

struct ListNode* createList(const int arr[], int size)
{
    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;

    for (int i = 0; i < size; i++)
    {
        struct ListNode *node = createNode(arr[i]);

        if (node == NULL)
        {
            // Free nodes already created
            while (head != NULL)
            {
                struct ListNode *next = head->next;
                free(head);
                head = next;
            }
            return NULL;
        }

        if (head == NULL)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }

    return head;
}

void printList(struct ListNode *head)
{
    struct ListNode *current = head;

    if (current == NULL)
    {
        printf("NULL\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d -> ", current->val);
        current = current->next;
    }

    printf("NULL\n");
}

bool compareList(struct ListNode *head, const int expected[], int expectedSize)
{
    int i = 0;

    while (head != NULL && i < expectedSize)
    {
        if (head->val != expected[i])
            return false;

        head = head->next;
        i++;
    }

    return head == NULL && i == expectedSize;
}

void freeList(struct ListNode *head)
{
    while (head != NULL)
    {
        struct ListNode *next = head->next;
        free(head);
        head = next;
    }
}

/* ===== Test Runner ===== */

void runTest(
    int testNumber,
    const int input[],
    int inputSize,
    const int expected[],
    int expectedSize)
{
    struct ListNode *head = createList(input, inputSize);

    printf("\n===== Test %d =====\n", testNumber);

    printf("Before:   ");
    printList(head);

    head = deleteDuplicates(head);

    printf("After:    ");
    printList(head);

    printf("Expected: ");

    for (int i = 0; i < expectedSize; i++)
        printf("%d -> ", expected[i]);

    printf("NULL\n");

    if (compareList(head, expected, expectedSize))
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");

    freeList(head);
}

/* ===== Main ===== */

int main(void)
{
    // Test 1: Duplicates in the middle
    int t1[] = {1, 2, 3, 3, 4, 4, 5};
    int e1[] = {1, 2, 5};

    runTest(1, t1, 7, e1, 3);

    // Test 2: Duplicates at head and tail
    int t2[] = {1, 1, 2, 3, 3};
    int e2[] = {2};

    runTest(2, t2, 5, e2, 1);

    // Test 3: All duplicates
    int t3[] = {1, 1, 2, 2, 3, 3};

    runTest(3, t3, 6, NULL, 0);

    // Test 4: No duplicates
    int t4[] = {1, 2, 3, 4, 5};
    int e4[] = {1, 2, 3, 4, 5};

    runTest(4, t4, 5, e4, 5);

    // Test 5: Single node
    int t5[] = {1};
    int e5[] = {1};

    runTest(5, t5, 1, e5, 1);

    // Test 6: Two identical nodes
    int t6[] = {1, 1};

    runTest(6, t6, 2, NULL, 0);

    // Test 7: Long duplicate sequence
    int t7[] = {1, 2, 2, 2, 3, 4, 4, 5};
    int e7[] = {1, 3, 5};

    runTest(7, t7, 8, e7, 3);

    // Test 8: Empty list
    runTest(8, NULL, 0, NULL, 0);
// Test 9: Multiple duplicates at head
int t9[] = {1, 1, 1, 2, 3};
int e9[] = {2, 3};

runTest(9, t9, 5, e9, 2);
// Test 10: All nodes have the same value
int t10[] = {1, 1, 1, 1, 1};

runTest(10, t10, 5, NULL, 0);

// Test 11: Multiple duplicates at head, followed by a unique node
int t11[] = {1, 1, 1, 1, 3};
int e11[] = {3};

runTest(11, t11, 5, e11, 1);
    return 0;
}