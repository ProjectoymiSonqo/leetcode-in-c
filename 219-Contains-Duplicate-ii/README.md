# 219. Contains Duplicate II

**Difficulty:** Easy  
**Language:** C  
**Data Structure:** Hash Table + Separate Chaining  
**Result:** Accepted — 72 / 72 Testcases

## 1. 題目描述

給定整數陣列 `nums` 和整數 `k`，判斷是否存在兩個不同的索引 `i`、`j`，使得：

```text
nums[i] == nums[j]
abs(i - j) <= k
```

如果符合條件，回傳 `true`，否則回傳 `false`。

### Example 1

```text
Input: nums = [1,2,3,1], k = 3
Output: true
```

因為：

```text
nums[0] == nums[3] == 1
abs(0 - 3) = 3 <= k
```

### Example 2

```text
Input: nums = [1,0,1,1], k = 1
Output: true
```

### Example 3

```text
Input: nums = [1,2,3,1,2,3], k = 2
Output: false
```

---

## 2. 我的解題思路

這題需要同時確認兩件事情：

1. 是否出現相同的數值。
2. 相同數值的索引距離是否小於或等於 `k`。

我決定使用 **Hash Table** 記錄已經出現過的數值及其位置。

每次讀取 `nums[i]` 時：

- 如果 Hash Table 中沒有這個數值，就新增 Node。
- 如果已經存在，就比較目前位置與先前記錄的位置。
- 如果距離符合 `k`，回傳 `true`。
- 如果距離超過 `k`，更新該數值的最新位置。

### 為什麼只需要記錄最新的 Position？

這是這題最重要的推導。

假設數值 `5` 出現在：

```text
Index:  0  1  2  3  4  5  6
Value:  5  2  3  5  4  1  5
        ↑        ↑        ↑
        A        B        C
```

當我們處理到 C（Index 6）時：

```text
Distance(A, C) = 6
Distance(B, C) = 3
```

因為 B 比 A 更接近 C，所以只需要保留 B 的位置。

對於未來任何位置 `i`：

若：

```text
position_A < position_B < i
```

則：

```text
i - position_B < i - position_A
```

因此，較舊的位置不可能比最新位置更容易滿足 `k` 的條件。

**結論：每個不同的 Value，只需要保留最新的 Position。**

這樣可以避免為相同數值建立多個 Node。

---

## 3. Hash Table 設計

我使用 Separate Chaining 處理 Hash Collision。

### Node Structure

```c
struct node {
    int val;
    int position;
    struct node *next;
};
```

各欄位的用途：

| Field | Description |
|---|---|
| `val` | 儲存 `nums[i]` |
| `position` | 該數值最近一次出現的索引 |
| `next` | 指向同一 Bucket 的下一個 Node |

### Hash Table

```c
int bucket_size = numsSize * 2 + 1;

struct node **table =
    calloc(bucket_size, sizeof(*table));
```

`table` 是一個 Pointer Array。

每個元素都是一條 Linked List 的 Head Pointer。

```text
Hash Table
    |
    +-- Bucket[0] --> Node --> Node --> NULL
    |
    +-- Bucket[1] --> Node --> NULL
    |
    +-- Bucket[2] --> NULL
    |
    +-- Bucket[3] --> Node --> Node --> NULL
    |
    ...
```

### Hash Function

```c
int index = abs(nums[i] % bucket_size);
```

使用取餘數決定 Bucket Index。

由於 `nums[i]` 可能是負數，因此使用 `abs()` 確保索引非負。

這個寫法是先取餘數再取絕對值，因此不會對 `INT_MIN` 本身直接取絕對值。

---

## 4. 搜尋與更新流程

每次處理 `nums[i]` 時，先計算 Bucket Index：

```c
int index = abs(nums[i] % bucket_size);

struct node *current = table[index];
```

### Case A：Bucket 是空的

直接建立新的 Node：

```c
struct node *new_node = malloc(sizeof(*new_node));

new_node->position = i;
new_node->val = nums[i];
new_node->next = NULL;

table[index] = new_node;
```

### Case B：Bucket 已經存在 Node

需要遍歷 Linked List。

```c
while (current != NULL)
{
    if (current->val == nums[i])
    {
        // Found same value
    }
    else
    {
        current = current->next;
    }
}
```

這裡必須注意：

**相同 Bucket Index 不代表相同 Value。**

不同數值可能因為 Hash Collision 被分配到同一個 Bucket。

