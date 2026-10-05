# LeetCode 21 - Merge Two Sorted Lists

## 題目

給定兩個已排序的 Linked List `list1` 與 `list2`，
將兩條 Linked List 合併成一條排序後的 Linked List，並回傳合併後的 `head`。

例如：

```text
list1:
1 -> 2 -> 4 -> NULL

list2:
1 -> 3 -> 4 -> NULL

Output:
1 -> 1 -> 2 -> 3 -> 4 -> 4 -> NULL
```

---

## 解題思路

因為 `list1` 和 `list2` 本身都已經排序完成，
所以可以同時使用兩個 pointer 從頭開始比較：

```c
list1->val
list2->val
```

每次選擇較小的 node，接到 result Linked List 的尾端，
再讓被選中的 list pointer 往下一個 node 移動。

---

## Head & Current

這題使用兩個 pointer：

```c
struct ListNode *result_head = NULL;
struct ListNode *current = NULL;
```

- `result_head`：保存結果 Linked List 的起點，不移動
- `current`：負責往後移動、接上新的 node

第一個 node：

```c
if (result_head == NULL)
{
    result_head = list1;
    current = list1;
    list1 = list1->next;
}
```

第二個 node 之後：

```c
current->next = list1;
current = list1;
list1 = list1->next;
```

### 接 → 走 → 推

```text
current->next = list1;   // 接：修改舊尾巴的 next
current = list1;         // 走：current 移到新尾巴
list1 = list1->next;     // 推：list1 移到下一個候選 node
```

其中：

```c
current = list1;
```

是移動 pointer 本身。

```c
current->next = list1;
```

則是修改 `current` 所指 node 的 `next`。

---

## 最後處理

當其中一條 list 走到 `NULL` 後，
不需要再一個一個 node 處理。

直接把還沒走完的那一串接上：

```c
current->next = list1;
```

因為剩下的 Linked List 本身就已經串好了。

如果一開始其中一條 list 就是 `NULL`，
則不需要建立新的 head，直接回傳另外一條 list。

---

## Complexity

- Time: `O(m + n)`
- Space: `O(1)`

## Key Point

> `current = list1` 是移動 pointer  
> `current->next = list1` 是修改 Linked List 的連線

Merge 的操作順序：

> **接 → 走 → 推**