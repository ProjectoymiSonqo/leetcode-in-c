# LeetCode 155 — Min Stack

**Difficulty:** Medium  
**Language:** C  
**Topics:** Stack, Design, Data Structure  
**Status:** Accepted — 46/46 Test Cases

## 1. Problem Description

Design a stack that supports the following operations:

- `push(val)` — Push an element onto the stack.
- `pop()` — Remove the element at the top.
- `top()` — Return the top element.
- `getMin()` — Return the minimum element currently in the stack.

All four operations must run in **O(1) time complexity**.

### Example

```text
push(-2)
push(0)
push(-3)

getMin() → -3

pop()

top()    → 0
getMin() → -2
```

The main challenge is maintaining the minimum value without traversing the entire stack.

---

## 2. Initial Approach — Tracking the Minimum

My first idea was to maintain a variable that stores the current minimum value.

```c
int min = INT_MAX;
```

Whenever a new value is pushed, compare it with the current minimum:

```c
if (val < min)
    min = val;
```

This allows `getMin()` to return the minimum in O(1) time.

However, there is a problem when removing elements.

Consider:

```text
push(5) → min = 5
push(3) → min = 3
push(7) → min = 3
push(2) → min = 2

pop()   → min should become 3
```

After removing `2`, the minimum value must return to `3`.

A single variable cannot restore the previous minimum without additional information.

Therefore, the stack needs to preserve the history of minimum values.

---

## 3. Solution — Two Synchronized Stacks

The solution is to maintain two stacks:

**Stack A — Value Stack**

Stores the actual values.

**Stack B — Minimum Stack**

Stores the minimum value corresponding to each stack depth.

Both stacks are updated simultaneously.

### Example

| Operation | Value Stack | Minimum Stack |
|---|---|---|
| push(5) | [5] | [5] |
| push(3) | [5, 3] | [5, 3] |
| push(7) | [5, 3, 7] | [5, 3, 3] |
| push(2) | [5, 3, 7, 2] | [5, 3, 3, 2] |
| pop() | [5, 3, 7] | [5, 3, 3] |
| pop() | [5, 3] | [5, 3] |

The top of the Minimum Stack always contains the current minimum.

When an element is removed, the corresponding minimum record is also removed.

This automatically restores the previous minimum.

### Minimum Update Rule

For each new value:

\[
min_{new}=\min(val,min_{previous})
\]

For the first element, the minimum is simply the inserted value.

---

## 4. Data Structure Design

Instead of implementing two independent stack objects, I used two fixed-size arrays with a shared `top` index.

```c
typedef struct {
    int min[30000];
    int stack[30000];
    int top;
} MinStack;
```

The arrays have different responsibilities:

- `stack[i]` stores the actual value at index `i`.
- `min[i]` stores the minimum value among `stack[0]` through `stack[i]`.
- `top` indicates the current top index of both stacks.

### Why Share One Top Index?

Both arrays always have the same number of valid elements.

Every Push updates both arrays.

Every Pop removes one logical element from both arrays.

Therefore, maintaining two separate indices is unnecessary.

This keeps the implementation simple and avoids synchronization mistakes between the two arrays.

---

## 5. C Implementation

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int min[30000];
    int stack[30000];
    int top;
} MinStack;

MinStack* minStackCreate() {
    MinStack *obj = malloc(sizeof(MinStack));

    if (obj == NULL)
        return NULL;

    obj->top = -1;
    return obj;
}

void minStackPush(MinStack* obj, int val) {
    if (obj->top == -1) {
        obj->top++;
        obj->min[obj->top] = val;
        obj->stack[obj->top] = val;
    }
    else {
        if (obj->min[obj->top] > val) {
            obj->top++;
            obj->min[obj->top] = val;
        }
        else {
            obj->top++;
            obj->min[obj->top] = obj->min[obj->top - 1];
        }

        obj->stack[obj->top] = val;
    }
}

void minStackPop(MinStack* obj) {
    if (obj->top != -1)
        obj->top--;
}

int minStackTop(MinStack* obj) {
    return obj->stack[obj->top];
}

int minStackGetMin(MinStack* obj) {
    return obj->min[obj->top];
}

