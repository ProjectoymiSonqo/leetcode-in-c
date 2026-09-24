# LeetCode 141 - Linked List Cycle

## Problem

Given the `head` of a linked list, determine whether the linked list contains a cycle.

A cycle exists when following the `next` pointer eventually leads back to a node that has already been visited.

---

## Understanding `pos`

One thing that confused me at first was `pos`.

For example:

```text
head = [3, 2, 0, -4]
pos = 1

pos is NOT an argument passed into hasCycle().

The function only receives:

bool hasCycle(struct ListNode *head)

pos is only used by LeetCode internally to construct the test case.

For:

[3, 2, 0, -4], pos = 1

LeetCode creates something equivalent to:

node1.val = 3;
node2.val = 2;
node3.val = 0;
node4.val = -4;

node1.next = &node2;
node2.next = &node3;
node3.next = &node4;
node4.next = &node2;

So the actual structure is:
3 → 2 → 0 → -4
    ↑         |
    └─────────┘
pos = 1 means that the last node points back to the node at index 1.

By the time hasCycle(head) is called, this structure has already been created.

⸻

What is the actual problem?

For a normal linked list:
node1 → node2 → node3 → NULL
If I repeatedly do:
p = p->next;

I will eventually get:
p == NULL
But if the list contains a cycle:
node1 → node2 → node3
  ↑               |
  └───────────────┘
then traversal becomes:
node1
node2
node3
node1
node2
node3
...
I will never reach NULL.

Therefore, simply writing:
while (p != NULL)
{
    p = p->next;
}
cannot detect a cycle because the loop itself will run forever.

The real question is:

How can I know that I have entered a repeating sequence of nodes?
Floyd’s Cycle Detection Algorithm

One solution is Floyd’s Cycle Detection Algorithm, also called the Tortoise and Hare Algorithm.

Use two pointers:
struct ListNode *slow = head;
struct ListNode *fast = head;
They move at different speeds:
slow = slow->next;
fast = fast->next->next;
* slow moves one node at a time.
* fast moves two nodes at a time.

If there is no cycle, fast will eventually reach NULL.

If there is a cycle, both pointers will eventually enter the cycle.

Because fast moves faster than slow, it will eventually catch up with slow.

Therefore:
if (slow == fast)
{
    return true;
}
means that both pointers are pointing to the same node in memory, so a cycle exists.

## Solution

bool hasCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return true;
        }
    }

    return false;
}
Why check fast and fast->next?

Before doing:
fast = fast->next->next;
I need to make sure both steps are valid.

Therefore:
while (fast != NULL && fast->next != NULL)

prevents dereferencing a NULL pointer.

If either condition fails, the linked list has reached its end, which means there is no cycle.

⸻

Important Pointer Concept

The comparison:
slow == fast
does not compare the values stored inside the nodes.

It compares their addresses.

For example, two different nodes could contain:

node A: val = 5
node B: val = 5
but:

&nodeA != &nodeB

A cycle is about visiting the same node again, not seeing the same value again.

Complexity

Time Complexity
O(n)
Space Complexity
O(1)

## What I learned

The biggest difficulty in this problem was not initially the Floyd algorithm itself.

I misunderstood what pos represented.

I originally thought pos was information that I needed to use inside hasCycle().

Actually, LeetCode has already used pos to construct the linked list before calling my function.

My function only receives:
head

and must discover the structure by following:

node->next
The important idea is:
Normal linked list:
state → state → state → NULL

Linked list with cycle:
state → state → state → previous state → ...

This is really a cycle detection problem.

It is similar to other iterative systems where the next state is determined by the current state:
x(n+1) = f(x(n))
If a previous state appears again, the system can enter a repeating cycle.

Floyd’s algorithm provides a way to detect this cycle without storing every previously visited state.

## Note to myself

I understand the problem itself now, but I am not yet fully comfortable with deriving Floyd’s algorithm by myself.

Things to revisit:

* Why fast and slow are guaranteed to meet inside a cycle.
* Why moving at speeds 1 and 2 works.
* How the algorithm can also find the entry point of the cycle.

For now, the most important thing I understand is:

I am not looking for pos.

I am following next pointers and trying to determine whether traversal eventually reaches NULL or becomes trapped in a repeating sequence of nodes.