# 8. String to Integer (atoi)

## 問題拆解

將字串轉換成 32-bit signed integer。

需要遵守以下規則：

1. 跳過開頭空白。
2. 判斷是否有正負號。
3. 讀取第一段連續數字。
4. 如果沒有合法數字，回傳 0。
5. 若超過 32-bit signed integer 範圍，回傳 INT_MAX 或 INT_MIN。

---

## 解題思路

將整個問題拆成四個階段。

### Step 1：Skip Space

先略過開頭所有空白字元。

```
"    -42"
     ↑
```

直到遇到第一個非空白字元。

---

### Step 2：Sign

判斷第一個字元是否為：

```
+
-
```

並記錄正負號。

---

### Step 3：Read Digit

持續讀取第一段連續數字。

利用：

```c
digit = s[i] - '0';
```

將字元轉成整數。

例如：

```
'7' - '0' = 7
```

每取得一位數就建立新的整數：

```c
x = x * 10 + digit;
```

直到遇到不是數字的字元停止。

---

### Step 4：Overflow Protection

建立目前允許的最大絕對值：

正數：

```
INT_MAX
```

負數：

```
2147483648
```

也就是：

```c
limit = -(long long)INT_MIN;
```

每加入一位數之前先檢查：

```c
if (x > limit / 10 ||
    (x == limit / 10 && digit > limit % 10))
```

如果超過範圍：

正數回傳

```
INT_MAX
```

負數回傳

```
INT_MIN
```

避免在乘 10 時產生 Overflow。

---

## 核心機制

Parser + Digit Builder

整個流程其實是一個 Parser：

```
Skip Space
        ↓
Sign
        ↓
Read Digit
        ↓
Overflow Check
        ↓
Finish
```

---

## Design Pattern

State Machine

每個階段都有不同的狀態。

```
Start

↓

Skip Space

↓

Sign

↓

Read Digit

↓

End
```

同時利用：

```
digit = s[i] - '0'
```

以及：

```
x = x * 10 + digit
```

逐步建立整數。

---

## Trade-off

第一版先建立完整 Parser。

之後再加入 Overflow 保護。

Overflow 的判斷不是等所有數字讀完，而是在每加入一位數之前確認是否仍然安全。

---

## 我學到什麼

一開始以為使用 long long 就能避免 Overflow。

實際測試後發現：

LeetCode 仍然會提供超過 long long 範圍的輸入。

因此必須在每次：

```
x = x * 10 + digit
```

之前先檢查是否會超過限制。

另外，32-bit signed integer 的上下界並不是對稱的：

```
INT_MAX =  2147483647
INT_MIN = -2147483648
```

因此負數允許的絕對值比正數多 1，需要依照正負號決定 Overflow 上限。