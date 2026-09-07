# 167. Two Sum II - Input Array Is Sorted

## 思路

一開始使用雙層迴圈搜尋所有組合，邏輯正確，但沒有利用陣列已排序的特性，因此大資料會 TLE。

改成 Two Pointers：

- `left` 從最小值開始
- `right` 從最大值開始
- 若 `numbers[left] + numbers[right] < target`
  - 代表目前最小值即使配最大值仍太小
  - 可以安全排除 `left`
  - `left++`
- 若總和 `> target`
  - 代表目前最大值即使配最小值仍太大
  - 可以安全排除 `right`
  - `right--`
- 相等時即找到答案

核心不是單純縮小範圍，而是利用 sorted 的單調性，證明被排除的端點不可能參與答案。

## 複雜度

- Time: `O(n)`
- Space: `O(1)`（不計題目要求回傳的陣列）