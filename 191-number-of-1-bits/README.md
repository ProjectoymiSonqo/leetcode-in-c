# 191. Number of 1 Bits

## Problem

Given a positive integer `n`, return the number of set bits (`1`s) in its binary representation.

---

## Approach 1: Check Each Bit

The most direct way is to inspect the least significant bit (LSB) one at a time.

We can use:

```c
n & 1u

to check the rightmost bit.

## Why does n & 1u work?

1u in binary is:
00000000 00000000 00000000 00000001

For example:
n       = 10110100
1u      = 00000001
-----------------
n & 1u  = 00000000

If the LSB is 1:
n       = 10110101
1u      = 00000001
-----------------
n & 1u  = 00000001

Therefore:


* n & 1u == 1u → the current bit is 1
* n & 1u == 0   → the current bit is 0

After checking the bit, shift n to the right:

n >>= 1u;

Then repeat until all 32 bits have been checked.

## Code

```c
int hammingWeight(int n) {
    int count_1 = 0;
    int count = 31;

    while (count >= 0) {
        if ((n & 1u) == 1u) {
            count_1++;
        }

        n >>= 1u;
        count--;
    }

    return count_1;
}

## Complexity

Time:  O(32) → O(1)
Space: O(1)
This method always checks all 32 bits.

## Approach 2: Brian Kernighan’s Algorithm

A more interesting approach is:
n & (n - 1)

This operation removes the rightmost 1 bit from n.

Why?

Suppose the binary number ends like this:

n     = xxxx1000

Subtracting 1 causes the rightmost 1 to become 0, while all zeros after it become 1:
n     = xxxx1000
n - 1 = xxxx0111

Now perform AND:

        xxxx1000
      & xxxx0111
      ------------
        xxxx0000

The rightmost 1 has disappeared.

Therefore:
n = n & (n - 1);

can be understood as:

Remove the rightmost set bit.

Example
n = 10110100

10110100
   ↓
10110000
   ↓
10100000
   ↓
10000000
   ↓
00000000

There were four 1s, so the loop executes exactly four times.

int hammingWeight(int n) {
    int count_1 = 0;

    while (n != 0) {
        n &= (n - 1);
        count_1++;
    }

    return count_1;
}

## Complexity

Let k be the number of set bits in n.
Time:  O(k)
Space: O(1)

## Key Takeaway

Both approaches are useful.

Bit-by-bit scanning

n & 1u;
n >>= 1u;
helps build the fundamental understanding of masks and bit shifting.

Brian Kernighan’s Algorithm
n &= (n - 1);

uses the structure of binary numbers to directly remove one set bit at a time.

The first approach teaches how to inspect bits.

The second approach teaches how to exploit bit patterns.