因此仍然需要比較：

```c
current->val == nums[i]
```

### Case C：找到相同 Value

比較兩個位置的距離：

```c
if (abs(current->position - i) <= k)
{
    result = true;
    break;
}
else
{
    current->position = i;
    break;
}
```

如果距離符合條件，就已經找到答案。

如果距離超過 `k`，則更新最新位置。

更新之後必須離開 `while`，否則下一次比較會變成：

```text
i - i = 0
```

導致錯誤判斷。

### Case D：沒有找到相同 Value

新增 Node，並使用 Head Insertion：

```c
new_node->next = table[index];
table[index] = new_node;
```

例如：

```text
Before:

table[index] --> Node A --> Node B --> NULL
```

插入新節點後：

```text
After:

table[index] --> New Node --> Node A --> Node B --> NULL
```

必須先讓 `new_node->next` 指向原本的 Head，再更新 `table[index]`。

否則會遺失原本 Linked List 的入口，造成資料遺失與 Memory Leak。

---

## 5. 完整程式碼

以下保留我使用的 Hash Table 架構，並加入 Memory Cleanup。

```c
#include <stdbool.h>
#include <stdlib.h>

bool containsNearbyDuplicate(int* nums, int numsSize, int k)
{
    bool result = false;

    struct node {
        int val;
        int position;
        struct node *next;
    };

    if (numsSize <= 1 || k <= 0)
    {
        return false;
    }

    int bucket_size = numsSize * 2 + 1;

    struct node **table =
        calloc(bucket_size, sizeof(*table));

    if (table == NULL)
    {
        return false;
    }

    for (int i = 0; i < numsSize; i++)
    {
        bool found = false;

        int index = abs(nums[i] % bucket_size);

        struct node *current = table[index];

        while (current != NULL)
        {
            if (current->val == nums[i])
            {
                found = true;

                if (i - current->position <= k)
                {
                    result = true;
                }
                else
                {
                    current->position = i;
                }

                break;
            }

            current = current->next;
        }

        if (result)
        {
            break;
        }

        if (!found)
        {
            struct node *new_node =
                malloc(sizeof(*new_node));

            if (new_node == NULL)
            {
                break;
            }

            new_node->position = i;
            new_node->val = nums[i];

            new_node->next = table[index];
            table[index] = new_node;
        }
    }

    // Memory Cleanup
    for (int i = 0; i < bucket_size; i++)
    {
        struct node *current = table[i];

        while (current != NULL)
        {
            struct node *next = current->next;

            free(current);

            current = next;
        }
    }

    free(table);

    return result;
}
```

**註：** 這份程式碼稍微整理了控制流程，將「找到相同 Value」統一使用 `found` 表示。`malloc()` 失敗時會停止搜尋並清理記憶體，但函式的 `false` 無法區分「沒有符合條件」與「記憶體配置失敗」。正式產品程式應另外設計錯誤回報機制。

---

## 6. Time Limit Exceeded：Bucket Size 的影響

這次實作過程中，我一開始使用：

```c
int bucket_size = 19;
```

但提交 LeetCode 後：

```text
Time Limit Exceeded
54 / 72 Testcases Passed
```

問題不是 Hash Table 的搜尋邏輯，而是 Bucket 數量太少。

假設有 100,000 個不同的數值：

```text
100000 / 19 ≈ 5263
```

平均每個 Bucket 可能包含數千個 Node。

即使使用 Hash Table，每次搜尋仍然需要遍歷 Linked List。

因此，當 Bucket 數量固定且輸入規模不斷增加時，搜尋成本可能接近線性，整體演算法也可能退化成：

\[
O(n^2)
\]

### 第一次改善：固定較大的 Bucket

將 Bucket Size 改成：

```c
int bucket_size = 10007;
```

10007 是質數，常見於使用取餘數的 Hash Table 設計。

修改後成功通過測試。

不過，質數並不保證沒有 Hash Collision。

### 第二次改善：動態 Bucket

我最後改成：

```c
int bucket_size = numsSize * 2 + 1;
```

讓 Bucket 數量跟著輸入規模增加。

這樣可以控制 Load Factor。

### Load Factor

負載因子定義為：

\[
\alpha = \frac{\text{Node Count}}{\text{Bucket Count}}
\]

由於最多只會儲存 `numsSize` 個不同的數值：

\[
\alpha \leq \frac{n}{2n+1}<0.5
\]

