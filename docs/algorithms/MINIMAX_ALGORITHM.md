# Minimax Algorithm
## Overview
The **Minimax algorithm** is a decision-making algorithm used in game theory and artificial intelligence for determining
the optimal move in two-player zero-sum games. The implementation in `ai.c` applies minimax to the game of Tic-Tac-Toe, 
providing a **provably optimal strategy** that is:

1. **Complete**: Explores all possible game states
2. **Optimal**: Against perfect play, guarantees the best possible outcome
3. **Deterministic**: Same board state always produces the same move

For Tic-Tac-Toe specifically, optimal play by both sides results in a **draw**.

---

## Walkthrough: How Minimax Actually Works
### The Big Picture
Imagine you're playing Tic-Tac-Toe and you want to figure out the *best* move. What would you do? You'd probably think ahead:

> "If I go here, then they'll go there, then I'll go here..."

That's exactly what minimax does—but it thinks through **every possible game** from the current position to the end.

### The Core Idea in Plain English
1. **The AI looks at every empty cell** where it could place its symbol
2. **For each possible move**, it asks: "If I play here, what's the best outcome I can guarantee?"
3. **To answer that question**, it simulates the opponent's response, then its own response, and so on until the game ends
4. **It picks the move** that leads to the best guaranteed outcome

### Walking Through the Code Step by Step
Let's trace through how the AI decides where to move:

#### Step 1: Entry Point — `ai_get_computer_move()`
```c
int ai_get_computer_move(GameState *state) {
    CellValue ai_symbol = (state->players[0].type == PLAYER_COMPUTER) ? CELL_X : CELL_O;
    int is_maximizing = (ai_symbol == CELL_X);
```

**What's happening:** The AI first figures out which symbol it's playing (X or O). If it's X, it wants to *maximize* the score. If it's O, it wants to *minimize* the score.

> 💡 **Why?** By convention, X winning = positive score, O winning = negative score. So X wants high numbers, O wants low numbers.

#### Step 2: Try Every Possible Move
```c
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(state->board, i)) {
            state->board[i] = ai_symbol;              // Make the move
            int score = minimax(state->board, 0, !is_maximizing);  // Evaluate it
            state->board[i] = CELL_EMPTY;             // Undo the move
```

**What's happening:** The AI loops through all 9 cells. For each empty cell:
1. It *temporarily* places its symbol there
2. It calls `minimax()` to find out how good this move is
3. It *undoes* the move (so the board returns to its original state)

> 💡 **Key insight:** We're not actually making moves—we're *simulating* to see what would happen.

#### Step 3: The Recursive Magic — `minimax()`
This is where the "thinking ahead" happens:

```c
static int minimax(CellValue board[BOARD_SIZE], int depth, int is_maximizing) {
    // First, check if the game is over
    CellValue winner = wcc_check_winner(board);
    if (winner != CELL_EMPTY) {
        return winner == CELL_X ? 10 - depth : -10 + depth;
    }
```

**What's happening:** Before thinking further, check if someone already won:
- X won? Return a positive score (good for X)
- O won? Return a negative score (good for O)

> 💡 **Why `10 - depth`?** We subtract depth so the AI prefers *faster* wins. Winning in 2 moves (score=8) is better than winning in 4 moves (score=6).

```c
    // Check for a draw
    int moves_left = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(board, i)) moves_left++;
    }
    if (moves_left == 0) return 0;  // Draw = neutral score
```

**What's happening:** If the board is full and nobody won, it's a draw (score = 0).

#### Step 4: Simulate Both Players' Best Moves
```c
    if (is_maximizing) {
        int best_score = -1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_X;
                int score = minimax(board, depth + 1, 0);  // Now it's O's turn
                board[i] = CELL_EMPTY;
                if (score > best_score) best_score = score;
            }
        }
        return best_score;
    }
```

