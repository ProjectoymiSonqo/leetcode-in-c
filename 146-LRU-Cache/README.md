# 146. LRU Cache

| 項目 | 結果 |
| --- | --- |
| Difficulty | Medium |
| Language | C |
| Result | Accepted (23/23 Testcases) |
| Runtime | 34 ms |
| Memory | 29.47 MB |

## 題目概述

設計一個 **LRU（Least Recently Used）Cache**，支援兩種操作：

- `get(key)`：如果 Key 存在，回傳對應的 Value，否則回傳 `-1`。
- `put(key, value)`：如果 Key 已存在，更新 Value；如果不存在，插入新資料。當 Cache 已滿時，必須淘汰最久未使用的資料。

每次成功執行 `get()` 或 `put()`，對應的資料都會成為最近使用的資料。

題目要求 `get()` 與 `put()` 的**平均時間複雜度為 O(1)**。

---

## 一、我的解題思路

一開始最大的問題是：

**如何同時做到快速搜尋，以及快速調整資料的使用順序？**

如果只使用 Linked List：

- 可以維護使用順序。
- 但搜尋指定 Key 需要 O(n)。

如果只使用 Hash Table：

- 可以快速找到指定 Key。
- 但不容易判斷哪筆資料最久沒有使用。

因此，我決定結合兩種資料結構：**Hash Table + Doubly Linked List**，讓兩者分別負責不同的工作。

| 資料結構 | 負責功能 |
| --- | --- |
| Hash Table | 根據 Key 快速找到 Node |
| Doubly Linked List | 維護最近使用順序 |
| Head | Most Recently Used（MRU） |
| Tail | Least Recently Used（LRU） |

### 資料結構示意

```text
Hash Table

table[0] → [A] → [C] → NULL
table[1] → [B] → NULL
table[2] → [D] → NULL

LRU Doubly Linked List

HEAD (MRU)                  TAIL (LRU)
    ↓                           ↓
   [C] ⇄ [A] ⇄ [D] ⇄ [B]
```

同一個 Node 同時存在於兩套鏈結關係中。Hash Table 負責找到 Node，而 Doubly Linked List 負責管理使用順序。

---

## 二、Node 結構設計

```c
struct Node {
    int key;
    int value;
    struct Node *hash_next;
    struct Node *prev;
    struct Node *next;
};
```

其中有三個 Pointer：

- `hash_next`：用於 Hash Table 的 Collision Chain。
- `prev`、`next`：用於 LRU Doubly Linked List。

**同一個 Node 可以同時參與不同的鏈結關係，而且這些關係可以獨立維護。**

```text
Hash Table:
[A] → [C] → [B]

LRU List:
[B] ⇄ [A] ⇄ [C]
```

調整 LRU 順序時，不需要改變 Hash Table 的鏈結。反過來，修改 Hash Table 的 Collision Chain，也不應該影響 LRU List。

### LRUCache 結構

```c
typedef struct {
    int capacity;
    int size;
    struct Node *head;
    struct Node *tail;
    int bucket_count;
    struct Node **table;
} LRUCache;
```

`table` 是一個 Pointer Array，每個元素指向一條 Collision Chain 的起點。

```text
obj->table
    |
    v
+---------+
| table[0]| → Node → Node → NULL
+---------+
| table[1]| → NULL
+---------+
| table[2]| → Node → NULL
+---------+
```

在我的實作中：

```c
bucket_count = capacity * 2 + 1;
index = key % bucket_count;
```

這是一種簡單的 Hash Function。Bucket 數量大於 Cache Capacity，有助於降低碰撞機率，但不能完全避免 Collision。

---

## 三、Hash Table 的操作

### 1. `hashFind()`

透過 Key 計算 Bucket Index，再沿著 Collision Chain 尋找指定 Node。

```c
static struct Node *hashFind(LRUCache *obj, int key)
{
    int index = key % obj->bucket_count;
    struct Node *current = obj->table[index];
    while (current != NULL)
    {
        if (current->key == key)
        {
            return current;
        }
        current = current->hash_next;
    }
    return NULL;
}
```

這裡使用 **Separate Chaining** 處理 Hash Collision。

### 2. Hash Table 的 Head Insertion

一開始，我打算將新 Node 插入 Collision Chain 的尾端。

```text
table[index] → A → B → C → NULL
```

