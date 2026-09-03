# LeetCode 26 - Remove Duplicates from Sorted Array

## 思路

因為 `nums` 已經排序，所以重複的數字一定會相鄰。

先記錄前一個數值：

```c
previous = nums[0];
接著從左到右掃描，當發現：
nums[i] != previous
代表找到新的、不重複的值，就依序放到 nums 前面：
previous = nums[i];

nums[k] = nums[i];

k++;
最後前 k 個元素就是所有不重複的值，回傳 k。