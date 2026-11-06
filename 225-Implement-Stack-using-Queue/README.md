# LeetCode 225 — Implement Stack using Queues

## 🥟 貪食蛇物流中心，開始經營饅頭店

**Language:** C  
**Difficulty:** Easy  
**Status:** Accepted — 22 / 22 Testcases  
**Runtime:** 0 ms — Beats 100.00%  
**Memory:** 8.88 MB — Beats 97.33%  
**Date:** 2026-11-06

---

## 1. Problem Description

Implement a Stack using only the standard operations of a Queue.

A Queue follows **FIFO (First In, First Out)**, while a Stack follows **LIFO (Last In, First Out)**.

For example:

```text
Push(2)
Push(3)
Push(4)
Push(5)
```

A normal Queue returns:

```text
2 → 3 → 4 → 5
```

But a Stack must return:

```text
5 → 4 → 3 → 2
```

The challenge is to implement LIFO behavior without violating the FIFO rules of the underlying Queue.

---

## 2. First Idea — Circular Linked List

最初想到的是使用 Circular Linked List。

假設：

```text
head              tail
 ↓                 ↓
 2 → 3 → 4 → 5 ───┐
 ↑                │
 └────────────────┘
```

如果直接讓 `head` 指向 `tail`，似乎就能取出最後加入的元素。

但這個方法違反了題目的操作限制。

因為 Queue 只能透過標準的 Enqueue、Dequeue、Front 等操作存取資料，不能直接修改讀取位置來取得尾端元素。

**Circular Linked List 是合法的儲存結構，但任意移動 head 並不是合法的 Queue 操作。**

---

## 3. Discovery — Queue Rotation

後來發現，可以利用 Queue 本身的 FIFO 操作來改變資料順序。

基本概念：

```c
int x = dequeue(queue);
enqueue(queue, x);
```

例如：

```text
Initial:
2 → 3 → 4 → 5

Rotation 1:
3 → 4 → 5 → 2

Rotation 2:
4 → 5 → 2 → 3

Rotation 3:
5 → 2 → 3 → 4
```

經過 `count - 1` 次輪轉後，最後加入的元素就會來到 Queue Front。

這時執行 Dequeue，就能取得 Stack Top。

因此：

**只需要一個 Queue，就可以實作 Stack。**

---

## 4. Second Idea — Rotated State Flag

為了避免每次 `Top()` 都重複輪轉，曾考慮在結構中加入：

```c
bool rotated;
```

假設：

```text
Initial:
2 → 3 → 4 → 5

Top():
5 → 2 → 3 → 4

rotated = true
```

此時再次呼叫 `Top()`，可以直接讀取 Front。

但問題出現在 `Push()`。

```text
Queue:
5 → 2 → 3 → 4

Push(6):
5 → 2 → 3 → 4 → 6
```

雖然 `rotated == true`，但 Front 已經不是最新加入的元素。

下一次 `Pop()` 若直接取出 Front，就會錯誤地得到 `5`，而不是 `6`。

這代表 `rotated` 不能單純表示 Queue 已經輪轉過，還必須考慮後續操作是否破壞了原本的排列條件。

**狀態曾經成立，不代表它在下一次操作後仍然成立。**

---

## 5. Third Idea — Two Queues

為了解決 `Top → Push` 的狀態衝突，曾考慮加入第二條 Queue。

例如：

```text
Queue1:
5 → 2 → 3 → 4

Push(6), Push(7)

Queue2:
6 → 7
```

當需要 Top 時，將 Queue2 輪轉：

```text
Queue2:
7 → 6
```

但如果接著又 Push(8)：

```text
Queue2:
7 → 6 → 8
```

問題再次出現。

兩條 Queue 都可能處於已輪轉狀態，新增資料又會破壞 Front 與 Stack Top 的對應關係。

因此第二條 Queue 並沒有真正消除問題，只是把狀態管理分散到不同容器。

這次決定放棄額外的狀態管理，回到更簡單的設計。

---

## 6. Final Design — One Queue, No Persistent Rotation State

最終採用 **Singly Linked List + Queue Rotation**。

```c
struct Node {
    int val;
    struct Node *next;
};

typedef struct {
    int count;
    struct Node *head;
    struct Node *tail;
} MyStack;
```

其中：

- `head`：Queue Front，負責 Dequeue。
- `tail`：Queue Rear，負責 Enqueue。
- `count`：記錄元素數量，決定輪轉次數。

不需要 Circular Linked List，也不需要 `rotated` flag。

### Push — O(1)

從 Queue Tail 加入新節點。

```text
Before:
2 → 3 → 4

Push(5):

After:
2 → 3 → 4 → 5
```

因為已經保存 `tail` 指標，所以不需要遍歷 Linked List。

### Pop — O(n)

先輪轉 `count - 1` 次，再移除 Front。

