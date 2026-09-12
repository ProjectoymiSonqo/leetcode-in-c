# 205. Isomorphic Strings

## 思路

判斷兩個字串的字符是否具有相同的 mapping pattern。

例如：

paper → 1 2 1 3 4  
title → 1 2 1 3 4

兩邊的 pattern 相同，因此為 isomorphic。

一開始想到建立一個 pool，遇到字符時搜尋它是否已經存在。
但如果每遇到一個字符都重新跑迴圈搜尋，會多出不必要的查找。

因此建立兩個 map，讓字符可以直接對應到自己的編號。

## 做法

`map_s` 與 `map_t` 初始值都是 0。

0 代表：

> 這個字符還沒有被 mapping 過。

掃描 `s` 與 `t`：

- 第一次遇到字符 → 給它一個新的 ID
- 已經遇過 → 使用原本的 ID
- 每個位置直接比較 `map_s[s[i]]` 與 `map_t[t[i]]`
- ID 不同 → pattern 不同，return false

例如：

paper

p → 1  
a → 2  
p → 1  
e → 3  
r → 4  

得到：

1 2 1 3 4

title 同樣得到：

1 2 1 3 4

因此 return true。

## Hash Map 的理解

這題讓我開始理解 mapping 的概念：

key → value

這裡可以把 character 當作 key，
mapping ID 當作 value。

重點不是「跑迴圈找 key 在哪裡」，
而是利用 key 直接找到對應的資料。

## Complexity

- Time: O(n)
- Space: O(1)