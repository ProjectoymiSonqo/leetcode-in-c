# 題目
2. Add Two Numbers

# 問題拆解

最小單位不是整串 linked list。

而是目前節點的加法。

每次只需要考慮：

- l1 目前節點
- l2 目前節點
- carry

完成後：

- 建立新的節點
- 記錄目前結果
- carry 傳到下一個節點
- 指標往下一格

一直做到兩個 linked list 都結束。

最後如果 carry 仍然存在，再補一個節點。

# Design Pattern（解題模式）

逐節點模擬加法

利用 carry 串接下一次運算。

# 我學到什麼

第一次真正熟悉 linked list 的建立與串接。