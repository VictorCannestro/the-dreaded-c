# Lesson 1: Basic Types

C provides fundamental data types for storing different kinds of values:

| Type     | Size (typical) | Range       | Use Case                          |
|----------|----------------|-------------|-----------------------------------|
| `char`   | 1 byte         | -128 to 127 | Characters, small numbers         |
| `int`    | 4 bytes        | ±2 billion  | General integers                  |
| `float`  | 4 bytes        | ±3.4×10³⁸   | Decimals (6-7 digits precision)   |
| `double` | 8 bytes        | ±1.7×10³⁰⁸  | Decimals (15-16 digits precision) |

```c
int move_count = 0;      // Counting moves in the game
char marker = 'X';       // Single character for display
double score = 95.5;     // If we added scoring
```

## Special Types

```c
void game_init(void);    // void = "nothing" (no return / no parameters)
```

## Type Sizes Can Vary!

C doesn't guarantee exact sizes. Use `<stdint.h>` when you need specific sizes:

```c
#include <stdint.h>

int8_t   small = 127;      // Exactly 8 bits, signed
uint32_t large = 4000000;  // Exactly 32 bits, unsigned
```

**In this project:** We use `int` for simplicity since exact sizes don't matter for a game.

---

## Next Steps

Continue to [Lesson 2: Enums](LESSON_2_Enums.md) →

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

