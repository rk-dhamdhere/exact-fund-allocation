# Exact Fund Allocation Checker — Subset Sum (Dynamic Programming)

A menu-driven C program that determines whether a subset of available fund
amounts can be combined to exactly match a target requirement, using the
**Dynamic Programming (Decision Problem)** approach to the classic
**Subset Sum** problem.

## Problem Statement

Given a set of fund amounts and a target fund requirement, determine whether
some subset of the available amounts sums **exactly** to the target. If
possible, report one such subset; otherwise report that no exact allocation
exists.

- **Module:** Dynamic Programming Approach
- **Category:** Decision-style DP (Sum of Subsets)

## Features

- Menu-driven interface — no need to restart the program between operations
- Validates number of items and amounts before computing
- Builds a `(n+1) × (target+1)` Boolean DP table
- Determines **possible / not possible** for the exact target
- **Backtracks** through the table to reconstruct one valid matching subset
- Displays the DP table itself when the input is small enough to fit on screen
- Handles edge cases: target = 0, duplicate amounts, target greater than the
  total available, and genuinely unachievable targets

## How the Algorithm Works

1. Let `dp[i][s]` = `1` if some subset of the first `i` items sums exactly to `s`, else `0`.
2. **Base case:** `dp[i][0] = 1` for every `i` — a sum of 0 is always achievable (take nothing).
3. **Recurrence:** for each item `i` and sum `s`:
   - **Exclude** item `i`: `dp[i][s] = dp[i-1][s]`
   - **Include** item `i` (only if `amount[i-1] <= s`): if `dp[i-1][s - amount[i-1]] == 1`, then `dp[i][s] = 1`
4. If `dp[n][target] == 1`, the exact allocation is possible.
5. **Backtracking:** starting from `dp[n][target]`, walk backward — if
   `dp[i-1][remaining] == 0`, item `i` must have been included; subtract its
   amount from `remaining` and continue until `remaining` reaches 0.

## Time Complexity

| Step | Complexity |
|---|---|
| Building the DP table | O(n × target) |
| Backtracking to find one subset | O(n) |
| **Overall** | **O(n × target)** |

This is *pseudo-polynomial* — efficient for reasonably sized inputs, but
grows with the magnitude of `target`, not just the number of items.

## DP vs. Backtracking: Comparison

Subset Sum can also be solved via **backtracking**, exploring a
state-space tree where each item is either included (`x_i = 1`) or
excluded (`x_i = 0`), with pruning applied whenever a branch can no
longer reach the target.

| | Dynamic Programming (this implementation) | Backtracking (state-space tree) |
|---|---|---|
| **Approach** | Builds a complete `(n+1) × (target+1)` table bottom-up | Explores an include/exclude decision tree top-down |
| **Time Complexity** | **O(n × target)** — always, regardless of input | **O(2ⁿ)** worst case — every item has 2 choices, with no guaranteed pruning |
| **Space Complexity** | O(n × target) for the table | O(n) for the recursion stack (no table stored) |
| **Guaranteed efficiency** | Yes — bounded by table size, predictable | No — depends entirely on how much pruning the bound condition achieves; a bad input can still approach O(2ⁿ) |
| **Finds all solutions?** | Finds **one** subset via backtracking through the table | Can be made to explore **all** valid subsets by not stopping at the first match |
| **Best suited for** | Larger `n` with a moderate target value | Small `n`, or when every valid subset (not just one) is needed |

**Why DP is generally preferred here:** since `target` in this problem is
capped at a reasonable size (`MAX_TARGET`), the DP table approach gives a
predictable O(n × target) runtime regardless of how the input is arranged.
Backtracking's O(2ⁿ) worst case only improves with *effective* pruning
(cutting a branch as soon as the partial sum exceeds the target or can no
longer reach it) — on poorly-prunable inputs (e.g. many small, similar-sized
items), it degrades toward checking almost every subset individually, which
DP avoids entirely by reusing overlapping subproblems instead of
recomputing them.

## Menu Options

```
1. Enter Fund Amounts
2. Display Fund Amounts
3. Check Exact Allocation
4. Exit
```

## Sample Run

```
Enter number of items: 5
Enter amount for item 1: 10
Enter amount for item 2: 10
Enter amount for item 3: 7
Enter amount for item 4: 3
Enter amount for item 5: 5

Fund amounts entered successfully.

Enter target fund amount: 10

Total of all available amounts: 35
Exact allocation is possible.
One matching subset:
Item    Amount
1       10

DP Table (1 = achievable, 0 = not achievable):
Items\Sum  0  1  2  3  4  5  6  7  8  9  10
0          1  0  0  0  0  0  0  0  0  0  0
1          1  0  0  0  0  0  0  0  0  0  1
2          1  0  0  0  0  0  0  0  0  0  1
3          1  0  0  0  0  0  0  1  0  0  1
4          1  0  0  1  0  0  0  1  0  0  1
5          1  0  0  1  0  1  0  1  1  0  1
```

## Edge Cases Tested

| Case | Input | Result |
|---|---|---|
| Normal case with duplicates | `[10,10,7,3,5]`, target `10` | Possible — `{10}` |
| Target = 0 | any set, target `0` | Possible — empty subset |
| Target > total available | `[5,10,15]` (total 30), target `100` | Not possible |
| Genuinely unachievable | `[3,5,9]`, target `4` | Not possible |

## Build & Run

```bash
gcc exact_fund_allocation.c -o exact_fund_allocation
./exact_fund_allocation
```

## Contributors

**Rishikesh Dhamdhere**
CMPN B - 25102B0075
Vidyalankar Institute of Technology (VIT)

**Harshad Patankar**
CMPN B - 25102B0080
Vidyalankar Institute of Technology (VIT)

**Shorang Singh**
CMPN B - 25102B0069
Vidyalankar Institute of Technology (VIT)

## Course

Analysis of Algorithms (AOA) — Mini Project