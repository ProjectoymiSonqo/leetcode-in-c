# 125. Valid Palindrome

## 思路

使用 Two Pointers，`left` 從左往右、`right` 從右往左搜尋。

- 遇到非英數字元直接跳過
- 大寫字母轉成小寫後再比較
- 左右字元不同時，代表找到反例，直接回傳 `false`
- 若兩個 pointer 相遇前都沒有找到反例，則為 palindrome

一開始將 `result` 設為 `false`，導致 `" "`、`"!!!"` 這類沒有合法比較字元的 testcase 判斷錯誤。

後來改成：

> 預設 palindrome 成立，只尋找能推翻它的反例。

因此 `result` 初始為 `true`，非法字元只需要跳過即可。

## 複雜度

- Time: `O(n)`
- Space: `O(1)`