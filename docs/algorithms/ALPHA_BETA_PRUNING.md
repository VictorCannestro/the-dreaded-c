# Alpha-Beta Pruning Algorithm
## Overview
**Alpha-Beta Pruning** is an optimization technique for the minimax algorithm that significantly reduces the number of nodes evaluated in a game tree. It produces the same result as minimax but eliminates branches that cannot possibly influence the final decision.

The algorithm maintains two values:
- **α (alpha)**: The best value the maximizer can guarantee at that level or above
- **β (beta)**: The best value the minimizer can guarantee at that level or above

When β ≤ α, the remaining branches can be **pruned** (skipped) without affecting the result.

**Key Properties:**
1. **Correct**: Returns the same value as standard minimax
2. **Optimal pruning**: With perfect move ordering, examines O(b^(d/2)) nodes instead of O(b^d)
3. **No additional memory**: Only requires two extra parameters (α and β)

---

## Walkthrough: How Alpha-Beta Pruning Works
### The Big Picture
Imagine you're evaluating job offers. Company A offers \$100k. Now Company B says "we'll match whatever department X 
offers, and department X is considering either \$80k or \$90k." 

You can stop evaluating Company B immediately—even if department X offers \$90k, it's still less than Company A's 
\$100k. You've **pruned** the remaining analysis.

Alpha-beta pruning applies this same logic to game trees.

### The Core Idea in Plain English
1. **α (alpha)** tracks the best score MAX can achieve so far
2. **β (beta)** tracks the best score MIN can achieve so far (lowest, since MIN wants low scores)
3. **At a MAX node**: If we find a move scoring ≥ β, stop searching—MIN will never allow this path
4. **At a MIN node**: If we find a move scoring ≤ α, stop searching—MAX will never choose this path

### Walking Through the Code Step by Step
Here's how alpha-beta pruning would be implemented for Tic-Tac-Toe:

#### Step 1: Entry Point with Initial Bounds
```c
int ai_get_computer_move(GameState *state) {
    CellValue ai_symbol = (state->players[0].type == PLAYER_COMPUTER) ? CELL_X : CELL_O;
    int is_maximizing = (ai_symbol == CELL_X);

    int best_move = -1;
    int alpha = -1000;  // Worst possible for MAX
    int beta = 1000;    // Worst possible for MIN
```

**What's happening:** We initialize α to negative infinity (represented as -1000) and β to positive infinity (1000). These represent "no good option found yet" for each player.

#### Step 2: Search with Pruning
```c
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(state->board, i)) {
            state->board[i] = ai_symbol;
            int score = alphabeta(state->board, 0, alpha, beta, !is_maximizing);
            state->board[i] = CELL_EMPTY;

            if (is_maximizing && score > alpha) {
                alpha = score;
                best_move = i;
            } else if (!is_maximizing && score < beta) {
                beta = score;
                best_move = i;
            }
        }
    }
    return best_move;
}
```

**What's happening:** As we find better moves, we tighten the α-β window, enabling more pruning in subsequent searches.

#### Step 3: The Recursive Alpha-Beta Function
```c
static int alphabeta(CellValue board[BOARD_SIZE], int depth, 
                     int alpha, int beta, int is_maximizing) {
    // Terminal check (same as minimax)
    CellValue winner = wcc_check_winner(board);
    if (winner != CELL_EMPTY) {
        return winner == CELL_X ? 10 - depth : -10 + depth;
    }
    
    int moves_left = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(board, i)) moves_left++;
    }
    if (moves_left == 0) return 0;
```

**What's happening:** Terminal conditions are identical to minimax—check for winner or draw.

#### Step 4: Maximizing with Alpha Cutoff
```c
    if (is_maximizing) {
        int value = -1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_X;
                value = max(value, alphabeta(board, depth + 1, alpha, beta, 0));
                board[i] = CELL_EMPTY;
                
                alpha = max(alpha, value);
                if (beta <= alpha) {
                    break;  // β cutoff - MIN won't allow this path
                }
            }
        }
        return value;
    }
```

**What's happening:**
1. Try each move and get its value recursively
2. Update α if we found a better option for MAX
3. **Key insight:** If α ≥ β, we can stop—MIN already has a better option elsewhere and won't let us reach this node

