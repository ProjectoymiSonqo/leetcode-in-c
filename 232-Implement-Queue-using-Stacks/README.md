# LeetCode 232 — Implement Queue using Stacks

## 🐍 貪食蛇物流中心：用兩個 Stack 實作 Queue

**Language:** C  
**Difficulty:** Easy  
**Status:** Accepted — 23 / 23 Testcases  
**Runtime:** 0 ms — Beats 100.00%  
**Memory:** 8.87 MB — Beats 97.34%

---

## 1. 題目概念

這道題要求使用兩個 Stack，實作具有 FIFO（First In, First Out）行為的 Queue。

兩種資料結構的差異：

| Data Structure | Principle | Output Order |
|---|---|---|
| Stack | LIFO — Last In, First Out | 5, 4, 3, 2, 1 |
| Queue | FIFO — First In, First Out | 1, 2, 3, 4, 5 |

假設依序輸入：

```text
1 → 2 → 3 → 4 → 5
```

Stack 會優先取出 `5`，但 Queue 必須優先取出 `1`。

問題是：**如何使用兩個只能從頂端 Push、Pop 的 Stack，實現 FIFO？**

---

## 2. 第一個想法：兩次反轉

如果把 Stack1 的資料依序 Pop，再 Push 到 Stack2，就會反轉資料順序。

```text
Stack1          Stack2

5 ← top         1 ← top
4               2
3       →       3
2               4
1               5
```

這樣 Stack2 的頂端就會是最早加入的資料。

但是，如果 Stack2 還有資料，這時又有新的資料進來，該怎麼辦？

最初的想法是：

1. 把 Stack2 的資料全部搬回 Stack1。
2. 把新資料 Push 到 Stack1。
3. 再把 Stack1 全部搬到 Stack2。

這個方法能維持 FIFO，但每次加入資料都可能需要大量搬運。

**功能正確，不代表效率理想。**

---

## 3. 第二個想法：反轉 top 的方向

既然搬運資料只是為了反轉讀取順序，那能不能直接改變 `top` 的移動方向？

例如：

```c
return stack[head++];
```

這個想法可以避免資料搬運，但已經不是標準 Stack 的 Pop 行為。

LeetCode 232 要求只能使用 Stack 的標準操作：

- Push
- Pop
- Peek
- isEmpty

不能直接從 Stack 底部取出元素。

因此，必須在遵守 LIFO 的前提下設計 Queue。

---

## 4. 最終思路：Lazy Transfer（延遲搬運）

真正的突破是發現：

**Stack2 如果還有舊資料等待輸出，就不需要急著把 Stack1 的新資料搬過去。**

於是將兩個 Stack 分工：

- **Stack1：** 接收新加入的資料。
- **Stack2：** 提供 Queue 的輸出資料。

搬運只會在 Stack2 為空、而且需要 Pop 或 Peek 時發生。

### 範例

先 Push 1、2、3：

```text
Stack1          Stack2

3 ← top         Empty
2
1
```

第一次 Pop 或 Peek 時，將 Stack1 全部搬到 Stack2：

```text
Stack1          Stack2

Empty           1 ← top
                2
                3
```

接著 Push 4、5：

```text
Stack1          Stack2

5 ← top         1 ← top
4               2
                3
```

此時 Stack2 仍有較早加入的資料，因此不需要搬運。

等 Stack2 全部 Pop 完：

```text
Stack1          Stack2

5 ← top         Empty
4
```

下一次 Pop 或 Peek 再搬運：

```text
Stack1          Stack2

Empty           4 ← top
                5
```

最後輸出順序：

```text
1 → 2 → 3 → 4 → 5
```

成功實現 FIFO。

---

## 5. C Implementation

