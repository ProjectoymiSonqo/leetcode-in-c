# 題目
5. Longest Palindromic Substring

# 問題拆解

先固定左邊。

從最右邊開始尋找相同字元。

找到候選後，
利用 is_palindrome() 往中間縮。

如果成立，
更新目前最大長度。

最後才 malloc 回傳字串。

# Design Pattern（解題模式）

固定一端

↓

搜尋另一端

↓

抽成驗證函式

↓

更新最佳答案

# 核心 Function

bool is_palindrome()

# 我學到什麼

Candidate 不需要 malloc。

malloc 只需要在最後建立回傳字串。

Function 應該只做一件事。