```text
Initial:
2 → 3 → 4 → 5

Rotate:
5 → 2 → 3 → 4

Pop() = 5

Remaining:
2 → 3 → 4
```

### Top — O(n)

先輪轉 `count - 1` 次，讀取 Front，最後再輪轉一次，恢復原本排列。

```text
Initial:
2 → 3 → 4 → 5

Rotate 3 times:
5 → 2 → 3 → 4

Top() = 5

Rotate 1 more time:
2 → 3 → 4 → 5
```

這是整個設計最重要的決定：

**Top 可以暫時改變 Queue 的排列，但必須在返回前恢復原狀。**

如此一來，每次呼叫 `Push()`、`Pop()`、`Top()`，都不需要知道之前是否發生過輪轉。

---

## 7. Implementation Details

### Node Rotation

使用 Linked List 時，可以直接將 Front 節點移到 Rear，而不需要重新配置記憶體。

```c
void dequeue_enqueue(MyStack* obj)
{
    struct Node *head = obj->head;

    obj->head = obj->head->next;
    obj->tail->next = head;
    obj->tail = head;
    obj->tail->next = NULL;
}
```

這個函式只在 Queue 至少包含兩個節點時呼叫。

每次輪轉的時間複雜度為 O(1)。

### Memory Management

這次實作也特別處理了 Linked List 的生命週期。

當 Pop 最後一個節點時：

```c
free(current);
obj->head = NULL;
obj->tail = NULL;
obj->count--;
```

當 Pop 一般節點時，也必須同步更新 `count`。

在 `myStackFree()` 中，逐一釋放剩餘節點，最後釋放 `MyStack` 本身。

這些處理能確保資料結構的狀態與實際記憶體配置一致。

---

## 8. Complexity Analysis

令 n 為 Stack 中的元素數量。

| Operation | Time Complexity |
|---|---|
| Push | O(1) |
| Pop | O(n) |
| Top | O(n) |
| Empty | O(1) |
| Free | O(n) |

**Space Complexity: O(n)**

每個元素使用一個 Linked List Node 儲存。

雖然 `Pop()` 與 `Top()` 需要線性時間，但符合題目要求。

LeetCode 顯示 Runtime 0 ms，代表這次測試的計時結果，並不表示演算法是 O(1)。

---

## 9. Learning Notes

這次的解題過程：

```text
Circular Linked List
        ↓
發現直接移動 head 違反 Queue 規則
        ↓
Queue Rotation
        ↓
想用 rotated flag 減少重複輪轉
        ↓
發現 Top → Push → Pop 會破壞狀態
        ↓
嘗試使用 Two Queues
        ↓
發現 Top → Push → Top → Push 仍然有問題
        ↓
重新檢視狀態管理的必要性
        ↓
決定讓 Top 恢復 Queue 原始排列
        ↓
One Queue + Singly Linked List
        ↓
修正 count、head、tail 與記憶體釋放
        ↓
Accepted — 22 / 22
```

### What I Learned

**1. Data Structure vs. Abstract Data Type**

Linked List 是資料的實作方式；Queue 和 Stack 是資料的操作規則。

不能因為底層使用 Linked List，就任意繞過 Queue 的操作限制。

**2. State Consistency**

`rotated` flag 雖然可以記錄狀態，但必須確保每一個操作都能維護該狀態的意義。

只要有一個操作破壞了假設，整個系統就可能產生錯誤。

**3. Invariant**

最終選擇維持一個簡單的不變條件：

在公開操作完成後，Queue 都保持原本的 FIFO 排列，而不保留尚未完成的輪轉狀態。

這讓各個函式之間的依賴關係更簡單。

**4. Simplicity Over Premature Optimization**

為了避免 O(n) 的輪轉成本，曾嘗試加入狀態變數與第二條 Queue。

但這些設計增加了複雜度，卻沒有真正解決問題。

最後選擇較容易驗證、狀態清楚的 O(n) 方法。

---

## 10. Final Thoughts

今天早上完成 LeetCode 232，用兩個 Stack 實作 Queue。

今天晚上完成 LeetCode 225，用一個 Queue 實作 Stack。

這兩題最大的差別，不只是 FIFO 與 LIFO 的方向相反。

在第 232 題，兩個 Stack 之間的資料轉移自然產生順序反轉，因此可以採用 Lazy Transfer。

但在第 225 題，Queue 之間的資料轉移不會反轉順序，必須利用 Rotation 改變 Front 的位置。

而這次真正花時間的，不是 C 語言語法，而是反覆驗證自己建立的模型。

從嘗試節省操作，到發現狀態衝突，再回到簡單可靠的設計。

**有時候，放棄一個看似聰明的優化，也是一種工程判斷。**

LeetCode Accepted #49.

貪食蛇物流中心，終於成功開設饅頭店。🥟🐍