#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

/* ===== 你要完成的地方 ===== */

struct ListNode* mergeTwoLists(struct ListNode* list1,
                               struct ListNode* list2)
{
    struct ListNode *result_head = NULL;
    struct ListNode *current = NULL;
    while(list1!=NULL && list2!=NULL)
    {
        if((list1->val)<(list2->val))
        {
            if(result_head == NULL)
            {
                result_head=list1;
                current=list1;
                list1=list1->next;
            }
            else
            {
                current->next=list1;
                current=list1;
                list1=list1->next;
            }
        }
        else
        {
            if(result_head == NULL)
            {
                result_head=list2;
                current=list2;
                list2=list2->next;   
            }
            else
            {
                current->next=list2;
                current=list2;
                list2=list2->next;
            }
        }
    }
    if (result_head == NULL)
    {
        if (list1 != NULL)
            return list1;
        else
            return list2;
    }
    if (list1 != NULL)
    {
        current->next = list1;
    }
    else
    {
        current->next = list2;
    }
    return result_head;
}


/* ===== 測試工具，不用修改 ===== */

void printList(struct ListNode *head)
{
    while (head != NULL)
    {
        printf("%d", head->val);

        if (head->next != NULL)
            printf(" -> ");

        head = head->next;
    }

    printf(" -> NULL\n");
}


/* ===== Main ===== */

int main(void)
{
    /*
        list1:
        A -> B -> C -> NULL
        1    2    4
    */

    struct ListNode A = {1, NULL};
    struct ListNode B = {2, NULL};
    struct ListNode C = {4, NULL};

    A.next = &B;
    B.next = &C;


    /*
        list2:
        D -> E -> F -> NULL
        1    3    4
    */

    struct ListNode D = {1, NULL};
    struct ListNode E = {3, NULL};
    struct ListNode F = {4, NULL};

    D.next = &E;
    E.next = &F;


    printf("Before merge:\n");

    printf("list1: ");
    printList(&A);

    printf("list2: ");
    printList(&D);


    /* 呼叫你寫的 function */

    struct ListNode *result = mergeTwoLists(&A, &D);


    printf("\nAfter merge:\n");

    printf("result: ");
    printList(result);


    return 0;
}