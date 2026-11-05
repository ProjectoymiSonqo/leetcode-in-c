# LeetCode 622 — Design Circular Queue

**Language:** C  
**Difficulty:** Medium  
**Status:** Accepted — 59/59 Test Cases

- Runtime: 20 ms (Beats 5.11%)
- Memory: 18.76 MB (Beats 75.14%)

## 1. Problem Overview

Implement a Circular Queue that follows the **FIFO (First In, First Out)** principle.

The queue has a fixed maximum capacity `k` and supports the following operations:

| Operation | Description |
|---|---|
| `Create(k)` | Initialize a queue with capacity `k` |
| `EnQueue(value)` | Insert a new element |
| `DeQueue()` | Remove the oldest element |
| `Front()` | Return the oldest element |
| `Rear()` | Return the newest element |
| `IsEmpty()` | Check whether the queue is empty |
| `IsFull()` | Check whether the queue is full |
| `Free()` | Release all allocated memory |

The implementation must reuse available capacity after elements are removed.

## 2. Design Concept

Instead of using an array-based Ring Buffer, I chose to implement the queue using a **Circular Doubly Linked List**.

My initial interpretation of the queue was:

- New elements enter from the `head`.
- Old elements leave from the `tail`.
- Every Node maintains both `next` and `prev` pointers.
- The first and last Nodes are connected to form a circular structure.

This design allows insertion and deletion without traversing the linked list.

### Node Structure

```c
struct Node {
    int val;
    struct Node *next;
    struct Node *prev;
};
```

### Queue Structure

```c
typedef struct {
    struct Node *head;
    struct Node *tail;
    int capacity;
    int count;
} MyCircularQueue;
```

Each variable has a specific responsibility:

| Variable | Purpose |
|---|---|
| `head` | Points to the newest element |
| `tail` | Points to the oldest element |
| `capacity` | Maximum number of elements |
| `count` | Current number of elements |

**Important:** In this implementation, `head` and `tail` describe the physical linked-list structure. They do not directly correspond to the LeetCode `Front()` and `Rear()` API names.

## 3. Circular Linked List Model

After inserting `10`, `20`, and `30`:

```text
head                 tail
 ↓                    ↓
[30] ⇄ [20] ⇄ [10]
```

The circular connections are:

```c
head->prev == tail;
tail->next == head;
```

Each Node can be reached in both directions.

For a queue containing only one element:

```c
node->next = node;
node->prev = node;
```

The Node points to itself, preserving the circular structure.

## 4. EnQueue — Insert a New Element

Before inserting a new element, the queue checks whether it has reached its maximum capacity.

```c
if (obj->count == obj->capacity)
    return false;
```

A new Node is allocated using `malloc()`.

### Empty Queue

When `count == 0`, the new Node becomes both `head` and `tail`.

Its `next` and `prev` pointers both point to itself.

### Non-empty Queue

The new Node is inserted before the existing `head` in the `next` traversal direction.

The important pointer updates are:

```c
obj->head->prev = New_Node;
obj->tail->next = New_Node;

New_Node->next = obj->head;
New_Node->prev = obj->tail;

obj->head = New_Node;
obj->count++;
```

The existing Nodes must also be updated.

Updating only the new Node's pointers is insufficient because the existing linked list would not know how to reach the new Node.

**Time Complexity: O(1)**

## 5. DeQueue — Remove the Oldest Element

The oldest element is always located at `tail`.

I separated deletion into three cases to make the boundary conditions explicit.

### Case 1: One Node

When only one Node remains:

1. Free the Node.
2. Set `head` and `tail` to `NULL`.
3. Decrease `count`.

### Case 2: Two Nodes

After removing the old `tail`, only one Node remains.

The remaining Node must point to itself through both `next` and `prev`.

### Case 3: More Than Two Nodes

First identify the new tail:

```c
struct Node *new_tail = obj->tail->prev;
```

Then reconnect the circular structure:

```c
new_tail->next = obj->head;
obj->head->prev = new_tail;
```

Finally, free the old tail and update the queue state.

