# LeetCode 169 - Majority Element

## 思路

建立 `check_value` 紀錄需要檢查的候選值。

每次選一個候選值，掃描 `nums` 並計算出現次數：

- 如果次數超過 `numsSize / 2`，代表找到 majority element。
- 如果遇到新的數值，先確認它是否已經存在於 `check_value`。
- 只有不存在的值才加入候選清單，避免重複檢查以及陣列越界。

```c
bool exist = false;

for (int k = 0; k < j + 1; k++)
{
    if (nums[i] == check_value[k])
    {
        exist = true;
        break;
    }
}

if (!exist)
{
    check_value[j + 1] = nums[i];
}

核心：建立不重複的候選值，逐一計算出現次數，找到超過一半的元素。