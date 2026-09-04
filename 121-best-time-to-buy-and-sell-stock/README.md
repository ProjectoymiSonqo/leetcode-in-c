# 121. Best Time to Buy and Sell Stock

## 思路

一開始使用雙層迴圈：

- 每一天作為買入點。
- 再搜尋後面所有可能的賣出點。
- 計算 `prices[j] - prices[i]` 並保留最大收益。

這個方法邏輯正確，但時間複雜度為 **O(n²)**，當 input 很大時會造成 Time Limit Exceeded。

## 改進

改成從最後一天開始往前搜尋：

- 記錄目前右側找到的最高賣出價格 `max_price`。
- 往前尋找更低的買入價格 `mini_price`。
- 確保買入日期在賣出日期之前。
- 每次計算收益並更新 `best_price`。

將原本重複搜尋未來價格的雙層迴圈，縮減為一次遍歷。

## Complexity

- Time: O(n)
- Space: O(1)