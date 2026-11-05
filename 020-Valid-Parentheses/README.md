# LeetCode 20 — Valid Parentheses
## 饅頭小插曲：從括號消消樂到 Stack

**Language:** C  
**Difficulty:** Easy  
**Status:** Accepted — 103 / 103 Testcases  
**Final Runtime:** 1 ms  
**Final Memory:** 9.94 MB

---

## 1. 起點：這真的是 Easy 嗎？

原本準備挑戰 LeetCode 232 — Implement Queue using Stacks，卻發現自己對 Stack 的概念還不夠清晰。

於是決定先從第 20 題 Valid Parentheses 開始。

一開始並沒有想到 Stack，而是先觀察合法括號的結構。

我把合法情況分成兩種：

**沒包餡：**

```text
(){}()[][][][]
```

**有包餡：**

```text
({}[][{}][(({}))])
```

仔細觀察後發現，無論外層有多少組括號，最內層一定能找到最簡單的合法配對。

於是產生一個想法：

**如果每次都把相鄰且合法的括號消除，最後能消到空字串，不就代表整個字串合法嗎？**

括號消消樂就這樣誕生了。

---

## 2. 第一版：括號消消樂

### 核心規則

合法配對只有三種：

```text
()
[]
{}
```

每找到一組就消除，重複執行直到：

- 所有括號消失：合法。
- 再也找不到可消除的配對：不合法。

### 為什麼使用標記陣列？

C 語言不像某些高階語言，可以方便地從字串中移除任意字元。

因此建立一個與字串等長的 `mark[]`：

```c
int mark[char_size];
```

定義：

```text
1 = 字元仍然有效
0 = 字元已經被消除
```

例如：

```text
字串： ( [ ] )
索引： 0 1 2 3
mark： 1 1 1 1
```

消除 `[]` 後：

```text
字串： ( [ ] )
mark： 1 0 0 1
```

此時有效字元就是 `()`，下一輪可以繼續消除。

### 實作時遇到的問題

**問題一：錯誤假設 ASCII 規律**

最初使用：

```c
s[i] == s[i+1] - 1
```

判斷配對。

但 ASCII 中，`()` 的編碼相差 1，`[]` 和 `{}` 則相差 2。

後來改為獨立的配對函式：

```c
bool check_continuous(char val1, char val2)
{
    if (val1 == '(' && val2 == ')')
        return true;
    else if (val1 == '[' && val2 == ']')
        return true;
    else if (val1 == '{' && val2 == '}')
        return true;

    return false;
}
```

**問題二：錯誤假設已消除的字元可以每次跳兩格**

最初使用：

```c
j = j + 2;
```

但「每次消除兩個字元」不代表掃描位置之後的無效區段可以直接按兩格跳過。

因此改成逐一尋找下一個有效字元。

**問題三：避免已消除字元再次參與配對**

除了比較括號種類，也必須檢查 `mark[]` 的有效狀態。

**問題四：陣列越界**

在 C 語言中：

```c
while (j < char_size && mark[j] == 0)
```

必須先確認索引有效，再存取陣列。

### 結果

**Accepted — 103 / 103**

- Runtime：534 ms
- Memory：8.55 MB
- 時間複雜度：最差 O(n²)
- 額外空間複雜度：O(n)

雖然速度不快，但成功證明了自己的模型可行。

---

## 3. 第二版：饅頭稽查員

為了理解 Stack，重新建立了一個故事模型。

今天我是饅頭稽查員，負責檢查工廠製作的饅頭。

| 括號 | 饅頭種類 |
|---|---|
| `()` | 白饅頭 |
| `[]` | 牛奶饅頭 |
| `{}` | 黑糖饅頭 |

饅頭裡可以包其他完整的饅頭，但不能有不正確的封口或多餘的外皮。

例如：

```text
{[][]{}}
```

這是一顆合格的千層黑糖饅頭，裡面包著兩顆牛奶饅頭和一顆黑糖饅頭。

但：

```text
{[]]}
```

牛奶饅頭已經封口，卻又出現一片多餘的 `]`。

