# 290. Word Pattern

## 思路

一開始把這題看成與 #205 Isomorphic Strings 類似的問題。

例如：

pattern = "abba"

可以 mapping 成：

1 2 2 1

而：

s = "dog cat cat dog"

也可以 mapping 成：

1 2 2 1

只要兩邊得到的 pattern 相同，就符合 Word Pattern。

---

## 1. 切割 s

`s` 是一整串 C string：

dog cat cat dog

先利用空格與 `'\0'` 判斷每個 word 的邊界。

每找到一個 word，就使用 `malloc()` 配置空間，
將字元複製到 `word[]`，並在最後補上 `'\0'`。

得到：

word[0] = "dog"
word[1] = "cat"
word[2] = "cat"
word[3] = "dog"

另外先確認 word 數量與 pattern 長度相同，
不同則直接為 false。

---

## 2. 將 word mapping 成 ID

逐一處理每個 word。

外層迴圈負責決定目前 word 的 ID，
內層迴圈往前尋找是否出現過相同的 word。

使用 `strcmp()` 比較：

- 找到相同 word → 沿用之前的 ID
- 沒找到 → 建立新的 ID

例如：

dog → 1
cat → 2
cat → 2
dog → 1

因此：

map_s = 1 2 2 1

---

## 3. Mapping pattern

pattern 只有小寫英文字母，
因此使用 `map_pattern[26]` 直接以字母作為 index。

abba

a → 1
b → 2
b → 2
a → 1

得到：

1 2 2 1

逐一與 `map_s` 比較，
只要其中一個位置不同就 return false。

全部相同則 return true。

---

## Complexity

假設有 n 個 word，word 最大長度為 L：

- Time: O(n² × L)
- Space: O(n × L)

目前使用線性 `strcmp()` 尋找重複 word，
並使用 `malloc()` 複製每個 word。

這不是最精簡的解法，但完整走過：

切字串 → malloc → strcmp → mapping → pattern compare

之後可以再嘗試 Hash Table 或 start + len 的 zero-copy 做法。