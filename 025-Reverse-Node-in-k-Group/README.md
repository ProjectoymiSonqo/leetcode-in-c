# 25. Reverse Nodes in k-Group

## 題目

給定一個 Linked List，每 `k` 個 node 為一組進行反轉。

如果最後剩餘的 node 不足 `k` 個，則保持原本順序。

例如：

```text
Input:
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7
k = 3

Output:
3 -> 2 -> 1 -> 6 -> 5 -> 4 -> 7
```

## 我的解法

延續 Q92 的解法，使用 `struct ListNode **map` 暫存每組 node 的 address，再透過 map 反轉 Linked List。

### 1. 建立 map

```c
struct ListNode **map = malloc(k * sizeof(struct ListNode *));
```

一開始曾考慮使用 `struct ListNode ***map` 保存所有分組，但後來發現不需要。

每次只需要處理一組，完成後就能重複利用同一個 `map[k]`。

### 2. 收集 k 個 node

```c
int map_count = 0;

for (int i = 0; i < k && current != NULL; i++)
{
    map[i] = current;
    current = current->next;
    map_count++;
}
```

不需要預先掃描整條 Linked List 計算長度。

利用 `map_count` 判斷：

- `map_count == k`：完整的一組，可以反轉。
- `map_count < k`：不足一組，保留原本順序。

### 3. 反轉 Linked List

假設：

```text
map[0] = &1
map[1] = &2
map[2] = &3
```

透過：

```c
for (int i = k - 1; i > 0; i--)
{
    map[i]->next = map[i - 1];
}
```

得到：

```text
3 -> 2 -> 1
```

接著：

```c
map[0]->next = current;
```

將反轉後的尾巴接回尚未處理的 Linked List，避免形成 cycle。

### 4. 連接不同分組

使用：

```c
struct ListNode *previous_tail;
```

記錄上一組反轉後的尾巴。

第一組反轉時：

```c
head = map[k - 1];
previous_tail = map[0];
```

後續每一組：

```c
previous_tail->next = map[k - 1];
previous_tail = map[0];
```

例如：

```text
第一組：
3 -> 2 -> 1
          ↑
     previous_tail

第二組：
6 -> 5 -> 4

連接：
3 -> 2 -> 1 -> 6 -> 5 -> 4
                         ↑
                    previous_tail
```

最後不足 `k` 個的 node 不進行反轉，直接接回原本的 Linked List。

## 這題學到的事

### 1. 不一定需要保存所有資料

原本考慮使用三重指標保存所有分組，但實際上只需要一個 `map[k]` 作為暫存空間。

### 2. 修改 next 時，要注意原本的連線

反轉不只是建立：

```c
map[i]->next = map[i - 1];
```

還需要處理：

```c
map[0]->next = current;
```

否則可能形成 cycle。

### 3. current == NULL 不代表這組不完整

當最後一組剛好有 `k` 個 node 時，`current` 同樣會是 `NULL`。

因此必須使用 `map_count` 判斷是否需要反轉。

### 4. free(map) 不會刪除 Linked List

`map` 只是保存 node address 的暫存陣列。

```c
free(map);
```

只會釋放 address array，不會釋放原本的 Linked List nodes。

## Complexity

| | Complexity |
|---|---|
| Time | O(N) |
| Space | O(k) |

每個 node 只需要走訪與處理固定次數，時間複雜度為 **O(N)**。

額外配置 `k` 個 node pointer，因此空間複雜度為 **O(k)**。

## 後續挑戰

目前使用 `map[k]` 完成分組反轉。

未來可以嘗試不使用 map，直接透過 `prev`、`current`、`next` 操作 Linked List，將額外空間降低至 **O(1)**。