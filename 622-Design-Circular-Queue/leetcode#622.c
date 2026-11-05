
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {

    // TODO: Define your data structure

} MyCircularQueue;

MyCircularQueue* myCircularQueueCreate(int k) {

    // TODO

}

bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {

    // TODO

}

bool myCircularQueueDeQueue(MyCircularQueue* obj) {

    // TODO

}

int myCircularQueueFront(MyCircularQueue* obj) {

    // TODO

}

int myCircularQueueRear(MyCircularQueue* obj) {

    // TODO

}

bool myCircularQueueIsEmpty(MyCircularQueue* obj) {

    // TODO

}

bool myCircularQueueIsFull(MyCircularQueue* obj) {

    // TODO

}

void myCircularQueueFree(MyCircularQueue* obj) {

    // TODO

}


int main(void)
{
    MyCircularQueue *q = myCircularQueueCreate(3);
    assert(q != NULL);

    assert(myCircularQueueIsEmpty(q) == true);
    assert(myCircularQueueFront(q) == -1);
    assert(myCircularQueueRear(q) == -1);

    assert(myCircularQueueEnQueue(q, 1) == true);
    assert(myCircularQueueEnQueue(q, 2) == true);
    assert(myCircularQueueEnQueue(q, 3) == true);

    assert(myCircularQueueIsFull(q) == true);
    assert(myCircularQueueEnQueue(q, 4) == false);

    assert(myCircularQueueRear(q) == 3);
    assert(myCircularQueueFront(q) == 1);

    assert(myCircularQueueDeQueue(q) == true);
    assert(myCircularQueueEnQueue(q, 4) == true);

    assert(myCircularQueueFront(q) == 2);
    assert(myCircularQueueRear(q) == 4);

    assert(myCircularQueueDeQueue(q) == true);
    assert(myCircularQueueDeQueue(q) == true);
    assert(myCircularQueueDeQueue(q) == true);

    assert(myCircularQueueIsEmpty(q) == true);
    assert(myCircularQueueDeQueue(q) == false);

    myCircularQueueFree(q);

    printf("All tests passed!\n");
    return 0;
}
