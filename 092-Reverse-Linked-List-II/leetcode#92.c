#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct ListNode {
    int val;
    struct ListNode *next;
};


/* ===== 你只需要寫這裡 ===== */

struct ListNode* reverseBetween(struct ListNode* head, int left, int right)
{
    struct ListNode *current = head;
    int n = right - left + 1;
    struct ListNode **map = malloc(n * sizeof(struct ListNode *));
    struct ListNode *priv_address = NULL;
    struct ListNode *next_address = NULL;
    int i=1;
    int map_i=0;
    
    printf("n=%d\r\n",n);
    while(current!=NULL)
    {
        if(i>=left && i<=right)
        {
            map[map_i]=current;
            printf("map[%d] = %d\n",map_i,map[map_i]->val);
            map_i++;   
        }
        else if(i==left-1)
        {
            priv_address=current;
        }
        else if(i==right+1)
        {
            next_address=current;
        }
        current=current->next;
        i++;       
    }
    current=head;
    
    for(int i=n-1;i>0;i--)
    {
        map[i]->next=map[i-1];
    }
    map[0]->next=next_address;
    if(left==1)
    {
        head=map[n-1];
    }
    else
    {
        priv_address->next=map[n-1];
    }
    free(map);
    return head;
    

}


/* ===== 測試工具 ===== */

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

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;

    struct ListNode *head = n1;

    printf("Before:\n");
    printList(head);

    head = reverseBetween(head, 2, 4);

    printf("\nAfter:\n");
    printList(head);

    freeList(head);

    return 0;
}