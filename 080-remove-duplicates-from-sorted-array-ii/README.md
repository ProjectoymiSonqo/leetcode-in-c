# LeetCode 80 - Remove Duplicates from Sorted Array II

## 思路

這題是 #26 的延伸。

因為 `nums` 已經排序，所以相同的數字一定會連續出現。

使用 `previous` 記錄目前的數字，並用 `count` 計算它出現的次數。

```c

if (nums[i] != previous)
代表遇到新的數字，重設：
previous = nums[i];

count = 1;
如果是相同數字，只有：
count < 2
時才將它寫入 nums[k]。

因此每個數字最多只會保留兩次，其餘重複值直接忽略。

最後前 k 個元素就是處理後的結果。