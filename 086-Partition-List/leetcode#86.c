#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

// ==========================================
// LeetCode 86 - Partition List
// 在這裡實作你的演算法
// ==========================================

struct ListNode* partition(struct ListNode* head, int x)
{
    // Your solution here
    if(head==NULL)
    {
        return head;
    }
    else if(head->next==NULL)
    {
        return head;
    }
    
    struct ListNode *current=head;
    int A=0;
    int B=0;
    while(current!=NULL)
    {
        if(current->val<x)
        {
            A++;
        }
        else
        {
            B++;
        }
        current=current->next;
    }
    struct ListNode **map_A=malloc(A*sizeof(struct ListNode *));
    struct ListNode **map_B=malloc(B*sizeof(struct ListNode *));
    current=head;
    int i=0;
    int j=0;
    while(current!=NULL)
    {
        if(current->val<x)
        {
            map_A[i]=current;
            i++;
        }
        else
        {
            map_B[j]=current;
            j++;
        }
        current=current->next;
    }   
    if(A==0)
    {
        free(map_A);
        free(map_B);
        return head;
    }
    i=0;
    j=0; 
    if(B!=0)
    {
        map_B[B-1]->next=NULL;
        map_A[A-1]->next=map_B[0];
    }
    else
    {
        map_A[A-1]->next=NULL;
    }
    for(int i=0;i<A-1;i++)
    {
        map_A[i]->next=map_A[i+1];
    }
    for(int j=0;j<B-1;j++)
    {
        map_B[j]->next=map_B[j+1];
    }
    head=map_A[0];
    free(map_A);
    free(map_B);
    
    return head;
}

// ==========================================
// Test Environment
// ==========================================

struct ListNode* createList(const int *arr, int size)
{
    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;

    for (int i = 0; i < size; i++)
    {
        struct ListNode *node = malloc(sizeof(*node));

        if (node == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        node->val = arr[i];
        node->next = NULL;

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

// 印出 Linked List
// 使用節點數量限制，避免形成 cycle 時無限輸出
void printList(const struct ListNode *head)
{
    int count = 0;

    while (head != NULL && count < 100)
    {
        printf("%d", head->val);

        head = head->next;

        if (head != NULL)
            printf(" -> ");

        count++;
    }

    if (head != NULL)
        printf(" -> ... (possible cycle)");

    printf("\n");
}

// 比對 Linked List 與預期結果
bool compareList(const struct ListNode *head,
                 const int *expected,
                 int expectedSize)
{
    for (int i = 0; i < expectedSize; i++)
    {
        if (head == NULL || head->val != expected[i])
            return false;

        head = head->next;
    }

    return head == NULL;
}

// 釋放 Linked List
// 假設串列沒有 cycle
void freeList(struct ListNode *head)
{
    while (head != NULL)
    {
        struct ListNode *next = head->next;
        free(head);
        head = next;
    }
}

// 執行單一測試
void runTest(const char *name,
             const int *input,
             int inputSize,
             int x,
             const int *expected,
             int expectedSize)
{
    printf("\n========== %s ==========\n", name);

    struct ListNode *head = createList(input, inputSize);

    printf("Input    : ");
    printList(head);

    printf("x        : %d\n", x);

    head = partition(head, x);

    printf("Output   : ");
    printList(head);

    printf("Expected : ");

    for (int i = 0; i < expectedSize; i++)
    {
        printf("%d", expected[i]);

        if (i < expectedSize - 1)
            printf(" -> ");
    }

    if (expectedSize == 0)
        printf("NULL");

    printf("\n");

    if (compareList(head, expected, expectedSize))
        printf("Result   : PASS\n");
    else
        printf("Result   : FAIL\n");

    freeList(head);
}

int main(void)
{
    // Case 1: LeetCode Example 1
    int a1[] = {1, 4, 3, 2, 5, 2};
    int e1[] = {1, 2, 2, 4, 3, 5};
    runTest("Case 1 - Example 1", a1, 6, 3, e1, 6);

    // Case 2: LeetCode Example 2
    int a2[] = {2, 1};
    int e2[] = {1, 2};
    runTest("Case 2 - Example 2", a2, 2, 2, e2, 2);

    // Case 3: Empty list
    runTest("Case 3 - Empty", NULL, 0, 3, NULL, 0);

    // Case 4: Single node (< x)
    int a4[] = {1};
    int e4[] = {1};
    runTest("Case 4 - Single less", a4, 1, 3, e4, 1);

    // Case 5: Single node (>= x)
    int a5[] = {5};
    int e5[] = {5};
    runTest("Case 5 - Single greater", a5, 1, 3, e5, 1);

    // Case 6: All nodes < x
    int a6[] = {1, 2, 3, 4};
    int e6[] = {1, 2, 3, 4};
    runTest("Case 6 - All less", a6, 4, 5, e6, 4);

    // Case 7: All nodes >= x
    int a7[] = {5, 7, 6, 8};
    int e7[] = {5, 7, 6, 8};
    runTest("Case 7 - All greater", a7, 4, 5, e7, 4);

    // Case 8: Already partitioned
    int a8[] = {1, 2, 3, 5, 4, 6};
    int e8[] = {1, 2, 3, 5, 4, 6};
    runTest("Case 8 - Already partitioned", a8, 6, 4, e8, 6);

    // Case 9: Reverse groups
    int a9[] = {5, 6, 7, 1, 2, 3};
    int e9[] = {1, 2, 3, 5, 6, 7};
    runTest("Case 9 - Reverse groups", a9, 6, 4, e9, 6);

    // Case 10: Alternating values
    int a10[] = {5, 1, 6, 2, 7, 3};
    int e10[] = {1, 2, 3, 5, 6, 7};
    runTest("Case 10 - Alternating", a10, 6, 4, e10, 6);

    // Case 11: Values equal to x
    int a11[] = {3, 1, 3, 2, 3};
    int e11[] = {1, 2, 3, 3, 3};
    runTest("Case 11 - Equal to x", a11, 5, 3, e11, 5);

    // Case 12: Negative numbers and duplicates
    int a12[] = {0, -2, 4, -1, 3, -2};
    int e12[] = {-2, -1, -2, 0, 4, 3};
    runTest("Case 12 - Negative values", a12, 6, 0, e12, 6);

    return 0;
}