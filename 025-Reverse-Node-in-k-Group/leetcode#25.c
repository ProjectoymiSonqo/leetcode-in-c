#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode *next;
};


/* ===== 你寫這裡 ===== */

struct ListNode* reverseKGroup(struct ListNode* head, int k)
{
    struct ListNode **map = malloc(k * sizeof(struct ListNode *));
    struct ListNode *current = head;
    struct ListNode *previous_tail = NULL;
    bool start=false;
    while(current!=NULL)
    {
        int map_count = 0;
        for(int i = 0; i < k && current != NULL; i++)
        {
            map[i] = current;
            current = current->next;
            map_count++;
        }
        if(map_count < k)
        {
            // 不足 k 個，不反轉
            if(start==false)//當開頭就不夠一組k時
            {
                break;
            }
            else//有開頭的情況 但不足k個 所以不進行反轉
            {
                previous_tail->next=map[0];
                break;
            }
        }
        else
        {
            // 收滿 k 個，執行反轉
            if(start==false) //開頭足夠一組k 從k-1的地方開始走
            {
                head=map[k-1];
                for(int i=k-1;i>0;i--)
                {
                    map[i]->next=map[i-1];
                }   
                map[0]->next = current;
                previous_tail=map[0];
                start=true;
            }
            else //再有開頭的條件下 繼續處理反轉
            {
                previous_tail->next=map[k-1];
                for(int i=k-1;i>0;i--)
                {
                    map[i]->next=map[i-1];
                }   
                map[0]->next = current;
                previous_tail=map[0];                
            }            
        }
    }
    free(map);
    return head;
}


/* ===== Test Tools ===== */

struct ListNode* createNode(int val)
{
    struct ListNode *node = malloc(sizeof(struct ListNode));

    node->val = val;
    node->next = NULL;

    return node;
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
    struct ListNode *n1 = createNode(1);
    struct ListNode *n2 = createNode(2);
    struct ListNode *n3 = createNode(3);
    struct ListNode *n4 = createNode(4);
    struct ListNode *n5 = createNode(5);
    struct ListNode *n6 = createNode(6);
    struct ListNode *n7 = createNode(7);

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;
    n6->next = n7;

    struct ListNode *head = n1;

    printf("Before:\n");
    printList(head);

    int k = 3;

    head = reverseKGroup(head, k);

    printf("\nAfter:\n");
    printList(head);

    printf("\nExpected:\n");
    printf("3 -> 2 -> 1 -> 6 -> 5 -> 4 -> 7 -> NULL\n");

    freeList(head);

    return 0;
}