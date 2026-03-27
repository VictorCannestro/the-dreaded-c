# Lesson 6: Structs
> **Before you start:** Make sure you're comfortable with [Lesson 5: Pointers](LESSON_5_Pointers.md).
> Structs and pointers work hand-in-hand — most functions in this project take `GameState *` parameters.

---

## Table of Contents
### Core Concepts
**Foundations**
0. [What Is a Struct?](#what-is-a-struct) — bundling related data into one unit
1. [Defining and Declaring Structs](#1-defining-and-declaring-structs) — `typedef struct`; creating variables
2. [Initializing Structs](#2-initializing-structs) — member-by-member, initializer list, designated initializers
3. [Accessing Struct Members](#3-accessing-struct-members) — `.` dot vs `->` arrow operators

**Project Connection**
4. [Structs in This Project](#4-structs-in-this-project) — `Player`, `GameState`
5. [Nested Struct Access](#5-nested-struct-access) — chaining `.` and `->` for arrays of structs

**Functions and Structs**
6. [Passing Structs to Functions](#6-passing-structs-to-functions) — by value (copy) vs by pointer (reference)
7. [Null-Checking Struct Pointers](#7-null-checking-struct-pointers) — guard clauses; avoiding segfaults

**Summary**
[Common Mistakes to Avoid](#8-common-mistakes-to-avoid)

### Practice
9. [Practical Exercises](#9-practical-exercises)

### Going Further
10. [Why Use Structs?](#why-use-structs-summary)
11. [Next Steps](#next-steps)
11. [Bonus Advanced Topics](#bonus-advanced-topics) *(return after Lesson 9)*
    - `const` Struct Pointers — read-only access
    - Returning Structs from Functions
    - Structs and Memory Layout — padding and alignment

---

## What Is a Struct?
A **struct** (short for *structure*) groups related data together into a single, named unit. Think of it like a 
container that holds multiple variables of different types, bundled under one name.

Without structs, you'd need separate variables for each piece of related data:

```c
// Messy! Three separate variables for one person
char person_name[50];
int person_age;
float person_height;
```

With a struct, you bundle them together:

```c
// Clean! One variable holds all person data
typedef struct {
    char name[50];
    int age;
    float height;
} Person;

Person alice;  // One variable, three fields
```

---

## 1. Defining and Declaring Structs
### Basic Syntax

```c
typedef struct {
    // member declarations go here
    type1 member1;
    type2 member2;
    // ...
} StructName;
```

Let's break this down:
- `typedef` — Creates a type alias so you can use `StructName` directly (instead of `struct StructName`)
- `struct { ... }` — Defines the structure's layout
- `StructName` — The name you'll use to declare variables of this type

### Example: A 2D Point
```c
typedef struct {
    int x;
    int y;
} Point;
```

Now you can declare `Point` variables just like `int` or `char`:

```c
Point origin;           // A Point variable
Point corners[4];       // An array of 4 Points
Point *ptr;             // A pointer to a Point
```

---

## 2. Initializing Structs
There are several ways to give a struct its initial values:

### Method 1: Member-by-Member Assignment
```c
Point p;
p.x = 10;
p.y = 20;
```

### Method 2: Initializer List (At Declaration)
```c
Point p = {10, 20};          // x=10, y=20 (order matches definition)
```

### Method 3: Designated Initializers (C99+) ✨ Recommended
```c
Point p = {.x = 10, .y = 20};   // Explicit and self-documenting
Point q = {.y = 5, .x = 3};     // Order doesn't matter with designators
```

**Why use designated initializers?**
- Self-documenting: You can read *what* each value is for
- Order-independent: Less error-prone if struct definition changes
- Partial initialization: Unspecified members are zeroed

```c
Point p = {.x = 10};   // y is automatically 0
```

---

## 3. Accessing Struct Members
### The Dot Operator (`.`) — Direct Access
When you have the struct itself (not a pointer), use the **dot operator**:

```c
Point p = {.x = 10, .y = 20};

printf("x = %d\n", p.x);   // Access: read the x member
printf("y = %d\n", p.y);

p.x = 50;                   // Modify: change the x member
```

### The Arrow Operator (`->`) — Pointer Access
> You saw `->` briefly in [Lesson 5](LESSON_5_Pointers.md). Here's the full picture in the context of structs.

When you have a **pointer** to a struct, use the **arrow operator**:

```c
Point p = {.x = 10, .y = 20};
Point *ptr = &p;             // ptr points to p

printf("x = %d\n", ptr->x);  // Access through pointer
printf("y = %d\n", ptr->y);

ptr->x = 50;                 // Modify through pointer
```

> `ptr->member` is shorthand for `(*ptr).member`. The arrow saves you parentheses and typing!

```c
// These are identical — always prefer the arrow form:
ptr->x = 50;
(*ptr).x = 50;   // Dereference first, then dot — works, but verbose and error-prone
```

### Quick Rule: Dot vs. Arrow

| You have…             | Use  | Example  |
|-----------------------|------|----------|
| The struct directly   | `.`  | `p.x`    |
| A pointer to a struct | `->` | `ptr->x` |

A helpful way to remember: the **arrow** points *through* the pointer to the member, just like a signpost pointing to a destination.

---

## 4. Structs in This Project
Now let's look at the real structs used in this tic-tac-toe project.

### The `Player` Struct
Represents one player in the game:

```c
typedef struct {
    PlayerType type;    // PLAYER_HUMAN or PLAYER_COMPUTER
    CellValue symbol;   // CELL_X or CELL_O
} Player;
```

Each field uses an enum type (from [Lesson 3: Varieties of Named Constants](LESSON_3_Named_Constants.md)):
- `PlayerType` can be `PLAYER_HUMAN` (0) or `PLAYER_COMPUTER` (1)
- `CellValue` can be `CELL_EMPTY` (0), `CELL_X` (1), or `CELL_O` (2)

### The `GameState` Struct
This is the central data structure — it holds *everything* about the current game:

```c
typedef struct {
    CellValue board[BOARD_SIZE];   // The NxN board (9 cells for 3x3)
    GameStatus status;              // ONGOING, X_WINS, O_WINS, or DRAW
    int move_count;                 // How many moves have been made
    Player players[2];              // players[0]=X, players[1]=O
    int game_count;                 // Which game number in the session
    CellValue last_winner;          // Who won the previous game
    Difficulty difficulty;          // DIFFICULTY_EASY or DIFFICULTY_HARD
} GameState;
```

Notice that `GameState` contains:
- An **array** (`board[BOARD_SIZE]`)
- An **array of structs** (`players[2]`)
- Multiple **enum values** (`status`, `last_winner`, `difficulty`)
- Plain **integers** (`move_count`, `game_count`)

See [include/tictactoe.h](../../include/tictactoe.h) for the actual definitions.

---

## 5. Nested Struct Access
When a struct contains other structs (or arrays of structs), you chain the access operators:

```c
GameState game;
game.players[0].type = PLAYER_HUMAN;     // First player is human
game.players[0].symbol = CELL_X;         // First player plays X
game.players[1].type = PLAYER_COMPUTER;  // Second player is computer
game.players[1].symbol = CELL_O;         // Second player plays O
```

With a pointer:

```c
GameState *state = &game;
state->players[0].type = PLAYER_HUMAN;   // Arrow for pointer, dot for nested struct
state->players[1].symbol = CELL_O;
```

Reading the chain **left to right**:
1. `state->` — Follow the pointer to the GameState
2. `players[1]` — Access the second element of the players array (which is a Player struct)
3. `.symbol` — Access the symbol member of that Player

### Real Example: `game_init_session`

Here's the actual initialization code from [`src/tictactoe.c`](../../src/tictactoe.c), showing every pattern at once:

```c
void game_init_session(GameState *state) {
    if (state == NULL) return;          // Guard clause (see §7)

    board_init(state->board);           // Pass array member via pointer
    state->status     = ONGOING;        // Set enum member
    state->move_count = 0;              // Set int member
    state->game_count = 0;
    state->last_winner = CELL_EMPTY;
    state->difficulty  = DIFFICULTY_HARD;

    state->players[0].type   = PLAYER_HUMAN;     // Nested: array of structs
    state->players[0].symbol = CELL_X;
    state->players[1].type   = PLAYER_COMPUTER;
    state->players[1].symbol = CELL_O;
}
```

Every `state->` dereferences the pointer to reach inside the struct. The final
four lines also chain into the `players` array with `.` — because after
`state->players[0]` you have a `Player` value, not a pointer to one.

---

## 6. Passing Structs to Functions
### Pass by Value (Copy)
If you pass a struct directly, C makes a **complete copy**:

```c
void print_point(Point p) {
    printf("(%d, %d)\n", p.x, p.y);
    p.x = 999;   // Modifies the COPY, not the original!
}

int main(void) {
    Point origin = {0, 0};
    print_point(origin);
    printf("x is still %d\n", origin.x);  // Prints 0, not 999
    return 0;
}
```

### Pass by Pointer (Reference) ✨ Preferred
Pass a **pointer** to avoid copying and to allow modifications:

```c
void move_point(Point *p, int dx, int dy) {
    p->x += dx;   // Modifies the ORIGINAL
    p->y += dy;
}

int main(void) {
    Point location = {10, 20};
    move_point(&location, 5, -3);
    printf("(%d, %d)\n", location.x, location.y);  // Prints (15, 17)
    return 0;
}
```

### Why Pointers Are Standard Practice
Almost every function in this project takes `GameState *state` instead of `GameState state`:

```c
// From tictactoe.h:
void game_init_session(GameState *state);
int game_make_move(GameState *state, int position);
GameStatus game_get_status(GameState *state);
```

**Two reasons:**

1. **Efficiency** — `GameState` contains an array of 9 cells, 2 Player structs, and several other fields. Copying all 
   that on every function call wastes time and memory.

2. **Modification** — Functions like `game_make_move` need to *change* the game state. With a copy, changes would be lost.

> 💡 If a function only *reads* the struct and never modifies it, you'll often see `const`:
> ```c
> void game_display(const GameState *state);  // Promise: won't modify
> ```

---

## 7. Null-Checking Struct Pointers
> [Lesson 5](LESSON_5_Pointers.md) introduced `NULL` for plain pointers. The same rule applies — and matters even
> more — when your pointer is to a struct, because a bad dereference crashes the whole program.

Any time your function receives a `GameState *` (or any struct pointer), the **first thing** it should do is check
for `NULL`:

```c
void game_new_game(GameState *state) {
    if (state == NULL) {   // ← guard clause: bail out immediately if NULL was passed
        return;
    }
    // Safe to use state-> below this point
    board_init(state->board);
    state->status = ONGOING;
    state->move_count = 0;
    state->game_count++;
}
```

This pattern — called a **guard clause** — appears at the top of every public function in this project. Without it,
passing `NULL` dereferences the pointer and causes an instant **segmentation fault**:

```c
GameState *state = NULL;
state->move_count = 0;   // 💥 CRASH: segmentation fault
```

**Returning a meaningful value on failure** is even better for functions that return something:

```c
int game_make_move(GameState *state, int position) {
    if (state == NULL || state->status != ONGOING) {
        return -1;   // Caller can detect and handle the failure
    }
    // ...
}
```

> 💡 The condition `state == NULL || state->status != ONGOING` is evaluated **left to right**. If `state` *is* `NULL`,
> C stops evaluating immediately (short-circuit) — so `state->status` is never touched. Safe!

---

## Common Mistakes to Avoid

| Mistake                                      | Problem                        | Fix                      |
|----------------------------------------------|--------------------------------|--------------------------|
| `state.status` when `state` is a pointer     | Won't compile — wrong operator | Use `state->status`      |
| `state->status` when `state` is a struct     | Won't compile — wrong operator | Use `state.status`       |
| Forgetting `&` when passing to pointer param | Wrong type, crashes or garbage | `func(&mystruct)`        |
| Assuming struct size equals sum of members   | Padding exists!                | Use `sizeof(StructType)` |

---

## 9. Practical Exercises
Try these exercises to solidify your understanding. Write the code, compile it, and verify the output.

---

### Exercise 1: Define and Initialize
**Task:** Define a struct called `Rectangle` with two integer members: `width` and `height`. Create a `Rectangle` 
variable with width 10 and height 5, then print both values.

<details>
<summary>💡 Hint</summary>

Use `typedef struct { ... } Rectangle;` syntax. Initialize with designated initializers: `{.width = ..., .height = ...}`.

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef struct {
    int width;
    int height;
} Rectangle;

int main(void) {
    Rectangle rect = {.width = 10, .height = 5};
    printf("Width: %d, Height: %d\n", rect.width, rect.height);
    return 0;
}
```

Output:
```
Width: 10, Height: 5
```

</details>

---

### Exercise 2: Calculate Area with a Function
**Task:** Write a function `calculate_area` that takes a `Rectangle` **by value** and returns its area (width × height).
Call it from `main` and print the result.

<details>
<summary>💡 Hint</summary>

The function signature is: `int calculate_area(Rectangle r)`. Access members with the dot operator since `r` is not a pointer.

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef struct {
    int width;
    int height;
} Rectangle;

int calculate_area(Rectangle r) {
    return r.width * r.height;
}

int main(void) {
    Rectangle rect = {.width = 10, .height = 5};
    int area = calculate_area(rect);
    printf("Area: %d\n", area);
    return 0;
}
```

Output:
```
Area: 50
```

</details>

---

### Exercise 3: Modify Through a Pointer
**Task:** Write a function `scale_rectangle` that takes a `Rectangle *` pointer and a scale factor (int). The function 
should multiply both width and height by the scale factor. Verify the original struct is modified.

<details>
<summary>💡 Hint</summary>

Use the arrow operator (`->`) to access and modify members through the pointer: `r->width *= scale;`

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef struct {
    int width;
    int height;
} Rectangle;

void scale_rectangle(Rectangle *r, int scale) {
    r->width *= scale;
    r->height *= scale;
}

int main(void) {
    Rectangle rect = {.width = 10, .height = 5};
    printf("Before: %d x %d\n", rect.width, rect.height);
    
    scale_rectangle(&rect, 3);  // Pass address with &
    
    printf("After:  %d x %d\n", rect.width, rect.height);
    return 0;
}
```

Output:
```
Before: 10 x 5
After:  30 x 15
```

</details>

---

### Exercise 4: Nested Structs
**Task:** Define a struct `Circle` with a `Point` center and an `int` radius. Create a circle centered at (100, 200) 
with radius 50. Print all three values.

<details>
<summary>💡 Hint</summary>

You can nest initializers: `{.center = {.x = ..., .y = ...}, .radius = ...}`

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point center;
    int radius;
} Circle;

int main(void) {
    Circle c = {
        .center = {.x = 100, .y = 200},
        .radius = 50
    };
    
    printf("Center: (%d, %d), Radius: %d\n", 
           c.center.x, c.center.y, c.radius);
    return 0;
}
```

Output:
```
Center: (100, 200), Radius: 50
```

</details>

---

### Exercise 5: Array of Structs
**Task:** Create an array of 3 `Player` structs (using the project's `Player` type). Initialize the array so that:
- Player 0: Human playing X
- Player 1: Computer playing O  
- Player 2: Human playing O

Then loop through and print each player's info.

<details>
<summary>💡 Hint</summary>

For this exercise, define `PlayerType` and `CellValue` enums yourself, or just use integer constants. Array 
initialization: `Player players[3] = { {...}, {...}, {...} };`

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef enum { PLAYER_HUMAN = 0, PLAYER_COMPUTER = 1 } PlayerType;
typedef enum { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 } CellValue;

typedef struct {
    PlayerType type;
    CellValue symbol;
} Player;

int main(void) {
    Player players[3] = {
        {.type = PLAYER_HUMAN,    .symbol = CELL_X},
        {.type = PLAYER_COMPUTER, .symbol = CELL_O},
        {.type = PLAYER_HUMAN,    .symbol = CELL_O}
    };
    
    const char *type_names[] = {"Human", "Computer"};
    const char *symbol_names[] = {"Empty", "X", "O"};
    
    for (int i = 0; i < 3; i++) {
        printf("Player %d: %s playing %s\n", 
               i, 
               type_names[players[i].type],
               symbol_names[players[i].symbol]);
    }
    return 0;
}
```

Output:
```
Player 0: Human playing X
Player 1: Computer playing O
Player 2: Human playing O
```

</details>

---

### Exercise 6: Pointer to Struct in Array
**Task:** Given the `players` array from Exercise 5, write a function `find_human_player` that takes the array and its 
size, and returns a **pointer** to the first human player found (or `NULL` if none). Test it by printing the found 
player's symbol.

<details>
<summary>💡 Hint</summary>

Loop through the array. When you find a human, return `&players[i]`. The return type is `Player *`.

</details>

<details>
<summary>Answer</summary>

```c
#include <stdio.h>

typedef enum { PLAYER_HUMAN = 0, PLAYER_COMPUTER = 1 } PlayerType;
typedef enum { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 } CellValue;

typedef struct {
    PlayerType type;
    CellValue symbol;
} Player;

Player *find_human_player(Player players[], int count) {
    for (int i = 0; i < count; i++) {
        if (players[i].type == PLAYER_HUMAN) {
            return &players[i];  // Return pointer to this element
        }
    }
    return NULL;  // No human found
}

int main(void) {
    Player players[3] = {
        {.type = PLAYER_COMPUTER, .symbol = CELL_X},
        {.type = PLAYER_HUMAN,    .symbol = CELL_O},
        {.type = PLAYER_COMPUTER, .symbol = CELL_X}
    };
    
    Player *human = find_human_player(players, 3);
    
    if (human != NULL) {
        const char *symbols[] = {"Empty", "X", "O"};
        printf("Found human playing: %s\n", symbols[human->symbol]);
    } else {
        printf("No human player found.\n");
    }
    return 0;
}
```

Output:
```
Found human playing: O
```

</details>

---

### Exercise 7: Project Connection — Understanding GameState
**Task:** Look at the `GameState` struct definition above. Without running any code, answer these questions:

1. What is the type of `state->board[4]`?
2. What is the type of `state->players`?
3. What is the type of `state->players[0]`?
4. What is the type of `state->players[0].symbol`?
5. What is the type of `&state->players[1]`?

<details>
<summary>Answers</summary>

1. `CellValue` — `board` is an array of `CellValue`, and `board[4]` is one element
2. `Player[2]` (or equivalently `Player *` when used in an expression) — it's an array of 2 Players
3. `Player` — one element from the players array
4. `CellValue` — the symbol member of a Player struct
5. `Player *` — a pointer to a Player (the second player)

</details>

---

### Exercise 8: Complete the Function
**Task:** Complete this function that checks if a specific player (by symbol) is a computer:

```c
int is_computer_player(const GameState *state, CellValue symbol) {
    // Your code here:
    // Return 1 if the player with 'symbol' is a computer, 0 otherwise
}
```

<details>
<summary>💡 Hint</summary>

Loop through `state->players` (there are 2 of them). Check if `players[i].symbol == symbol`. If so, check if 
`players[i].type == PLAYER_COMPUTER`.

</details>

<details>
<summary>Answer</summary>

```c
int is_computer_player(const GameState *state, CellValue symbol) {
    for (int i = 0; i < 2; i++) {
        if (state->players[i].symbol == symbol) {
            return state->players[i].type == PLAYER_COMPUTER;
        }
    }
    return 0;  // Symbol not found
}
```

Alternative using the existing `game_get_player_type` function from the project:
```c
int is_computer_player(const GameState *state, CellValue symbol) {
    return game_get_player_type((GameState *)state, symbol) == PLAYER_COMPUTER;
}
```

</details>

---

## 10. Why Use Structs?

| Benefit              | Explanation                                                 |
|----------------------|-------------------------------------------------------------|
| **Organization**     | Related data stays together, not scattered across variables |
| **Abstraction**      | Work with a "game" or "player" concept, not raw integers    |
| **Pass Easily**      | One parameter instead of many                               |
| **Return Multiple**  | Return a struct with all results, or use output pointers    |
| **Self-Documenting** | Field names like `state->move_count` explain themselves     |
| **Maintainability**  | Add new fields to one struct, not many function signatures  |

---

## Next Steps

Continue to [Lesson 7: Header Files](LESSON_7_Header_Files.md) →

← Previous: [Lesson 5: Pointers](LESSON_5_Pointers.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics

> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 5: Pointers](LESSON_5_Pointers.md) — for deeper understanding of `const` with pointers
> - [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) — for understanding memory layout and alignment


### `const` Struct Pointers
When a function only needs to *read* a struct, declare the parameter as `const StructType *`:

```c
// ✅ Reads the board, promises never to modify it
GameStatus game_get_status(const GameState *state) {
    return state->status;
}
```

The `const` makes two things clear:
1. **Intent** — this function is read-only; callers can trust their struct won't change
2. **Safety** — the compiler will refuse to compile any accidental writes:

```c
GameStatus game_get_status(const GameState *state) {
    state->move_count = 0;   // ❌ compiler error: cannot assign to const member
}
```

#### The Two `const` Pointer Forms
Read the declaration **right-to-left** to understand it:

```c
const GameState *state;   // "state is a pointer to a const GameState"
                          //  → can't modify the GameState, but can re-point the pointer
GameState *const state;   // "state is a const pointer to a GameState"
                          //  → can modify the GameState, but can't re-point the pointer
```

In practice, you'll almost always see the first form (`const T *`).

#### `const` in This Project

| Function signature                                        | `const`?  | Why?                              |
|-----------------------------------------------------------|-----------|-----------------------------------|
| `void game_init_session(GameState *state)`                | No        | Writes initial values             |
| `int game_make_move(GameState *state, int pos)`           | No        | Updates board, move count, status |
| `int board_is_valid_move(const CellValue board[], int p)` | Yes       | Read-only board check             |
| *Ideal but missing:* `game_get_status`                    | Should be | Only reads `state->status`        |

> ⚠️ Some read-only functions in this project (like `game_get_status`) don't use `const` even though they should.
> When you write your own code, prefer `const` for any pointer parameter you don't intend to modify.


### Returning Structs from Functions
Functions can return structs directly:

```c
Point create_point(int x, int y) {
    Point p = {.x = x, .y = y};
    return p;
}

// Usage:
Point center = create_point(50, 50);
```

However, for large structs, it's often more efficient to pass a pointer to an output parameter:

```c
void create_point(int x, int y, Point *out) {
    out->x = x;
    out->y = y;
}

// Usage:
Point center;
create_point(50, 50, &center);
```


### Structs and Memory Layout
Structs are stored in **contiguous memory**, with members laid out in order:

```c
typedef struct {
    char grade;    // 1 byte
    int score;     // 4 bytes
    char pass;     // 1 byte
} Result;
```

But wait — you might expect `Result` to be 6 bytes (1+4+1), but it's often **12 bytes**! Why?

**Padding:** The compiler adds invisible padding bytes for *alignment* (so multi-byte values start at efficient memory 
addresses). This is an advanced topic, but be aware that `sizeof(YourStruct)` may be larger than the sum of its members.

```c
printf("Size of Result: %zu bytes\n", sizeof(Result));  // Might print 12
```
