# 61. Rotate List

**Difficulty:** Medium  
**Language:** C  
**Result:** Accepted — 232 / 232  
**Time:** O(n) | **Extra space:** O(1)

## 題目

將 Linked List 向右旋轉 `k` 次。

```text
Input:  1 → 2 → 3 → 4 → 5, k = 2
Output: 4 → 5 → 1 → 2 → 3
```

## 我的思路

不需要真的旋轉 `k` 次。先走訪一次取得長度 `cnt`，再利用 `k % cnt` 算出實際旋轉量，直接找到切割點並重新連接原本的節點。

### 1. 計算旋轉量

```c
int right_times = k % cnt;
```

空串列、單節點或 `right_times == 0` 可以直接回傳原本的 `head`。

### 2. 切成兩段

切割位置是原始串列的第 `cnt - right_times` 個節點（從 1 開始計算）。

```c
while (new_cnt < cnt - right_times - 1)
{
    new_cnt++;
    current = current->next;
}

endpoint = current;
newhead = current->next;
endpoint->next = NULL;
```

```text
原始：1 → 2 → 3 → 4 → 5 → NULL

前段：1 → 2 → 3 → NULL   (endpoint = 3)
後段：4 → 5 → NULL       (newhead = 4)
```

**關鍵是先執行 `endpoint->next = NULL`。** 這一步明確切斷兩段串列，讓原本的 `endpoint` 成為最終串列的尾巴，避免接回前段後形成環。

### 3. 接回原始 head

```c
current = head;  // 保存原始 head
head = newhead;  // 更新新 head

while (newhead != endpoint)
{
    if (newhead->next == NULL)
    {
        newhead->next = current;
        newhead = newhead->next;
    }
    else
    {
        newhead = newhead->next;
    }
}
```

走到後段尾巴 `5` 時，將它接回原始 `head`（`1`）。接著指標沿著前段走到 `endpoint`（`3`），迴圈結束。

```text
4 → 5 → 1 → 2 → 3 → NULL
                ↑
             endpoint
```

這裡的 `while (newhead != endpoint)` 利用預先保存的尾巴指標作為停止條件；而先前設置的 `endpoint->next = NULL` 則確保**最終串列正確終止**。兩者配合，無須額外配置節點。

## 複雜度

- **Time: O(n)** — 計算長度、找切割點、走訪並接回串列，總共仍是線性時間。
- **Extra space: O(1)** — 只使用固定數量的指標和計數器。

## 心得

這次先想清楚整體流程：**計算長度 → 確定切割位置 → 切成兩段 → 重新連接**，再開始實作。比起上一題 Q82 反覆調整指標規則，這題大約一小時就完成，通過 **232 / 232** 個測資。
