# 題目

4. Median of Two Sorted Arrays

# 問題拆解

不直接合併兩個 sorted array。

先根據總長度，確認中位數對應的 rank。

接著對兩個陣列建立切點，並觀察中間值的位置：

- 中間值是否剛好落在切點
- 中間值是否位於切點左側
- 中間值是否位於切點右側

根據目前切點可以推得的 rank 範圍，
逐步縮小中位數可能所在的位置。

每移動到下一個切點後，
重新確認目前已知的 rank 資訊。

一直到目標中位數的 rank 能被確定。

# 解題思路

核心不是比較 index，
而是比較「某個值在兩個 sorted array 合併後的 rank」。

每一次切點都會提供新的 rank 資訊，
再利用這些已知資訊縮小搜尋範圍。

# 核心資料

- TargetRank
- left_rank
- right_rank
- cut / overlap
- 已知 rank 範圍

# 目前狀態

尚未完成。

目前已經建立 rank-based 的搜尋架構，
但 `solve_overlap()` 在處理不同 overlap 數量時仍然過於複雜。

先暫停，
等 Binary Search / Partition 類型題目更熟悉後再回來重構。

# 我學到什麼

知道問題的機制，
不代表已經能把它整理成簡潔的程式。

複雜問題如果拆到最後，
最小子問題本身仍然很複雜，
代表抽象層次可能還需要重新調整。