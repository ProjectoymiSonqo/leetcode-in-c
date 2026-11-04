# LeetCode 15 — 3Sum

**Difficulty:** Medium  
**Language:** C  
**Result:** Accepted (316/316)  
**Runtime:** 124 ms  
**Memory:** 36.90 MB

## 1. 題目描述

給定一個整數陣列 `nums`，找出所有不重複的三元組 `[a, b, c]`，使得：

```c
a + b + c == 0
```

同一個陣列元素不能重複使用，但相同數值若出現多次，可以組成合法三元組。

例如：

```c
nums = {-1, 0, 1, 2, -1, -4};
```

結果：

```c
[[-1, -1, 2], [-1, 0, 1]]
```

三元組內部的排列順序不影響答案，但不能出現重複的三元組。

---

## 2. 解題思路

最初的想法是固定兩個數值，計算第三個數：

```c
target = -(long long)ans - know1;
```

再利用 Hash Table 查找 `target` 是否存在。

但除了確認三數相加為零，還必須解決：

1. 相同數值出現次數是否足夠。
2. 如何避免重複三元組。
3. 如何控制搜尋時間複雜度。
4. 如何管理動態配置的記憶體。

最終採用：

**Hash Table + Contiguous Memory Pool + Two Loops + Pointer Arithmetic**

## 3. 資料結構設計

```c
struct Node {
    int val;
    int cnt;
    struct Node *next;
};
```

其中：

- `val`：儲存唯一數值。
- `cnt`：記錄該數值出現次數。
- `next`：處理 Hash Collision，連接同一個 bucket 的 Node。

### 3.1 Hash Table

使用 Separate Chaining 處理碰撞。

```c
int bucket = 2 * numsSize + 1;

struct Node **table =
    calloc(bucket, sizeof(struct Node *));
```

Hash Index：

```c
static int hash_index(int value, int bucket)
{
    int index = value % bucket;

    if (index < 0)
        index += bucket;

    return index;
}
```

透過 `hascontain()` 查找數值是否存在，平均查找時間接近 O(1)，但碰撞嚴重時可能退化。

### 3.2 Memory Pool

與其為每個 Node 個別配置：

```c
struct Node *new_node = malloc(sizeof(struct Node));
```

改成一次配置連續記憶體：

```c
struct Node *pool =
    malloc(numsSize * sizeof(struct Node));
```

建立新 Node：

```c
struct Node *new_node = &pool[used++];

new_node->val = nums[i];
new_node->cnt = 1;
new_node->next = table[index];

table[index] = new_node;
```

當數值已經存在時，只需要：

```c
found->cnt++;
```

因此 `pool[0]` 到 `pool[used-1]` 分別儲存不同數值。

例如：

```c
nums = {3, -2, 0, 1, -2, 0, 0};
```

Memory Pool：

| Index | val | cnt |
|---|---|---|
| 0 | 3 | 1 |
| 1 | -2 | 2 |
| 2 | 0 | 3 |
| 3 | 1 | 1 |

`used = 4`

這讓同一批 Node 同時具備兩種存取方式：

- Hash Table：透過數值快速查找 Node。
- Memory Pool：透過陣列索引遍歷所有唯一數值。

不需要額外複製一份 Unique Array。

---

## 4. 搜尋演算法

### 4.1 前兩層迴圈

```c
for (int i = 0; i < used; i++)
{
    int ans = pool[i].val;

    for (int j = i; j < used; j++)
    {
        int know1 = pool[j].val;

        long long target =
            -(long long)ans - know1;
    }
}
```

因為使用 Unique Pool，搜尋次數取決於 `used`，而不是原始 `numsSize`。

`j` 從 `i` 開始，允許兩個數值相同。

### 4.2 尋找第三個數

```c
if (target < INT_MIN || target > INT_MAX)
    continue;

struct Node *found =
    hascontain(table, bucket, (int)target);

if (found == NULL)
    continue;
```

使用 `long long` 避免計算第三個數時發生 signed integer overflow。

## 5. 核心突破：利用 Pointer Arithmetic 避免重複

最初使用三層迴圈：

```c
for (int i = 0; i < used; i++)
    for (int j = i; j < used; j++)
        for (int k = j; k < used; k++)
```

限制：

```c
i <= j <= k
```

這可以避免同一組三元組因索引排列不同而重複出現。

但時間複雜度是 O(used³)。

改成 Hash Lookup 後，第三個數可能來自 `pool` 的任何位置，原本的 `k >= j` 限制便不再自動成立。

由於 Hash Table 的所有 Node 都來自同一個連續配置的 `pool`，因此可以透過指標相減取得索引：

```c
int k = found - pool;
```

例如：

```c
found = &pool[4];

found - pool;  // 4
```

指標相減得到的是元素距離，而不是 byte 數。

因此可以恢復原本的去重規則：

```c
if (k < j)
    continue;
```

這樣不需要第三層迴圈，也不需要另外建立已使用結果的 Hash Set。

