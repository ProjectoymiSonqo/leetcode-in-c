# 92. Reverse Linked List II

這題的目標是反轉 Linked List 中 `left` 到 `right` 之間的所有節點。

例如：

```text
Input:
head = [1,2,3,4,5]
left = 2
right = 4

Original:
1 -> 2 -> 3 -> 4 -> 5

Result:
1 -> 4 -> 3 -> 2 -> 5
```

## 我的思路

Singly Linked List 只有 `next`：

```c
struct ListNode {
    int val;
    struct ListNode *next;
};
```

所以我先把 `left ~ right` 之間的 node address 存進 `map`：

```c
struct ListNode **map =
    malloc(n * sizeof(struct ListNode *));
```

例如：

```text
map[0] = &node2
map[1] = &node3
map[2] = &node4
```

另外記住反轉區間前後的 node：

```c
priv_address   // left 前一個
next_address   // right 後一個
```

接著利用 `map` 的 index 倒著重新連接：

```c
for (int i = n - 1; i > 0; i--)
{
    map[i]->next = map[i - 1];
}

map[0]->next = next_address;
```

最後把前半段接回去：

```c
if (left == 1)
    head = map[n - 1];
else
    priv_address->next = map[n - 1];
```

## 這題學到的

`struct ListNode **map` 的意思是：

> `map` 指向的資料型別是 `struct ListNode *`

所以：

```text
map[i]      = node address
*map[i]     = node 本身
map[i]->val = node 的值
```

這次用額外的 address map 解決 singly linked list 無法往前走的問題。

- Time: `O(N)`
- Space: `O(right - left + 1)`

之後可以再挑戰不用 map，只靠 `prev / current / next` 做到 `O(1)` space。