# 6. Zigzag Conversion

## 問題拆解

將輸入字串搭配一組相同長度的 `index` 陣列。

`index[i]` 用來記錄 `s[i]` 應該被放在哪一個 row。

例如 `numRows = 5`：

```text
char :  P A Y P A L I S H I R I N G
row  :  1 2 3 4 5 4 3 2 1 2 3 4 5 4

## DesignPattern

State Machine / 狀態機

狀態包含：

* 目前 row
* 目前方向

狀態轉換條件：

* 到達最下面 → 往上
* 到達最上面 → 往下

我學到什麼

一開始看起來像是要真的畫出 Zigzag 圖形，
但實際上只需要記錄每一個字元屬於哪一個 row。

不需要為每個 row 建立不同長度的字串，
只需要一個和輸入字串相同長度的 index array。

最後依 row 分組輸出即可。

## 設計筆記

```text

想建立每個 row 的動態陣列

        ↓

想到 struct { char, row }

        ↓

發現 char 本來就在 s 裡

        ↓

只需要 index[]

        ↓

方向狀態機產生 row mapping

        ↓

兩層 for 分組輸出