**不合格！**

### Stack 與饅頭的關係

製作千層饅頭時，最後開始製作的內層饅頭，必須最先封口。

這正是 Stack 的 LIFO（Last In, First Out）特性。

因此只需要記錄尚未封口的饅頭。

- 遇到左括號：Push。
- 遇到右括號：Peek，檢查最上層的饅頭。
- 配對成功：Pop。
- 配對失敗：直接判定不合格。
- 全部掃描完後 Stack 非空：仍有饅頭尚未封口，不合格。

---

## 4. 自己實作 Stack

沒有直接把 Stack 操作寫進 `isValid()`，而是先建立獨立的資料結構。

```c
typedef struct {
    char stack[10000];
    int top;
} Mystack;
```

定義 `top` 為最上層有效元素的索引。

```text
top = -1：Stack 為空
top =  0：Stack 有一個元素
top =  1：Stack 有兩個元素
```

接著封裝三個核心操作。

**Push — 放入元素**

```c
bool stack_push(Mystack *obj, char val)
{
    if (obj->top >= 9999)
        return false;

    if (val == '[' || val == '(' || val == '{')
    {
        obj->top++;
        obj->stack[obj->top] = val;
        return true;
    }

    return false;
}
```

**Peek — 查看頂端元素**

```c
char stack_peek(Mystack *obj)
{
    if (obj->top == -1)
        return '\0';

    return obj->stack[obj->top];
}
```

**Pop — 移除頂端元素**

```c
bool stack_pop(Mystack *obj)
{
    if (obj->top == -1)
        return false;

    obj->top--;
    return true;
}
```

這裡學到一個重要觀念：

**Pop 不一定需要真的清除陣列內容。**

只要改變 `top`，就能讓原本的資料在邏輯上失效。

這和消消樂的 `mark[]` 有相似之處，只是 Stack 使用索引維護有效範圍，不需要逐一標記每個元素。

---

## 5. Stack 版本的完整流程

```text
建立 Stack

從左到右掃描字串：

    遇到左括號：
        Push

    遇到右括號：
        如果 Stack 為空：
            不合法

        Peek 頂端元素

        如果括號不匹配：
            不合法

        如果匹配：
            Pop

掃描結束：

    Stack 為空：
        合法

    Stack 非空：
        不合法
```

因為每個字元只需要處理一次：

- 時間複雜度：O(n)
- 額外空間複雜度：O(n)

最終結果：

**Accepted — 103 / 103**

- Runtime：1 ms
- Memory：9.94 MB

從消消樂的 534 ms，進步到 Stack 版本的 1 ms。這是兩次提交的測量結果，實際倍率可能受執行環境影響。

---

## 6. 今天真正學到的事情

這道題原本只是為了補強 Stack 的觀念。

但整個過程意外變成：

**觀察規律 → 建立模型 → 設計演算法 → 驗證模型 → 學習資料結構 → 封裝工具 → 比較效能**

消消樂讓我理解問題本身。

Stack 則讓我理解，為什麼適當的資料結構可以減少重複工作。

我也發現，Stack 並不是某種必須依靠特定語法才能使用的東西。

它是一套由 Push、Pop、Peek 和 LIFO 規則組成的抽象資料結構，可以使用 Array 或 Linked List 自行實作。

而將這些操作封裝成工具，也讓 Stack 的概念變得更加具體。

### 最後的心得

原本只想學 Stack，結果先發明了括號消消樂。

明知道不是最有效率的解法，還是想把它實作出來，驗證自己的推論。

最後再回到 Stack，發現標準解法其實是在用更適合的資料結構，處理同一個問題。

**消消樂負責讓問題變得有趣，Stack 負責讓問題變得有效率。**

兩個版本都完成了它們的任務。

這是 LeetCode 20，一段意外有趣的饅頭小插曲。

---

## 7. 下一站

明天挑戰：

- **LeetCode 232 — Implement Queue using Stacks**
- **LeetCode 225 — Implement Stack using Queues**

今天正式收工。

饅頭稽查員下班！🥟