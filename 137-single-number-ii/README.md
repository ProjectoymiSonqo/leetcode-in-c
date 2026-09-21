# 137. Single Number II

## Problem

Given an integer array `nums`, every element appears three times except for one,
which appears exactly once.

Find and return the single number.

Requirements:

- Linear runtime complexity
- Constant extra space

---

## Idea

如果每個數字都出現三次，只有一個數字出現一次，
單純使用 XOR 無法像 `Single Number` 一樣消除重複值。

因為：

```text
a ^ a = 0
a ^ a ^ a = a
所以需要換一個角度：

不要把 integer 當成完整的數字，而是把它拆成 32 個獨立的 bits。

例如：
nums = [2, 2, 3, 2]

2 = 0010
2 = 0010
3 = 0011
2 = 0010
計算每一個 bit 出現 1 的次數：
bit 0: 0 + 0 + 1 + 0 = 1
bit 1: 1 + 1 + 1 + 1 = 4
bit 2: 0
bit 3: 0

所有出現三次的數字，在每一個 bit 上所貢獻的 1
一定也是 3 的倍數。
因此：
count[bit] % 3

就可以消除所有出現三次的數字留下的 bit，
剩下的 bit 就屬於 single number。
bit 0: 1 % 3 = 1
bit 1: 4 % 3 = 1

result = 0011 = 3

Handling Negative Numbers

一開始直接對 int 做：
nums[i] >>= 1;
在負數上會遇到 signed right shift 的問題。

例如 32-bit two’s complement：
-4 = 11111111 11111111 11111111 11111100

負數右移在常見系統上通常是 arithmetic right shift，
左側補 1：
111...11100  (-4)
111...11110  (-2)
111...11111  (-1)
111...11111  (-1)
因此改成先把數值轉成 uint32_t：
uint32_t x = (uint32_t)nums[i];
此時 bit pattern 不變：
-4 (int32_t)
11111111 11111111 11111111 11111100

(uint32_t)-4
11111111 11111111 11111111 11111100
但現在 x >> 1 是 unsigned logical right shift，
左側會補 0。

重要的是：

這個演算法不需要特別處理負數。

它只在乎 32-bit pattern。

即使 single number 是負數，
count[i] % 3 最後仍然會重建出正確的 two’s complement pattern。

Reading Each Bit

使用：
x & 1u
xxxxxxx1 & 00000001 = 1
xxxxxxx0 & 00000001 = 0
Rebuilding the Result
count[i] % 3 != 0
代表答案的第 i bit 應該是 1。

建立 mask：
1u << i

result |= (1u << i);

result      00100101
mask        00001000
            -------- OR
result      00101101

Complexity

There are always 32 bits in an integer.

Time:  O(32 * n) = O(n)
Space: O(32)     = O(1)

5. Common bit operations
(x >> i) & 1u       // read bit i

x |= (1u << i);     // set bit i to 1

x &= ~(1u << i);    // clear bit i to 0

x ^= (1u << i);     // toggle bit i