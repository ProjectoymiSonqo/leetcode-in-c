# LeetCode 142 — Linked List Cycle II

**Difficulty:** Medium  
**Language:** C  
**Topic:** Linked List, Two Pointers, Floyd's Cycle Detection  
**Status:** Accepted — 18/18 Test Cases

## 1. Problem Description

Given the `head` of a singly linked list, return the node where the cycle begins.

If there is no cycle, return `NULL`.

Example:

```text
A → B → C → D → E → F
        ↑           │
        └───────────┘
```

The cycle begins at node `C`.

The objective is to return the pointer to `C`, not its value.

**Follow-up:** Solve the problem using O(1) extra memory.

---

## 2. Initial Approach

My first idea was to divide the problem into two stages:

1. Detect whether the linked list contains a cycle.
2. If a cycle exists, locate its entry point.

Since I had already solved LeetCode #141 using Fast and Slow Pointers, I decided to reuse the same approach for cycle detection.

### Original Entry Detection Strategy

After detecting a cycle:

1. Start from the meeting point.
2. Traverse the cycle once to determine its length.
3. Store the pointers of all nodes belonging to the cycle in an array.
4. Start traversing from `head`.
5. Return the first node that appears in the cycle array.

For example:

```text
A → B → C → D → E → F
        ↑           │
        └───────────┘
```

If Fast and Slow meet at `E`, the cycle array could be:

```text
[E, F, C, D]
```

Starting from `head`:

```text
A → Not in cycle
B → Not in cycle
C → Found!
```

This approach is logically correct, but requires O(L) extra space for the cycle array, where L is the cycle length.

If the array is searched linearly for each node, the overall time complexity can also become quadratic.

---

## 3. Optimization — Using the Cycle Length

The next idea was to avoid storing the cycle nodes.

Suppose the cycle length is L.

Instead of maintaining an array, use two pointers:

```c
struct ListNode *first = head;
struct ListNode *second = head;
```

Move `first` forward by L steps.

Then move both pointers forward at the same speed.

```text
A → B → C → D → E → F
        ↑           │
        └───────────┘
```

Here, L = 4.

| Step | first | second |
|---|---|---|
| Initial offset | E | A |
| 1 | F | B |
| 2 | C | C |

Both pointers meet at the cycle entry.

### Why Does This Work?

The two pointers maintain a distance difference of exactly L steps.

Once both pointers are inside the cycle, a difference of L steps means they occupy the same node.

Before `second` enters the cycle, they cannot meet because `second` is still outside the cycle.

Therefore, their first meeting point must be the cycle entry.

This reduces the additional space requirement to O(1).

---

## 4. Further Optimization — Counting Slow Pointer Steps

During implementation, I noticed another possibility.

Instead of explicitly calculating the cycle length, I could count how many steps `slow` takes before meeting `fast`.

```c
fast = fast->next->next;
slow = slow->next;
cnt++;
```

When the two pointers meet, `cnt` is not necessarily equal to the cycle length.

However, it has an important mathematical property:

**The number of steps taken by Slow is always a positive integer multiple of the cycle length.**

This means `cnt` can be used directly as the offset between the two pointers.

There is no need to traverse the cycle again.

---

## 5. Mathematical Explanation

Let:

- \(a\): Distance from `head` to the cycle entry.
- \(L\): Length of the cycle.
- \(D_s\): Total steps taken by Slow.
- \(D_f\): Total steps taken by Fast.

Since Fast moves twice as quickly as Slow:

\[
D_f = 2D_s
\]

When they meet inside the cycle, both pointers occupy the same node.

Therefore, the difference between their total distances must be a multiple of the cycle length:

\[
D_f-D_s=kL
\]

Substituting \(D_f=2D_s\):

\[
2D_s-D_s=kL
\]

Thus:

\[
\boxed{D_s=kL}
\]

Since `cnt` records the number of steps taken by Slow:

\[
\boxed{cnt=kL}
\]

This is the key observation.

Even if `cnt` is not exactly one cycle long, it is still an integer multiple of the cycle length.

Therefore, advancing one pointer by `cnt` steps creates the same alignment needed to find the cycle entry.

---

## 6. Final Solution — C

```c
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *fast = head;
    struct ListNode *slow = head;

    int cnt = 0;
    bool flag = false;
    bool find = false;

    // Phase 1: Detect cycle
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
        cnt++;

        if (slow == fast) {
            flag = true;
            break;
        }
    }

    if (flag == false)
        return NULL;

    // Phase 2: Create an offset of cnt steps
    struct ListNode *first = head;
    struct ListNode *second = head;
    struct ListNode *result = NULL;

    while (cnt != 0) {
        first = first->next;
        cnt--;
    }

    // Phase 3: Find the cycle entry
    while (find != true) {
        if (first == second) {
            find = true;
            result = first;
            break;
        }

        first = first->next;
        second = second->next;
    }

    return result;
}
```

### Implementation Notes

The first `while` loop uses Floyd's Fast and Slow Pointer technique.

Once a cycle is detected, the loop must stop immediately.

Without `break`, both pointers would continue moving inside the cycle indefinitely.

The second phase advances `first` by `cnt` steps.

The final phase moves `first` and `second` together until they point to the same node.

That node is the cycle entry.

---

## 7. Complexity Analysis

Let \(N\) be the number of distinct nodes reachable from `head` before a node repeats, or the list length when there is no cycle.

### Time Complexity: O(N)

- Cycle detection takes O(N).
- Advancing `first` by `cnt` steps takes O(N).
- Moving both pointers until they meet takes O(N).

Therefore:

\[
\boxed{T(N)=O(N)}
\]

### Space Complexity: O(1)

Only a fixed number of pointers and variables are used.

No additional arrays, hash tables, or dynamically allocated memory are required.

\[
\boxed{S(N)=O(1)}
\]

---

## 8. LeetCode Result

**Accepted — November 16, 2026**

| Metric | Result |
|---|---|
| Test Cases | 18 / 18 |
| Runtime | 8 ms |
| Runtime Beats | 36.24% |
| Memory | 10.91 MB |
| Memory Beats | 64.21% |

Performance measurements may vary between submissions.

---

## 9. What I Learned

This problem began with a straightforward approach: detect the cycle, record its nodes, and search for the first repeated node.

The solution evolved through three stages:

**Stage 1 — Store Cycle Nodes**

Record every node belonging to the cycle and search for the first matching node from `head`.

**Stage 2 — Use Cycle Length**

Replace the array with two pointers separated by one complete cycle length.

**Stage 3 — Reuse the Detection Step Count**

Recognize that the distance traveled by Slow at the meeting point is already a multiple of the cycle length.

This eliminates the need to calculate the exact cycle length.

### Key Takeaways

- Fast and Slow Pointers can detect cycles in O(1) extra space.
- The meeting point is not necessarily the cycle entry.
- At the meeting point, Slow has traveled an integer multiple of the cycle length.
- Two pointers separated by an integer multiple of the cycle length will meet at the cycle entry when advanced together from the head.
- Understanding a mathematical property can eliminate unnecessary data structures.

**Final Reflection**

The most interesting part of this problem was not implementing Floyd's algorithm itself.

It was discovering that the information required to solve the second half of the problem was already available from the first half.

Instead of recording every node in the cycle, I only needed to preserve the number of steps taken during cycle detection.

A problem that initially appeared to require extra memory could be solved with only a few pointers and one counter.

**LeetCode #142 — Accepted.**