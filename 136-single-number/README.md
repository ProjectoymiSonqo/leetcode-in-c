# 136. Single Number

## Problem

Given a non-empty array of integers, every element appears twice except for one.

Find the element that appears only once.

Requirements:

- Linear runtime complexity: `O(n)`
- Constant extra space: `O(1)`

---

## Approach - XOR

Use XOR (`^`) to cancel out numbers that appear twice.

XOR has three useful properties:

```text
x ^ x = 0
x ^ 0 = x

A ^ B = B ^ A

It is also associative:

(A ^ B) ^ C = A ^ (B ^ C)

Therefore, the order of the numbers does not matter.
nums = [4, 1, 2, 1, 2]

4 ^ 1 ^ 2 ^ 1 ^ 2

= 4 ^ (1 ^ 1) ^ (2 ^ 2)

= 4 ^ 0 ^ 0

= 4
Every number that appears twice cancels itself out, leaving only the single number.

Remind: XOR truth table

Complexity
Time:  O(n)
Space: O(1)

What I Learned

XOR is not only useful for toggling individual bits.

Its algebraic properties can also be used to design algorithms:
