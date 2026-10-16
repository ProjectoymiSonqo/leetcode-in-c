# 19. Remove Nth Node From End of List

**Difficulty:** Medium  
**Language:** C  
**Result:** Accepted (208/208)

## 解題思路

題目要求刪除 Linked List 倒數第 `n` 個節點。

我的做法是先遍歷一次 Linked List，計算節點總數，再將倒數位置轉換成正向位置。

接著透過 Pointer 保存要刪除節點的前後位置，重新連接 Linked List。

### 1. 計算要刪除的節點位置

先遍歷整條 Linked List：

```c
int cnt = 1;

while (current != NULL)
{
    current = current->next;
    cnt++;
}

delete_node = cnt - n;
```

這裡 `cnt` 從 1 開始，因此遍歷結束後：

```text
cnt = Linked List 長度 + 1
```

所以：

```c
delete_node = cnt - n;
```

例如：

```text
1 -> 2 -> 3 -> 4 -> 5 -> NULL

n = 2

cnt = 6
delete_node = 6 - 2 = 4
```

代表要刪除第 4 個節點。

### 2. 保存節點位置

重新遍歷 Linked List，記錄三個 Pointer：

```c
struct ListNode *previous_node = NULL;
struct ListNode *current_node = NULL;
struct ListNode *next_node = NULL;
```

分別代表：

- `previous_node`：要刪除節點的前一個節點。
- `current_node`：要刪除的節點。
- `next_node`：要刪除節點的下一個節點。

例如刪除節點 4：

```text
1 -> 2 -> 3 -> 4 -> 5 -> NULL
          ↑    ↑    ↑
       previous current next
```

遍歷時保存對應的 address：

```c
if (cnt == delete_node - 1)
{
    previous_node = current;
}
else if (cnt == delete_node)
{
    current_node = current;
}
else if (cnt == delete_node + 1)
{
    next_node = current;
}
```

### 3. 重新連接 Linked List

找到節點後，讓前一個節點直接指向下一個節點：

```c
previous_node->next = next_node;
```

原本：

```text
1 -> 2 -> 3 -> 4 -> 5 -> NULL
```

修改後：

```text
1 -> 2 -> 3 ------> 5 -> NULL
               4
```

最後釋放被移除的節點：

```c
free(current_node);
```

### 4. 特殊情況：刪除 Head

如果要刪除的是第一個節點，就不能使用 `previous_node`，因為它不存在。

因此需要另外處理：

```c
if (delete_node == 1)
{
    current = head->next;
    free(head);
    head = current;
}
```

這個方式也能處理只有一個節點的情況：

```text
1 -> NULL

刪除 Head

head = NULL
```

## Debug 紀錄

這次遇到的問題是在刪除唯一節點時，原本寫成：

```c
while (current != NULL)
{
    if (cnt == 2)
    {
        head = current;
    }

    current = current->next;
    cnt++;
}
```

原本以為 `cnt == 2` 時，`current` 已經指向 `NULL`，因此可以讓 `head = NULL`。

但忽略了 `while (current != NULL)` 的終止條件。

當 `current` 變成 `NULL` 時，迴圈就不會再執行，因此 `cnt == 2` 的判斷根本沒有機會成立。

**這次學到：變數最後的狀態，不代表程式一定有機會使用那個狀態。**

另外，移除節點時也要注意記憶體管理。

如果直接修改 Pointer，卻沒有保存被移除節點的 address，就可能失去釋放該節點的機會，造成 Memory Leak。

因此需要先保存節點，再重新連接，最後執行 `free()`。

## Complexity Analysis

假設 Linked List 有 `N` 個節點。

**Time Complexity：O(N)**

- 第一次遍歷：計算 Linked List 長度。
- 第二次遍歷：尋找要刪除的節點。
- 重新連接與釋放節點：O(1)。

總時間複雜度：

```text
O(N) + O(N) = O(N)
```

**Space Complexity：O(1)**

只使用固定數量的 Pointer 與計數變數，不需要額外配置與節點數量成比例的記憶體。

## 總結

這題延續前面 Linked List 的練習，使用 Pointer 完成節點刪除。

主要收穫：

1. 將倒數位置轉換成正向位置。
2. 使用 Pointer 保存節點之間的關係。
3. 處理刪除 Head 的特殊情況。
4. 理解 `while` 終止條件對程式執行流程的影響。
5. 刪除節點時，正確管理動態配置的記憶體。

這次使用兩次遍歷完成題目，沒有採用 Fast / Slow Pointer。

目前先以自己能理解、驗證且正確執行的解法為主。

**Accepted：208 / 208**