如果要插入 D，就必須先找到 C。後來發現：

**Hash Table 的 Collision Chain 不需要維持特定順序。**

因為搜尋時只需要找到符合 Key 的 Node，因此可以直接使用 Head Insertion：

```c
node->hash_next = obj->table[index];
obj->table[index] = node;
```

```text
Before:
table[index] → A → B → C → NULL

After:
table[index] → D → A → B → C → NULL
```

這樣有幾個好處：

- 不需要遍歷 Collision Chain。
- 不需要額外尋找 Tail。
- 不需要特別補上 `NULL`。
- 空 Bucket 與非空 Bucket 都能使用相同邏輯。

插入操作的時間複雜度為 **O(1)**。這是這題讓我印象很深刻的一個發現。

### 3. `hashRemove()`

當 Cache 已滿，需要淘汰 LRU Node 時，必須先將它從 Hash Table 移除。

```c
static void hashRemove(LRUCache *obj, struct Node *node)
{
    int index = node->key % obj->bucket_count;
    struct Node *current = obj->table[index];
    struct Node *previous = NULL;
    while (current != NULL)
    {
        if (current == node)
        {
            if (previous == NULL)
            {
                obj->table[index] = current->hash_next;
            }
            else
            {
                previous->hash_next = current->hash_next;
            }
            break;
        }
        previous = current;
        current = current->hash_next;
    }
}
```

**Case A：移除 Bucket Head**

```text
Before:
table[index] → A → B → C → NULL

After:
table[index] → B → C → NULL
```

因此：

```c
obj->table[index] = current->hash_next;
```

**Case B：移除 Middle 或 Tail**

```text
Before:
A → B → C → NULL

After removing B:
A → C → NULL
```

因此：

```c
previous->hash_next = current->hash_next;
```

如果移除的是 Tail，因為 `current->hash_next == NULL`，也能自然完成鏈結。

這裡我曾經犯了一個錯誤：

```c
if (previous == NULL)
{
    obj->table[index] = NULL;
}
```

當時下意識認為 `previous == NULL`，後面就沒有資料。但實際上：

> `previous == NULL` 只代表 current 前面沒有 Node，不代表 current 後面沒有 Node。

另外，也曾經把 `previous->next` 和 `previous->hash_next` 搞混。這提醒我，在同一個 Node 維護多套鏈結時，必須清楚區分每個 Pointer 的職責。

---

## 四、Doubly Linked List：`moveToHead()`

LRU Cache 的核心操作之一，就是將指定 Node 移動到 Head。

```text
Before:
HEAD                  TAIL
 ↓                     ↓
[A] ⇄ [B] ⇄ [C] ⇄ [D]

After using C:
HEAD                  TAIL
 ↓                     ↓
[C] ⇄ [A] ⇄ [B] ⇄ [D]
```

我的做法是分成兩個階段：

1. 將 Node 從原本位置移除。
2. 將 Node 插入 Head。

```c
static void moveToHead(LRUCache *obj, struct Node *node)
{
    if (node == obj->head)
    {
        return;
    }
    else if (node == obj->tail)
    {
        obj->tail = obj->tail->prev;
        obj->tail->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    node->next = obj->head;
    obj->head->prev = node;
    node->prev = NULL;
    obj->head = node;
}
```

這裡需要處理三種情況：

| Node 位置 | 操作 |
| --- | --- |
| Head | 不需要移動 |
| Middle | 將前後 Node 重新連接 |
| Tail | 更新 Tail，再移動 Node |

### Pointer 的相對關係

這部分是我花了不少時間才釐清的地方。

```c
node->next = obj->head;
```

只代表 Node 的 `next` 指向原本的 Head。

```text
Node ──next──> Head
```

但這不代表 Head 的 `prev` 會自動指回 Node。因此還需要：

```c
obj->head->prev = node;
```

最後才更新：

```c
obj->head = node;
```

**在 C 語言中，修改其中一個 Pointer，不會自動維護另一個 Pointer 的關係。**

而且操作順序很重要。在更新 `obj->head` 之前，它仍然指向原本的 Head，因此可以利用這個地址完成鏈結。

---

## 五、`get(key)`

有了 `hashFind()` 和 `moveToHead()` 之後，`get()` 就很簡單了。

