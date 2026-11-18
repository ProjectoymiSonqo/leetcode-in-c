# LeetCode 739 — Daily Temperatures

**Difficulty:** Medium  
**Language:** C  
**Topics:** Array, Stack, Monotonic Stack  
**Status:** Accepted — 48/48 Test Cases

## 1. Problem Description

Given an array of daily temperatures, return an array `answer` where `answer[i]` represents the number of days until a warmer temperature occurs.

If there is no warmer day in the future, `answer[i]` should be `0`.

### Example

```text
Input:
temperatures = [73, 74, 75, 71, 69, 72, 76, 73]

Output:
answer = [1, 1, 4, 2, 1, 1, 0, 0]
```

For example, the temperature on Day 2 is 75. The next warmer temperature is 76 on Day 6, so the answer is `6 - 2 = 4`.

---

## 2. Initial Approach — Brute Force

My first idea was straightforward:

For each day, check the following days one by one until a warmer temperature is found.

If no warmer temperature exists, return `0`.

### Pseudocode

```text
for each day i:
    for each future day j:
        if temperature[j] > temperature[i]:
            answer[i] = j - i
            break
```

### Complexity

- Time: O(n²) in the worst case
- Auxiliary space: O(1), excluding the output array

This approach is simple and correct, but it repeatedly compares temperatures that may have already been examined.

The next question was:

**Can previously examined temperature relationships help reduce repeated comparisons?**

---

## 3. Exploring Temperature Relationships

I initially considered dividing the temperatures into increasing sequences.

For example:

```text
[73, 71, 69, 72, 75]
```

Could be divided into:

```text
[73]
[71]
[69, 72, 75]
```

This representation captures some local temperature relationships.

However, a warmer temperature in a later sequence may also resolve a day from an earlier sequence.

For example, the final temperature of 75 resolves the earlier temperature of 73.

Therefore, dividing the array into increasing sequences does not directly provide all the answers.

I also considered using Binary Search to take advantage of ordered temperatures.

But finding a position in an ordered collection is not the entire problem.

We must also determine which previous days have found their next warmer temperature.

This led me to experiment with Stack operations.

---

## 4. Key Idea — Push, Compare, and Pop

Instead of storing every temperature relationship, I decided to preserve only the days that were still waiting for a warmer temperature.

The idea developed from experimenting with Push and Pop operations.

When the next temperature is lower than or equal to the current temperature, the current day cannot be resolved immediately.

Therefore, it is pushed onto a Stack.

When the next temperature is higher, the current day's answer is immediately `1`.

The new temperature may also resolve earlier days stored in the Stack.

Starting from the top:

1. Compare the new temperature with the temperature stored at the top.
2. If the new temperature is higher, calculate the waiting days.
3. Pop the resolved entry.
4. Continue until the Stack is empty or the top temperature is greater than or equal to the new temperature.

### Example

```text
Temperatures: [75, 71, 69, 72, 76]
Indices:       0   1   2   3   4
```

After processing the decreasing temperatures:

```text
Stack temperatures:
[75, 71]
       ↑ top
```

When processing Day 3, the new temperature is 72.

The current temperature, 69, is immediately resolved:

```text
answer[2] = 1
```

Then check the Stack:

```text
71 < 72 → answer[1] = 3 - 1 = 2
          Pop 71

75 < 72 → false
          Stop
```

The remaining Stack contains:

```text
[75]
```

When Day 4 reaches 76:

```text
72 < 76 → answer[3] = 1

75 < 76 → answer[0] = 4 - 0 = 4
          Pop 75
```

All unresolved earlier days now have their answers.

---

## 5. Data Structure Design

I used two synchronized arrays:

```c
int stack[temperaturesSize];
int day[temperaturesSize];
int top = -1;
```

Their responsibilities are:

| Variable | Purpose |
|---|---|
| `stack[]` | Stores unresolved temperatures |
| `day[]` | Stores the corresponding day indices |
| `top` | Tracks the top of both arrays |
| `result[]` | Stores the number of waiting days |

The two arrays share the same `top` index.

For example:

```text
stack = [75, 71]
day   = [ 2,  3]
top   = 1
```

If the new temperature is 72, the top entry can be resolved:

```c
result[day[top]] = i + 1 - day[top];
top--;
```

The Stack stores unresolved temperatures in non-increasing order from bottom to top.

This ordering is important because it allows us to stop checking as soon as the top temperature is not lower than the new temperature.

---

## 6. C Implementation

