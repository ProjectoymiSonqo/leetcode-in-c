
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/*
 * LeetCode 225 — Implement Stack using Queues
 *
 * TODO:
 * 使用一個或兩個 Queue 實作 Stack。
 *
 * 必須符合：
 * Push -> 加入 Stack 頂端
 * Pop  -> 移除並回傳 Stack 頂端
 * Top  -> 查看 Stack 頂端，不移除
 * Empty -> 判斷 Stack 是否為空
 */

 struct Node{
    int val;
    struct Node *next;
 };
typedef struct {
    // TODO: Design your data structure
    int count;
    struct Node *head;
    struct Node *tail;
} MyStack;

MyStack* myStackCreate(void)
{
    // TODO
    MyStack *stack=malloc(sizeof(MyStack));
    if(stack==NULL)
        return NULL;
    stack->count=0;
    stack->head=NULL;
    stack->tail=NULL;
    return stack;
}

void myStackPush(MyStack* obj, int x)
{
    // TODO
    struct Node *New_Node=malloc(sizeof(struct Node));
    if(New_Node==NULL)
        return;
    New_Node->val=x;
    New_Node->next=NULL;
    obj->count++;
    if(obj->head==NULL)
    {
        obj->head=New_Node;
        obj->tail=New_Node;
    }
    else
    {
        obj->tail->next=New_Node;
        obj->tail=New_Node;
    }
}
void dequeue_enqueue(MyStack* obj)
{
    struct Node *head=obj->head;
    obj->head=obj->head->next;
    obj->tail->next=head;
    obj->tail=head;
    obj->tail->next=NULL;
}

int myStackPop(MyStack* obj)
{
    // TODO
    if(obj->count==0)
        return -1;
    else if(obj->count==1)
    {
        int value=obj->head->val;
        struct Node *current=obj->head;
        obj->count--;
        free(current);
        obj->head=NULL;
        obj->tail=NULL;
        return value;
    }
    else
    {
        int rotate_times=obj->count-1;
        for(int i=0;i<rotate_times;i++)
        {
            dequeue_enqueue(obj);
        }
        int front=obj->head->val;
        struct Node *head=obj->head;
        obj->head=obj->head->next;
        free(head);
        obj->count--;
        return front;
    }

}

int myStackTop(MyStack* obj)
{
    // TODO
    if(obj->count<=0)
        return -1;
    else if(obj->count==1)
        return obj->head->val;
    else
    {
        int rotate_times=obj->count-1;
        for(int i=0;i<rotate_times;i++)
        {
            dequeue_enqueue(obj);
        }
        int front=obj->head->val;
        dequeue_enqueue(obj);
        return front;
    }
}

bool myStackEmpty(MyStack* obj)
{
    // TODO
    if(obj->head==NULL)
        return true;
    else
        return false;
}

void myStackFree(MyStack* obj)
{
    if(obj->count!=0)
    {
        struct Node *current=obj->head;
        while(current!=NULL)
        {
            struct Node *next=current->next;
            free(current);
            current=next;
        }
    }
    free(obj);

}

/* ========== Test Environment ========== */

int main(void)
{
    printf("LeetCode 225 Test Start\n");

    // Test 1: Basic LIFO
    {
        MyStack *s = myStackCreate();
        assert(s != NULL);

        myStackPush(s, 1);
        myStackPush(s, 2);

        assert(myStackTop(s) == 2);
        assert(myStackPop(s) == 2);
        assert(myStackEmpty(s) == false);
        assert(myStackPop(s) == 1);
        assert(myStackEmpty(s) == true);

        myStackFree(s);
        printf("Test 1 Passed\n");
    }

    // Test 2: Multiple Push / Pop
    {
        MyStack *s = myStackCreate();

        myStackPush(s, 1);
        myStackPush(s, 2);
        myStackPush(s, 3);
        myStackPush(s, 4);
        myStackPush(s, 5);

        assert(myStackPop(s) == 5);
        assert(myStackPop(s) == 4);
        assert(myStackPop(s) == 3);
        assert(myStackPop(s) == 2);
        assert(myStackPop(s) == 1);
        assert(myStackEmpty(s) == true);

        myStackFree(s);
        printf("Test 2 Passed\n");
    }

    // Test 3: Interleaved Operations
    {
        MyStack *s = myStackCreate();

        myStackPush(s, 1);
        myStackPush(s, 2);
        assert(myStackPop(s) == 2);

        myStackPush(s, 3);
        assert(myStackTop(s) == 3);

        myStackPush(s, 4);
        assert(myStackPop(s) == 4);
        assert(myStackPop(s) == 3);
        assert(myStackPop(s) == 1);
        assert(myStackEmpty(s) == true);

        myStackFree(s);
        printf("Test 3 Passed\n");
    }

    // Test 4: Top Must Not Remove Element
    {
        MyStack *s = myStackCreate();

        myStackPush(s, 7);

        assert(myStackTop(s) == 7);
        assert(myStackTop(s) == 7);
        assert(myStackTop(s) == 7);

        assert(myStackPop(s) == 7);
        assert(myStackEmpty(s) == true);

        myStackFree(s);
        printf("Test 4 Passed\n");
    }

    // Test 5: Reuse After Empty
    {
        MyStack *s = myStackCreate();

        myStackPush(s, 1);
        assert(myStackPop(s) == 1);
        assert(myStackEmpty(s) == true);

        myStackPush(s, 8);
        myStackPush(s, 9);

        assert(myStackTop(s) == 9);
        assert(myStackPop(s) == 9);
        assert(myStackPop(s) == 8);
        assert(myStackEmpty(s) == true);

        myStackFree(s);
        printf("Test 5 Passed\n");
    }

    printf("All Tests Passed!\n");
    return 0;
}
