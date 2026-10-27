# 82. Remove Duplicates from Sorted List II

**Language:** C · **Result:** Accepted (168/168)  
**Time:** O(n) · **Extra space:** O(1)

## 題目

給定一條已排序的 Linked List，移除**所有數值重複的節點**，只留下出現一次的數值。

```text
Input:  1 → 2 → 3 → 3 → 4 → 4 → 5
Output: 1 → 2 → 5
```

## 我的解法：三位冒險者

我想試著在**一次遍歷**中完成判斷、刪除與重新連接，因此替三個指標分配了角色：

| 指標 | 角色 | 工作 |
| --- | --- | --- |
| `Memorizer` 🧙‍♂️ | 保存者 | 記住最後確認可保留的位置，負責重新連接鏈結 |
| `Executor` 🥷 | 處決者 | 判斷並刪除重複節點 |
| `Explorer` 🧑‍🚀 | 探索者 | 向前探索，提供下一個節點供比較 |

### 關鍵：記住重複狀態

只比較 `Executor->val` 與 `Explorer->val` 還不夠：

```text
1 → 2 → 2 → 3
        E    X
```

此時 `E != X`，但 `E` 仍是重複的 `2`，不能保留。因此用 `in_duplicate` 記錄是否正在處理一組重複值；即使遇到下一個不同數值，也要先刪除該組最後一個節點。

確認 `Executor` 可保留後，`Memorizer` 才更新連接：

```c
Memorizer->next = Executor;
Memorizer = Memorizer->next;
```

### 特殊情況：Head 重複

原始 `head` 可能也要刪除。我選擇**先把它當作暫時的錨點**，讓主要迴圈繼續處理後面的節點，最後再根據 `head_duplicate`、`tri_duplicate` 更新 `head`。

## Debug 紀錄

- **連續重複值：** 用 `in_duplicate` 避免誤留重複群組的最後一個節點。
- **Head 連續重複：** 額外測試 `[1,1,1,2,3]`、`[1,1,1,1,1]` 等情況。
- **Heap-use-after-free：** 調整收尾流程後，忘了刪除舊的 `Memorizer->next = Executor`，把已釋放的節點重新接回鏈結。

## 心得

這題花了很久，最初甚至替保存者設計了 `skill_jump` 移動許可。後來重新釐清三個角色的職責，才發現這個旗標已經不需要了。

**這次最大的收穫不是找到最短的寫法，而是學會把指標的責任分清楚，再讓它們合作。**

三位冒險者，終於通關。🎉
