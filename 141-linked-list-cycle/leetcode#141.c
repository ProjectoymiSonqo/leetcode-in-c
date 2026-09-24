#include <stdio.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

bool hasCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;  
    while (fast != NULL && fast->next != NULL) 
    {
        slow = slow->next;
        fast = fast->next->next;  
        printf("slow=%d\n",slow->val);
        printf("fast=%d\n",fast->val);
        if (slow == fast) 
        {
            return true;
        } 
    }
    return false;
}

int main(void)
{
    struct ListNode node1;
    struct ListNode node2;
    struct ListNode node3;

    node1.val = 10;
    node2.val = 20;
    node3.val = 30;

    node1.next = &node2;
    node2.next = &node3;

    //node3.next = NULL;
    node3.next = &node1;

    struct ListNode *head = &node1;

    printf("result = %d\n", hasCycle(head));

    return 0;
}