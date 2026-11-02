# 128. Longest Consecutive Sequence

**Difficulty:** Medium  
**Language:** C  
**Topic:** Hash Table, Separate Chaining, Bidirectional Search

## 題目說明

給定一個未排序的整數陣列 `nums`，找出其中最長連續整數序列的長度。

題目要求設計時間複雜度為 O(n) 的演算法。

例如：

```text
Input:  [100, 4, 200, 1, 3, 2]
Output: 4
```

因為最長的連續序列是：

```text
1 → 2 → 3 → 4
```

注意：連續序列指的是**數值連續**，不要求數字在原始陣列中的位置連續。

## 我的解法

這題我選擇自己建立 Hash Table，使用 **Separate Chaining（鏈結法）** 處理 Hash Collision，再搭配 **雙向搜尋與 Visited Flag** 找出最長連續序列。

核心想法是：

**不一定要從連續序列的頭或尾開始搜尋，也可以從任意一個數字出發，分別向左右探索。**

但這樣會產生重複搜尋，因此需要額外記錄哪些數字已經處理過。

### 1. 建立 Hash Table

先定義 Linked List Node：

```c
struct node {
    int val;
    bool visited;
    struct node *next;
};
```

各欄位的用途：

- `val`：儲存數字。
- `visited`：記錄數字是否已經參與過完整搜尋。
- `next`：處理 Hash Collision。

Bucket 數量使用：

```c
int bucket = numsSize * 2 + 1;
struct node **table = calloc(bucket, sizeof(*table));
```

Hash Index 計算：

```c
int index = nums[i] % bucket;

if (index < 0)
    index += bucket;
```

因為 C 語言對負數取餘數可能得到負值，所以必須修正 Index，確保落在合法的 Bucket 範圍內。

插入時，先搜尋該 Bucket 的 Linked List，確認數字是否已存在，避免重複建立 Node。

如果沒有重複，使用 Head Insertion：

```c
new_node->val = nums[i];
new_node->visited = false;
new_node->next = table[index];
table[index] = new_node;
```

### 2. 從任意數字開始雙向搜尋

例如：

```text
nums = {3, 2, 4, 5, 6, 7, 8}
```

從 `3` 開始：

```text
       3
      / \
     2   4 → 5 → 6 → 7 → 8
```

向左尋找 `3 - 1`，向右尋找 `3 + 1`。

每找到一個連續數字：

1. `length++`
2. 將對應 Node 的 `visited` 設為 `true`
3. 更新下一個要尋找的 Target
4. 繼續搜尋

直到 Hash Table 中找不到下一個連續數字為止。

最後更新：

```c
if (length > max_length)
    max_length = length;
```

由於起點本身也屬於連續序列，所以：

```c
int length = 1;
```

### 3. 使用 Visited Flag 避免重複搜尋

這是雙向搜尋最重要的部分。

假設從 `3` 出發，已經找到：

```text
2 → 3 → 4 → 5 → 6 → 7 → 8
```

如果接下來遍歷到 `4`，又重新向左右搜尋，就會重複處理同一條序列。

因此，搜尋時將所有找到的 Node 標記為：

```c
current->visited = true;
```

後續遍歷 `nums[]` 時，如果起點已經被標記，就直接跳過。

這樣可以避免對同一條完整連續序列反覆探索。

### 4. Hash Collision 與 Index 環繞

Hash Collision 不代表數值相同。

例如：

```text
bucket = 10007

1 % 10007     = 1
10008 % 10007 = 1
```

兩個數字落在同一個 Bucket，但它們不是相同的數字。

因此搜尋時不能只判斷：

```c
table[index] != NULL
```

還必須遍歷 Linked List，確認：

```c
current->val == target
```

另外，由於 Hash Function 使用取餘數，當 Target 增加或減少 1 時，Bucket Index 也會相應移動。

向右：

```c
right_index = (right_index + 1) % bucket;
```

向左：

```c
left_index--;

if (left_index < 0)
    left_index += bucket;
```

這裡必須注意，**Hash Index 可以環繞，但整數數值本身不能任意環繞。**

### 5. 處理 INT_MAX 與 INT_MIN

