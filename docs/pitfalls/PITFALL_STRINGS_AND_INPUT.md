# Pitfalls: Strings & Input

> Part of the [C Pitfalls](README.md) series.

Errors in this category involve C string semantics and unsafe input handling. These bugs are a leading source of security vulnerabilities in C programs.

## Pitfalls in This Section

- [10. String Literals Are Immutable](#10-string-literals-are-immutable)
- [11. `scanf` Buffer Overflow](#11-scanf-buffer-overflow)
- [13. Ignoring Return Values](#13-ignoring-return-values)

---

### 10. String Literals Are Immutable

**Severity:** 🔴 Critical - Segmentation Fault  
**Frequency:** Common for beginners

#### The Problem

```c
char *str = "Hello";
str[0] = 'h';  // SEGFAULT! String literals are in read-only memory
```

#### Why This Crashes

String literals are stored in the **read-only text segment** of the program:

```c
char *str = "Hello";
// "Hello" lives in read-only memory
// str is a pointer that points to that read-only memory

str[0] = 'h';  // Attempt to write to read-only memory
               // OS protection triggers: SIGSEGV
```

**Memory Layout:**
```
[Text Segment - Read Only]  [Stack]        [Heap]
    "Hello"                  local vars     malloc'd data
       ↑
       |
    str points here
```

#### The Solution

**Use array initialization for mutable strings:**

```c
// ✅ CORRECT - Mutable copy on the stack
char str[] = "Hello";  // Array, contents can be modified
str[0] = 'h';          // OK - writes to stack memory
printf("%s\n", str);   // Prints: hello
```

**Or allocate on the heap:**

```c
// ✅ CORRECT - Mutable copy on the heap
char *str = malloc(6);  // 5 chars + null terminator
strcpy(str, "Hello");
str[0] = 'h';           // OK
free(str);              // Don't forget!
```

**Or use `strdup` (POSIX):**

```c
// ✅ CORRECT - Duplicates string to the heap
char *str = strdup("Hello");  // Allocates and copies
str[0] = 'h';                 // OK
free(str);                    // Must free
```

#### When Read-Only is Fine

```c
// For read-only access, mark the pointer const to be explicit
const char *str = "Hello";
// str[0] = 'h';  // Compile error: assignment to read-only location
printf("%s\n", str);  // OK - reading is fine
```

#### Complete Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void demonstrate_strings(void) {
    // ❌ WRONG - pointer to read-only memory
    char *literal = "Hello";
    // literal[0] = 'h';  // CRASH!
    
    // ✅ CORRECT - array copy on stack
    char mutable1[] = "Hello";
    mutable1[0] = 'h';
    printf("%s\n", mutable1);  // hello
    
    // ✅ CORRECT - heap allocation
    char *mutable2 = malloc(strlen("Hello") + 1);
    strcpy(mutable2, "Hello");
    mutable2[0] = 'h';
    printf("%s\n", mutable2);  // hello
    free(mutable2);
    
    // ✅ CORRECT - read-only, marked const
    const char *readonly = "Hello";
    printf("%s\n", readonly);
}
```

#### String Literals May Be Shared

```c
char *str1 = "Hello";
char *str2 = "Hello";

// The compiler may make both point to the same location
if (str1 == str2) {
    printf("Same address!\n");  // Often true
}

// Arrays are always independent copies
char arr1[] = "Hello";
char arr2[] = "Hello";
// arr1 != arr2 always (different stack addresses)
```

#### Best Practices

1. **Mark literal pointers `const`:**
   ```c
   const char *message = "Error!";
   ```

2. **Use arrays when you need to modify:**
   ```c
   char name[50] = "John";  // Can modify
   ```

3. **Document pointer mutability requirements:**
   ```c
   /**
    * @param str Must point to mutable memory (not a string literal)
    */
   void uppercase(char *str);
   ```

4. **Enable literal-to-non-const-pointer warnings:**
   ```bash
   gcc -Wwrite-strings program.c
   # Warns when assigning a literal to a non-const char*
   ```

---

### 11. `scanf` Buffer Overflow

**Severity:** 🔴 Critical - Security Vulnerability  
**Frequency:** Very common in beginner code

#### The Problem

```c
char name[10];
scanf("%s", name);  // User types 50 characters - BUFFER OVERFLOW!
```

**What happens:**
```
Buffer (10 bytes): [n][a][m][e][\0][ ][ ][ ][ ][ ]
                                                    ↓ overflow
User types:  "ThisIsAVeryLongNameThatDoesNotFit"
             [T][h][i][s][I][s][A][V][e][r][y]...
                         Corrupts adjacent memory!
```

#### Why This is Dangerous

1. **No bounds checking:** `scanf("%s")` has no concept of buffer size
2. **Stack corruption:** Overwrites other variables and return addresses
3. **Security exploit:** Attackers can overwrite the return address to execute arbitrary code
4. **Unpredictable crashes:** May corrupt unrelated data far from the overflow

#### Real-World Example

```c
void get_player_name(void) {
    char name[20];
    int score = 0;
    
    printf("Enter name: ");
    scanf("%s", name);  // User types 50 characters
    
    // Buffer overflow has corrupted 'score'
    printf("Score: %d\n", score);  // Garbage value
}
```

#### The Solutions

**Solution 1: Use a width specifier (quick fix):**
```c
char name[10];
scanf("%9s", name);  // Reads at most 9 chars + null terminator
```

**Solution 2: Use `fgets` (preferred for strings):**
```c
char name[10];
if (fgets(name, sizeof(name), stdin) != NULL) {
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline
}
```

**Solution 3: Use `getline` (POSIX — auto-allocates):**
```c
char *line = NULL;
size_t len = 0;
ssize_t nread = getline(&line, &len, stdin);
if (nread != -1) {
    // Use line...
    free(line);  // Must free
}
```

#### Why `fgets` is Better than `scanf("%s")`

| Feature | `scanf("%s")` | `scanf("%9s")` | `fgets()` |
|---------|---------------|----------------|-----------|
| Bounds checking | ❌ No | ✅ Yes | ✅ Yes |
| Reads whitespace | ❌ No (stops at space) | ❌ No | ✅ Yes |
| Handles newline | Skips | Skips | Includes (easy to strip) |
| Buffer overflow risk | ⚠️ High | ⚠️ Low | ✅ Safe |
| Recommended | ❌ Never | ⚠️ OK | ✅ Best |

#### Safe Integer Input with `fgets` + `sscanf`

```c
int number;
char buffer[100];

if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    if (sscanf(buffer, "%d", &number) == 1) {
        printf("Got: %d\n", number);
    } else {
        printf("Invalid input\n");
    }
}
```

#### Reusable Helper Function

```c
/**
 * Read a line from stdin safely.
 * Strips trailing newline. Returns NULL on EOF or error.
 */
char *read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}
```

#### Best Practices

1. **Never use unbounded `scanf("%s")`:**
   ```c
   scanf("%s", buffer);  // ❌ NEVER
   ```

2. **Always use `fgets` for string input:**
   ```c
   fgets(buffer, sizeof(buffer), stdin);  // ✅
   ```

3. **Check the return value of `fgets`:**
   ```c
   if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
       // Handle EOF or I/O error
   }
   ```

4. **Strip the newline `fgets` includes:**
   ```c
   buffer[strcspn(buffer, "\n")] = '\0';
   ```

---

### 13. Ignoring Return Values

**Severity:** 🟡 Medium - Silent Failures  
**Frequency:** Common

#### The Problem

```c
scanf("%d", &position);  // Returns number of items successfully read
// What if the user typed "abc"? position contains garbage!