This is my original accepted implementation.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int* dailyTemperatures(
    int* temperatures,
    int temperaturesSize,
    int* returnSize
) {
    *returnSize = temperaturesSize;

    int* result = calloc(temperaturesSize, sizeof(int));
    int stack[temperaturesSize];
    int day[temperaturesSize];
    int top = -1;

    for (int i = 0; i < temperaturesSize - 1; i++)
    {
        int current = temperatures[i];
        int next = temperatures[i + 1];

        if (next > current)
        {
            result[i] = 1;

            while (top != -1)
            {
                if (stack[top] < next)
                {
                    result[day[top]] = i + 1 - day[top];
                    top--;
                }
                else
                    break;
            }
        }
        else
        {
            top++;
            stack[top] = temperatures[i];
            day[top] = i;
        }
    }

    return result;
}
```

### Why Use `calloc()`?

```c
int* result = calloc(temperaturesSize, sizeof(int));
```

The result array is initialized to zero.

If a day never encounters a warmer temperature, its answer remains `0`.

This avoids the need to explicitly initialize each result element.

---

## 7. Why the Algorithm Works

The algorithm maintains a Stack containing days whose next warmer temperature has not yet been found.

The Stack has an important property:

**Temperatures stored in the Stack are non-increasing from bottom to top.**

When a new temperature is higher than the current temperature, it can resolve the current day immediately.

It may also resolve earlier days stored in the Stack.

Because the Stack is ordered, checking from the top is sufficient.

If the top temperature is lower than the new temperature, that day is resolved and removed.

If the top temperature is greater than or equal to the new temperature, all remaining entries below it are at least as warm as the top entry.

Therefore, none of those entries can be resolved by the new temperature, and the search can stop.

### Handling Equal Temperatures

A day is resolved only when a **strictly higher** temperature appears.

For example:

```text
Input:  [73, 73, 74]
Output: [ 2,  1,  0]
```

The comparison uses:

```c
stack[top] < next
```

Not:

```c
stack[top] <= next
```

This ensures that equal temperatures do not incorrectly resolve earlier days.

---

## 8. Complexity Analysis

### Time Complexity — O(n)

Although the implementation contains a nested `while` loop, its total number of operations is linear.

Each unresolved day can be:

- Pushed onto the Stack at most once.
- Popped from the Stack at most once.

Once a day is popped, it is never inserted again.

Therefore, the total number of Push and Pop operations is bounded by O(n).

The outer loop also runs O(n) times.

Thus, the overall time complexity is:

**O(n)**

This is an example of **Amortized Analysis**.

A single iteration may process multiple Stack entries, but the total number of Stack operations across the entire algorithm remains linear.

### Space Complexity — O(n)

The implementation allocates:

- `result[]`: O(n) output space
- `stack[]`: O(n) auxiliary space
- `day[]`: O(n) auxiliary space

Therefore:

- Auxiliary space: **O(n)**
- Total space including output: **O(n)**

The two auxiliary arrays are variable-length arrays allocated with automatic storage duration.

An alternative implementation could store only day indices in the Stack and retrieve temperatures from the original input array.

However, the current design explicitly separates temperatures and their corresponding indices.

---

## 9. Edge Cases

| Case | Input | Expected |
|---|---|---|
| Single day | `[42]` | `[0]` |
| Increasing | `[30,40,50,60]` | `[1,1,1,0]` |
| Decreasing | `[90,80,70,60]` | `[0,0,0,0]` |
| Equal temperatures | `[50,50,50]` | `[0,0,0]` |
| Repeated temperatures | `[30,30,40]` | `[2,1,0]` |
| Long waiting period | `[80,70,60,50,90]` | `[4,3,2,1,0]` |

The algorithm handles these cases using the same Push, Compare, and Pop rules.

---

## 10. LeetCode Result

**Accepted — November 18, 2026**

| Metric | Result |
|---|---|
| Test Cases | 48 / 48 |
| Runtime | 9 ms |
| Runtime Beats | 76.28% |
| Memory | 69.50 MB |
| Memory Beats | 91.48% |

The solution passed all official test cases.

Runtime and memory measurements may vary between submissions.

---

## 11. What I Learned

The most important part of this problem was not learning how to implement a Stack.

It was discovering how Stack operations could help eliminate repeated work.

My initial solution would have used nested loops to search for the next warmer temperature.

After becoming more familiar with Stack operations, I started asking whether previously examined data could be preserved and reused.

I experimented with Push and Pop operations before having a complete algorithm.

Gradually, I noticed that a new temperature could resolve several previously unresolved days.

Once those days were resolved, they could be permanently removed from the Stack.

This changed the problem from repeatedly searching future temperatures into maintaining a collection of unresolved days.

### Key Takeaways

- A correct brute-force solution is a useful starting point.
- Recognizing repeated comparisons can reveal optimization opportunities.
- Stack operations can be used to preserve unresolved states.
- Maintaining an ordering invariant allows unnecessary comparisons to be skipped.
- Each element entering and leaving a Stack at most once can lead to O(n) amortized time complexity.
- A nested loop does not necessarily imply O(n²) complexity.

### Final Reflection

Previously, I might have stopped after finding an O(n²) solution because it produced the correct result.

This time, I wanted to understand whether the complexity could be reduced.

Rather than starting with a known algorithm, I experimented with Stack operations and gradually discovered a structure that could solve the problem in linear time.

**The biggest improvement was not writing faster code, but learning to recognize opportunities to avoid repeated computation.**

---

**LeetCode #739 — Accepted.**