```c
free(obj->tail);
obj->tail = new_tail;
obj->count--;
```

**Time Complexity: O(1)**

## 6. Front, Rear, and Queue State

Because new elements are inserted at `head`, the API mapping is:

```c
Front() -> tail->val;
Rear()  -> head->val;
```

Both operations return `-1` when the queue is empty.

The queue state is determined by:

```c
IsEmpty: count == 0
IsFull:  count == capacity
```

These conditions are important because `count` is not merely a statistic. It controls whether operations are permitted and how deletion handles boundary cases.

## 7. Memory Management

Unlike a conventional linear linked list, a circular linked list does not terminate at `NULL`.

Therefore, this traversal pattern is unsuitable:

```c
while (current != NULL)
```

The traversal would eventually return to an already visited Node.

My approach was to use the original `head` as the stopping reference and release the final Node separately.

```c
if (!myCircularQueueIsEmpty(obj))
{
    struct Node *current = obj->head;

    while (current->next != obj->head)
    {
        struct Node *next = current->next;
        free(current);
        current = next;
    }

    free(obj->tail);
}

free(obj);
```

The empty-queue condition must be handled before accessing `current->next`, because `head` is `NULL` when the queue is empty.

The next pointer must also be saved before freeing the current Node.

## 8. Complexity Analysis

Let `n` be the current number of elements and `k` the queue capacity.

| Operation | Time Complexity |
|---|---|
| Create | O(1) |
| EnQueue | O(1) |
| DeQueue | O(1) |
| Front | O(1) |
| Rear | O(1) |
| IsEmpty | O(1) |
| IsFull | O(1) |
| Free | O(n) |

**Space Complexity: O(k)**

Each Node requires storage for its value and two pointers. The implementation also allocates and frees individual Nodes dynamically.

The queue's maximum number of Nodes is limited by `capacity`.

## 9. Debugging Notes

Several issues appeared during implementation.

### Missing Backward Links

Initially, I updated the new Node's pointers but forgot to update the existing `head` and `tail`.

The result was an incomplete circular connection.

**Lesson:** A linked-list insertion must preserve the relationships of both new and existing Nodes.

### Missing `count--`

Some `DeQueue()` branches correctly modified the pointers but did not decrease `count`.

This created an inconsistency between the actual linked-list structure and the queue's recorded state.

**Lesson:** Correct pointer operations do not guarantee correct system state.

### Front and Rear Confusion

Because my design inserts new elements at `head`, the oldest element is stored at `tail`.

I initially returned these values in the opposite order.

**Lesson:** Internal variable names and external API semantics must be considered separately.

### Incorrect Empty Condition

I initially used:

```c
capacity > count
```

to determine whether the queue was empty.

However, this condition only indicates that the queue is not full.

The correct condition is:

```c
count == 0
```

### Circular Memory Release

I initially reused a `NULL`-terminated linked-list cleanup pattern.

However, the circular structure never reaches `NULL` during normal traversal.

I had to redesign the stopping condition around the circular structure.

**Lesson:** An implementation technique that works for one data structure may fail when its underlying assumptions change.

## 10. Reflection

My goal was not to reproduce a standard Circular Queue implementation.

I wanted to build a structure that followed my own understanding of FIFO:

**New data enters from the Head, and old data leaves from the Tail.**

This led me to choose a Circular Doubly Linked List.

The implementation required several corrections, particularly around pointer relationships, state consistency, and memory cleanup.

However, the overall architecture remained valid throughout the process.

Compared with my previous 3Sum implementation, this problem did not require a major algorithmic redesign. Most corrections involved making individual operations consistent with the original model.

The most important lesson was:

**A correct data structure is not only about connecting pointers correctly. Its memory, state variables, and external behavior must remain consistent.**

The solution passed all 59 test cases.

Although the runtime ranking leaves room for optimization, the current implementation provides a working and understandable model of a Circular Queue.

Future optimization could compare this linked-list design with a fixed-size array-based Ring Buffer, particularly in terms of allocation overhead, memory locality, and implementation complexity.