**What's happening when maximizing (X's perspective):**
1. Try every possible X move
2. Recursively call `minimax()` to see what happens (switching to O's turn)
3. Keep track of the *highest* score found
4. Return that best score

```c
    } else {
        int best_score = 1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_O;
                int score = minimax(board, depth + 1, 1);  // Now it's X's turn
                board[i] = CELL_EMPTY;
                if (score < best_score) best_score = score;
            }
        }
        return best_score;
    }
```

**What's happening when minimizing (O's perspective):**
Same idea, but O wants the *lowest* score.

### A Visual Example
Let's say the board looks like this, and it's O's turn:

```
 X | O | X
---+---+---
   | X |  
---+---+---
 O |   |  
```

The AI (playing O) considers four empty cells: positions 3, 5, 7, and 8.

**For position 5:**
```
 X | O | X          The AI simulates: "If I go here..."
---+---+---         Then X might go to 3, 7, or 8...
   | X | O          For each of those, I respond...
---+---+---         Keep going until game ends.
 O |   |  
```

The algorithm builds a mental "game tree" exploring every possibility, then bubbles up the scores to find the best move.

### Why It's Called "Minimax"
The name comes from the alternating strategy:
- **MAX**imizing player (X): Picks moves that **max**imize the score
- **MIN**imizing player (O): Picks moves that **min**imize the score

Each player assumes the opponent will play optimally. X asks "what's the best I can do if O plays perfectly?" and vice versa.

### Common Questions from New Engineers
**Q: Doesn't this take forever?**
A: For Tic-Tac-Toe, no—there are only 255,168 possible games. A modern computer handles this instantly. For chess, though, you'd need optimizations like alpha-beta pruning.

**Q: Why undo the move after each simulation?**
A: We're not actually playing—we're just looking ahead. We need to restore the board to try other possibilities.

**Q: What if multiple moves have the same score?**
A: The current implementation picks the first one found. You could randomize among equally-good moves for variety.

**Q: Can the AI ever lose?**
A: Against a perfect opponent, Tic-Tac-Toe always ends in a draw. Against an imperfect opponent, the AI will win or draw—never lose.

---

## Implementation Details

### Key Functions

#### `minimax(board, depth, is_maximizing)`
- **Purpose**: Recursively evaluates all possible game states
- **Parameters**:
    - `board`: Current game board state
    - `depth`: Current recursion depth
    - `is_maximizing`: Boolean indicating current player's objective

#### `ai_get_computer_move(state)`
- **Purpose**: Entry point that selects the best move
- **Process**:
    1. Identifies AI's symbol (X or O)
    2. Iterates through all empty cells
    3. Simulates each move and calls minimax
    4. Returns the move with optimal score

### State Management
The implementation uses an **in-place modification** strategy:
```c
board[i] = ai_symbol;        // Make move
score = minimax(...);        // Evaluate
board[i] = CELL_EMPTY;       // Undo move (backtrack)
```

This approach minimizes memory allocation overhead.

---

## Theoretical Foundations
### Game-Theoretic Framework
#### Formal Game Definition
Tic-Tac-Toe can be formally defined as a tuple **G = (N, A, H, Z, χ, ρ, σ, u)** where:

| Symbol   | Definition                                              |
|----------|---------------------------------------------------------|
| **N**    | Set of players: N = {X, O}                              |
| **A**    | Action set: A = {0, 1, 2, ..., 8} (board positions)     |
| **H**    | Set of non-terminal game histories (sequences of moves) |
| **Z**    | Set of terminal histories (completed games)             |
| **χ(h)** | Function returning legal actions at history h           |
| **ρ(h)** | Player function: indicates whose turn it is at h        |
| **σ**    | Strategy profile: σ = (σ_X, σ_O)                        |
| **u**    | Utility function: u: Z → ℝ² (payoffs for each player)   |

#### Zero-Sum Property
Minimax operates under the **zero-sum** constraint, formally expressed as:

$$\forall z \in Z: u_X(z) + u_O(z) = 0$$

This means one player's gain exactly equals the other's loss. In our implementation:

