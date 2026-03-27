# Lesson 1: Control Flow & Loops
> **Before you start:** Make sure you're comfortable with
> [Lesson 0: Basic Types](LESSON_0_Basic_Types.md) — especially comparison operators, logical operators, boolean values, and the ternary operator `?:`.
> Control flow is how you make your program **make decisions** and **repeat actions**.

Every interesting program needs to make choices and repeat tasks. Should we validate this move? Keep looping until the game ends? Check multiple win conditions in sequence? This lesson builds your understanding from simple `if` statements all the way to complex nested loops and early exits — the patterns that power game logic, input validation, and algorithm implementation.

> 💡 **Quick tip:** You already know one form of conditional expression — the **ternary operator** `?:` from Lesson 0.
> This lesson covers **statement-level** control flow (`if`, `switch`, loops), while the ternary operator is an 
> **expression-level** conditional. Remember: use ternary for simple value selection; use `if`/`switch` when you need 
> to execute multiple statements.

---

## Table of Contents
### Core Concepts
**Decision Making**
1. [The `if` Statement](#1-the-if-statement) — basic conditional execution
2. [`if-else` and `else if`](#2-if-else-and-else-if) — branching logic
3. [Guard Clauses — Early Returns](#3-guard-clauses--early-returns) — defensive programming pattern
4. [The `switch` Statement](#4-the-switch-statement) — multiple-choice selection

**Repetition**
5. [The `while` Loop](#5-the-while-loop) — repeat while condition is true
6. [The `do-while` Loop](#6-the-do-while-loop) — execute first, then check
7. [The `for` Loop](#7-the-for-loop) — counted iteration
8. [Nested Loops](#8-nested-loops) — loops within loops

**Loop Control**
9. [`break` — Exit Early](#9-break--exit-early) — jump out of a loop or switch
10. [`continue` — Skip to Next Iteration](#10-continue--skip-to-next-iteration) — skip the rest of the loop body
11. [Infinite Loops and Input Validation](#11-infinite-loops-and-input-validation) — the `while (1)` pattern

### Practice & Exercises
12. [Real Code Examples from This Project](#12-real-code-examples-from-this-project)
13. [Practical Exercises](#13-practical-exercises)
14. [Summary](#14-summary)

### Going Further
15. [Next Steps](#15-next-steps)
16. [Bonus: Advanced Topics](#16-bonus-advanced-topics) *(return after Lessons 4 & 5)*
    - `goto` — When and Why to Avoid It
    - Loop Invariants and Correctness
    - Duff's Device — The Strangest Loop

---

## 1. The `if` Statement
The **`if` statement** executes code only when a condition is true. It's the most fundamental control flow structure in C.

### How It Works — Visual Flow
```
     ┌─────────────┐
     │ Start / ... │
     └──────┬──────┘
            │
            ▼
    ┌───────────────┐
    │ Is condition  │
    │    true?      │
    └───────┬───────┘
            │
       ┌────┴────┐
       │         │
      Yes        No
       │         │
       ▼         │
┌─────────────┐  │
│ Execute the │  │
│ if-block    │  │
└──────┬──────┘  │
       │         │
       └────┬────┘
            │
            ▼
     ┌─────────────┐
     │ Continue... │
     └─────────────┘
```

### Simple Examples
```c
int position = 5;

if (position >= 0 && position < 9) {
    printf("Valid board position\n");
}

int score = 85;
if (score >= 60) {
    printf("Pass\n");
}

// Works with any expression that evaluates to 0 or non-zero
int count = 0;
if (count) {           // if (count != 0)
    printf("Non-zero\n");
}
if (!count) {          // if (count == 0)
    printf("Zero\n");
}
```


> 💡 **Mental model:** Think of `if` as a gate. Your code approaches the gate, checks the condition, and either passes through (condition true) or walks around it (condition false).

### Single-Statement `if` (No Braces)
```c
// Legal, but risky
if (x > 0)
    printf("Positive\n");

// Looks like both lines are in the if, but only the first one is!
if (x > 0)
    printf("Positive\n");
    printf("This ALWAYS runs!\n");  // ⚠️ Not part of the if!
```

**Best practice:** Always use braces, even for single statements. It prevents bugs when you add a second line later.

```c
// Safe and clear
if (x > 0) {
    printf("Positive\n");
}
```

### Quick Check: `if` Statement
What does this code print when x = 0?

```c
int x = 0;
if (x) {
    printf("A");
}
if (!x) {
    printf("B");
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** `B`

**Explanation:** When `x = 0`, the condition `x` evaluates to false (0 is "falsy" in C), so "A" doesn't print. The condition `!x` evaluates to true (NOT 0 = 1), so "B" prints.

</details>

---

## 2. `if-else` and `else if`
### The `if-else` Statement
Execute one block or another, but never both:

### How It Works — Visual Flow
```
         ┌─────────────┐
         │ Start / ... │
         └──────┬──────┘
                │
                ▼
        ┌───────────────┐
        │ Is condition  │
        │    true?      │
        └───────┬───────┘
                │
        ┌───────┴───────┐
        │               │
       Yes              No
        │               │
        ▼               ▼
 ┌────────────┐  ┌────────────┐
 │ Execute    │  │ Execute    │
 │ if-block   │  │ else-block │
 └─────┬──────┘  └──────┬─────┘
       │                │
       └───────┬────────┘
               │
               ▼
        ┌─────────────┐
        │ Continue... │
        └─────────────┘
```

**Examples:**
```c
int move_count = 5;

if (move_count % 2 == 0) {
    printf("X's turn\n");
} else {
    printf("O's turn\n");
}

// Validating input
if (index >= 0 && index < 10) {
    printf("Valid index: %d\n", index);
} else {
    printf("Invalid index: %d\n", index);
}
```

### The `else if` Chain
Test multiple conditions in sequence:

```c
if (condition1) {
    // Runs if condition1 is true
} else if (condition2) {
    // Runs if condition1 is false AND condition2 is true
} else if (condition3) {
    // Runs if condition1 and condition2 are false AND condition3 is true
} else {
    // Runs if all conditions are false (optional)
}
```

**Key behavior:** C checks conditions **top to bottom** and stops at the first true condition.

```c
int score = 85;

if (score >= 90) {
    printf("Grade: A\n");
} else if (score >= 80) {
    printf("Grade: B\n");  // This one runs
} else if (score >= 70) {
    printf("Grade: C\n");  // Never checked (stopped at B)
} else if (score >= 60) {
    printf("Grade: D\n");
} else {
    printf("Grade: F\n");
}
```


### ⚠️ Common Mistakes with `if-else`
**Mistake 1: Assignment instead of comparison**
```c
// BUG: This ASSIGNS 5 to x, then evaluates to true!
if (x = 5) {
    printf("This always runs!\n");
}

// CORRECT: Use == for comparison
if (x == 5) {
    printf("x is five\n");
}
```

**Mistake 2: Forgetting that conditions are checked in order**
```c
int score = 95;

// BUG: score >= 60 is true, so we never reach score >= 90!
if (score >= 60) {
    printf("Pass\n");           // This runs (wrong!)
} else if (score >= 90) {
    printf("Excellent!\n");     // Never reached
}

// CORRECT: Check most specific conditions first
if (score >= 90) {
    printf("Excellent!\n");     // This runs (correct!)
} else if (score >= 60) {
    printf("Pass\n");
}
```

**Mistake 3: Dangling else**
```c
// Which 'if' does this 'else' belong to?
if (a > 0)
    if (b > 0)
        printf("Both positive\n");
else
    printf("Which condition failed?\n");

// Answer: 'else' binds to the NEAREST 'if' (the inner one)
// This is confusing! Use braces to make it clear:
if (a > 0) {
    if (b > 0) {
        printf("Both positive\n");
    } else {
        printf("b is not positive\n");
    }
}
```

### Quick Check: `else if` Chains
What does this code print?

```c
int value = 15;

if (value > 20) {
    printf("A");
} else if (value > 10) {
    printf("B");
} else if (value > 5) {
    printf("C");
} else {
    printf("D");
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** `B`

**Explanation:** 
- `value > 20` (15 > 20) is false → skip
- `value > 10` (15 > 10) is true → print "B" and **stop** (don't check remaining conditions)
- Even though `value > 5` is also true, it's never checked because we already found a match.

</details>

---

## 3. Guard Clauses — Early Returns
A **guard clause** checks for invalid conditions at the start of a function and exits early. This pattern keeps your main logic clean and unindented.

> 📚 **Preview:** The examples below use **functions** (covered in [Lesson 4](LESSON_4_Functions.md)). For now, focus on the *pattern* — checking error conditions first and returning early. You'll write your own functions soon!

### Without Guard Clauses (Nested Indentation)
```c
int calculate_score(int base_score, int multiplier) {
    if (base_score >= 0) {
        if (multiplier > 0) {
            int result = base_score * multiplier;
            if (result <= 1000) {
                return result;
            }
        }
    }
    return -1;  // Error
}
```

### With Guard Clauses (Flat Structure)
```c
int calculate_score(int base_score, int multiplier) {
    // Guard: check base_score is valid
    if (base_score < 0) {
        return -1;
    }
    
    // Guard: check multiplier is valid
    if (multiplier <= 0) {
        return -1;
    }

    // Main logic at the "happy path" indentation level
    int result = base_score * multiplier;
    
    // Guard: check result is within bounds
    if (result > 1000) {
        return -1;
    }
    
    return result;
}
```

### Why Guard Clauses Are Better
| Aspect      | Without Guards                  | With Guards              |
|-------------|---------------------------------|--------------------------|
| Readability | Deep nesting is hard to follow  | Flat, linear flow        |
| Error cases | Hidden in nesting               | Visible at the top       |
| Main logic  | Buried at the deepest level     | At natural indentation   |
| Maintenance | Adding checks increases nesting | Adding checks stays flat |

### The Pattern
```c
int validate_move(int position, int board_size) {
    // Guard: check position is non-negative
    if (position < 0) {
        return -1;
    }
    
    // Guard: check position is within bounds
    if (position >= board_size) {
        return -1;
    }
    
    // Main work here (not nested!)
    // ...
    return 0;  // Success
}
```

> 💡 **Important:** When using guard clauses with error return values (like `-1`), make sure to:
> 1. **Document** what the return values mean (e.g., "returns -1 on error")
> 2. **Check** the return value when calling the function
> 3. **Be consistent** with error codes across your codebase
>
> Example:
> ```c
> int result = validate_move(position, 9);
> if (result == -1) {
>     printf("Invalid move!\n");
> }
> ```

> 📚 **Coming later:** In [Lesson 5: Pointers](LESSON_5_Pointers.md), you'll learn about checking for `NULL` pointers — one of the most common uses of guard clauses in C.

---

## 4. The `switch` Statement

The **`switch` statement** is a cleaner way to handle multiple exact-value comparisons against a single variable.

### Basic Syntax
```c
switch (expression) {
    case value1:
        // Code for value1
        break;
    case value2:
        // Code for value2
        break;
    case value3:
    case value4:
        // Code for value3 OR value4 (fall-through)
        break;
    default:
        // Code if no case matches (optional)
        break;
}
```

### Simple Example
```c
int day = 3;

switch (day) {
    case 1:
        printf("Monday\n");
        break;
    case 2:
        printf("Tuesday\n");
        break;
    case 3:
        printf("Wednesday\n");  // This one runs
        break;
    case 4:
        printf("Thursday\n");
        break;
    case 5:
        printf("Friday\n");
        break;
    default:
        printf("Weekend\n");
        break;
}
```

### The Importance of `break`
**Without `break`, execution "falls through" to the next case:**

```c
int x = 1;

switch (x) {
    case 1:
        printf("One\n");
        // Missing break! Falls through to case 2
    case 2:
        printf("Two\n");
        break;
    case 3:
        printf("Three\n");
        break;
}

// Output:
// One
// Two
```

### Intentional Fall-Through
Sometimes fall-through is useful for grouping cases:

```c
char grade = 'B';

switch (grade) {
    case 'A':
    case 'B':
    case 'C':
        printf("Pass\n");
        break;
    case 'D':
    case 'F':
        printf("Fail\n");
        break;
    default:
        printf("Invalid grade\n");
        break;
}
```

### `switch` vs. `if-else`

| Use `switch` when:                                 | Use `if-else` when:          |
|----------------------------------------------------|------------------------------|
| Testing one variable against multiple exact values | Testing complex conditions   |
| Working with integer or character constants        | Comparing ranges (`x > 10`)  |
| Need fall-through behavior                         | Checking different variables |
| Readability is improved                            | Only 1-2 conditions          |

```c
// Better with switch (testing one variable against constants)
switch (day) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        printf("Weekday\n");
        break;
    case 6:
    case 7:
        printf("Weekend\n");
        break;
}

// Better with if-else (range checks)
if (score >= 90) {
    // ...
} else if (score >= 60) {
    // ...
}

// Better with if-else (different variables)
if (x < 0 || y < 0) {
    // ...
}
```

### ⚠️ Common Mistakes with `switch`
**Mistake 1: Forgetting `break`**
```c
int choice = 1;

// BUG: Falls through to case 2!
switch (choice) {
    case 1:
        printf("One\n");
        // Missing break!
    case 2:
        printf("Two\n");
        break;
}
// Output: One Two (probably not what you wanted!)

// CORRECT:
switch (choice) {
    case 1:
        printf("One\n");
        break;
    case 2:
        printf("Two\n");
        break;
}
// Output: One
```

**Mistake 2: Using non-integer types**
```c
// BUG: switch only works with integer types!
float grade = 3.5;
switch (grade) {  // Compilation error!
    case 3.5:     // Not allowed
        // ...
}

// CORRECT: Use if-else for floating-point comparisons
if (grade >= 3.5) {
    // ...
}
```

> 💡 **Note:** `switch` works with any **integer type**: `int`, `char`, `short`, `long`. It does **not** work with `float`, `double`, or strings.

**Mistake 3: Duplicate case values**
```c
// BUG: Compilation error — duplicate case!
switch (value) {
    case 1:
        printf("First\n");
        break;
    case 1:  // Error: duplicate case value
        printf("Also first?\n");
        break;
}
```

**Mistake 4: Variables in case labels**
```c
int MAX = 10;

// BUG: case labels must be compile-time constants!
switch (value) {
    case MAX:     // Error: not a constant expression
        // ...
}

// Also a BUG in C: const doesn't help!
const int MAX_CONST = 10;
switch (value) {
    case MAX_CONST:  // Error in C: still not a constant expression!
        // ...
}

// CORRECT: Use literal values
switch (value) {
    case 10:  // OK — literal constant
        // ...
    case 20:  // OK — literal constant
        // ...
}
```

> 📚 **Coming later:** In [Lesson 3: Named Constants](LESSON_3_Named_Constants.md), you'll learn about `#define` and `enum` — two ways to create named constants that work in `case` labels. For now, just use literal numbers like `case 10:`.

> ⚠️ **C vs C++ difference:** In C++, `const int` creates a compile-time constant that works in `case` labels. In C, `const` just means "read-only" — it's not a compile-time constant.

### Quick Check: `switch` Statement
What does this code print?

```c
int x = 2;

switch (x) {
    case 1:
        printf("A");
    case 2:
        printf("B");
    case 3:
        printf("C");
        break;
    default:
        printf("D");
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** `BC`

**Explanation:** 
- `x` is 2, so we jump to `case 2`
- Print "B"
- No `break`, so fall through to `case 3`
- Print "C"
- `break` exits the switch
- "D" is not printed because we exited before reaching `default`

</details>

---

## 5. The `while` Loop
The **`while` loop** repeats a block of code as long as a condition remains true. The condition is checked **before** each iteration.

### How It Works — Visual Flow
```
      ┌─────────────┐
      │ Start / ... │
      └──────┬──────┘
             │
             ▼
     ┌───────────────┐◄───────────┐
     │ Is condition  │            │
     │    true?      │            │
     └───────┬───────┘            │
             │                    │
        ┌────┴────┐               │
        │         │               │
       Yes        No              │
        │         │               │
        ▼         │               │
 ┌─────────────┐  │               │
 │ Execute     │  │               │
 │ loop body   │──┼───────────────┘
 └─────────────┘  │
                  │
                  ▼
          ┌─────────────┐
          │ Continue... │
          └─────────────┘
```

**Key insight:** The condition is checked **before** the first iteration. If the condition is initially false, the loop body never runs.


### Simple Example
```c
int count = 0;

while (count < 5) {
    printf("Count: %d\n", count);
    count++;
}
// Output: 0, 1, 2, 3, 4
```

### Searching with `while`
```c
// Find the first zero in an array
int arr[] = {5, 3, 0, 8, 2};
int size = 5;
int i = 0;

while (i < size && arr[i] != 0) {
    i++;
}

if (i < size) {
    printf("Found zero at index %d\n", i);
} else {
    printf("No zero found\n");
}
```


### ⚠️ Common Mistakes with `while`
**Mistake 1: Infinite loop — forgetting to update the condition**
```c
// INFINITE LOOP — never terminates!
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    // Forgot to increment i!
}

// CORRECT:
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    i++;  // Don't forget this!
}
```

**Mistake 2: Off-by-one errors**
```c
// Prints 0 through 10 (11 numbers!) — probably not intended
int i = 0;
while (i <= 10) {
    printf("%d\n", i);
    i++;
}

// For exactly 10 numbers (0-9):
int i = 0;
while (i < 10) {  // Use < not <=
    printf("%d\n", i);
    i++;
}
```

**Mistake 3: Semicolon after `while`**
```c
// BUG: Infinite loop! The semicolon creates an empty loop body
int i = 0;
while (i < 10);  // ← Empty loop body! i never increments, so i < 10 is always true
{
    printf("%d\n", i);  // This block runs AFTER the infinite loop (never reached)
    i++;
}

// CORRECT: No semicolon after while
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    i++;
}
```

### Quick Check: `while` Loop
How many times does this loop execute?

```c
int x = 5;
while (x > 0) {
    printf("%d ", x);
    x -= 2;
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** 3 times

**Trace:**
- Iteration 1: `x=5`, print "5 ", `x` becomes 3
- Iteration 2: `x=3`, print "3 ", `x` becomes 1
- Iteration 3: `x=1`, print "1 ", `x` becomes -1
- Check: `-1 > 0` is false → exit

**Output:** `5 3 1 `

</details>

---

## 6. The `do-while` Loop
The **`do-while` loop** is like `while`, but it checks the condition **after** each iteration. This guarantees the loop body runs **at least once**.

### How It Works — Visual Flow
```
      ┌─────────────┐
      │ Start / ... │
      └──────┬──────┘
             │
             ▼
      ┌─────────────┐
      │ Execute     │◄────────────┐
      │ loop body   │             │
      └──────┬──────┘             │
             │                    │
             ▼                    │
     ┌───────────────┐            │
     │ Is condition  │            │
     │    true?      │            │
     └───────┬───────┘            │
             │                    │
        ┌────┴────┐               │
        │         │               │
       Yes        No              │
        │         │               │
        └─────────┼───────────────┘
                  │
                  ▼
          ┌─────────────┐
          │ Continue... │
          └─────────────┘
```

**Key insight:** The body executes **first**, then the condition is checked. The loop always runs at least once.

### When to Use `do-while`
Use `do-while` when you need to execute the loop body first, then decide whether to repeat:

```c
// Example: Print at least once, then continue based on condition
int attempts = 0;
do {
    printf("Attempt %d\n", attempts + 1);
    attempts++;
} while (attempts < 3);
// Output: Attempt 1, Attempt 2, Attempt 3
```

### `while` vs. `do-while`

**`while` — check first:**
```c
int count = 10;
while (count < 5) {
    printf("This never runs\n");
}
```

**`do-while` — execute first:**
```c
int count = 10;
do {
    printf("This runs once\n");
} while (count < 5);
```

### Real-World Example

> 📚 **Preview:** This example shows the pattern for a menu system. It uses `scanf` (covered in later lessons) to read user input. Focus on the do-while structure — the menu displays at least once!

```c
// Menu system that always shows at least once
int choice = 0;  // Initialize to trigger the loop
do {
    printf("1. New Game\n");
    printf("2. Quit\n");
    printf("Choice: ");
    // Imagine we read the user's choice into 'choice' here
    
    if (choice == 1) {
        printf("Starting new game...\n");
    }
} while (choice != 2);
```

### ⚠️ Common Mistakes with `do-while`

**Mistake 1: Forgetting the semicolon**
```c
// BUG: Missing semicolon!
do {
    printf("Hello\n");
} while (condition)  // ← Needs semicolon!

// CORRECT:
do {
    printf("Hello\n");
} while (condition);  // ← Semicolon required!
```

**Mistake 2: Using `do-while` when `while` is clearer**
```c
// Unnecessary do-while — the condition could be checked first
int i = 0;
do {
    if (i >= 10) {
        break;  // Why check inside the loop?
    }
    printf("%d ", i);
    i++;
} while (i < 10);

// CLEARER: Just use while
int i = 0;
while (i < 10) {
    printf("%d ", i);
    i++;
}
```

**Mistake 3: Variable scope — declaring inside the loop body**
```c
// BUG: 'done' goes out of scope before the condition is checked!
do {
    int done = 0;  // Declared inside the block
    // ... some work ...
    done = 1;
} while (!done);  // Error: 'done' is not declared in this scope

// CORRECT: Declare the variable before the loop
int done = 0;
do {
    // ... some work ...
    done = 1;
} while (!done);  // OK: 'done' is in scope here
```

### Quick Check: `do-while` vs `while`
What's the difference in output?

```c
// Code A:
int x = 0;
while (x > 0) {
    printf("A");
    x--;
}

// Code B:
int y = 0;
do {
    printf("B");
    y--;
} while (y > 0);
```

<details>
<summary>Click to reveal answer</summary>

**Answer:**
- Code A: Prints nothing (condition `0 > 0` is false before loop starts)
- Code B: Prints `B` once (body executes, then condition `−1 > 0` is false)

**Key insight:** `do-while` always executes at least once; `while` might not execute at all.

</details>

---

## 7. The `for` Loop
The **`for` loop** is C's most compact loop, designed for counted iteration. It's the workhorse of array processing.

### How It Works — Visual Flow
```
        ┌─────────────────┐
        │  Initialization │  (runs once at the start)
        └────────┬────────┘
                 │
                 ▼
        ┌────────────────┐◄───────────────┐
        │ Is condition   │                │
        │    true?       │                │
        └───────┬────────┘                │
                │                         │
           ┌────┴────┐                    │
           │         │                    │
          Yes        No                   │
           │         │                    │
           ▼         │          ┌─────────┴─────────┐
    ┌─────────────┐  │          │  Update expression │
    │ Execute     │  │          │  (i++, etc.)       │
    │ loop body   │──┼─────────►└───────────────────┘
    └─────────────┘  │
                     │
                     ▼
             ┌─────────────┐
             │ Continue... │
             └─────────────┘
```

**Execution order:**
1. **Initialization** runs once at the start
2. **Condition** is checked before each iteration
3. **Loop body** runs if condition is true
4. **Update** runs after each iteration
5. Go back to step 2

### Simple Example
```c
for (int i = 0; i < 5; i++) {
    printf("%d\n", i);
}
// Output: 0, 1, 2, 3, 4
```

**Equivalent `while` loop:**
```c
int i = 0;           // Initialization
while (i < 5) {      // Condition
    printf("%d\n", i);
    i++;             // Update
}
```

### Why `for` is Better for Counting
All the loop control is in one place:

```c
// for loop — everything visible at a glance
for (int i = 0; i < 10; i++) {
    printf("%d ", i);
}

// while loop — control spread across multiple lines
int i = 0;
while (i < 10) {
    printf("%d ", i);
    i++;
}
```

### Common Patterns
**Counting up:**
```c
for (int i = 0; i < 10; i++) {
    // i: 0, 1, 2, ..., 9
}
```

**Counting down:**
```c
for (int i = 9; i >= 0; i--) {
    // i: 9, 8, 7, ..., 0
}
```

**Stepping by 2:**
```c
for (int i = 0; i < 10; i += 2) {
    // i: 0, 2, 4, 6, 8
}
```

**Processing an array:**
```c
int scores[5] = {80, 90, 75, 88, 92};

for (int i = 0; i < 5; i++) {
    printf("Score %d: %d\n", i, scores[i]);
}
```


### Multiple Variables in `for`
You can use the comma operator to manage multiple loop variables:

```c
// Two counters
for (int i = 0, j = 10; i < j; i++, j--) {
    printf("i=%d, j=%d\n", i, j);
}
// Output:
// i=0, j=10
// i=1, j=9
// i=2, j=8
// ...
```

### Infinite `for` Loop
Any part of the `for` can be empty:

```c
for (;;) {
    // Infinite loop (equivalent to while(1))
    // Must use break to exit
}
```

### ⚠️ Common Mistakes with `for`

**Mistake 1: Off-by-one error**
```c
// BUG: Runs 11 times (0 through 10) when you probably wanted 10!
for (int i = 0; i <= 10; i++) {  // <= instead of <
    printf("%d\n", i);
}

// CORRECT: Use < for 0-based counting
for (int i = 0; i < 10; i++) {  // Runs exactly 10 times (0 through 9)
    printf("%d\n", i);
}
```

> 📚 **Coming later:** In [Lesson 2: Arrays](LESSON_2_Arrays.md), you'll learn why `<` is especially important — using `<=` with arrays causes out-of-bounds access!

**Mistake 2: Semicolon after `for`**
```c
// BUG: Empty loop body due to semicolon!
for (int i = 0; i < 5; i++);  // ← Semicolon creates empty loop body
{
    printf("Iteration %d\n", i);  // Compile error: 'i' is out of scope!
}
printf("Done\n");

// What actually happens:
// 1. for loop runs 5 times with empty body
// 2. 'i' goes out of scope after the loop
// 3. The block {} is separate and can't access 'i'

// CORRECT: No semicolon after for
for (int i = 0; i < 5; i++) {
    printf("Iteration %d\n", i);
}
```

**Mistake 3: Modifying the loop variable inside the loop**
```c
// CONFUSING: What does this print?
for (int i = 0; i < 10; i++) {
    printf("%d ", i);
    i++;  // Incrementing again! Skips values
}
// Output: 0 2 4 6 8 (probably not intended)

// If you need to skip, use continue instead:
for (int i = 0; i < 10; i++) {
    if (i % 2 == 1) {
        continue;  // Skip odd numbers
    }
    printf("%d ", i);
}
// Output: 0 2 4 6 8 (clear intent)
```

**Mistake 4: Using wrong comparison for counting down**
```c
// BUG: Infinite loop! i is unsigned, never negative
for (unsigned int i = 10; i >= 0; i--) {
    printf("%u\n", i);
    // When i becomes 0 and decrements, it wraps to 4294967295!
}

// CORRECT: Use > 0 and adjust logic, or use signed int
for (int i = 10; i >= 0; i--) {  // Signed int can be negative
    printf("%d\n", i);
}
```

### Quick Check: `for` Loop
What values does `i` take in each iteration?

```c
for (int i = 2; i < 8; i += 3) {
    printf("%d ", i);
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** `i` takes values `2` and `5`

**Trace:**
- Start: `i = 2`
- Iteration 1: `2 < 8` ✓, print "2 ", `i` becomes `2 + 3 = 5`
- Iteration 2: `5 < 8` ✓, print "5 ", `i` becomes `5 + 3 = 8`
- Check: `8 < 8` ✗ → exit

**Output:** `2 5 `

</details>

---

## 8. Nested Loops
A **nested loop** is a loop inside another loop. The inner loop runs completely for each iteration of the outer loop.

### Basic Example
```c
for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
        printf("(%d,%d) ", row, col);
    }
    printf("\n");
}

// Output:
// (0,0) (0,1) (0,2) 
// (1,0) (1,1) (1,2) 
// (2,0) (2,1) (2,2)
```

### Processing a 2D Grid with a 1D Array

> 📚 **Preview:** This example uses **arrays** (covered in [Lesson 2](LESSON_2_Arrays.md)). For now, just notice how the nested loop calculates `index = row * 3 + col` to convert 2D coordinates to a 1D position.

```c
// A 3x3 grid stored as 1D array (you'll learn array syntax in Lesson 2)
int grid[9];

// Process row by row
for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
        int index = row * 3 + col;  // Convert 2D to 1D
        printf("Position %d is at (%d,%d)\n", index, row, col);
    }
}
// Output shows: Position 0 is at (0,0), Position 1 is at (0,1), etc.
```


### Triple-Nested Loops
```c
// Example: 3D grid iteration
for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            // Process voxel at (x, y, z)
        }
    }
}
```

> ⚠️ **Performance note:** Nested loops multiply iterations. A 100×100 double loop runs 10,000 times. A 100×100×100 triple loop runs 1,000,000 times!

### Quick Check: Nested Loops
In what order are these coordinates printed?

```c
for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 3; col++) {
        printf("(%d,%d) ", row, col);
    }
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** `(0,0) (0,1) (0,2) (1,0) (1,1) (1,2) `

**Explanation:**
- Outer loop starts with `row = 0`
- Inner loop runs completely: `col = 0, 1, 2`
- Prints `(0,0) (0,1) (0,2)`
- Outer loop increments to `row = 1`
- Inner loop runs completely again: `col = 0, 1, 2`
- Prints `(1,0) (1,1) (1,2)`
- Outer loop checks `2 < 2` → false, exits

**Key insight:** The inner loop "resets" and runs fully for each iteration of the outer loop.

</details>

---

## 9. `break` — Exit Early

The **`break` statement** immediately exits the innermost loop or switch.

### Breaking Out of a Loop
```c
// Find the first multiple of 7 between 1 and 100
for (int i = 1; i <= 100; i++) {
    if (i % 7 == 0) {
        printf("Found first multiple of 7: %d\n", i);
        break;  // Stop searching — prints 7 and exits
    }
}
```

### Breaking from Nested Loops
`break` only exits the **innermost** loop:

```c
// Find a specific coordinate in a grid
for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
        int value = row * 3 + col;  // Calculate position value
        if (value == 4) {
            printf("Found 4 at (%d,%d)\n", row, col);
            break;  // Exits inner loop only
        }
    }
    // Outer loop continues!
}
```

**To break out of multiple loops, use a flag:**
```c
int found = 0;
for (int row = 0; row < 3 && !found; row++) {
    for (int col = 0; col < 3; col++) {
        int value = row * 3 + col;
        if (value == 4) {
            printf("Found target at (%d,%d)\n", row, col);
            found = 1;
            break;  // Exits inner loop
        }
    }
    // Outer loop checks !found and exits
}
```

### `break` in `switch`
```c
switch (choice) {
    case 1:
        printf("Option 1 selected\n");
        break;  // Exit switch, not a loop
    case 2:
        printf("Option 2 selected\n");
        break;
    default:
        printf("Unknown option\n");
        break;
}
```

---

## 10. `continue` — Skip to Next Iteration
The **`continue` statement** skips the rest of the current iteration and jumps to the next one.

### Basic Example
```c
// Print only even numbers from 0 to 9
for (int i = 0; i < 10; i++) {
    if (i % 2 != 0) {
        continue;  // Skip odd numbers
    }
    printf("%d ", i);
}
// Output: 0 2 4 6 8
```

### `continue` vs. `break`

| Statement  | Effect                     |
|------------|----------------------------|
| `break`    | Exit the loop completely   |
| `continue` | Skip to the next iteration |

```c
for (int i = 0; i < 5; i++) {
    if (i == 2) {
        break;  // Stops at 2
    }
    printf("%d ", i);
}
// Output: 0 1

for (int i = 0; i < 5; i++) {
    if (i == 2) {
        continue;  // Skips 2
    }
    printf("%d ", i);
}
// Output: 0 1 3 4
```

### Real-World Use Case
```c
// Process only numbers divisible by 3
for (int i = 1; i <= 20; i++) {
    if (i % 3 != 0) {
        continue;  // Skip numbers not divisible by 3
    }
    
    // Only multiples of 3 reach here
    printf("Processing: %d\n", i);
}
```

### In `while` and `do-while`
```c
int i = 0;
while (i < 5) {
    i++;
    if (i == 3) {
        continue;  // Jumps back to while condition
    }
    printf("%d ", i);
}
// Output: 1 2 4 5
```

> ⚠️ **Caution with `while`:** Put the increment **before** `continue`, or you'll create an infinite loop!

---

## 11. Infinite Loops and Input Validation
An **infinite loop** runs forever unless you explicitly exit it with `break` or `return`. This is the standard pattern for input validation.

### The `while (1)` Pattern
```c
while (1) {
    // Runs forever
}

// Equivalent forms (all mean "infinite loop"):
while (1) { }
for (;;) { }
```

### Input Validation Pattern
> 📚 **Preview:** This example uses `scanf` with the `&` operator and functions. You'll learn about these in [Lesson 4: Functions](LESSON_4_Functions.md) and [Lesson 5: Pointers](LESSON_5_Pointers.md). For now, focus on the *loop pattern* — keep asking until you get valid input.

```c
// Pseudocode for the pattern:
while (1) {
    // 1. Prompt user for input
    // 2. Read input
    // 3. Validate input
    //    - If invalid: print error, continue (back to step 1)
    //    - If valid: break or return (exit the loop)
}
```

**Simple example using just what we know:**
```c
int guess = 0;
int secret = 7;

while (1) {
    printf("Guess a number (1-10): ");
    // Imagine we got a number from the user into 'guess'
    // (We'll learn how to actually read input in later lessons)
    
    if (guess < 1 || guess > 10) {
        printf("Out of range! Try again.\n");
        continue;  // Go back to the start of the loop
    }
    
    if (guess == secret) {
        printf("Correct!\n");
        break;  // Exit the loop
    }
    
    printf("Wrong! Try again.\n");
}
```

Key advantages:
- ✅ Guarantees valid input before proceeding
- ✅ Clear structure: keep looping until success
- ✅ User gets immediate feedback on errors
- ✅ No need to repeat prompt code

### Quick Check: Infinite Loop with `break`
What does this code print?

```c
int count = 0;
while (1) {
    count++;
    printf("%d ", count);
    if (count >= 3) {
        break;
    }
}
printf("Done!\n");
```

<details>
<summary>Click to reveal answer</summary>

**Output:** `1 2 3 Done!`

**Explanation:**
1. `count` becomes 1, print "1 ", `1 >= 3` is false, continue looping
2. `count` becomes 2, print "2 ", `2 >= 3` is false, continue looping
3. `count` becomes 3, print "3 ", `3 >= 3` is true, `break` exits the loop
4. Print "Done!"

**Key insight:** `while (1)` runs forever unless you use `break` (or `return` in a function) to exit.

</details>

### Quick Check: Combining Patterns

What state transitions occur in this game loop?

```c
int state = 0;  // 0=menu, 1=play, 2=quit
int lives = 3;

while (state != 2) {
    switch (state) {
        case 0:
            state = 1;  // Auto-start for this example
            lives = 3;
            break;
        case 1:
            lives--;
            if (lives <= 0) {
                state = 0;
            }
            if (lives == 1) {
                printf("Last life!\n");
            }
            break;
    }
}
```

<details>
<summary>Click to reveal answer</summary>

**State transitions:**
1. Start: `state=0, lives=3`
2. Case 0: `state=1, lives=3` (starts game)
3. Case 1: `lives=2` (no state change)
4. Case 1: `lives=1`, prints "Last life!"
5. Case 1: `lives=0`, `state=0` (back to menu)
6. Case 0: `state=1, lives=3` (restarts game)
7. ... (continues forever — there's no way to reach state=2!)

**Key insight:** This loop runs forever because no code ever sets `state = 2`. Always ensure your loops have a valid exit condition!

</details>

---

## 12. Real Code Examples from This Project

> 📚 **Come back later!** This section references implementations that use concepts from later lessons:
> - **Arrays** ([Lesson 2](LESSON_2_Arrays.md))
> - **Functions** ([Lesson 4](LESSON_4_Functions.md))
> - **Pointers** ([Lesson 5](LESSON_5_Pointers.md))
>
> Bookmark this section and return after completing those lessons to see control flow patterns in real code!

Once you've mastered arrays, functions, and pointers, you can see control flow in action in the project's algorithm implementations:

### Win Condition Calculator
The win condition checker demonstrates:
- **Guard clauses** for early returns
- **Nested `for` loops** for checking rows, columns, and diagonals
- **Early exit patterns** when a winner is found

📖 **See:** [Win Condition Calculator Algorithm](../algorithms/WIN_CONDITION_CALCULATOR.md)

### CLI Board Display
The board display function demonstrates:
- **Nested loops** for row/column iteration
- **Conditional formatting** inside loops
- **2D to 1D array index conversion** (`row * 3 + col`)

📖 **See:** [CLI Display Board Algorithm](../algorithms/CLI_DISPLAY_BOARD.md)

### Control Flow Patterns You'll Recognize

| Pattern                        | What It Does                    | Covered In   |
|--------------------------------|---------------------------------|--------------|
| Guard clause + early return    | Handle errors at function start | Section 3    |
| Nested `for` loops             | Process 2D grids                | Section 8    |
| Loop with early `break`        | Stop searching when found       | Section 9    |
| `continue` to skip iterations  | Filter out unwanted values      | Section 10   |

---

## 13. Practical Exercises

> 🎯 **How to use these exercises:**
> 1. Try to solve each exercise **before** revealing the answer
> 2. Write your code in a file and test it — don't just read the solutions
> 3. If you get stuck, try breaking the problem into smaller pieces
> 4. Compare your solution with the provided answer — there's often more than one correct approach!
>
> Exercises are organized by difficulty:
> - **Exercises 1-5:** Reading & tracing control flow
> - **Exercises 6-10:** Writing basic loops
> - **Exercises 11-15:** Debugging & algorithm exercises
> - **Exercises 16-20:** Challenges (patterns, nested loops)
> - **Exercises 21-25:** State machines, arrays, and games
> - **Exercises 16-20:** Challenges using nested loops and patterns

---

### Exercise 1: Reading Control Flow
What does this code print? Trace through it step by step:

```c
int x = 0;
int y = 10;

while (x < 5) {
    if (x % 2 == 0) {
        y += x;
    }
    x++;
}

printf("x=%d, y=%d\n", x, y);
```

<details>
<summary>Click to reveal answer</summary>

**Trace:**
- Start: `x=0, y=10`
- Iteration 1: `x=0`, `0 % 2 == 0` is true, `y = 10 + 0 = 10`, `x=1`
- Iteration 2: `x=1`, `1 % 2 == 0` is false (skip), `x=2`
- Iteration 3: `x=2`, `2 % 2 == 0` is true, `y = 10 + 2 = 12`, `x=3`
- Iteration 4: `x=3`, `3 % 2 == 0` is false (skip), `x=4`
- Iteration 5: `x=4`, `4 % 2 == 0` is true, `y = 12 + 4 = 16`, `x=5`
- Loop ends: `x=5` is not `< 5`

**Output:** `x=5, y=16`

</details>

---

### Exercise 2: `break` vs. `continue`
What's the difference in output between these two loops?

```c
// Loop A
for (int i = 0; i < 5; i++) {
    if (i == 3) {
        break;
    }
    printf("%d ", i);
}
printf("\n");

// Loop B
for (int i = 0; i < 5; i++) {
    if (i == 3) {
        continue;
    }
    printf("%d ", i);
}
printf("\n");
```

<details>
<summary>Click to reveal answer</summary>

**Loop A output:** `0 1 2`
- When `i` reaches 3, `break` exits the entire loop

**Loop B output:** `0 1 2 4`
- When `i` reaches 3, `continue` skips the `printf` but the loop continues with `i=4`

</details>

---

### Exercise 3: Nested Loops
How many times does "Hello" print?

```c
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("Hello\n");
    }
}
```

<details>
<summary>Click to reveal answer</summary>

**Answer:** 9 times

**Explanation:**
- Outer loop runs 3 times (`i = 0, 1, 2`)
- For each outer iteration, inner loop runs 3 times (`j = 0, 1, 2`)
- Total: 3 × 3 = 9

</details>

---

### Exercise 4: Switch Statement
What does this code print for each value of `day`?

```c
int day = 3;

switch (day) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        printf("Weekday\n");
        break;
    case 6:
    case 7:
        printf("Weekend\n");
        break;
    default:
        printf("Invalid day\n");
        break;
}
```

Test with `day = 1`, `day = 6`, and `day = 0`.

<details>
<summary>Click to reveal answer</summary>

- `day = 1`: Prints "Weekday" (falls through cases 1-5)
- `day = 6`: Prints "Weekend" (falls through cases 6-7)
- `day = 0`: Prints "Invalid day" (no matching case, uses default)

**Key insight:** Cases 1-5 all "fall through" to the same `printf` because there's no `break` between them.

</details>

---

### Exercise 5: Tracing a `do-while` Loop
What does this code print?

```c
int n = 1;
int sum = 0;

do {
    sum += n;
    n++;
} while (n <= 5);

printf("Sum: %d\n", sum);
```

<details>
<summary>Click to reveal answer</summary>

**Trace:**
- Start: `n=1, sum=0`
- Iteration 1: `sum = 0 + 1 = 1`, `n = 2`, check `2 <= 5` ✓
- Iteration 2: `sum = 1 + 2 = 3`, `n = 3`, check `3 <= 5` ✓
- Iteration 3: `sum = 3 + 3 = 6`, `n = 4`, check `4 <= 5` ✓
- Iteration 4: `sum = 6 + 4 = 10`, `n = 5`, check `5 <= 5` ✓
- Iteration 5: `sum = 10 + 5 = 15`, `n = 6`, check `6 <= 5` ✗

**Output:** `Sum: 15`

This calculates the sum of numbers from 1 to 5: 1+2+3+4+5 = 15.

</details>

---

### Exercise 6: Write a Loop — Sum of Numbers
Write code that calculates and prints the sum of all integers from 1 to 100.

<details>
<summary>Click to reveal answer</summary>

```c
int sum = 0;

for (int i = 1; i <= 100; i++) {
    sum += i;
}

printf("Sum: %d\n", sum);
// Output: Sum: 5050
```

**Alternative with `while`:**
```c
int sum = 0;
int i = 1;

while (i <= 100) {
    sum += i;
    i++;
}

printf("Sum: %d\n", sum);
```

**Mathematical note:** The sum 1+2+3+...+n = n×(n+1)/2. For n=100: 100×101/2 = 5050.

</details>

---

### Exercise 7: Write a Loop — Count Digits
Write code that counts how many digits are in a positive integer. For example, 12345 has 5 digits.

```c
int number = 12345;
// Your code here to count the digits
```

<details>
<summary>Click to reveal answer</summary>

```c
int number = 12345;
int count = 0;
int temp = number;

while (temp > 0) {
    count++;
    temp /= 10;  // Remove the last digit
}

printf("%d has %d digits\n", number, count);  // Output: 12345 has 5 digits
```

**Edge case:** What if `number = 0`? The loop doesn't run, so `count = 0`. You might want to handle this specially:
```c
if (number == 0) {
    count = 1;  // Zero has one digit
}
```

</details>

---

### Exercise 8: Write a Loop — Reverse a Number
Write code that reverses the digits of a positive integer. For example, 12345 becomes 54321.

```c
int number = 12345;
// Your code here to reverse it
```

<details>
<summary>Click to reveal answer</summary>

```c
int number = 12345;
int reversed = 0;

while (number > 0) {
    int digit = number % 10;      // Get last digit
    reversed = reversed * 10 + digit;  // Append to result
    number /= 10;                 // Remove last digit
}

printf("Reversed: %d\n", reversed);  // Output: Reversed: 54321
```

**Trace for 12345:**
| Step | `number` | `digit` | `reversed` |
|------|----------|---------|------------|
| Start | 12345 | - | 0 |
| 1 | 1234 | 5 | 5 |
| 2 | 123 | 4 | 54 |
| 3 | 12 | 3 | 543 |
| 4 | 1 | 2 | 5432 |
| 5 | 0 | 1 | 54321 |

</details>

---

### Exercise 9: Write a Loop — Factorial
Write code that calculates the factorial of a number. Factorial of n (written n!) is the product of all positive integers up to n. For example, 5! = 5×4×3×2×1 = 120.

```c
int n = 5;
// Your code here to calculate n!
```

<details>
<summary>Click to reveal answer</summary>

```c
int n = 5;
int factorial = 1;

for (int i = 1; i <= n; i++) {
    factorial *= i;
}

printf("%d! = %d\n", n, factorial);  // Output: 5! = 120
```

**Alternative counting down:**
```c
int n = 5;
int factorial = 1;

while (n > 1) {
    factorial *= n;
    n--;
}

printf("Factorial: %d\n", factorial);
```

**Note:** Factorials grow very quickly! 13! = 6,227,020,800 which overflows a 32-bit `int`. For larger factorials, you'd need `long long`.

</details>

---

### Exercise 10: Write a Loop — Power Calculation
Write code that calculates x raised to the power n (x^n) without using any library functions. Assume n is a non-negative integer.

```c
int x = 2;
int n = 10;
// Your code here to calculate x^n
```

<details>
<summary>Click to reveal answer</summary>

```c
int x = 2;
int n = 10;
int result = 1;

for (int i = 0; i < n; i++) {
    result *= x;
}

printf("%d^%d = %d\n", x, n, result);  // Output: 2^10 = 1024
```

**Alternative with `while`:**
```c
int x = 2;
int n = 10;
int result = 1;
int count = n;

while (count > 0) {
    result *= x;
    count--;
}

printf("%d^%d = %d\n", x, n, result);  // Output: 2^10 = 1024
```

**Edge case:** When `n = 0`, the loop doesn't run, so `result = 1`. This is correct since x^0 = 1 for any x.

</details>

---

### Exercise 11: Debug the Bug
This code is supposed to print numbers 1 through 10, but it has a bug. Find and fix it.

```c
for (int i = 1; i <= 10; i++); {
    printf("%d\n", i);
}
```

<details>
<summary>Click to reveal answer</summary>

**The bug:** There's a semicolon after the `for` statement!

```c
for (int i = 1; i <= 10; i++);  // ← This semicolon creates an empty loop body!
{
    printf("%d\n", i);  // Error: 'i' is out of scope here
}
```

**What happens:**
1. The `for` loop runs 10 times with an empty body (the semicolon)
2. After the loop, `i` is out of scope
3. The `printf` block is not part of the loop at all

**Fixed code:**
```c
for (int i = 1; i <= 10; i++) {  // No semicolon!
    printf("%d\n", i);
}
```

**Lesson:** Watch out for accidental semicolons after `for`, `while`, and `if`!

</details>

---

### Exercise 12: FizzBuzz
Write a program that prints numbers from 1 to 20, but:
- For multiples of 3, print "Fizz" instead of the number
- For multiples of 5, print "Buzz" instead of the number
- For multiples of both 3 and 5, print "FizzBuzz"

<details>
<summary>Click to reveal answer</summary>

```c
for (int i = 1; i <= 20; i++) {
    if (i % 3 == 0 && i % 5 == 0) {
        printf("FizzBuzz\n");
    } else if (i % 3 == 0) {
        printf("Fizz\n");
    } else if (i % 5 == 0) {
        printf("Buzz\n");
    } else {
        printf("%d\n", i);
    }
}
```

**Output:**
```
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
16
17
Fizz
19
Buzz
```

**Key insight:** Check the most specific condition (`% 3 == 0 && % 5 == 0`) first!

**Alternative approach (build string):**
```c
for (int i = 1; i <= 20; i++) {
    int printed = 0;
    
    if (i % 3 == 0) {
        printf("Fizz");
        printed = 1;
    }
    if (i % 5 == 0) {
        printf("Buzz");
        printed = 1;
    }
    if (!printed) {
        printf("%d", i);
    }
    printf("\n");
}
```

</details>

---

### Exercise 13: Find Divisors
Write code that prints all divisors of a given number. For example, divisors of 12 are: 1, 2, 3, 4, 6, 12.

```c
int number = 12;
// Your code here
```

<details>
<summary>Click to reveal answer</summary>

```c
int number = 12;

printf("Divisors of %d: ", number);

for (int i = 1; i <= number; i++) {
    if (number % i == 0) {
        printf("%d ", i);
    }
}
printf("\n");
// Output: Divisors of 12: 1 2 3 4 6 12
```

</details>

---

### Exercise 14: Check Prime Number
Write code that checks if a given number is prime. A prime number is only divisible by 1 and itself.

```c
int number = 17;
// Your code here to check if it's prime
```

<details>
<summary>Click to reveal answer</summary>

```c
int number = 17;
int is_prime = 1;  // Assume prime until proven otherwise

if (number <= 1) {
    is_prime = 0;  // 0 and 1 are not prime
} else {
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            is_prime = 0;  // Found a divisor, not prime
            break;
        }
    }
}

if (is_prime) {
    printf("%d is prime\n", number);
} else {
    printf("%d is not prime\n", number);
}
// Output: 17 is prime
```

**Why `i * i <= number`?** If a number has a divisor greater than its square root, it must also have one smaller than its square root. So we only need to check up to √n.

**Test cases:**
- `number = 2` → prime (smallest prime)
- `number = 4` → not prime (2×2)
- `number = 17` → prime
- `number = 1` → not prime (by definition)

</details>

---

### Exercise 15: Collatz Conjecture
The Collatz conjecture says: start with any positive integer n. If n is even, divide by 2. If n is odd, multiply by 3 and add 1. Repeat until you reach 1.

Write code that prints the Collatz sequence starting from n=13.

<details>
<summary>Click to reveal answer</summary>

```c
int n = 13;

printf("Collatz sequence starting at %d:\n", n);

while (n != 1) {
    printf("%d -> ", n);
    
    if (n % 2 == 0) {
        n = n / 2;      // Even: divide by 2
    } else {
        n = 3 * n + 1;  // Odd: multiply by 3, add 1
    }
}
printf("1\n");
```

**Output:**
```
Collatz sequence starting at 13:
13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
```

**Using ternary operator:**
```c
int n = 13;

while (n != 1) {
    printf("%d -> ", n);
    n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
}
printf("1\n");
```

**Fun fact:** No one has proven that this sequence always reaches 1 for all starting values, but it's been verified 
computationally for extremely large numbers (well beyond 10^20)!

</details>

---

### Exercise 16: Challenge — Print a Triangle
Write code that prints this triangle pattern for a given height:

```
n=5:
*
**
***
****
*****
```

<details>
<summary>Click to reveal answer</summary>

```c
int n = 5;

for (int row = 1; row <= n; row++) {
    for (int col = 0; col < row; col++) {
        printf("*");
    }
    printf("\n");
}
```

**How it works:**
- Outer loop: iterate through rows (1 to n)
- Inner loop: print `row` number of stars
- Each row prints more stars than the previous

**Variation — right-aligned triangle:**
```c
int n = 5;

for (int row = 1; row <= n; row++) {
    // Print leading spaces
    for (int space = 0; space < n - row; space++) {
        printf(" ");
    }
    // Print stars
    for (int star = 0; star < row; star++) {
        printf("*");
    }
    printf("\n");
}

// Output:
//     *
//    **
//   ***
//  ****
// *****
```

</details>

---

### Exercise 17: Challenge — Multiplication Table
Write code that prints a multiplication table from 1×1 to 10×10.

<details>
<summary>Click to reveal answer</summary>

```c
// Print header row
printf("    ");
for (int i = 1; i <= 10; i++) {
    printf("%4d", i);
}
printf("\n");

// Print separator
printf("    ");
for (int i = 1; i <= 10; i++) {
    printf("----");
}
printf("\n");

// Print table rows
for (int row = 1; row <= 10; row++) {
    printf("%2d |", row);
    for (int col = 1; col <= 10; col++) {
        printf("%4d", row * col);
    }
    printf("\n");
}
```

**Simpler version without formatting:**
```c
for (int row = 1; row <= 10; row++) {
    for (int col = 1; col <= 10; col++) {
        printf("%d x %d = %d\t", row, col, row * col);
    }
    printf("\n");
}
```

</details>

---

### Exercise 18: Challenge — Diamond Pattern
Write code that prints a diamond pattern for a given size (use an odd number):

```
n=5:
  *
 ***
*****
 ***
  *
```

<details>
<summary>Click to reveal answer</summary>

```c
int n = 5;
int mid = n / 2;  // Middle row index

for (int row = 0; row < n; row++) {
    int stars;
    int spaces;
    
    if (row <= mid) {
        // Top half (including middle): growing
        stars = 1 + 2 * row;
        spaces = mid - row;
    } else {
        // Bottom half: shrinking
        stars = 1 + 2 * (n - 1 - row);
        spaces = row - mid;
    }
    
    // Print leading spaces
    for (int s = 0; s < spaces; s++) {
        printf(" ");
    }
    
    // Print stars
    for (int st = 0; st < stars; st++) {
        printf("*");
    }
    
    printf("\n");
}
```

**Alternative — separate top and bottom halves:**
```c
int n = 5;
int mid = n / 2;

// Top half (including middle)
for (int i = 0; i <= mid; i++) {
    for (int s = 0; s < mid - i; s++) printf(" ");
    for (int st = 0; st < 2 * i + 1; st++) printf("*");
    printf("\n");
}

// Bottom half
for (int i = mid - 1; i >= 0; i--) {
    for (int s = 0; s < mid - i; s++) printf(" ");
    for (int st = 0; st < 2 * i + 1; st++) printf("*");
    printf("\n");
}
```

</details>

---

### Exercise 19: Challenge — Number Pyramid
Write code that prints this number pyramid:

```
n=5:
    1
   121
  12321
 1234321
123454321
```

<details>
<summary>Click to reveal answer</summary>

```c
int n = 5;

for (int row = 1; row <= n; row++) {
    // Print leading spaces
    for (int s = 0; s < n - row; s++) {
        printf(" ");
    }
    
    // Print ascending numbers (1 to row)
    for (int num = 1; num <= row; num++) {
        printf("%d", num);
    }
    
    // Print descending numbers (row-1 down to 1)
    for (int num = row - 1; num >= 1; num--) {
        printf("%d", num);
    }
    
    printf("\n");
}
```

**Trace for row 4:**
- Spaces: 1 (n - row = 5 - 4)
- Ascending: 1, 2, 3, 4
- Descending: 3, 2, 1
- Output: ` 1234321`

</details>

---

### Exercise 20: Challenge — GCD (Greatest Common Divisor)
Write code that finds the greatest common divisor (GCD) of two numbers using the Euclidean algorithm:
- GCD(a, 0) = a
- GCD(a, b) = GCD(b, a % b)

Find the GCD of 48 and 18.

<details>
<summary>Click to reveal answer</summary>

```c
int a = 48;
int b = 18;

// Save original values for printing
int original_a = a;
int original_b = b;

while (b != 0) {
    int temp = b;
    b = a % b;
    a = temp;
}

printf("GCD(%d, %d) = %d\n", original_a, original_b, a);
// Output: GCD(48, 18) = 6
```

**Trace:**
| Step | a | b | a % b |
|------|---|---|-------|
| Start | 48 | 18 | - |
| 1 | 18 | 12 | 48 % 18 = 12 |
| 2 | 12 | 6 | 18 % 12 = 6 |
| 3 | 6 | 0 | 12 % 6 = 0 |
| End | 6 | 0 | - |

**Result:** GCD(48, 18) = 6

**Using do-while:**
```c
int a = 48;
int b = 18;

do {
    int remainder = a % b;
    a = b;
    b = remainder;
} while (b != 0);

printf("GCD = %d\n", a);
```

**Verification:** 
- 48 = 6 × 8
- 18 = 6 × 3
- 6 is indeed the largest number that divides both!

</details>

---

### Exercise 21: State Machine — Traffic Light
Write a program that simulates a traffic light. The light cycles: GREEN (3 seconds) → YELLOW (1 second) → RED (4 seconds) → GREEN → ...

Print the state for each "second" (iteration). Run for 16 seconds total.

<details>
<summary>Click to reveal answer</summary>

```c
#define GREEN  0
#define YELLOW 1
#define RED    2

int state = GREEN;
int timer = 0;

for (int second = 1; second <= 16; second++) {
    // Print current state
    switch (state) {
        case GREEN:  printf("Second %2d: GREEN\n", second);  break;
        case YELLOW: printf("Second %2d: YELLOW\n", second); break;
        case RED:    printf("Second %2d: RED\n", second);    break;
    }
    
    timer++;
    
    // Check for state transition
    switch (state) {
        case GREEN:
            if (timer >= 3) {
                state = YELLOW;
                timer = 0;
            }
            break;
        case YELLOW:
            if (timer >= 1) {
                state = RED;
                timer = 0;
            }
            break;
        case RED:
            if (timer >= 4) {
                state = GREEN;
                timer = 0;
            }
            break;
    }
}
```

**Output:**
```
Second  1: GREEN
Second  2: GREEN
Second  3: GREEN
Second  4: YELLOW
Second  5: RED
Second  6: RED
Second  7: RED
Second  8: RED
Second  9: GREEN
...
```

**Key insight:** State machines separate "what state are we in" from "what triggers a transition."

</details>

---

### Exercise 22: Find First and Last Occurrence
Given an array of integers, find both the **first** and **last** position where a target value appears.

```c
int arr[] = {5, 3, 7, 3, 8, 3, 2};
int size = 7;
int target = 3;
// Find first and last occurrence of 3
```

<details>
<summary>Click to reveal answer</summary>

```c
int arr[] = {5, 3, 7, 3, 8, 3, 2};
int size = 7;
int target = 3;

int first = -1;  // -1 means "not found"
int last = -1;

for (int i = 0; i < size; i++) {
    if (arr[i] == target) {
        if (first == -1) {
            first = i;  // First time we found it
        }
        last = i;  // Update last every time we find it
    }
}

if (first != -1) {
    printf("First occurrence of %d: index %d\n", target, first);
    printf("Last occurrence of %d: index %d\n", target, last);
} else {
    printf("%d not found in array\n", target);
}
// Output:
// First occurrence of 3: index 1
// Last occurrence of 3: index 5
```

**Alternative with two loops (more explicit):**
```c
// Find first (front to back)
int first = -1;
for (int i = 0; i < size; i++) {
    if (arr[i] == target) {
        first = i;
        break;  // Stop at first match
    }
}

// Find last (back to front)
int last = -1;
for (int i = size - 1; i >= 0; i--) {
    if (arr[i] == target) {
        last = i;
        break;  // Stop at first match (from end)
    }
}
```

</details>

---

### Exercise 23: Menu System with Validation
Write a menu system that:
1. Shows options: 1) Say Hello, 2) Say Goodbye, 3) Quit
2. Validates input (must be 1, 2, or 3)
3. Keeps running until the user chooses Quit

<details>
<summary>Click to reveal answer</summary>

```c
int choice;
int running = 1;

while (running) {
    // Display menu
    printf("\n=== Menu ===\n");
    printf("1) Say Hello\n");
    printf("2) Say Goodbye\n");
    printf("3) Quit\n");
    printf("Enter choice: ");
    
    // Get input (simplified — assume scanf works)
    if (scanf("%d", &choice) != 1) {
        printf("Please enter a number!\n");
        while (getchar() != '\n');  // Clear input buffer
        continue;
    }
    
    // Process choice
    switch (choice) {
        case 1:
            printf("Hello, World!\n");
            break;
        case 2:
            printf("Goodbye, World!\n");
            break;
        case 3:
            printf("Exiting...\n");
            running = 0;  // Exit the loop
            break;
        default:
            printf("Invalid choice! Please enter 1, 2, or 3.\n");
            break;
    }
}
```

**Key patterns used:**
- `while (running)` — loop with flag variable
- `switch` — handle different menu options
- `continue` — skip to next iteration on invalid input
- Flag variable `running` — clean exit mechanism

</details>

---

### Exercise 24: Pascal's Triangle
Write code that prints the first n rows of Pascal's Triangle:

```
n=5:
    1
   1 1
  1 2 1
 1 3 3 1
1 4 6 4 1
```

Each number is the sum of the two numbers directly above it.

<details>
<summary>Click to reveal answer</summary>

```c
int n = 5;

for (int row = 0; row < n; row++) {
    // Print leading spaces for centering
    for (int space = 0; space < n - row - 1; space++) {
        printf(" ");
    }
    
    // Calculate and print each number in the row
    int value = 1;
    for (int col = 0; col <= row; col++) {
        printf("%d ", value);
        // Calculate next value using the formula:
        // C(row, col+1) = C(row, col) * (row - col) / (col + 1)
        value = value * (row - col) / (col + 1);
    }
    
    printf("\n");
}
```

**Alternative using a 2D-like approach:**
```c
int n = 5;
int triangle[10][10];  // Max 10 rows

// Build the triangle
for (int row = 0; row < n; row++) {
    for (int col = 0; col <= row; col++) {
        if (col == 0 || col == row) {
            triangle[row][col] = 1;  // Edges are always 1
        } else {
            // Sum of two numbers above
            triangle[row][col] = triangle[row-1][col-1] + triangle[row-1][col];
        }
    }
}

// Print the triangle
for (int row = 0; row < n; row++) {
    for (int space = 0; space < n - row - 1; space++) printf(" ");
    for (int col = 0; col <= row; col++) {
        printf("%d ", triangle[row][col]);
    }
    printf("\n");
}
```

</details>

---

### Exercise 25: Rock Paper Scissors (Best of N)
Write a "Rock Paper Scissors" game where:
- The computer always plays "Rock" (to keep it simple)
- The user enters R, P, or S
- First to 3 wins takes the match
- Invalid input asks again without counting as a turn

<details>
<summary>Click to reveal answer</summary>

```c
int player_wins = 0;
int computer_wins = 0;
int target = 3;
char choice;

printf("=== Rock Paper Scissors — Best of 5 ===\n");
printf("Computer always plays Rock (shhh!)\n\n");

while (player_wins < target && computer_wins < target) {
    printf("Your move (R/P/S): ");
    scanf(" %c", &choice);  // Note: space before %c skips whitespace
    
    // Convert to uppercase for easier comparison
    if (choice >= 'a' && choice <= 'z') {
        choice = choice - 32;  // Convert to uppercase
    }
    
    // Validate input
    if (choice != 'R' && choice != 'P' && choice != 'S') {
        printf("Invalid! Please enter R, P, or S.\n\n");
        continue;  // Don't count this turn
    }
    
    // Determine winner (computer always plays Rock)
    printf("You played: %c, Computer played: R\n", choice);
    
    switch (choice) {
        case 'R':
            printf("Tie!\n");
            break;
        case 'P':
            printf("Paper covers Rock — You win this round!\n");
            player_wins++;
            break;
        case 'S':
            printf("Rock crushes Scissors — Computer wins this round!\n");
            computer_wins++;
            break;
    }
    
    printf("Score: You %d - %d Computer\n\n", player_wins, computer_wins);
}

// Announce final winner
if (player_wins == target) {
    printf("🎉 Congratulations! You won the match!\n");
} else {
    printf("💻 Computer wins the match. Better luck next time!\n");
}
```

**Key patterns used:**
- `while` with compound condition
- Input validation with `continue`
- `switch` for game logic
- Score tracking with counter variables

</details>

---

## 14. Summary

| Concept           | Key Points                                                        |
|-------------------|-------------------------------------------------------------------|
| `if`              | Execute code conditionally; always use braces                     |
| `if-else`         | Choose between two paths                                          |
| `else if`         | Check multiple conditions in sequence; first match wins           |
| Guard clauses     | Check errors first, keep main logic unindented                    |
| `switch`          | Multiple exact-value checks; don't forget `break`                 |
| `while`           | Check condition first, may not run at all                         |
| `do-while`        | Execute first, then check; always runs once                       |
| `for`             | Best for counted iteration; init, condition, update in one place  |
| Nested loops      | Inner loop completes fully for each outer iteration               |
| `break`           | Exit loop or switch immediately                                   |
| `continue`        | Skip to next iteration                                            |
| `while (1)`       | Infinite loop for input validation; exit with `return` or `break` |
| State machines    | Use `switch` inside `while` to handle different program states    |
| Search patterns   | Combine `for` with early exit (`break` or flag) for efficiency    |
| Input validation  | `while (1)` + multiple `if`/`continue` for robust user input      |

### 🎯 Key Takeaways

1. **Always use braces** — Even for single-statement `if` or loops. It prevents bugs.

2. **Guard clauses improve readability** — Check error conditions first, return early, keep main logic at a low indentation level.

3. **Choose the right loop:**
   - `for` — When you know how many iterations (counting, arrays)
   - `while` — When you don't know in advance (until condition met)
   - `do-while` — When you need at least one iteration (menus, prompts)

4. **Remember the difference between `break` and `continue`:**
   - `break` = exit the loop entirely
   - `continue` = skip to the next iteration

5. **Nested loops multiply iterations** — A loop inside a loop means O(n²) complexity. Be mindful of performance.

### ⚠️ Common Pitfalls to Avoid

| Pitfall                           | Example                            | Fix                                           |
|-----------------------------------|------------------------------------|-----------------------------------------------|
| Semicolon after control statement | `if (x);` or `for(i=0;i<10;i++);`  | Remove the semicolon                          |
| Assignment instead of comparison  | `if (x = 5)`                       | Use `if (x == 5)`                             |
| Forgetting `break` in `switch`    | Unintended fall-through            | Add `break` after each case                   |
| Off-by-one errors                 | `i <= size` accessing `arr[size]`  | Use `i < size` for arrays                     |
| Infinite loops                    | Forgetting to update loop variable | Ensure condition eventually becomes false     |
| `break` only exits innermost loop | Using `break` in nested loops      | Use a flag variable or refactor to a function |

---

## 15. Next Steps

Continue to [Lesson 2: Arrays](LESSON_2_Arrays.md) to see how loops and arrays work together →

← Previous: [Lesson 0: Basic Types](LESSON_0_Basic_Types.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## 16. Bonus: Advanced Topics
> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 4: Functions](LESSON_4_Functions.md) — for understanding function control flow
> - [Lesson 5: Pointers](LESSON_5_Pointers.md) — for understanding how loops work with pointers
> - [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) — for understanding error handling patterns

### `goto` — When and Why to Avoid It

The **`goto` statement** transfers control to a labeled statement. It's controversial and rarely needed in modern C.

#### Basic Syntax
```c
goto label;
// ... other code ...
label:
    // Code here
```

#### Example
```c
int x = 0;
start:
    printf("%d ", x);
    x++;
    if (x < 5) {
        goto start;  // Jump back
    }
// Output: 0 1 2 3 4
```

#### The One Good Use Case — Error Cleanup
```c
int process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        goto error;
    }
    
    char *buffer = malloc(1024);
    if (buffer == NULL) {
        goto cleanup_file;
    }
    
    // Process file...
    if (some_error) {
        goto cleanup_buffer;
    }
    
    // Success
    free(buffer);
    fclose(file);
    return 0;

cleanup_buffer:
    free(buffer);
cleanup_file:
    fclose(file);
error:
    return -1;
}
```

**Why this is acceptable:**
- Single exit point with cleanup
- Avoids deeply nested error handling
- Used in Linux kernel and other large C projects

#### Why to Avoid `goto` in General
```c
// DON'T DO THIS
int x = 0;
loop:
    printf("%d ", x);
    x++;
    if (x < 10) goto loop;  // Use a while loop instead!

// DON'T DO THIS
if (condition) goto skip_block;
    // ... some code ...
skip_block:
    // Use an if-else instead!
```

**Problems with `goto`:**
- Makes code hard to follow ("spaghetti code")
- Easy to create bugs by jumping into the middle of a block
- Almost always replaceable with proper control flow

**Rule of thumb:** If you're considering `goto`, first try `break`, `continue`, or restructuring with functions.

---

### Loop Invariants and Correctness

A **loop invariant** is a condition that's true before and after every iteration. Understanding invariants helps you write correct loops and prove they work.

#### Example: Array Sum
```c
int sum_array(int arr[], int size) {
    int sum = 0;
    int i = 0;
    
    // Loop invariant: sum equals the sum of arr[0]..arr[i-1]
    while (i < size) {
        sum += arr[i];
        i++;
        // Invariant still holds: sum now includes arr[i-1]
    }
    
    // After loop: i == size, so sum is arr[0]..arr[size-1]
    return sum;
}
```

#### The Three Properties

1. **Initialization:** Invariant is true before the loop starts
   - `i=0`, `sum=0` → sum of zero elements is 0 ✓

2. **Maintenance:** If invariant is true before an iteration, it's true after
   - Before: sum = arr[0]..arr[i-1]
   - Execute: sum += arr[i], i++
   - After: sum = arr[0]..arr[i-1] (new i) ✓

3. **Termination:** When loop ends, invariant + exit condition gives the result
   - Invariant: sum = arr[0]..arr[i-1]
   - Exit condition: i == size
   - Together: sum = arr[0]..arr[size-1] ✓

#### Practical Use
Loop invariants help you:
- **Write correct loops** — think about what should stay true
- **Debug loops** — if invariant breaks, that's where the bug is
- **Prove correctness** — formal verification in critical systems

#### Example: Binary Search
```c
int binary_search(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;
    
    // Invariant: if target exists, it's in arr[left..right]
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;  // Invariant maintained: can't be in left half
        } else {
            right = mid - 1;  // Invariant maintained: can't be in right half
        }
    }
    
    // left > right: searched entire range, target not found
    return -1;
}
```

---

### Duff's Device — The Strangest Loop

**Duff's Device** is a famous (infamous?) example of C's flexibility. It unrolls a loop using a `switch` statement **inside** the loop. Named after Tom Duff, who invented it while working on Star Wars graphics.

#### The Original
```c
// Copy 'count' bytes from 'from' to 'to'
void duff_copy(char *to, char *from, int count) {
    int n = (count + 7) / 8;  // Number of blocks
    switch (count % 8) {      // Handle remainder first
        case 0: do { *to++ = *from++;
        case 7:      *to++ = *from++;
        case 6:      *to++ = *from++;
        case 5:      *to++ = *from++;
        case 4:      *to++ = *from++;
        case 3:      *to++ = *from++;
        case 2:      *to++ = *from++;
        case 1:      *to++ = *from++;
                } while (--n > 0);
    }
}
```

#### How It Works
1. Divides work into blocks of 8
2. Uses `switch` to jump to the right spot for the remainder
3. Fall-through copies 8 bytes per iteration
4. `do-while` repeats for all complete blocks

> ⚠️ **Note:** The original version has a bug when `count=0` (it would still copy 8 bytes). Production code needs a guard clause: `if (count <= 0) return;`

#### Why It Exists
**Loop unrolling** reduces loop overhead:

```c
// Normal loop: 8 checks, 8 increments
for (int i = 0; i < 8; i++) {
    *to++ = *from++;
}

// Unrolled: 1 check, 8 increments
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
*to++ = *from++;
```

Duff's Device combines unrolling with `switch` for efficiency on old hardware.

#### Should You Use It?
**No.** Here's why:

| Aspect                | Duff's Device    | Modern Approach                |
|-----------------------|------------------|--------------------------------|
| Readability           | ❌ Cryptic        | ✅ Clear                        |
| Maintainability       | ❌ Hard to modify | ✅ Easy                         |
| Performance           | Obsolete         | Compilers unroll automatically |
| Compiler optimization | Works against it | Works with it                  |

**Modern equivalent:**
```c
void modern_copy(char *to, char *from, int count) {
    // Compiler will unroll this automatically with -O2 or -O3
    for (int i = 0; i < count; i++) {
        to[i] = from[i];
    }
    
    // Or just use the library:
    memcpy(to, from, count);
}
```

#### Why You Should Know About It
- **Historical significance** — shows C's flexibility
- **Interview question** — occasionally appears
- **Educational** — demonstrates fall-through and loop control
- **Warning** — clever code isn't always good code

> "Everyone knows that debugging is twice as hard as writing a program in the first place. So if you're as clever as you can be when you write it, how will you ever debug it?"  
> — Brian Kernighan, co-author of "The C Programming Language"

**Lesson:** Write clear, maintainable code. Let the compiler optimize.

