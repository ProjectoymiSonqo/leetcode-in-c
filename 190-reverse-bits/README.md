# 190. Reverse Bits

## Problem

Reverse the bits of a given 32-bit integer.

Example:

```text
Input:
00000010100101000001111010011100

Output:
00111001011110000010100101000000

Bitwise Operators

AND &

Truth table:
⸻

Common Bit Operations
// Read bit k

(n >> k) & 1u;

// Set bit k

n |= (1u << k);

// Clear bit k

n &= ~(1u << k);

// Toggle bit k

n ^= (1u << k);

OR:

0 → keep original value

1 → force to 1

AND:

1 → keep original value

0 → force to 0

XOR:

0 → keep original value

1 → toggle


1. Integer does not need to be converted to binary first

2. Bit manipulation is mainly about masks

3. Be careful with signed integers

Bit manipulation 關心的是 bit pattern，而 signed integer 的最高位同時具有 sign 的語意。

因此操作完整 32-bit pattern 時，使用 unsigned int / uint32_t 可以避免許多 signed shift 的問題。
4. 1u matters