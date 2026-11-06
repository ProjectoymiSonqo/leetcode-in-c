
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {

    // TODO: Design your data structure
    int stack1[100];
    int top1;
    int stack2[100];
    int top2;

} MyQueue;

MyQueue* myQueueCreate(void)
{
    // TODO
    MyQueue *queue=malloc(sizeof(MyQueue));
    if(queue==NULL)
        return NULL;
    queue->top1=-1;
    queue->top2=-1;

    return queue;
}

void myQueuePush(MyQueue* obj, int x)
{
    // enqueue
    if(x>9||x<1)
        return;
    obj->top1++;
    obj->stack1[obj->top1]=x;
}

int myQueuePop(MyQueue* obj)
{
    //dequeue
    if(obj->top2==-1)
    {
        if(obj->top1!=-1)
        {
            
            while(obj->top1!=-1)
            {
                obj->top2++;
                obj->stack2[obj->top2]=obj->stack1[obj->top1];
                obj->top1--;
            }
            
        }
        else
            return -1;
    }
    return obj->stack2[obj->top2--];
            
}

int myQueuePeek(MyQueue* obj)
{
    //front
    if(obj->top2!=-1)
        return obj->stack2[obj->top2];
    else//top2=-1
    {
        if(obj->top1!=-1)//top1>=0
        {
            while(obj->top1!=-1)
            {
                obj->top2++;
                obj->stack2[obj->top2]=obj->stack1[obj->top1];
                obj->top1--;
            }
        }
        else
            return -1;
    }
    return obj->stack2[obj->top2];
    // TODO
}

bool myQueueEmpty(MyQueue* obj)
{
    // TODO
    if(obj->top2==-1&&obj->top1==-1)
        return true;
    else
        return false;
}

void myQueueFree(MyQueue* obj)
{
    // TODO
    free(obj);
}

int main(void)
{
    // Test 1: Basic FIFO
    MyQueue *q = myQueueCreate();
    assert(q != NULL);
    assert(myQueueEmpty(q) == true);

    myQueuePush(q, 10);
    myQueuePush(q, 20);
    myQueuePush(q, 30);

    assert(myQueueEmpty(q) == false);
    assert(myQueuePeek(q) == 10);
    assert(myQueuePop(q) == 10);
    assert(myQueuePop(q) == 20);
    assert(myQueuePop(q) == 30);
    assert(myQueueEmpty(q) == true);

    myQueueFree(q);

    // Test 2: Interleaved operations
    q = myQueueCreate();

    myQueuePush(q, 1);
    myQueuePush(q, 2);

    assert(myQueuePop(q) == 1);

    myQueuePush(q, 3);
    myQueuePush(q, 4);

    assert(myQueuePeek(q) == 2);
    assert(myQueuePop(q) == 2);
    assert(myQueuePop(q) == 3);
    assert(myQueuePop(q) == 4);

    myQueueFree(q);

    // Test 3: Repeated peek
    q = myQueueCreate();

    myQueuePush(q, 42);

    assert(myQueuePeek(q) == 42);
    assert(myQueuePeek(q) == 42);
    assert(myQueuePop(q) == 42);
    assert(myQueueEmpty(q) == true);

    myQueuePush(q, 99);

    assert(myQueuePeek(q) == 99);
    assert(myQueuePop(q) == 99);

    myQueueFree(q);

    // Test 4: Negative values
    q = myQueueCreate();

    myQueuePush(q, -5);
    myQueuePush(q, 0);
    myQueuePush(q, 5);

    assert(myQueuePop(q) == -5);
    assert(myQueuePop(q) == 0);
    assert(myQueuePop(q) == 5);

    myQueueFree(q);

    // Test 5: Free an empty queue
    q = myQueueCreate();
    myQueueFree(q);

    printf("All tests passed!\n");
    return 0;
}
