#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

/* ===== 你的解法 ===== */

struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
    struct ListNode *current = head;
    struct ListNode *previous_node=NULL;
    struct ListNode *next_node=NULL;
    int cnt=1;
    int delete_node;
    while(current!=NULL)
    {
        current=current->next;
        cnt++;
    }
    printf("n at %d\r\n",cnt-n);
    delete_node=cnt-n;
    cnt=1;
    current=head;
    if(delete_node==1)
    {
        current=current->next;
        head=current;
    }
    else
    {
        while(current!=NULL)
        {
            if(cnt==delete_node-1)
            {
                previous_node=current;
            }
            else if(cnt==delete_node+1)
            {
                next_node=current;
            }
            current=current->next;
            cnt++;
        }
        previous_node->next=next_node;
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

struct ListNode* createList(int arr[], int size)
{
    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;

    for (int i = 0; i < size; i++)
    {
        struct ListNode *node = createNode(arr[i]);

        if (node == NULL)
            return head;

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

    while (current != NULL)
    {
        printf("%d", current->val);

        if (current->next != NULL)
            printf(" -> ");

        current = current->next;
    }

    printf(" -> NULL\n");
}

void freeList(struct ListNode *head)
{
    while (head != NULL)
    {
        struct ListNode *temp = head;
        head = head->next;
        free(temp);
    }
}

/* ===== Main ===== */

int main(void)
{
    int test1[] = {1, 2, 3, 4, 5};
    int test2[] = {1};
    int test3[] = {1, 2};
    int test4[] = {1, 2};
    int test5[] = {1, 2, 3, 4, 5};

    struct {
        int *arr;
        int size;
        int n;
        const char *expected;
    } tests[] = {
        {test1, 5, 2, "1 -> 2 -> 3 -> 5 -> NULL"},
        {test2, 1, 1, "NULL"},
        {test3, 2, 1, "1 -> NULL"},
        {test4, 2, 2, "2 -> NULL"},
        {test5, 5, 5, "2 -> 3 -> 4 -> 5 -> NULL"}
    };

    int testCount = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < testCount; i++)
    {
        printf("\n===== Test %d =====\n", i + 1);

        struct ListNode *head =
            createList(tests[i].arr, tests[i].size);

        printf("Before:   ");
        printList(head);

        printf("n = %d\n", tests[i].n);

        head = removeNthFromEnd(head, tests[i].n);

        printf("After:    ");
        printList(head);

        printf("Expected: %s\n", tests[i].expected);

        freeList(head);
    }

    return 0;
}