```c
int lRUCacheGet(LRUCache *obj, int key)
{
    struct Node *node = hashFind(obj, key);
    if (node != NULL)
    {
        moveToHead(obj, node);
        return node->value;
    }
    return -1;
}
```

執行流程：

```text
get(key)
   |
   v
hashFind()
   |
   +── Not Found → return -1
   |
   v
moveToHead()
   |
   v
return node->value
```

值得注意的是，`moveToHead()` 只修改 Node 之間的鏈結關係。**Node 本身的記憶體地址並沒有改變。**

所以即使移動到 Head，仍然可以透過原本的 Pointer 取得 `node->value`。

---

## 六、`put(key, value)`

我將 `put()` 分成三種情況。

### Case 1：Key 已存在

直接更新 Value，再移動到 Head。

```c
struct Node *node = hashFind(obj, key);
if (node != NULL)
{
    node->value = value;
    moveToHead(obj, node);
}
```

這個情況不需要配置新的記憶體，也不需要修改 Hash Table。

### Case 2：Key 不存在，Cache 已滿

這是整題最複雜的部分。

原本我考慮：

1. 刪除 Tail。
2. `free()` 舊 Node。
3. `malloc()` 新 Node。
4. 插入 Hash Table。
5. 插入 LRU Head。

但後來想到：

**既然 Tail Node 已經有配置好的記憶體，為什麼不直接重複利用？**

因此改成：

```c
struct Node *node = obj->tail;

// 1. 從舊 Hash Bucket 移除
hashRemove(obj, node);

// 2. 重複利用 Tail Node
node->key = key;
node->value = value;

// 3. 插入新的 Hash Bucket
int index = key % obj->bucket_count;
node->hash_next = obj->table[index];
obj->table[index] = node;

// 4. 移動到 LRU Head
moveToHead(obj, node);
```

流程：

```text
Cache Full
    |
    v
取得 Tail Node
    |
    v
從舊 Hash Bucket 移除
    |
    v
更新 Key / Value
    |
    v
插入新 Hash Bucket
    |
    v
moveToHead()
```

這個設計的優點：

- 不需要額外 `malloc()`。
- 不需要 `free()`。
- Cache Size 不變。
- 重複利用原本的 Node。
- 不需要重新建立 Doubly Linked List。

這裡有一個重要的操作順序：

**必須先從舊 Hash Bucket 移除 Node，再修改 Key。**

因為 `hashRemove()` 需要使用舊 Key 計算原本的 Bucket Index。如果先修改 Key，就可能找錯 Bucket。

### Case 3：Key 不存在，Cache 尚未滿

這個情況需要建立新 Node。

```c
struct Node *new_node = malloc(sizeof(struct Node));
if (new_node == NULL)
{
    return;
}
obj->size++;
new_node->key = key;
new_node->value = value;

// Hash Table
int index = key % obj->bucket_count;
new_node->hash_next = obj->table[index];
obj->table[index] = new_node;

// Doubly Linked List
if (obj->size == 1)
{
    obj->tail = new_node;
    new_node->next = NULL;
}
else
{
    obj->head->prev = new_node;
    new_node->next = obj->head;
}
obj->head = new_node;
new_node->prev = NULL;
```

如果是第一個 Node：

```text
HEAD ─┐
      v
     [A]
      ^
TAIL ─┘
```

如果已經有其他 Node：

```text
Before:
HEAD             TAIL
 ↓                ↓
[A] ⇄ [B] ⇄ [C]

After inserting D:
HEAD                   TAIL
 ↓                      ↓
[D] ⇄ [A] ⇄ [B] ⇄ [C]
```

這裡也要注意：**只有在 `malloc()` 成功後，才能增加 `obj->size`。**

避免記憶體配置失敗時，Cache Size 與實際 Node 數量不一致。

---

## 七、記憶體管理

這題讓我重新思考了 `malloc()`、`free()` 與 Pointer Ownership。

`LRUCache`、Hash Table Pointer Array，以及每個 Node，都是獨立配置的記憶體。

```text
LRUCache (0x1000)
    |
    +── table → 0x2000
    |
    +── head  → 0x3000
    |
    +── tail  → 0x5000
```

執行：

```c
free(obj);
```

只會釋放 `LRUCache` 本身的記憶體，不會自動釋放 `obj->table`，也不會自動釋放 Linked List 中的 Node。