| Terminal State | u_X | u_O |
|----------------|-----|-----|
| X wins         | +10 | -10 |
| O wins         | -10 | +10 |
| Draw           | 0   | 0   |

#### Perfect Information
Tic-Tac-Toe is a game of **perfect information**, meaning:
1. All players observe the complete game state
2. No hidden information or chance elements exist
3. The game tree is fully determinable

### The Minimax Theorem
#### Von Neumann's Theorem (1928)
For any finite, two-player, zero-sum game with perfect information, there exists a value **V** (the *game value*) and optimal strategies σ*_X and σ*_O such that:

$$V = \max_{\sigma_X} \min_{\sigma_O} u_X(\sigma_X, \sigma_O) = \min_{\sigma_O} \max_{\sigma_X} u_X(\sigma_X, \sigma_O)$$

**Interpretation:** Player X can guarantee at least V regardless of O's play, and O can prevent X from achieving more than V.

#### Implications for Tic-Tac-Toe
Since Tic-Tac-Toe is finite, two-player, and zero-sum:
- A deterministic optimal strategy exists for both players
- The game value V = 0 (draw with perfect play)
- The minimax algorithm computes these optimal strategies

### Backward Induction
Minimax implements **backward induction**, a solution concept for extensive-form games:

**Definition:** Starting from terminal nodes, recursively assign values to each node based on the optimal choice of the player at that node.

