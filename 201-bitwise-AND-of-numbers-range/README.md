# 201. Bitwise AND of Numbers Range

## Problem

Given two integers `left` and `right`, return the bitwise AND of all
numbers in the inclusive range `[left, right]`.

```text
result = left & (left + 1) & ... & right

Example:
left = 5
right = 7

5 = 101
6 = 110
7 = 111
    ---
    100 = 4

Initial Idea

For a bit to remain 1 after AND operations, that bit must be 1
in every number between left and right.

For example:
5 = 101
6 = 110
7 = 111

Counting each bit:
bit 0: not always 1 -> 0
bit 1: not always 1 -> 0
bit 2: always 1     -> 1
Therefore:
result = 100 = 4

This gives a correct brute-force idea:

* Check every number from left to right
* Count each of the 32 bits
* Keep a bit only if it is 1 for the entire range

However, the range can be extremely large:

left  = 1
right = 2147483647
So iterating through every number is too expensive.

Observation

At first, I considered checking whether the highest 1 bit of
left and right was at the same position.

However, this is not enough.

Example:

left  = 413
right = 416

413 = 110011101
416 = 110100000

Their highest 1 is at the same position, but the bits differ shortly
after that.

Comparing from the most significant bit:

413 = 110011101
416 = 110100000
      ^^
      common prefix

At the first different bit, the range crosses a binary boundary.

After that boundary, the lower bits will vary somewhere inside the
continuous range, so they cannot remain 1 after ANDing every number.

Therefore only the common binary prefix of left and right
can survive.

413 = 110011101
416 = 110100000
      ^^
      
result = 110000000
       = 384

## Algorithm

Start from the most significant bit.

For each bit:

1. Read the bit from left
2. Read the same bit from right
3. If they are different, stop
4. If they are both 1, set that bit in result
5. Continue toward the least significant bit

The first different bit ends the common prefix.

## Solution

#include <stdint.h>

int rangeBitwiseAnd(int left, int right)
{
    uint32_t result = 0;

    for (int i = 31; i >= 0; i--)
    {
        uint32_t l = ((uint32_t)left >> i) & 1u;
        uint32_t r = ((uint32_t)right >> i) & 1u;

        if (l != r)
        {
            break;
        }

        if (l == 1u)
        {
            result |= (1u << i);
        }
    }

    return (int)result;
}

## Complexity

Only 32 bits need to be checked.
Time:  O(32) = O(1)
Space: O(1)

The runtime does not depend on the width of [left, right].