```c
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int stack1[100];
    int top1;

    int stack2[100];
    int top2;
} MyQueue;

MyQueue* myQueueCreate(void)
{
    MyQueue *queue = malloc(sizeof(MyQueue));

    if (queue == NULL)
        return NULL;

    queue->top1 = -1;
    queue->top2 = -1;

    return queue;
}

void myQueuePush(MyQueue* obj, int x)
{
    // Enqueue
    if (x > 9 || x < 1)
        return;

    obj->top1++;
    obj->stack1[obj->top1] = x;
}

int myQueuePop(MyQueue* obj)
{
    // Dequeue
    if (obj->top2 == -1)
    {
        if (obj->top1 != -1)
        {
            while (obj->top1 != -1)
            {
                obj->top2++;
                obj->stack2[obj->top2] =
                    obj->stack1[obj->top1];
                obj->top1--;
            }
        }
        else
            return -1;
    }

    return obj->stack2[obj->top2--];
}

int myQueuePeek(MyQueue* obj)
{
    // Front
    if (obj->top2 != -1)
        return obj->stack2[obj->top2];

    if (obj->top1 != -1)
    {
        while (obj->top1 != -1)
        {
            obj->top2++;
            obj->stack2[obj->top2] =
                obj->stack1[obj->top1];
            obj->top1--;
        }
    }
    else
        return -1;

    return obj->stack2[obj->top2];
}

bool myQueueEmpty(MyQueue* obj)
{
    return obj->top1 == -1 &&
           obj->top2 == -1;
}

void myQueueFree(MyQueue* obj)
{
    free(obj);
}
```

這是通過測試的版本。使用固定容量 100，依照題目限制配置；若作為通用 Queue，應加入容量檢查或動態擴充。

---

## 6. Debugging：Restrictions Failed

第一次提交時，程式雖然能產生正確結果，卻被 LeetCode 拒絕。

原因是使用了額外的 `flag` 索引，直接讀取 Stack1 的內部元素：

```c
int flag = obj->top1;

while (flag != -1)
{
    obj->stack2[++obj->top2] =
        obj->stack1[flag];

    flag--;
}

obj->top1 = -1;
```

這個方法可以得到正確的 Queue 順序，但不符合題目要求的 Stack 操作限制。

因為它直接存取陣列內部元素，而不是逐一從 Stack 頂端 Pop。

後來改成：

```c
while (obj->top1 != -1)
{
    obj->stack2[++obj->top2] =
        obj->stack1[obj->top1--];
}
```

現在每次都從 Stack1 的頂端取出資料，再放入 Stack2 的頂端。

同時也不再需要額外的 `flag`。

這次學到：

**演算法輸出正確，不代表實作方式符合所有規格。**

---

## 7. Complexity Analysis

令 n 為 Queue 中的元素數量。

| Operation | Time Complexity |
|---|---|
| Push | O(1) |
| Pop | Amortized O(1) |
| Peek | Amortized O(1) |
| Empty | O(1) |
| Space | O(n) |

### 為什麼 Pop 和 Peek 是均攤 O(1)？

某次 Pop 可能需要將 Stack1 的所有元素搬到 Stack2，因此單次最差為 O(n)。

但每個元素在整個生命週期中：

1. Push 到 Stack1 一次。
2. 從 Stack1 Pop 一次。
3. Push 到 Stack2 一次。
4. 從 Stack2 Pop 一次。

每個元素只會被轉移一次，不會反覆搬回 Stack1。

因此對一連串操作而言，每個元素所需的 Stack 操作次數是常數級，得到 **Amortized O(1)**。

---

## 8. Learning Notes

這次最有趣的地方，是演算法並非一開始就完整出現。

整個推導過程：

```text
理解 LIFO / FIFO
       ↓
發現兩個 Stack 可以反轉順序
       ↓
嘗試反覆搬運
       ↓
思考能不能反轉 top
       ↓
發現不需要每次同步資料
       ↓
Stack2 清空時才搬運
       ↓
完成 Lazy Transfer
       ↓
修正 Stack 操作限制
       ↓
Accepted
```

和 LeetCode 20 不同，這次沒有發明另一套非標準演算法，而是從問題本身一步一步推導出有效率的雙 Stack 解法。

其中最重要的觀察：

**不需要立即處理的資料，就不必立即搬運。**

這不只是 Queue 的技巧，也是一種減少不必要工作的設計思路。

### 最後心得

昨天學 Stack，今天用兩個 Stack 實作 Queue。

從理解資料結構的基本規則，到利用兩個 LIFO 容器組合出 FIFO 行為，開始感受到抽象資料結構真正有趣的地方。

而且這次的關鍵想法，居然是在玩聯盟戰棋的時候冒出來的。

聯盟戰棋第二名，LeetCode 232 Accepted。

**被遊戲耽誤的 LeetCode 大師，今天也順利交差了。** 🐍

---

## Next Challenge

**LeetCode 225 — Implement Stack using Queues**

這次反過來：

用 FIFO 的 Queue，實作 LIFO 的 Stack。

貪食蛇物流中心，準備迎接下一項業務。