#### Step 5: Minimizing with Beta Cutoff
```c
    } else {
        int value = 1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_O;
                value = min(value, alphabeta(board, depth + 1, alpha, beta, 1));
                board[i] = CELL_EMPTY;
                
                beta = min(beta, value);
                if (beta <= alpha) {
                    break;  // α cutoff - MAX won't choose this path
                }
            }
        }
        return value;
    }
}
```

**What's happening:** Mirror image for MIN—update β and prune when β ≤ α.

### A Visual Example
Consider this game tree (MAX moves first):

```
                    MAX (root)
                   /    \
                MIN     MIN
               / \      / \
              3   5    6   ?
```

**Without pruning:** Evaluate all 4 leaf nodes.

**With alpha-beta:**
1. Evaluate left MIN node: sees 3, then 5 → returns 3 (MIN picks lowest)
2. α = 3 (MAX's best guarantee so far)
3. Evaluate right MIN node: sees 6
4. **Prune!** MIN will return ≤ 6, but MAX already has 3. Since 6 > 3, we need to check more... 
   
Let's try a clearer example:

```
                    MAX (root)
                   /    \
                MIN     MIN
               / \      / \
              3   5    2   ?
```

1. Left subtree: MIN returns min(3,5) = 3
2. α = 3
3. Right subtree: MIN sees 2
4. **Prune!** β = 2, and β ≤ α (2 ≤ 3), so we skip "?"
5. MAX will choose left path (score 3), so "?" is irrelevant

### Why Pruning is Safe
The pruning condition β ≤ α means:
- MAX already has a path guaranteeing at least α
- MIN can force a result of at most β in the current subtree
- Since β ≤ α, this subtree is worse for MAX than what MAX already has
- Therefore, MAX will never choose this path—we can skip remaining children

### Common Questions from New Engineers
**Q: Does pruning change the result?**
A: No! Alpha-beta returns the exact same value as minimax. It just avoids computing values that can't affect the final decision.

**Q: How much faster is it?**
A: With random move ordering: reduces nodes from O(b^d) to roughly O(b^(3d/4)). With perfect ordering: O(b^(d/2))—a dramatic improvement.

**Q: What's "perfect move ordering"?**
A: Examining the best move first at each node. If MAX looks at its highest-scoring moves first, and MIN looks at its lowest-scoring moves first, pruning is maximized.

**Q: Why isn't alpha-beta used in the current ai.c?**
A: For Tic-Tac-Toe's small game tree, minimax is fast enough. Alpha-beta becomes essential for games like chess with enormous search spaces.

---

## Implementation Details
### Key Functions
#### `alphabeta(board, depth, alpha, beta, is_maximizing)`
- **Purpose**: Recursively evaluates game states with pruning
- **Parameters**:
  - `board`: Current game board state
  - `depth`: Current recursion depth
  - `alpha`: Best value MAX can guarantee (lower bound)
  - `beta`: Best value MIN can guarantee (upper bound)
  - `is_maximizing`: Boolean indicating current player's objective
- **Returns**: The minimax value of the position

#### Pruning Conditions
| Player | Condition | Name         | Meaning                                       |
|--------|-----------|--------------|-----------------------------------------------|
| MAX    | β ≤ α     | Beta cutoff  | MIN has a better option; skip remaining moves |
| MIN    | β ≤ α     | Alpha cutoff | MAX has a better option; skip remaining moves |

### State Management
Identical to minimax—uses in-place board modification with backtracking:
```c
board[i] = symbol;           // Make move
score = alphabeta(...);      // Evaluate with pruning
board[i] = CELL_EMPTY;       // Undo move
```

---

## Theoretical Foundations
### Formal Definition
Alpha-beta pruning extends minimax by maintaining a **window** [α, β] of relevant values:

```
αβ(s, α, β) = 
    ⎧ eval(s)                                           if s ∈ Z (terminal)
    ⎪
    ⎨ max-value(s, α, β)                                if ρ(s) = MAX
    ⎪
    ⎩ min-value(s, α, β)                                if ρ(s) = MIN
```

Where:

```
max-value(s, α, β):
    v ← -∞
    for each s' ∈ succ(s):
        v ← max(v, αβ(s', α, β))
        α ← max(α, v)
        if β ≤ α: break          // Prune
    return v

min-value(s, α, β):
    v ← +∞
    for each s' ∈ succ(s):
        v ← min(v, αβ(s', α, β))
        β ← min(β, v)
        if β ≤ α: break          // Prune
    return v
```

### The Alpha-Beta Invariant
**Definition:** At any point during search, the following invariant holds:

$$\alpha < \beta \implies \text{minimax}(s) \in [\alpha, \beta] \text{ is possible}$$

**Corollary:** When β ≤ α, we know minimax(s) falls outside the window of interest, and further search is unnecessary.

### Pruning Correctness Theorem
**Theorem:** Alpha-beta pruning returns the same value as minimax for any game tree when called with α = -∞ and β = +∞.

**Proof:**

We prove a stronger result. Let V(s) denote the true minimax value. Then αβ(s, α, β) returns:
- V(s) if α < V(s) < β
- A value ≤ α if V(s) ≤ α (fail-low)
- A value ≥ β if V(s) ≥ β (fail-high)

*Proof by strong induction on tree height h:*

*Base case (h = 0):* Terminal nodes return eval(s) = V(s), satisfying all conditions. ✓

*Inductive hypothesis:* The claim holds for all states at height < h.

*Inductive step for MAX node s at height h:*

Let V(s) = max{V(s₁), ..., V(sₖ)} where s₁, ..., sₖ are children of s.

**Case 1: No pruning occurs (all children evaluated).**
By induction, each recursive call returns the correct value for that child (within the current window). The maximum of these equals V(s). ✓

**Case 2: Pruning at child sⱼ (β cutoff).**
We have found value v ≥ β after evaluating s₁, ..., sⱼ.

Since v ≤ V(s) (v is a lower bound on the max), we have V(s) ≥ v ≥ β.

The algorithm returns v ≥ β, which satisfies the fail-high condition. ✓

*Symmetric argument for MIN nodes (fail-low when v ≤ α).* ∎

**Corollary:** With initial call αβ(root, -∞, +∞), since -∞ < V(root) < +∞, the algorithm returns exactly V(root).

### Shallow vs Deep Cutoffs
**Definition:** A cutoff is classified by distance from the pruning point:

| Type               | Description                         | Nodes Saved      |
|--------------------|-------------------------------------|------------------|
| **Shallow cutoff** | Prune after examining few children  | Many nodes saved |
| **Deep cutoff**    | Prune after examining most children | Few nodes saved  |

**Optimal move ordering** produces shallow cutoffs by examining best moves first.

### Move Ordering and Efficiency
The effectiveness of alpha-beta depends critically on **move ordering**:

**Definition:** Let T(b, d) denote nodes examined for branching factor b and depth d.

| Move Ordering         | Nodes Examined  | Formula         |
|-----------------------|-----------------|-----------------|
| Worst case (pessimal) | Same as minimax | T = O(b^d)      |
| Random ordering       | Average case    | T ≈ O(b^(3d/4)) |
| Perfect ordering      | Best case       | T = O(b^(d/2))  |

**Theorem (Knuth & Moore, 1975):** With optimal move ordering, alpha-beta examines exactly:

$$N_{opt} = 2b^{d/2} - 1 \quad \text{(for even } d \text{)}$$

$$N_{opt} = b^{(d+1)/2} + b^{(d-1)/2} - 1 \quad \text{(for odd } d \text{)}$$

### Effective Branching Factor Reduction
**Definition:** The effective branching factor b* satisfies:

$$N = (b^*)^d$$

where N is the number of nodes examined.

For alpha-beta with perfect ordering:
$$b^* = \sqrt{b}$$

**Implication:** Alpha-beta with perfect ordering searches twice as deep as minimax in the same time.

### Formal Analysis of Pruning
#### Alpha Cutoff (at MIN node)
**Condition:** β ≤ α (equivalently, found value v ≤ α)

**Interpretation:**
When MIN finds a move with value v ≤ α, and we know MAX can guarantee α elsewhere, MAX will never choose this path. Formally:

$$v \leq \alpha \implies \text{MIN's value here} \leq \alpha \leq \text{MAX's guaranteed alternative}$$

Since MAX can already guarantee α elsewhere, this subtree is irrelevant to MAX's decision.

#### Beta Cutoff (at MAX node)
**Condition:** β ≤ α (equivalently, found value v ≥ β)

**Interpretation:**
When MAX finds a move with value v ≥ β, and we know MIN can guarantee β elsewhere, MIN will never allow MAX to reach this node. Formally:

$$v \geq \beta \implies \text{MAX's value here} \geq \beta \geq \text{MIN's guaranteed alternative}$$

Since MIN can already guarantee β elsewhere, this subtree is irrelevant to MIN's decision.

### The Principal Variation
**Definition:** The **principal variation (PV)** is the sequence of moves that both players will make with optimal play.

**Property:** Alpha-beta must fully evaluate all nodes on the principal variation. Pruning only eliminates non-PV branches.

**Corollary:** The minimum number of nodes that must be examined is the size of the PV plus its critical siblings.

### Termination Proof
**Theorem:** Alpha-beta terminates for any finite game tree.

**Proof:**

The termination argument is identical to minimax:
1. Each recursive call operates on a state with strictly fewer empty cells
2. The number of empty cells is bounded ∈ [0, 9]
3. Pruning only reduces the number of recursive calls
4. Therefore, alpha-beta terminates in at most O(b^d) calls ∎

### Complexity Analysis
#### Time Complexity
| Scenario     | Complexity  | Notes                          |
|--------------|-------------|--------------------------------|
| Worst case   | O(b^d)      | No pruning (pessimal ordering) |
| Average case | O(b^(3d/4)) | Random move ordering           |
| Best case    | O(b^(d/2))  | Perfect move ordering          |

For Tic-Tac-Toe (b ≈ 4.15, d = 9):

| Algorithm            | Estimated Nodes |
|----------------------|-----------------|
| Minimax              | ~986,410        |
| Alpha-beta (random)  | ~15,000         |
| Alpha-beta (optimal) | ~1,500          |

*Note: Actual node counts vary due to early terminations (wins before depth 9) and non-uniform branching.*

#### Space Complexity
| Component        | Complexity               |
|------------------|--------------------------|
| Call stack depth | O(d)                     |
| α, β parameters  | O(d) total across stack  |
| Board state      | O(1) - modified in-place |
| **Total**        | **O(d)**                 |

---

## Comparison with Minimax
| Aspect               | Minimax | Alpha-Beta              |
|----------------------|---------|-------------------------|
| Result               | V(s)    | V(s) (identical)        |
| Time (worst)         | O(b^d)  | O(b^d)                  |
| Time (best)          | O(b^d)  | O(b^(d/2))              |
| Space                | O(d)    | O(d)                    |
| Implementation       | Simpler | Slightly more complex   |
| Move ordering impact | None    | Critical for efficiency |

---

## Advanced Optimizations
Building on alpha-beta, several enhancements exist:

| Optimization             | Description                                              | Benefit                      |
|--------------------------|----------------------------------------------------------|------------------------------|
| **Aspiration Windows**   | Start with narrow α-β window around expected value       | Faster if guess is accurate  |
| **Transposition Tables** | Cache positions to avoid re-evaluation                   | Handles transpositions       |
| **Killer Heuristic**     | Try moves that caused cutoffs at sibling nodes           | Better move ordering         |
| **History Heuristic**    | Order moves by how often they caused cutoffs             | Statistical move ordering    |
| **Iterative Deepening**  | Search depth 1, then 2, etc., using results for ordering | Natural move ordering        |
| **Null Move Pruning**    | Skip a turn to get quick bound                           | Prunes more aggressively     |
| **Late Move Reductions** | Search later moves to reduced depth                      | Saves time on unlikely moves |

---

## References
1. Knuth, D. E., & Moore, R. W. (1975). An analysis of alpha-beta pruning. *Artificial Intelligence*, 6(4), 293-326.
2. Pearl, J. (1982). The solution for the branching factor of the alpha-beta pruning algorithm and its optimality. *Communications of the ACM*, 25(8), 559-564.
3. Russell, S., & Norvig, P. (2020). *Artificial Intelligence: A Modern Approach* (4th ed.). Pearson. Chapter 5: Adversarial Search.
4. Marsland, T. A. (1986). A review of game-tree pruning. *ICCA Journal*, 9(1), 3-19.
5. Schaeffer, J. (1989). The history heuristic and alpha-beta search enhancements in practice. *IEEE Transactions on Pattern Analysis and Machine Intelligence*, 11(11), 1203-1212.


