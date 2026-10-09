#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;
    struct Node *next;
    struct Node *random;
};


/* ===== 你要完成的地方 ===== */

struct Node* copyRandomList(struct Node* head)
{
    if (head == NULL)
    {
        return NULL;
    }
    struct Node *current = head;
    struct Node *copy_head = NULL;
    struct Node *copy_current = NULL;
    struct Map  
    {
        struct Node *original;
        struct Node *copy;
    };
    copy_head = malloc(sizeof(struct Node));
    copy_head->val = head->val;
    copy_head->next = NULL;
    copy_head->random = NULL;
    copy_current = copy_head;
    int cnt=0;
     while(current!=NULL)
     {
        if(cnt!=0)
        {
            struct Node *new_node = malloc(sizeof(struct Node));   
            copy_current->next=new_node;
            copy_current=new_node;
            copy_current->val = current->val;
            copy_current->next = NULL;       
            copy_current->random = NULL;     
        }
        // printf("head address = %p\n", (void *)head);
        // printf("head val = %d\n", head->val);
        current=current->next;
        cnt++;
     }
     struct Map *map = malloc(cnt * sizeof(struct Map));
     current=head;
     copy_current = copy_head;
     int i=0;
     while(current!=NULL)
     {
        map[i].original = current;
        map[i].copy = copy_current;
        current = current->next;
        copy_current = copy_current->next;
        i++;
     }
    current=head;
    copy_current = copy_head;
    while (current != NULL)
    {
        if(current->random==NULL)
        {
            copy_current->random=NULL;
        }
        else
        {
            for (int j = 0; j < cnt; j++)
            {
                if (map[j].original == current->random)
                {
                    copy_current->random = map[j].copy;
                    break;
                }
            }
        }
        current = current->next;
        copy_current = copy_current->next;
    } 
    free(map);
    return copy_head;
}


/* ===== 測試工具，不用修改 ===== */

void printList(struct Node *head)
{
    struct Node *current = head;

    while (current != NULL)
    {
        printf("Node address : %p\n", (void *)current);
        printf("val          : %d\n", current->val);

        if (current->next != NULL)
            printf("next         : %p (val=%d)\n",
                   (void *)current->next,
                   current->next->val);
        else
            printf("next         : NULL\n");

        if (current->random != NULL)
            printf("random       : %p (val=%d)\n",
                   (void *)current->random,
                   current->random->val);
        else
            printf("random       : NULL\n");

        printf("\n");

        current = current->next;
    }
}


/* ===== Main ===== */

int main(void)
{
    /*
        next:

        A ------> B ------> C ------> NULL

        random:

        A -----------------> C
        B ------> A
        C ------> B
    */

    struct Node A = {10, NULL, NULL};
    struct Node B = {20, NULL, NULL};
    struct Node C = {30, NULL, NULL};


    /* 建立 next */

    A.next = &B;
    B.next = &C;
    C.next = NULL;


    /* 建立 random */

    A.random = &C;
    B.random = &A;
    C.random = &B;


    printf("===== Original List =====\n\n");

    printList(&A);


    /* 呼叫你寫的 function */

    struct Node *copy_head = copyRandomList(&A);


    printf("===== Copied List =====\n\n");

    printList(copy_head);


    return 0;
}