board_place(board, position, CELL_X);  // Uses garbage value!
```

#### Why This is Dangerous

Many C functions communicate failure through their return value. Ignoring them causes:
1. Using invalid or garbage data
2. Dereferencing NULL pointers
3. Silent, hard-to-diagnose failures
4. Undefined behavior

**Key functions that must be checked:**

| Function | Success | Failure | How to Check |
|----------|---------|---------|--------------|
| `scanf` | Items read (≥1) | 0 or `EOF` | `!= expected_count` |
| `malloc` | Valid pointer | `NULL` | `== NULL` |
| `fopen` | Valid `FILE*` | `NULL` | `== NULL` |
| `fgets` | Buffer pointer | `NULL` | `== NULL` |
| `fclose` | `0` | `EOF` | `!= 0` |
| `fread` | Items read | `< expected` | `!= expected` |
| `fwrite` | Items written | `< expected` | `!= expected` |
| `remove` | `0` | `-1` | `!= 0` |
| `rename` | `0` | `-1` | `!= 0` |

#### Real-World Examples

**Example 1: `scanf` failure**
```c
// ❌ WRONG
int position;
scanf("%d", &position);       // User types "abc" — fails silently
board[position] = CELL_X;     // UNDEFINED BEHAVIOR!

// ✅ CORRECT
int position;
if (scanf("%d", &position) != 1) {
    printf("Invalid input\n");
    clear_input_buffer();
    // retry or exit
}
board[position] = CELL_X;  // Safe to use
```

**Example 2: `malloc` failure**
```c
// ❌ WRONG
GameState *state = malloc(sizeof(GameState));
state->status = ONGOING;  // CRASH if malloc returned NULL!

// ✅ CORRECT
GameState *state = malloc(sizeof(GameState));
if (state == NULL) {
    fprintf(stderr, "Error: Out of memory\n");
    return NULL;
}
state->status = ONGOING;  // Safe
```

**Example 3: `fopen` failure**
```c
// ❌ WRONG
FILE *file = fopen("config.txt", "r");
fgets(buffer, sizeof(buffer), file);  // CRASH if fopen failed!

// ✅ CORRECT
FILE *file = fopen("config.txt", "r");
if (file == NULL) {
    perror("fopen");  // Prints: "fopen: No such file or directory"
    return -1;
}
fgets(buffer, sizeof(buffer), file);
fclose(file);
```

#### Using `perror` and `strerror`

```c
#include <errno.h>
#include <string.h>

FILE *file = fopen("data.txt", "r");
if (file == NULL) {
    // Method 1: perror — prints label + system error string
    perror("fopen");

    // Method 2: strerror — more control over formatting
    fprintf(stderr, "Error opening data.txt: %s\n", strerror(errno));

    return -1;
}
```

#### The Solution — Always Check

```c
// ✅ scanf
int value;
if (scanf("%d", &value) != 1) {
    fprintf(stderr, "Invalid input\n");
    return -1;
}

// ✅ malloc
int *array = malloc(size * sizeof(int));
if (array == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
}

// ✅ fopen
FILE *file = fopen("data.txt", "r");
if (file == NULL) {
    perror("fopen");
    return -1;
}

// ✅ fgets
char buffer[100];
if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    fprintf(stderr, "Read error or EOF\n");
    return -1;
}
```

#### Best Practices

1. **Check every critical function call:**
   ```c
   void *p = malloc(n);
   if (p == NULL) { /* handle */ }
   ```

2. **Enable unused-result warnings:**
   ```bash
   gcc -Wunused-result program.c
   ```

3. **Mark your own functions as must-check:**
   ```c
   __attribute__((warn_unused_result))
   int save_game(GameState *state);
   ```

4. **Document return values in headers:**
   ```c
   /**
    * @return 0 on success, -1 on failure
    */
   int game_save(GameState *state);
   ```

