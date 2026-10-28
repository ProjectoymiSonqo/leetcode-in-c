#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

// ==========================================
// LeetCode 61 - Rotate List
// 在這裡實作你的演算法
// ==========================================

struct ListNode* rotateRight(struct ListNode* head, int k)
{
    // Your solution here
    struct ListNode *current=head;
    if(head==NULL)
    {
        return head;
    }
    int cnt=0;
    while(current!=NULL)
    {
        cnt++;
        current=current->next;   
    }
    if(cnt==1)
    {
        return head;
    }
    printf("cnt=%d\r\n",cnt);
    int right_times=k%cnt;
    printf("operation times is %d\r\n",right_times);
    if(right_times==0)
    {
        return head;
    }
    else
    {
        current=head;
        struct ListNode *newhead;
        struct ListNode *endpoint;
        int new_cnt=0;
        while(new_cnt<cnt-right_times-1)
        {
            new_cnt++;
            current=current->next;
        }
        endpoint=current;
        newhead=current->next;
        endpoint->next=NULL;
        printf("end point is %d\r\n",endpoint->val);
        printf("new head is %d\r\n",newhead->val);
         current=head;
         head=newhead;
         while(newhead!=endpoint)
         {
            if(newhead->next==NULL)
            {
                newhead->next=current;
                newhead=newhead->next;
            }
            else
            {
                newhead=newhead->next;
            }
        }
        // while(newhead!=NULL)
        // {
        //     newhead=newhead->next;
        // }
        // newhead->next=current;
    }
    return head;
}

// ==========================================
// Test Environment
// ==========================================

// 建立 Linked List
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

// 比對結果
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
// 防止意外形成 cycle 時無限迴圈
void freeList(struct ListNode *head)
{
    int count = 0;

    while (head != NULL && count < 100)
    {
        struct ListNode *next = head->next;
        free(head);
        head = next;
        count++;
    }
}

// 執行單一測試
void runTest(const char *name,
             const int *input,
             int inputSize,
             int k,
             const int *expected,
             int expectedSize)
{
    printf("\n========== %s ==========\n", name);

    struct ListNode *head = createList(input, inputSize);

    printf("Input    : ");
    printList(head);

    printf("k        : %d\n", k);

    head = rotateRight(head, k);

    printf("Output   : ");
    printList(head);

    printf("Expected : ");
    for (int i = 0; i < expectedSize; i++)
    {
        printf("%d", expected[i]);

        if (i < expectedSize - 1)
            printf(" -> ");
    }
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
    int a1[] = {1, 2, 3, 4, 5};
    int e1[] = {4, 5, 1, 2, 3};
    runTest("Case 1", a1, 5, 2, e1, 5);

    // Case 2: LeetCode Example 2
    int a2[] = {0, 1, 2};
    int e2[] = {2, 0, 1};
    runTest("Case 2", a2, 3, 4, e2, 3);

    // Case 3: k = 0
    int a3[] = {1, 2, 3};
    int e3[] = {1, 2, 3};
    runTest("Case 3 - k=0", a3, 3, 0, e3, 3);

    // Case 4: k = list length
    int a4[] = {1, 2, 3};
    int e4[] = {1, 2, 3};
    runTest("Case 4 - k=n", a4, 3, 3, e4, 3);

    // Case 5: k > list length
    int a5[] = {1, 2, 3};
    int e5[] = {2, 3, 1};
    runTest("Case 5 - k>n", a5, 3, 5, e5, 3);

    // Case 6: Single node
    int a6[] = {1};
    int e6[] = {1};
    runTest("Case 6 - single node", a6, 1, 100, e6, 1);

    // Case 7: Empty list
    runTest("Case 7 - empty", NULL, 0, 3, NULL, 0);

    // Case 8: Two nodes
    int a8[] = {1, 2};
    int e8[] = {2, 1};
    runTest("Case 8 - two nodes", a8, 2, 1, e8, 2);

    // Case 9: Large k
    int a9[] = {1, 2, 3, 4, 5};
    int e9[] = {4, 5, 1, 2, 3};
    runTest("Case 9 - large k", a9, 5, 1000002, e9, 5);

    // Case 10: Duplicate values
    int a10[] = {1, 1, 2, 2, 3};
    int e10[] = {2, 3, 1, 1, 2};
    runTest("Case 10 - duplicates", a10, 5, 2, e10, 5);

    return 0;
}