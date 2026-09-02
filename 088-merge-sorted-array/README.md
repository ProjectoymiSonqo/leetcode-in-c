# LeetCode 88 - Merge Sorted Array

## 思路

一開始只用 `nums2` 的元素去和 `nums1` 比較，較大的值往後放。

但當：

```c

nums2[j] < nums1[i]時，原本的邏輯就會卡住。

後來意識到這題的回圈主體其實不是 m 或 n，而是合併後的：
m + n
因此改成從 nums1 最後一格開始，使用 i、j 分別指向兩個陣列目前的最大值：
int i = m - 1;

int j = n - 1;

for (int k = m + n - 1; k >= 0; k--)
每次比較 nums1[i] 和 nums2[j]，將較大的值放入 nums1[k]，再動態調整對應的 index。

邊界條件

另外處理：

* m == 0
* n == 0
* 其中一個陣列已經先比較完的情況
* 0 本身可能是合法元素，不能單純把 0 當成空位

Takeaway

不要只想著「怎麼把 nums2 塞進 nums1」，而是思考「m+n 個位置應該依序放誰」。