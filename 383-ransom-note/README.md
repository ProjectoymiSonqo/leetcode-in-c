# 383. Ransom Note

## 思路

判斷 `magazine` 中的字符數量是否足夠組成 `ransomNote`。

一開始考慮使用 struct 紀錄：
- 字符
- 需要的數量

但因為題目只有 `a ~ z`，可以直接使用長度 26 的陣列：

`char - 'a'` → 對應到 `0 ~ 25`

## 做法

1. 掃描 `ransomNote`
   - 對應字符的 `count++`
   - 代表還需要多少個字符

2. 掃描 `magazine`
   - 對應字符的 `count--`

3. 檢查 `count[26]`
   - 如果還有 `count > 0`，代表字符不足 → `false`
   - 否則 → `true`

## Complexity

- Time: O(n + m)
- Space: O(1)