# Lesson 3: Pointers

A **pointer** stores the memory address of another variable. This is one of C's most powerful and most confusing features.

## Visual Model

```
Variable x:     ┌───────┐
                │  42   │  ← value
                └───────┘
                  0x7ffc   ← address (memory location)

Pointer ptr:    ┌───────┐
                │0x7ffc │  ← stores the ADDRESS of x
                └───────┘
```

## Pointer Operators

| Operator | Name | Meaning |
|----------|------|---------|
| `&` | Address-of | Get the address of a variable |
| `*` | Dereference | Get the value at an address |
| `->` | Arrow | Access struct member through pointer |

## Basic Example

```c
int x = 42;
int *ptr = &x;      // ptr holds the address of x

printf("%d\n", x);      // 42 (the value)
printf("%p\n", &x);     // 0x7ffc... (address of x)
printf("%p\n", ptr);    // 0x7ffc... (same address)
printf("%d\n", *ptr);   // 42 (value at that address)

*ptr = 100;             // Modify x through the pointer
printf("%d\n", x);      // 100 (x changed!)
```

## Why Pointers Matter

**1. Functions Can Modify Variables (Pass by Reference)**

```c
// Without pointer - value is copied, original unchanged
void broken_increment(int num) {
    num = num + 1;  // Only modifies the copy
}

// With pointer - function modifies the original
void increment(int *num) {
    *num = *num + 1;  // Modifies the actual variable
}

int value = 5;
broken_increment(value);   // value still 5
increment(&value);         // value now 6
```

**2. Avoid Copying Large Data**

```c
// BAD: Copies entire GameState (expensive if large)
void display_board_bad(GameState state) { ... }

// GOOD: Only passes 8-byte address
void display_board(GameState *state) { ... }
```

**3. Return Multiple Values**

```c
void get_min_max(int arr[], int size, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

int minimum, maximum;
get_min_max(numbers, 5, &minimum, &maximum);
```

## Pointers in This Project

Almost every function takes a `GameState *` pointer:

```c
void game_init_session(GameState *state) {
    if (state == NULL) return;  // Always check for NULL!
    
    state->status = ONGOING;        // Arrow: access through pointer
    state->move_count = 0;
    board_init(state->board);       // Pass board array to another function
}
```

## Struct Pointer Syntax

```c
GameState game;
GameState *ptr = &game;

// These are equivalent:
(*ptr).status = ONGOING;   // Dereference, then access member
ptr->status = ONGOING;     // Arrow notation (preferred)
```

## Common Pointer Mistakes

```c
int *ptr;                // ❌ Uninitialized - points to garbage!
*ptr = 5;                // ❌ CRASH: writing to random memory

int *ptr = NULL;         // ✅ Initialize to NULL if no value yet
if (ptr != NULL) {       // ✅ Check before using
    *ptr = 5;
}
```

See [src/tictactoe.c](../../src/tictactoe.c) for pointer usage throughout.

---

## Next Steps

Continue to [Lesson 4: Functions](LESSON_4_Functions.md) →

← Previous: [Lesson 2: Enums](LESSON_2_Enums.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