**Formal procedure:**
1. Let **v(z)** = utility for terminal states z ∈ Z
2. For non-terminal h where player i moves:
   - If i = MAX: **v(h)** = max{v(h') | h' is a successor of h}
   - If i = MIN: **v(h)** = min{v(h') | h' is a successor of h}

**Theorem:** Backward induction yields a subgame perfect equilibrium (SPE).

### Algorithm Structure
#### Recursive Definition
The minimax function can be expressed as:

```
V(s) = 
    ⎧ eval(s)                           if s ∈ Z (terminal)
    ⎨ max{V(s') | s' ∈ succ(s)}         if ρ(s) = MAX
    ⎩ min{V(s') | s' ∈ succ(s)}         if ρ(s) = MIN
```

Where:
- **s** denotes a game state
- **Z** is the set of terminal states
- **succ(s)** returns successor states after legal moves
- **ρ(s)** identifies the player to move at state s

#### Terminal Conditions
The recursion terminates when reaching **Z**, defined as:

$$Z = \{s \in H : \text{winner}(s) \neq \emptyset\} \cup \{s \in H : |\chi(s)| = 0\}$$

That is, states where either:
1. A player has achieved three in a row, or
2. No legal moves remain (full board)

### Evaluation Function
#### Depth-Adjusted Utility
The implementation uses a **depth-adjusted scoring function** to induce preferences over strategically equivalent outcomes:

$$\text{eval}(s, d) = \begin{cases} 
+10 - d & \text{if winner}(s) = X \\
-10 + d & \text{if winner}(s) = O \\
0 & \text{if draw}
\end{cases}$$

Where **d** is the search depth (number of moves from root).

#### Rationale
This adjustment creates a **lexicographic preference ordering**:
1. **Primary:** Win > Draw > Loss
2. **Secondary:** Among wins, prefer faster victories; among losses, prefer slower defeats

**Formal property:** For winning terminal states at depths d₁ < d₂:
$$\text{eval}(s_1, d_1) > \text{eval}(s_2, d_2)$$

This ensures the AI plays **shortest-path-to-victory** strategies.

### Correctness Proof Sketch
**Theorem:** The minimax algorithm returns the game-theoretic value V(s₀) for any initial state s₀.

**Proof (by structural induction on the game tree):**

*Base case:* For terminal states s ∈ Z, minimax returns eval(s), which equals the true utility u(s). ✓

*Inductive hypothesis:* Assume minimax correctly computes V(s') for all successors s' of state s.

*Inductive step:* 
- If ρ(s) = MAX: minimax returns max{V(s') | s' ∈ succ(s)}, which is the optimal value for MAX by definition.
- If ρ(s) = MIN: minimax returns min{V(s') | s' ∈ succ(s)}, which is the optimal value for MIN by definition.

By the principle of mathematical induction, minimax computes V(s) correctly for all states. ∎

### Termination Proof
**Theorem:** The minimax algorithm terminates for all valid Tic-Tac-Toe game states.

**Proof:**

Define a **well-founded ordering** on game states using the number of empty cells:

$$\text{rank}(s) = |\{i \in \{0,...,8\} : s[i] = \text{EMPTY}\}|$$

**Observations:**
1. For any state s, rank(s) ∈ {0, 1, ..., 9}
2. Each recursive call decreases rank by exactly 1 (one cell is filled)
3. Terminal states have either rank = 0 (full board) or a winner (recursion stops)

Since the natural numbers are well-ordered and rank strictly decreases with each recursive call, the algorithm must terminate after at most 9 recursive calls per branch. ∎

### Game Tree Analysis
#### State Space Size
The **state space** of Tic-Tac-Toe can be bounded as follows:

**Upper bound (naive):** 3⁹ = 19,683 possible board configurations (each cell is X, O, or empty)

**Tighter bound:** Accounting for turn alternation and early termination:
- Legal positions: 5,478
- Distinct game sequences: 255,168

#### Branching Factor
The **effective branching factor** b(d) varies with depth:

| Depth d | Empty Cells | b(d) |
|---------|-------------|------|
| 0       | 9           | 9    |
| 1       | 8           | 8    |
| 2       | 7           | 7    |
| ...     | ...         | ...  |
| 8       | 1           | 1    |

**Average branching factor:**

$$\bar{b} = \sqrt[9]{\prod_{i=1}^{9} i} = \sqrt[9]{9!} \approx 4.15$$

#### Worst-Case Node Count
Without pruning, the maximum nodes evaluated is:

$$N_{max} = \sum_{d=0}^{9} \frac{9!}{(9-d)!} = 1 + 9 + 72 + 504 + 3024 + 15120 + 60480 + 181440 + 362880 + 362880 = 986,410$$

In practice, early termination (wins before board fills) significantly reduces this.

#### Time Complexity
| Metric                | Value                                  |
|-----------------------|----------------------------------------|
| Branching factor (b)  | Up to 9 (decreases as game progresses) |
| Maximum depth (d)     | 9 moves                                |
| Worst-case complexity | **O(b^d)** where b ≤ 9, d ≤ 9          |
| Upper bound (nodes)   | ~986,410 (see above)                   |

#### Space Complexity
| Type                | Complexity               |
|---------------------|--------------------------|
| Call stack depth    | O(d) = O(9)              |
| Board state storage | O(1) - modified in-place |
| **Total**           | **O(d)**                 |


## Potential Optimizations
While not implemented in this version, common enhancements include:

| Optimization             | Description                                               | Benefit                               |
|--------------------------|-----------------------------------------------------------|---------------------------------------|
| **Alpha-Beta Pruning**   | Eliminates branches that cannot affect the final decision | Reduces time complexity to O(b^(d/2)) |
| **Transposition Tables** | Caches previously evaluated positions                     | Avoids redundant calculations         |
| **Move Ordering**        | Evaluates promising moves first                           | Improves pruning efficiency           |
| **Iterative Deepening**  | Progressively increases search depth                      | Provides anytime behavior             |

## References
1. Von Neumann, J., & Morgenstern, O. (1944). *Theory of Games and Economic Behavior*. Princeton University Press.
2. Russell, S., & Norvig, P. (2020). *Artificial Intelligence: A Modern Approach* (4th ed.). Pearson. Chapter 5: Adversarial Search.
3. Knuth, D. E., & Moore, R. W. (1975). An analysis of alpha-beta pruning. *Artificial Intelligence*, 6(4), 293-326.