void minStackFree(MinStack* obj) {
    free(obj);
}
```

This is my original accepted implementation.

---

## 6. Implementation Details

### Create

```c
obj->top = -1;
```

An initial index of `-1` represents an empty stack.

The first Push increments `top` to `0`.

### Push

When the stack is empty, the inserted value becomes the minimum.

Otherwise, compare the new value with the current minimum.

If the new value is smaller, store it in the Minimum Stack.

If it is greater than or equal to the current minimum, preserve the previous minimum.

Both arrays are updated at the same index.

### Pop

```c
if (obj->top != -1)
    obj->top--;
```

Since both arrays share `top`, decrementing the index logically removes the top element from both stacks.

There is no need to erase the old array values.

They will be overwritten by future Push operations.

### Top

```c
return obj->stack[obj->top];
```

Direct array access provides O(1) time complexity.

### Get Minimum

```c
return obj->min[obj->top];
```

The minimum is already stored at the current stack depth.

No traversal is required.

---

## 7. Edge Cases

### Duplicate Minimum Values

```text
push(3)
push(1)
push(1)
pop()

getMin() → 1
```

Both occurrences of `1` have corresponding minimum records.

Removing one does not incorrectly restore the minimum to `3`.

### Negative Values

```text
push(-10)
push(-20)
push(-5)

getMin() → -20
```

The comparison works for negative integers without modification.

### Integer Boundaries

```text
push(INT_MAX)
push(INT_MIN)

getMin() → INT_MIN
```

The solution does not rely on arithmetic encoding or subtraction, so it avoids integer overflow from such techniques.

### Empty Stack and Reuse

After all elements are popped, `top` returns to `-1`.

The next Push is treated as the first insertion.

The problem guarantees valid calls to `pop()`, `top()`, and `getMin()`.

---

## 8. Complexity Analysis

### Time Complexity

| Operation | Complexity | Explanation |
|---|---|---|
| `push()` | O(1) | One comparison and constant-time assignments |
| `pop()` | O(1) | Decrement the shared index |
| `top()` | O(1) | Direct array access |
| `getMin()` | O(1) | Direct array access |

All required operations satisfy the problem constraints.

### Space Complexity

The algorithm uses O(n) space to store values and their corresponding minimum records for a stack with n elements.

In this implementation, both arrays have a fixed capacity of 30,000 elements, so memory is reserved in advance regardless of how many elements are currently stored.

Assuming 4-byte integers:

\[
2\times30000\times4=240000\text{ bytes}
\]

This is approximately 234.4 KiB, excluding small amounts of structure padding and allocator overhead.

The fixed capacity is sufficient for the problem's maximum of 30,000 operations.

For a general-purpose stack, capacity checking or dynamic resizing would be preferable.

---

## 9. LeetCode Result

**Accepted — November 16, 2026**

| Metric | Result |
|---|---|
| Test Cases | 46 / 46 |
| Runtime | 239 ms |
| Runtime Beats | 10.39% |
| Memory | 112.72 MB |
| Memory Beats | 90.50% |

Runtime and memory measurements depend on the execution environment and may vary between submissions.

The implementation also passed the local test suite after resolving an Xcode license configuration issue.

---

## 10. What I Learned

This problem initially appeared to be a simple stack implementation with an additional minimum-value variable.

However, the real challenge was not finding a new minimum during Push.

It was restoring the correct minimum after Pop.

The key realization was that **LIFO allows historical states to be restored in reverse order**.

By storing the minimum value associated with each stack depth, the previous minimum becomes available automatically when the top element is removed.

The final design uses two synchronized arrays and one shared index.

### Key Takeaways

- Maintaining the current minimum is easy during Push but requires additional information during Pop.
- A second stack can preserve the history of minimum values.
- The Minimum Stack must be updated for every Push, including duplicate and non-minimum values.
- Two synchronized arrays can share a single `top` index.
- Precomputing information during Push makes `getMin()` an O(1) operation.
- Fixed-capacity arrays simplify implementation but introduce capacity and memory considerations.

### Final Reflection

The important part of this problem was understanding what information must be preserved when the data structure changes.

Instead of recalculating the minimum whenever an element is removed, I stored the minimum corresponding to each stack state.

This transformed `getMin()` from an O(n) traversal into a simple O(1) array lookup.

**LeetCode #155 — Accepted.**