這題遇到一個重要的 Boundary Condition：

```c
int nums[] = {
    INT_MAX,
    INT_MAX - 1,
    INT_MIN,
    INT_MIN + 1
};
```

正確答案應該是：

```text
2
```

因為：

```text
INT_MAX - 1 → INT_MAX

INT_MIN → INT_MIN + 1
```

是兩條獨立的連續序列。

如果直接執行：

```c
int target_right = nums[i] + 1;
int target_left  = nums[i] - 1;
```

當數字位於整數邊界時，可能造成 Signed Integer Overflow，屬於 C 語言的 Undefined Behavior。

因此我將邊界處理拆成兩個階段。

**初始化階段**

- 起點為 `INT_MAX`，不啟動右側搜尋。
- 起點為 `INT_MIN`，不啟動左側搜尋。

**搜尋階段**

- 找到 `INT_MAX` 後，將它計入長度，但不再增加 Target。
- 找到 `INT_MIN` 後，將它計入長度，但不再減少 Target。

這樣可以避免整數溢位，也不會漏算邊界上的數字。

### 6. Memory Management

Hash Table 使用動態記憶體配置，因此函式結束前需要釋放所有 Node 和 Bucket Array。

```c
for (int i = 0; i < bucket; i++)
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

必須先保存 `current->next`，再釋放 `current`，避免 Use-After-Free。

## 時間與空間複雜度

令：

- `n`：輸入陣列長度。
- `u`：不重複數字的數量。
- `b`：Bucket 數量。

在 Hash 分布良好、平均查詢成本接近 O(1) 的情況下：

| 階段 | 平均時間複雜度 |
|---|---|
| 建立 Hash Table | O(n) |
| 雙向搜尋與 Visited 標記 | O(n) |
| 清理記憶體 | O(b + u) |

由於使用 `bucket = numsSize * 2 + 1`，因此 `b = O(n)`。

**平均時間複雜度：O(n)**

但 Separate Chaining 仍然可能出現大量 Hash Collision，使單次查詢退化成 O(n)。

**最壞時間複雜度：O(n²)**

空間部分需要儲存 Bucket Array 和不重複數字的 Node：

**空間複雜度：O(n)**

因此這份實作在平均情況下符合線性時間的設計目標，但不保證最壞情況仍為 O(n)。

## 效能實驗

在 LeetCode 上通過全部 85 個測資後，我嘗試調整 Hash Table 的 Bucket 數量，以及比較是否釋放記憶體對執行時間的影響。

| 實驗版本 | Runtime |
|---|---:|
| 固定 10007 Buckets，包含清理 | 120 ms |
| `numsSize * 2 + 1` Buckets，包含清理 | 82 ms |
| 動態 Buckets，暫時停用清理 | 55 ms |

以上是個別提交的觀察結果，LeetCode 的執行時間可能波動，不能單靠這些數字確認效能改善的原因。

不釋放記憶體雖然在某次測試中得到較短的 Runtime，但會造成 Memory Leak，因此不採用作為正式版本。

後續可以嘗試使用 **Memory Pool**，減少大量獨立 `malloc()` 與 `free()` 的成本，同時保留正確的記憶體生命週期。

## 心得

這題一開始最困難的不是 Hash Table，而是如何利用 Hash Table 找出連續序列。

我最初考慮過利用相鄰 Bucket 判斷數字是否連續，但發現 Hash Collision 會讓這種判斷不可靠。

後來改成直接查詢指定數值是否存在，並發展出從任意數字開始的雙向搜尋。

接著又遇到重複搜尋的問題，因此在 Node 中加入 `visited`，讓每條連續序列只需要完整探索一次。

最後再逐步處理負數 Index、Bucket 環繞、整數上下限與記憶體釋放。

這題讓我更清楚理解：

**Hash Table 負責快速查詢，搜尋策略決定如何使用查詢結果，而記憶體配置策略則會影響實際執行效能。**

雖然程式碼不算精簡，但這是我從資料結構開始，逐步建立、驗證並修正出來的解法。

也以這題作為 Top Interview 150 的 Hashmap 章節練習總結。