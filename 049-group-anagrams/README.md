# 49. Group Anagrams

## 思路

這題可以拆成三個階段：

1. 每個字串建立字母統計 signature
2. 比較 signature，替每個字串分配 group
3. 根據 group 建立 `char***` 回傳結果

---

## 1. 建立字串 Signature

因為題目只有小寫英文字母，可以替每個字串建立 26 格的統計表：

```c
int map[strsSize][26];
例如：
"eat" → a:1 e:1 t:1
"tea" → a:1 e:1 t:1
"tan" → a:1 n:1 t:1
只要兩個字串的 26 格完全相同，就代表它們互為 Anagram。

因此：
eat == tea == ate
tan == nat
bat

2. 分配 Group

建立：
int group[strsSize];
每處理一個新的字串 a，只搜尋它前面已經處理過的資料：
for (int b = 0; b < a; b++)
如果找到相同 signature：
group[a] = group[b];
found = true;
break;
代表直接沿用以前建立的 group。

如果把前面的資料全部找完仍然：
found == false
才建立新的 group：
group_index++;
group[a] = group_index;
strs  = [eat, tea, tan, ate, nat, bat]
group = [ 1,   1,   2,   1,   2,   3 ]

關鍵

group_index 代表「目前建立過多少種 group」，不是目前處理到第幾個字串。

而且：
if (!found)
必須放在搜尋用的 for (b...) 外面。

因為：

某一次比較不同，不代表前面的資料全部都不同。

只有搜尋完整個範圍後仍然沒找到，才能確定需要建立新 group。

3. 理解 char***

題目的 function：
char*** groupAnagrams(
    char** strs,
    int strsSize,
    int* returnSize,
    int** returnColumnSizes
)

char        → 'e'
char*       → "eat"
char**      → ["eat", "tea", "ate"]
char***     → [
                ["eat", "tea", "ate"],
                ["tan", "nat"],
                ["bat"]
              ]
也就是：
ans[0]        // char**：一個 group
ans[0][0]     // char* ：一個 string
ans[0][0][0]  // char  ：一個字元

4. returnSize

returnSize 用來告訴 LeetCode：

最後總共有幾個 group？

因為 group_index 已經記錄 group 數量：
*returnSize = group_index;
例如：
*returnSize = 3

5. returnColumnSizes

LeetCode 還需要知道每個 group 裡有幾個字串。
[
    ["eat","tea","ate"],  // 3
    ["tan","nat"],        // 2
    ["bat"]               // 1
]

所以：
*returnColumnSizes = [3, 2, 1]

先配置空間：
*returnColumnSizes =
    malloc(group_index * sizeof(int));

memset(
    *returnColumnSizes,
    0,
    group_index * sizeof(int)
);

6. 建立 char*** ans

先建立最外層，每一格代表一個 group：

char*** ans =
    malloc(group_index * sizeof(char**));

概念：
ans
 │
 ├── ans[0] → group 1
 ├── ans[1] → group 2
 └── ans[2] → group 3

再替每個 group 建立存放 char* 的空間：
for (int i = 0; i < group_index; i++) {
    ans[i] = malloc(strsSize * sizeof(char*));
}
目前先讓每組都配置最多 strsSize 個位置，寫法比較直觀，之後可以再改成精準配置。

7. 把字串放進對應 Group
for (int i = 0; i < strsSize; i++) {
    int g_id = group[i] - 1;

    int current_size =
        (*returnColumnSizes)[g_id];

    ans[g_id][current_size] = strs[i];

    (*returnColumnSizes)[g_id]++;
}
例如第一個：
eat → group 1
Array index 從 0 開始，因此：
g_id = 1 - 1 = 0
一開始：
returnColumnSizes = [0,0,0]
所以：
ans[0][0] = "eat";
然後：
returnColumnSizes = [1,0,0]
下一個 "tea" 也是 group 1：
ans[0][1] = "tea";
最後：
ans[0] → ["eat","tea","ate"]
ans[1] → ["tan","nat"]
ans[2] → ["bat"]

returnColumnSizes → [3,2,1]
returnSize        → 3
returnColumnSizes 在這裡同時有兩個用途：

1. 最後回傳每個 group 的大小
2. 分組過程中充當每個 group 自己的 write index

複雜度

建立所有 signature：
O(n² × 26) → O(n²)

空間主要包含：
map[n][26]
group[n]
ans
returnColumnSizes

這版不是最省空間或最快的 Hash Table 解法，但流程清楚：
String
  ↓
26 格 Signature
  ↓
Group ID
  ↓
char*** Output
這題除了 Anagram，本身也是一次很完整的 C 多層指標與動態記憶體練習。