在 Hash 分布均勻的情況下，較低的 Load Factor 通常可以縮短 Linked List，改善平均搜尋效率。

但代價是需要更多記憶體儲存 Bucket Pointer。

**這是時間與空間的取捨。**

---

## 7. Memory Management

這次所有 Node 都透過 `malloc()` 動態配置。

Hash Table 本身則使用 `calloc()`。

因此函式結束前，必須釋放：

1. 每個 Bucket 裡的所有 Node。
2. Hash Table 的 Pointer Array。

### Cleanup Code

```c
for (int i = 0; i < bucket_size; i++)
{
    struct node *current = table[i];

    while (current != NULL)
    {
        struct node *next = current->next;

        free(current);

        current = next;
    }
}

free(table);
```

這裡最重要的順序是：

```c
struct node *next = current->next;
free(current);
current = next;
```

必須先保存下一個 Node 的 Address，才能釋放目前的 Node。

否則如果先執行：

```c
free(current);
```

再讀取：

```c
current->next
```

就會發生 Use-After-Free，屬於 Undefined Behavior。

### 為什麼使用統一 Cleanup？

當找到符合條件的數值時，不直接在 Linked List 搜尋過程中 `return true`。

而是設定：

```c
result = true;
```

再跳出主迴圈。

這樣無論最後結果是 `true` 或 `false`，都會執行同一段 Cleanup。

避免因為提前回傳而造成 Memory Leak。

---

## 8. Complexity Analysis

設：

- `n`：`numsSize`
- `m`：Bucket 數量
- `α`：Load Factor

### Time Complexity

在 Hash 分布均勻的假設下，每次搜尋的平均成本約為：

\[
O(1+\alpha)
\]

由於：

```c
bucket_size = numsSize * 2 + 1;
```

因此：

\[
\alpha < 0.5
\]

平均搜尋成本可視為：

\[
O(1)
\]

總共處理最多 `n` 個元素：

**Average Time Complexity：**

\[
\boxed{O(n)}
\]

但如果大量不同數值都碰撞到同一個 Bucket，Linked List 仍然可能變得很長。

**Worst-case Time Complexity：**

\[
\boxed{O(n^2)}
\]

### Space Complexity

Hash Table 有 `2n + 1` 個 Bucket。

最多建立 `n` 個 Node。

因此：

\[
O(2n+1)+O(n)=O(n)
\]

**Space Complexity：**

\[
\boxed{O(n)}
\]

---

## 9. 這題學到了什麼？

這次最大的收穫不只是完成 Hash Table，而是開始思考資料應該如何被維護。

### ① 不需要保留所有歷史資料

當同一個 Value 重複出現時，較舊的 Position 已經沒有保留價值。

因此可以直接更新現有 Node，而不需要建立新的 Node。

這讓 Hash Table 不只是儲存資料，也能維護資料的最新狀態。

### ② Hash Table 的效能與 Bucket 數量有關

一開始使用 19 個 Bucket，雖然邏輯正確，卻遇到 TLE。

這讓我理解：

**使用 Hash Table 不代表時間複雜度一定是 O(1)。**

Bucket 數量、Hash Function、Load Factor 和 Collision Handling 都會影響實際效能。

### ③ Linked List 的 Pointer 必須小心維護

Head Insertion 時：

```c
new_node->next = table[index];
table[index] = new_node;
```

Memory Cleanup 時：

```c
struct node *next = current->next;
free(current);
current = next;
```

這兩個操作雖然目的不同，但核心概念相同：

**在修改或釋放 Node 之前，先保留後續仍然需要的 Address。**

### ④ 狀態變數應該有明確的意義

一開始使用 `end`，後來改成 `update_value`，最後整理成 `found`。

這讓我發現：

- `result`：是否找到符合題目條件的答案。
- `found`：這次搜尋是否找到相同的 Value。

兩者代表不同的狀態，不應混在一起。

---

## 10. Result

```text
LeetCode 219. Contains Duplicate II

Status: Accepted
Testcases: 72 / 72

Runtime: 85 ms
Memory: 44.96 MB
```

> 從 Happy Number 到 Contains Duplicate II，這次不只重新使用 Hash Table，也開始理解如何根據輸入規模調整資料結構。
>
> 最後，所有透過 malloc 配置的 Node 都有正確釋放。
>
> 小狗清潔隊，任務完成。🐶🧹