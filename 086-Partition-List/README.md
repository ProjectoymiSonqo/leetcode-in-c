# 86. Partition List

- **Difficulty:** Medium
- **Language:** C
- **Result:** Accepted（169 / 169 testcases）
- **Runtime:** 0 ms（本次測量）
- **Memory:** 11.12 MB

## 題目

給定 Linked List `head` 與整數 `x`，將所有 `< x` 的節點排在 `>= x` 的節點之前，**兩組都要維持原本的相對順序**。

```text
Input:  head = [1,4,3,2,5,2], x = 3
Output:        [1,2,2,4,3,5]
```

## 我的解法：雙 Map 分類法

最初想用 `newlist`、`newhead` 直接重新串接，但它們仍指向原本的節點；如果在遍歷途中修改 `next`，容易影響尚未處理的鏈結。

因此改成**先記錄節點位址，最後統一重組鏈結**。

1. **統計：**第一次遍歷，計算 `< x` 的節點數 `A`，以及 `>= x` 的節點數 `B`。
2. **配置：**使用 `malloc()` 建立 `map_A`、`map_B` 兩個節點指標陣列。
3. **分類：**第二次遍歷，依原始順序將節點位址放入對應 Map。
4. **重組：**先處理兩組的尾端與交界，再依 Map 順序重接各節點的 `next`。
5. **收尾：**更新 `head`，釋放兩個 Map（只釋放指標陣列，不釋放原始節點）。

```text
Original: 1 → 4 → 3 → 2 → 5 → 2

map_A:   [1, 2, 2]    (< 3)
map_B:   [4, 3, 5]    (>= 3)

Result:   1 → 2 → 2 → 4 → 3 → 5 → NULL
```

Map 儲存的是 `struct ListNode *`，因此不需要複製或重新配置節點。當 `A == 0` 時，原串列已符合條件，直接返回；當 `B == 0` 時，A 組尾端接上 `NULL`。

## 關鍵程式碼

```c
struct ListNode **map_A = malloc(A * sizeof(*map_A));
struct ListNode **map_B = malloc(B * sizeof(*map_B));

// 第二次遍歷時，依序保存原始節點位址
if (current->val < x)
    map_A[i++] = current;
else
    map_B[j++] = current;
```

重新串接：

```c
if (B != 0) {
    map_B[B - 1]->next = NULL;
    map_A[A - 1]->next = map_B[0];
} else {
    map_A[A - 1]->next = NULL;
}

for (int i = 0; i < A - 1; i++)
    map_A[i]->next = map_A[i + 1];

for (int j = 0; j < B - 1; j++)
    map_B[j]->next = map_B[j + 1];

head = map_A[0];
free(map_A);
free(map_B);
return head;
```

> 上述為核心片段；`A == 0` 的提前返回與記憶體釋放需在進入重組前處理。正式實作也應考慮 `malloc()` 失敗的情況。

## 複雜度

| 項目 | 複雜度 |
| --- | --- |
| 第一次遍歷：統計 | O(n) |
| 第二次遍歷：分類 | O(n) |
| 重新串接 | O(n) |
| **時間** | **O(n)** |
| **額外空間** | **O(n)** |

## 這題的收穫

這次最大的收穫，是發現**共用節點並不是問題，真正要小心的是修改 `next` 的時機**。

我選擇用 O(n) 的額外空間，換取更清楚的操作流程：先完成分類，再修改鏈結。這不是最省空間的方式，但讓我能獨立完成設計、測試與除錯。

另外也提醒自己：`malloc()` 配置的暫存 Map，所有返回路徑都要記得 `free()`。