因此在 `lRUCacheFree()` 中，需要先釋放所有 Node，再釋放 Hash Table，最後釋放 Cache 本身。

```c
void lRUCacheFree(LRUCache *obj)
{
    if (obj == NULL)
    {
        return;
    }
    struct Node *current = obj->head;
    while (current != NULL)
    {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
    free(obj->table);
    free(obj);
}
```

這裡也學到一個重要技巧：

```c
struct Node *next = current->next;
free(current);
current = next;
```

必須先保存下一個 Node 的地址，再釋放目前的 Node。否則 `free(current)` 後，再存取 `current->next` 就會造成 **Undefined Behavior**。

另外，雖然每個 Node 同時存在於 Hash Table 和 LRU List 中，但它們指向的是**同一塊記憶體**。

所以只需要沿著其中一套完整的鏈結釋放所有 Node，不能在兩套鏈結中各自 `free()` 一次。

---

## 八、時間複雜度

設 Cache Capacity 為 `n`，某個 Bucket 的 Collision Chain 長度為 `k`。

| 操作 | 時間複雜度 |
| --- | --- |
| `hashFind()` | 平均 O(1)，最差 O(n) |
| `hashRemove()` | 平均 O(1)，最差 O(n) |
| Hash Head Insertion | O(1) |
| `moveToHead()` | O(1) |
| `get()` | 平均 O(1)，最差 O(n) |
| `put()` | 平均 O(1)，最差 O(n) |

Hash Table 在碰撞分布良好的情況下，可以達到平均 O(1) 搜尋。但因為使用 Separate Chaining，如果大量 Key 落在同一個 Bucket，最差仍可能需要 O(n) 遍歷。

### 空間複雜度

**O(n)**，主要來自：

- 最多 `capacity` 個 Node。
- Hash Table 的 Bucket Pointer Array。

---

## 九、這題遇到的主要困難

### 1. Pointer 與記憶體地址的關係

一開始容易混淆：

```c
current = current->next;
```

與：

```c
current->next = node;
```

前者只改變 `current` 這個 Pointer 的指向；後者才是修改 Node 內部的鏈結關係。

### 2. Doubly Linked List 的雙向維護

修改 `node->next`，不代表另一個 Node 的 `prev` 會自動更新。必須明確維護兩個方向。

### 3. Head / Middle / Tail 的邊界條件

在操作 `node->prev->next`、`node->next->prev` 之前，需要先確認 `prev` 與 `next` 是否可能為 `NULL`。

### 4. Hash Table 與 LRU List 的職責

`hash_next` 與 `next` 很容易混淆，但兩者屬於不同的鏈結關係，應該獨立處理。

### 5. 不必要的遍歷

一開始以為 Hash Table 插入需要找到 Collision Chain 的尾端。後來理解到，因為不需要維持順序，直接 Head Insertion 就能完成。

### 6. Node Recycling

Cache 已滿時，不一定需要釋放舊 Node 並重新配置。只要先解除舊的 Hash 關係，就可以更新 Tail Node 的內容並重複利用。

---

## 十、心得

這題花了我大約兩天才完成，過程去找了不熟悉的Hash table及Doubly linked list用法，是目前 LeetCode 練習中，讓我最有成就感的一題。

最困難的部分不是 C 語法，而是要在腦袋裡同時維護：

- Hash Table 的 Collision Chain。
- Doubly Linked List 的前後關係。
- Head / Tail 的位置。
- 每個 Node 的記憶體地址。
- `malloc()` 與 `free()` 的生命週期。

一開始，這些 Pointer 的關係其實很模糊。有時候知道自己想做什麼，卻沒辦法確定修改某個 Pointer 後，其他 Node 會變成什麼狀態。

所以我花了不少時間畫圖、追蹤地址，以及反覆確認邊界條件。

隨著每個函式逐漸完成，整個資料結構的圖像也慢慢變得清楚。到後來，我已經可以在腦袋裡想像 Node 如何從 Tail 移除、如何重新插入 Hash Bucket，以及如何移動到 Head。

**最讓我開心的，不只是最後看到 Accepted，而是那個原本模糊的系統模型，逐漸變得清晰的過程。**

這題讓我更深刻地理解：

> Pointer 不只是記憶體地址，也是建立資料之間關係的工具。

而當這些關係能夠被清楚理解時，原本複雜的程式，也開始變得有跡可循。
