# 67. Add Binary

## Problem
Given two binary strings `a` and `b`, return their sum as a binary string.

## Approach

Binary addition works the same way as normal addition.

Start from the end of both strings and calculate each bit with a `carry`.

For each position:

```c
sum = bit_a + bit_b + carry;
The result bit can be calculated by:
result = sum % 2;
If sum >= 2, set:
carry = 1;
Otherwise:
carry = 0;
Because a and b may have different lengths, continue until both indexes reach the beginning of the strings.

If there is still a carry after the loop, append one more 1.

⸻

Reverse Storage

Since the calculation starts from the least significant bit (right side), the result is temporarily stored in reverse order.

Example:
Result: 1010

c_int:
index    0  1  2  3
value    0  1  0  1
Then copy c_int backwards into the final string.

int count = k - 1;

for (int i = 0; i < k; i++)
{
    c[i] = c_int[count] + '0';
    count--;
}

char *c = malloc((k + 1) * sizeof(char));
The extra byte is used for the null terminator:
c[k] = '\0';
Complexity

* Time: O(max(a.length, b.length))
* Space: O(max(a.length, b.length))