**這是本題最重要的設計：利用 Memory Pool 的連續記憶體特性，讓 Hash Lookup 與陣列索引產生關聯。**

---

## 6. 重複數值合法性檢查

找到第三個數後，還需要確認輸入中是否有足夠數量的元素。

```c
int legal = false;

if (i == j && j == k)
{
    if (pool[j].cnt >= 3)
        legal = true;
}
else if (i == j || j == k)
{
    if (pool[j].cnt >= 2)
        legal = true;
}
else
{
    legal = true;
}
```

分為三種互斥情況：

| 情況 | 範例 | 條件 |
|---|---|---|
| 三個數相同 | `[0,0,0]` | `cnt >= 3` |
| 恰好兩個數相同 | `[-2,1,1]` | `cnt >= 2` |
| 三個數不同 | `[-3,1,2]` | 不需額外檢查 |

由於 `i <= j <= k`，當恰好兩個索引相同時，`j` 一定是重複值所對應的索引。

因此只需要檢查 `pool[j].cnt`。

---

## 7. 動態記憶體管理

結果使用 `int **result` 儲存，每個三元組另外配置三個整數。

```c
result[count] = malloc(3 * sizeof(int));
```

結果陣列使用 Capacity 管理：

```c
int capacity = 1;
```

當容量不足時，以兩倍容量擴充：

```c
int new_capacity = capacity * 2;

int **temp = realloc(
    result,
    new_capacity * sizeof(int *)
);
```

必須使用暫存指標 `temp`，避免 `realloc()` 失敗時遺失原本的 `result`。

### Memory Cleanup

由於 C 沒有自動記憶體管理，配置失敗時必須依序釋放：

1. 已配置的 `result[i]`。
2. `result` 指標陣列。
3. `table`。
4. `pool`。

另外，當 `count == 0` 時，直接釋放資源並回傳 `NULL`，避免依賴 `malloc(0)` 的實作行為。

---

## 8. 時間與空間複雜度

令：

- `n = numsSize`
- `u = used`，即不重複數值數量。
- `R` 為合法三元組數量。

### Time Complexity

建立 Hash Table：

平均 O(n)。

雙層迴圈：

O(u²) 次組合檢查，每次 Hash Lookup 平均接近 O(1)。

因此平均時間複雜度：

**O(n + u² + R)**

其中 `R` 包含輸出結果的配置成本，且 `R = O(u²)`。

Hash Collision 嚴重時，單次查找可能退化為 O(u)，因此最壞情況仍可能達到 O(nu + u³)。

### Space Complexity

- Hash Table：O(n)
- Memory Pool：O(n)
- Result：O(R)

總空間複雜度：

**O(n + R)**

---

## 9. 開發過程與問題紀錄

### Version 1：Original Array + Hash Lookup

使用原始 `nums` 雙層迴圈，透過 Hash Table 尋找第三個數。

問題：

- 重複數值導致大量重複搜尋。
- 需要逐一檢查已儲存的結果是否重複。
- 初期 Result Capacity 逐次增加，造成額外的 `realloc` 成本。
- 遇到 MLE，調整後仍在最後測資遇到 TLE。

結果：315/316，TLE。

### Version 2：Unique Pool + Triple Loop

發現原本用來配置 Node 的 `pool`，其實也可以作為 Unique Array 使用。

改成遍歷 `pool`，並透過 `cnt` 驗證相同數值的合法性。

優點：

- 大量重複數值時，搜尋範圍大幅縮小。
- `i <= j <= k` 自然避免重複結果。

問題：

- 三層迴圈的時間複雜度是 O(u³)。
- 當輸入包含大量不同數值時，效能反而更差。

結果：311/316，TLE。

### Version 3：Unique Pool + Hash Lookup

保留 Unique Pool 的雙層迴圈，使用 Hash Table 尋找第三個數。

透過：

```c
int k = found - pool;
```

恢復第三個數的索引，再使用：

```c
if (k < j)
    continue;
```

避免重複三元組。

最終通過全部測試。

**Result: Accepted — 316/316**

---

## 10. 心得

這題最困難的部分，不是計算三個數相加是否為零，而是如何同時處理：

- Pointer 與 Struct 的記憶體關係。
- Hash Table 與 Collision。
- Memory Pool 的配置與存取。
- Dynamic Allocation 與 Memory Leak。
- Duplicate Detection。
- Time Complexity Optimization。

原本使用 Memory Pool，只是為了避免反覆配置 Node。

但後來發現，連續記憶體不只改善記憶體管理，也能透過 Pointer Arithmetic 將 Hash Table 的搜尋結果轉換成陣列索引。

這讓原本三層迴圈的去重規則，可以在雙層迴圈中繼續使用。

**最大的收穫是：資料結構不只是儲存資料的方法，它的記憶體布局與存取方式，也可能直接影響演算法的設計。**

雖然最終 Runtime 只有 Beats 5.05%，但這次優先完成的是一套能夠理解、驗證並自行推導的解法。

後續若需要優化，可以再研究 Sorting + Two Pointers 的標準解法，與目前的 Hash Table 方法比較。