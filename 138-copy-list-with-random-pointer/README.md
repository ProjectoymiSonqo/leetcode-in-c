# 138. Copy List with Random Pointer

這題卡滿久，主要難點不是複製 `next`，而是怎麼讓 copy 的 `random` 指向「對應的 copy node」，而不是原本的 node。

## 我的解法

### 1. 先複製 next

先走一次 original list，建立：

```text
A -> B -> C
```

對應：

```text
A' -> B' -> C'
```

同時用 `cnt` 記錄總共有幾個 node。

### 2. 建立 original / copy mapping

用一個 struct 記錄每個 node 的對應關係：

```c
struct Map {
    struct Node *original;
    struct Node *copy;
};
```

最後會得到：

```text
A <-> A'
B <-> B'
C <-> C'
```

### 3. 根據 mapping 複製 random

例如原本：

```text
A.random -> C
```

拿 `C` 的 address 去 map 裡找：

```text
C <-> C'
```

所以就可以設定：

```text
A'.random -> C'
```

核心判斷：

```c
if (map[j].original == current->random)
{
    copy_current->random = map[j].copy;
}
```

## 複雜度

前兩圈都是 `O(n)`。

第三圈每個 node 都要搜尋一次 map，所以是：

- **Time:** `O(n²)`
- **Space:** `O(n)`

不是最佳解，但這版是自己從 pointer 關係推導出來的，先留著。

## 這題學到的東西

- Node 是否相同要比較 **address**，不是 `val`
- `malloc()` 後記得初始化 `next` / `random`
- `p->next` 是誰的 `next`，取決於「現在 `p` 指向哪顆 node」
- 遇到不知道 original 和 copy 怎麼對應時，可以自己建立 mapping