# 202. Happy Number

**Difficulty:** Easy  
**Language:** C  
**Topics:** Hash Table, Math, Cycle Detection

## 題目概念

給定一個正整數 `n`，反覆執行以下操作：

1. 將整數拆成各個位數。
2. 計算每個位數的平方和。
3. 將計算結果作為下一次的輸入。

如果最後得到 `1`，代表這是一個 **Happy Number**，回傳 `true`。

如果數字不斷循環，無法抵達 `1`，則回傳 `false`。

### Example 1

```text
Input: 19

19 → 82 → 68 → 100 → 1

Output: true
```

### Example 2

```text
Input: 2

2 → 4 → 16 → 37 → 58 → 89 → 145 → 42 → 20
    ↑                                  │
    └──────────────────────────────────┘

Output: false
```

---

## 我的解法

這題最初讓我思考的是：

**數字經過不斷平方加總後，有沒有可能無限發散？**

假設一個整數有 `k` 位數，每個位數最大為 `9`。

因此，平方和的最大值為：

$$
f(n) \leq 81k
$$

但一個 `k` 位數的正整數至少為：

$$
n \geq 10^{k-1}
$$

當位數夠大時，原始數字的成長速度遠大於平方和，因此轉換後的數字會縮小。

這表示數列不可能無限發散，最終會進入有限範圍。

由於相同的輸入一定產生相同的輸出，因此只要某個數字重複出現，就代表接下來會不斷循環。

所以終止條件只有兩種：

- **出現 `1`：** 回傳 `true`。
- **出現重複數字：** 回傳 `false`。

為了練習 Hash Table，我選擇使用 **Separate Chaining** 記錄已經出現過的數字。

---

## 1. 計算各位數平方和

使用 `% 10` 取得最後一位數字，再透過 `/ 10` 移除最後一位。

```c
int calculate(int n)
{
    int result = 0;

    while (n / 10 != 0)
    {
        result += (n % 10) * (n % 10);
        n /= 10;
    }

    result += (n % 10) * (n % 10);

    return result;
}
```

例如：

```text
n = 193

193 % 10 = 3 → 3² = 9
19  % 10 = 9 → 9² = 81
1   % 10 = 1 → 1² = 1

result = 91
```

---

## 2. 建立 Hash Table

這次不需要儲存額外的 Value，只需要記錄出現過的整數。

因此 Node 結構可以簡化為：

```c
struct ListNode
{
    int value;
    struct ListNode *next;
};
```

使用 `calloc()` 建立 17 個 Bucket：

```c
struct ListNode **table;

table = calloc(17, sizeof(struct ListNode *));
```

每個 Bucket 都是一個 Linked List 的 Head Pointer。

```text
table
  │
  ├── [0]  → NULL
  ├── [1]  → Node → Node → NULL
  ├── [2]  → NULL
  ├── [3]  → Node → NULL
  │
  ...
  └── [16] → NULL
```

Hash Function：

```c
index = new_value % 17;
```

這裡選擇 17 個 Bucket 作為練習用的固定大小 Hash Table。

---

## 3. 偵測重複數字

每次取得新的平方和，先檢查是否等於 `1`。

```c
if (new_value == 1)
{
    result = true;
    end = true;
    break;
}
```

如果不是 `1`，就查詢 Hash Table。

```c
index = new_value % 17;

struct ListNode *current = table[index];

while (current != NULL)
{
    if (current->value == new_value)
    {
        result = false;
        end = true;
        break;
    }

    current = current->next;
}
```

這裡需要注意：

**Hash Collision 不代表數字重複。**

即使兩個數字落在相同的 Bucket，也必須逐一比較 Node 中的 `value`。

只有找到完全相同的數字，才能確定已經進入循環。

---

## 4. 插入新的數字

如果 Hash Table 中沒有出現過目前的數字，就建立新的 Node。

使用 **Head Insertion**：

```c
struct ListNode *new_node = malloc(sizeof(struct ListNode));

new_node->value = new_value;
new_node->next = table[index];

table[index] = new_node;
```

例如：

```text
Before:

table[4] → Node(21) → Node(4) → NULL
```

插入 `38`：

```text
After:

table[4] → Node(38) → Node(21) → Node(4) → NULL
```

這種方式不需要走訪整條 Linked List，就能完成插入。

時間複雜度為：

$$
O(1)
$$

---

## 5. Memory Management

由於 Hash Table 中的 Node 是透過 `malloc()` 動態配置，因此結束前必須逐一釋放。

不能只執行：

```c
free(table);
```

因為這只會釋放 Bucket Pointer Array，不會釋放 Linked List 中的 Node。

我的做法是使用兩個 Pointer：

```c
for (int i = 0; i < 17; i++)
{
    if (table[i] != NULL)
    {
        struct ListNode *current = table[i]->next;
        struct ListNode *prev = table[i];

        while (prev != NULL)
        {
            free(prev);

            prev = current;

            if (current != NULL)
            {
                current = current->next;
            }
        }
    }
}

free(table);
```

核心概念是：

**在釋放目前的 Node 之前，必須確保仍然保有下一個 Node 的位址。**

這樣才能避免在 `free()` 之後，繼續存取已經失效的記憶體。

---
