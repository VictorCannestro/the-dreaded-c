# Lesson 8: Function Pointers and Abstraction
> **Learning Objectives**: By the end of this lesson, you will understand how to declare, assign, and call function 
> pointers, and how to use them to create flexible, reusable abstractions in C.

Function pointers are one of C's most powerful features. They allow you to **store addresses of functions** and call
them indirectly, enabling dynamic behavior selection at runtime. This is the foundation for callbacks, plugin systems,
and **interface abstraction**.

Think of function pointers like this: if a regular pointer says "the data I need is at this address," a function
pointer says "the code I need to execute is at this address."

---

## Table of Contents
### Core Concepts
1. [Understanding Pointers to Functions](#understanding-pointers-to-functions)
2. [Function Pointer Syntax (Step by Step)](#function-pointer-syntax-step-by-step)
3. [Why Function Pointers Matter](#why-function-pointers-matter)
4. [Real-World Example: Calculator Operations](#real-world-example-calculator-operations)
5. [The UserInterface Pattern in This Project](#the-userinterface-pattern-in-this-project)
6. [Adding a GUI Implementation](#adding-a-gui-implementation)

### Patterns & Pitfalls
7. [Benefits of This Pattern](#benefits-of-this-pattern)
8. [Common Pitfalls and How to Avoid Them](#common-pitfalls-and-how-to-avoid-them)

### Practice
9. [Self-Assessment Checklist](#self-assessment-checklist)
10. [Practical Exercises](#practical-exercises)

### Going Further
11. [Next Steps](#next-steps)
12. [Bonus: Advanced Topics](#bonus-advanced-topics) *(come back after completing Lessons 9-10)*

---

## Understanding Pointers to Functions
You already know that pointers can point to data:
```c
int x = 42;
int *ptr = &x;  // ptr points to the integer x
```

But did you know that **functions also have addresses**? When you write a function, the compiler places its machine code somewhere in memory. A function pointer stores that address.

```c
void greet(void) {
    printf("Hello!\n");
}

int main(void) {
    printf("Address of greet function: %p\n", (void *)greet);
    // Output: Address of greet function: 0x100003f40 (example)
}
```

Just like you can pass data pointers to functions, you can pass function pointers too!

---

## Function Pointer Syntax (Step by Step)
The syntax for function pointers can be confusing at first. Let's build it up progressively.

### Step 1: A Simple Function
```c
int add(int a, int b) {
    return a + b;
}
```

### Step 2: Declaring a Function Pointer
To declare a pointer to this function, we need to match its **signature** (return type and parameter types):

```c
int (*operation)(int, int);
//  ^           ^        ^
//  |           |        |
//  |           |        +--- Parameters: two ints
//  |           +------------ Name of the pointer variable
//  +------------------------ Return type: int
```

**Reading tip**: Start from the middle and work outward:
- `operation` is...
- `(*operation)` a pointer to...
- `int (*operation)(int, int)` a function taking two ints and returning an int

### Step 3: Assigning a Function to the Pointer
```c
operation = add;              // Point to the add function
// OR equivalently:
operation = &add;             // Explicit address-of operator (both work!)
```

### Step 4: Calling Through the Pointer
```c
int result = operation(3, 4);     // Call via pointer: result = 7
// OR equivalently:
result = (*operation)(3, 4);      // Explicit dereference (both work!)
```

### Complete Example
```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main(void) {
    int (*operation)(int, int);   // Declare function pointer
    
    operation = add;              // Point to add function
    printf("3 + 4 = %d\n", operation(3, 4));  // Output: 3 + 4 = 7
    
    operation = multiply;         // Now point to multiply function
    printf("3 * 4 = %d\n", operation(3, 4));  // Output: 3 * 4 = 12
    
    return 0;
}
```

**Key Insight**: We changed the behavior of our code without using `if` statements or recompiling! We just swapped which function `operation` points to.

---

## Why Function Pointers Matter
Function pointers enable **runtime polymorphism** in C. This means you can decide which function to call while your program is running, not just when you compile it.

### Without Function Pointers (Rigid Code)
```c
void calculate(int a, int b, char operation) {
    if (operation == '+') {
        printf("%d\n", a + b);
    } else if (operation == '-') {
        printf("%d\n", a - b);
    } else if (operation == '*') {
        printf("%d\n", a * b);
    } else if (operation == '/') {
        printf("%d\n", a / b);
    }
    // To add modulo, we must modify this function!
}
```

### With Function Pointers (Flexible Code)
```c
typedef int (*MathOperation)(int, int);

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }
int modulo(int a, int b) { return b != 0 ? a % b : 0; }  // Easy to add!

void calculate(int a, int b, MathOperation op) {
    printf("%d\n", op(a, b));
}
```

**Benefits**:
- ✅ **Open/Closed Principle**: Add new operations without modifying existing code
- ✅ **Testability**: Easy to swap in mock functions for testing
- ✅ **Plugin Architecture**: Load operations from external modules
- ✅ **Strategy Pattern**: Choose algorithms at runtime

This is crucial for:
- **Swapping CLI for GUI** (our project's main use case)
- **Testing with mock functions** (replace real network calls with test doubles)
- **Plugin architectures** (load `.dll` or `.so` files with custom functionality)
- **Callbacks** (pass functions to be called later, like event handlers)

---

## Real-World Example: Calculator Operations
Let's build a simple calculator that demonstrates function pointers in action.

### Version 1: Array of Function Pointers
```c
#include <stdio.h>

// Define operation functions
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int main(void) {
    // Array of function pointers
    int (*operations[])(int, int) = {add, subtract, multiply, divide};
    const char *symbols[] = {"+", "-", "*", "/"};
    
    int a = 10, b = 5;
    
    for (int i = 0; i < 4; i++) {
        int result = operations[i](a, b);
        printf("%d %s %d = %d\n", a, symbols[i], b, result);
    }
    
    return 0;
}

/* Output:
10 + 5 = 15
10 - 5 = 5
10 * 5 = 50
10 / 5 = 2
*/
```

### Version 2: Typedef for Readability
```c
#include <stdio.h>

// Create a type alias for function pointers
typedef int (*MathOperation)(int, int);

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

// Function that takes another function as a parameter
void apply_operation(int a, int b, MathOperation op, const char *name) {
    printf("%s: %d\n", name, op(a, b));
}

int main(void) {
    apply_operation(10, 5, add, "Addition");
    apply_operation(10, 5, subtract, "Subtraction");
    apply_operation(10, 5, multiply, "Multiplication");
    
    return 0;
}

/* Output:
Addition: 15
Subtraction: 5
Multiplication: 50
*/
```

**Notice**: Using `typedef` makes the code much more readable. Compare:
- Without typedef: `void apply_operation(int a, int b, int (*op)(int, int), const char *name)`
- With typedef: `void apply_operation(int a, int b, MathOperation op, const char *name)`

---

## The UserInterface Pattern in This Project
> **Why This Matters**: In real-world projects, you often need to support multiple interfaces (CLI, GUI, web, mobile) without duplicating your core logic. Function pointers make this possible by creating a "contract" that any implementation must follow.

The key insight is **separation of concerns**:
- **Game logic** knows *what* to display, but not *how*
- **UI implementation** knows *how* to display, but not *when*

This separation means you can:
- Test game logic without a real UI (use mock functions)
- Add new UIs without touching game code
- Have different teams work on UI and logic independently

We use function pointers to abstract the UI layer:

**Step 1: Define the interface (what functions must exist)**
```c
// ui_interface.h
typedef struct UserInterface {
    void (*display_board)(const GameState *state);
    void (*display_status)(GameStatus status);
    void (*display_message)(const char *message);
    void (*display_formatted)(const char *format, ...);
    int  (*get_move)(GameState *game);
    int  (*get_yes_no)(const char *prompt);
    CellValue (*get_symbol_choice)(const char *prompt);
    Difficulty (*get_difficulty)(void);
    void (*display_error)(const char *message);
} UserInterface;

// Factory function declaration
UserInterface *ui_get_cli_interface(void);
```

> 📝 **Note**: This matches the actual `ui_interface.h` in the project. Each function pointer represents a specific UI capability that any implementation must provide.

**Step 2: Implement the interface (CLI version)**
```c
// ui_cli.c
static void cli_display_board(const GameState *state) {
    // printf the board to terminal...
}

static int cli_get_move(GameState *game) {
    // scanf from user...
}

// ... other implementation functions ...

// Create the interface struct with function pointers
static UserInterface cli_interface = {
    .display_board    = cli_display_board,
    .display_status   = cli_display_status,
    .display_message  = cli_display_message,
    .display_formatted = cli_display_formatted,
    .get_move         = cli_get_move,
    .get_yes_no       = cli_get_yes_no,
    .get_symbol_choice = cli_get_symbol_choice,
    .get_difficulty   = cli_get_difficulty,
    .display_error    = cli_display_error,
};

UserInterface *ui_get_cli_interface(void) {
    return &cli_interface;
}
```

**Step 3: Use the interface (game logic doesn't know about CLI)**
```c
// main.c
int main(void) {
    GameState game;
    UserInterface *ui = ui_get_cli_interface();  // Could be GUI!
    
    ui->display_message("Welcome to Tic-Tac-Toe!");
    
    while (!game_is_over(&game)) {
        ui->display_board(&game);
        int move = ui->get_move(&game);  // Works for CLI or GUI
        game_make_move(&game, move);
    }
    
    ui->display_status(game.status);
    return 0;
}
```

---

## Adding a GUI Implementation
To add a GUI, you just implement the same interface with different functions:

```c
// ui_gui.c (example with SDL2)
static void gui_display_board(const GameState *state) {
    // SDL_RenderDrawLine, SDL_RenderCopy, etc.
}

static int gui_get_move(GameState *game) {
    // SDL_PollEvent for mouse clicks, return clicked cell
}

static void gui_display_message(const char *message) {
    // Render text using SDL_ttf
}

// ... implement ALL interface functions ...

static UserInterface gui_interface = {
    .display_board     = gui_display_board,
    .display_status    = gui_display_status,
    .display_message   = gui_display_message,
    .display_formatted = gui_display_formatted,
    .get_move          = gui_get_move,
    .get_yes_no        = gui_get_yes_no,
    .get_symbol_choice = gui_get_symbol_choice,
    .get_difficulty    = gui_get_difficulty,
    .display_error     = gui_display_error,
};

UserInterface *ui_get_gui_interface(void) {
    return &gui_interface;
}
```

**To switch from CLI to GUI:** Change ONE line in main.c:
```c
// UserInterface *ui = ui_get_cli_interface();
UserInterface *ui = ui_get_gui_interface();  // That's it!
```

## Benefits of This Pattern

| Without Abstraction              | With UserInterface                      |
|----------------------------------|-----------------------------------------|
| `printf` scattered throughout    | All I/O in one place                    |
| Changing UI = rewrite game logic | Changing UI = new implementation file   |
| Can't test without terminal      | Can mock the interface for tests        |
| Hard to add web/mobile UI        | Each platform implements same interface |

---

## Common Pitfalls and How to Avoid Them
### Pitfall 1: Confusing Declaration Syntax
```c
// ❌ WRONG: This declares a function that returns an int pointer
int *operation(int, int);

// ✅ CORRECT: This declares a pointer to a function that returns int
int (*operation)(int, int);
```

**Rule of thumb**: Parentheses around `*name` make it a function pointer.

### Pitfall 2: Calling NULL Function Pointers
```c
void (*callback)(void) = NULL;
callback();  // ❌ CRASH! Segmentation fault

// ✅ CORRECT: Always check before calling
if (callback != NULL) {
    callback();
}
```

### Pitfall 3: Mismatched Signatures
```c
void print_int(int x) {
    printf("%d\n", x);
}

void print_float(float x) {
    printf("%f\n", x);
}

void (*printer)(int);
printer = print_int;     // ✅ OK: signatures match
printer = print_float;   // ❌ WARNING: incompatible pointer types
```

**The compiler will warn you**, but be careful! Calling a function through a pointer with the wrong signature leads to undefined behavior.

### Pitfall 4: Forgetting That Function Names Are Already Pointers
```c
int add(int a, int b) { return a + b; }

int (*op)(int, int);

// These are equivalent (function name decays to pointer):
op = add;      // ✅ Recommended: clean and simple
op = &add;     // ✅ OK: explicit address-of (same result)

// These work but are confusing - AVOID:
op = *add;     // ⚠️ Works but misleading (dereference then decay)

// Calling through the pointer:
int result = op(3, 4);       // ✅ Recommended: direct call
result = (*op)(3, 4);        // ✅ OK: explicit dereference (old style)

// This is valid C but NEVER do this:
result = (***op)(3, 4);      // ⚠️ Weird but valid - please don't!
```

> ⚠️ **Why does `***op` work?** Function pointers have a quirk: dereferencing a function pointer gives you... a function, which immediately decays back to a pointer. This can repeat infinitely. It's valid C but confusing and pointless.

**Recommendation**: Use the simplest form: `op = add;` and `op(3, 4);`

---


## Self-Assessment Checklist

Before moving on, make sure you can:

- [ ] **Declare** a function pointer with the correct syntax
- [ ] **Assign** a function to a function pointer
- [ ] **Call** a function through a pointer
- [ ] **Explain** why `(*ptr)(args)` and `ptr(args)` both work
- [ ] **Use typedef** to simplify function pointer declarations
- [ ] **Create an array** of function pointers
- [ ] **Pass** a function pointer as a parameter to another function
- [ ] **Implement** an interface pattern using a struct of function pointers
- [ ] **Identify** when function pointers are useful (callbacks, plugins, abstraction)
- [ ] **Avoid** common pitfalls (NULL checks, signature matching)

> 💡 **Tip**: If you're unsure about any item, revisit that section or try the related exercise again.

---

## Practical Exercises

### Exercise 1: Basic Function Pointer
**Goal**: Declare and use a function pointer to call different greeting functions.

**Task**: Complete the following code:

```c
#include <stdio.h>

void greet_formal(const char *name) {
    printf("Good day, %s.\n", name);
}

void greet_casual(const char *name) {
    printf("Hey %s!\n", name);
}

int main(void) {
    // TODO: Declare a function pointer 'greeter' that can point to either function
    
    // TODO: Point greeter to greet_formal and call it with "Alice"
    
    // TODO: Point greeter to greet_casual and call it with "Bob"
    
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>

void greet_formal(const char *name) {
    printf("Good day, %s.\n", name);
}

void greet_casual(const char *name) {
    printf("Hey %s!\n", name);
}

int main(void) {
    // Declare a function pointer that takes a const char* and returns void
    void (*greeter)(const char *);
    
    // Point to greet_formal and call it
    greeter = greet_formal;
    greeter("Alice");  // Output: Good day, Alice.
    
    // Point to greet_casual and call it
    greeter = greet_casual;
    greeter("Bob");    // Output: Hey Bob!
    
    return 0;
}
```

**Key Points**:
- The function pointer signature must match: `void (*name)(const char *)`
- You can reassign the pointer to different functions with the same signature
- Calling through the pointer is the same as calling the function directly

</details>

---

### Exercise 2: Array of Function Pointers
**Goal**: Create a menu system using an array of function pointers.

**Task**: Implement a simple menu with options to print shapes.

```c
#include <stdio.h>

void print_square(void) {
    printf("####\n");
    printf("####\n");
    printf("####\n");
    printf("####\n");
}

void print_triangle(void) {
    printf("  *\n");
    printf(" ***\n");
    printf("*****\n");
}

void print_circle(void) {
    printf("  ***\n");
    printf(" *   *\n");
    printf("  ***\n");
}

int main(void) {
    // TODO: Create an array of function pointers containing the three print functions
    
    // TODO: Create an array of strings for menu options: "Square", "Triangle", "Circle"
    
    // TODO: Print a menu and let the user choose (0, 1, or 2)
    
    // TODO: Call the appropriate function using the array
    
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>

void print_square(void) {
    printf("####\n");
    printf("####\n");
    printf("####\n");
    printf("####\n");
}

void print_triangle(void) {
    printf("  *\n");
    printf(" ***\n");
    printf("*****\n");
}

void print_circle(void) {
    printf("  ***\n");
    printf(" *   *\n");
    printf("  ***\n");
}

int main(void) {
    // Array of function pointers
    void (*shapes[])(void) = {print_square, print_triangle, print_circle};
    
    // Array of menu options
    const char *menu[] = {"Square", "Triangle", "Circle"};
    
    // Display menu
    printf("Choose a shape:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i, menu[i]);
    }
    
    // Get user choice
    int choice;
    printf("Enter choice (0-2): ");
    scanf("%d", &choice);
    
    // Validate and call the function
    if (choice >= 0 && choice < 3) {
        printf("\n");
        shapes[choice]();  // Call through function pointer array
    } else {
        printf("Invalid choice!\n");
    }
    
    return 0;
}
```

**Key Points**:
- Arrays of function pointers: `void (*array[])(void) = {func1, func2, func3};`
- Access and call: `array[index]();`
- This pattern is perfect for menu systems, command dispatchers, and state machines

</details>

---

### Exercise 3: Function Pointers as Parameters
**Goal**: Write a generic "apply" function that takes a function pointer.

**Task**: Create a function that applies a transformation to an array of integers.

```c
#include <stdio.h>

// TODO: Write a function 'apply_to_array' that takes:
//   - An array of integers
//   - The size of the array
//   - A function pointer that transforms an int to an int
// The function should apply the transformation to each element

int square(int x) {
    return x * x;
}

int triple(int x) {
    return x * 3;
}

int negate(int x) {
    return -x;
}

int main(void) {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;
    
    printf("Original: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // TODO: Call apply_to_array with square function
    // TODO: Print the result
    
    // Reset array
    int numbers2[] = {1, 2, 3, 4, 5};
    
    // TODO: Call apply_to_array with triple function
    // TODO: Print the result
    
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>

// Generic function that applies a transformation
void apply_to_array(int *arr, int size, int (*transform)(int)) {
    for (int i = 0; i < size; i++) {
        arr[i] = transform(arr[i]);
    }
}

int square(int x) {
    return x * x;
}

int triple(int x) {
    return x * 3;
}

int negate(int x) {
    return -x;
}

void print_array(const int *arr, int size, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = 5;
    
    print_array(numbers, size, "Original");
    
    // Apply square
    apply_to_array(numbers, size, square);
    print_array(numbers, size, "Squared");
    // Output: Squared: 1 4 9 16 25
    
    // Reset and apply triple
    int numbers2[] = {1, 2, 3, 4, 5};
    apply_to_array(numbers2, size, triple);
    print_array(numbers2, size, "Tripled");
    // Output: Tripled: 3 6 9 12 15
    
    // Reset and apply negate
    int numbers3[] = {1, 2, 3, 4, 5};
    apply_to_array(numbers3, size, negate);
    print_array(numbers3, size, "Negated");
    // Output: Negated: -1 -2 -3 -4 -5
    
    return 0;
}
```

**Key Points**:
- Function pointers as parameters: `void func(int (*callback)(int))`
- This is the foundation of **higher-order functions** (functions that take functions as arguments)
- Used in: sorting with custom comparators, event handlers, callbacks

</details>

---

### Exercise 4: Implementing a Simple Interface
**Goal**: Create your own abstraction using function pointers (like our UserInterface pattern).

**Task**: Design a `Logger` interface that can have multiple implementations (Console, File, Network).

```c
#include <stdio.h>

// TODO: Define a Logger struct with function pointers for:
//   - void (*log_info)(const char *message);
//   - void (*log_warning)(const char *message);
//   - void (*log_error)(const char *message);

// TODO: Implement console logger functions:
//   - console_log_info
//   - console_log_warning
//   - console_log_error

// TODO: Create a console_logger struct with the implementations

// TODO: Write a function that uses the logger interface:
//   void process_data(Logger *logger) {
//       logger->log_info("Processing started");
//       // ... do work ...
//       logger->log_error("An error occurred");
//   }

int main(void) {
    // TODO: Get the console logger and use it
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <time.h>

// Logger interface definition
typedef struct Logger {
    void (*log_info)(const char *message);
    void (*log_warning)(const char *message);
    void (*log_error)(const char *message);
} Logger;

// Console logger implementation
static void console_log_info(const char *message) {
    printf("[INFO] %s\n", message);
}

static void console_log_warning(const char *message) {
    printf("[WARNING] %s\n", message);
}

static void console_log_error(const char *message) {
    printf("[ERROR] %s\n", message);
}

// Console logger struct
static Logger console_logger = {
    .log_info = console_log_info,
    .log_warning = console_log_warning,
    .log_error = console_log_error,
};

Logger *get_console_logger(void) {
    return &console_logger;
}

// Function that uses the logger interface
void process_data(Logger *logger) {
    logger->log_info("Processing started");
    logger->log_info("Reading data from database...");
    logger->log_warning("Database connection is slow");
    logger->log_info("Data processed: 1000 records");
    logger->log_error("Failed to write to cache");
    logger->log_info("Processing completed");
}

int main(void) {
    Logger *logger = get_console_logger();
    process_data(logger);
    
    return 0;
}

/* Output:
[INFO] Processing started
[INFO] Reading data from database...
[WARNING] Database connection is slow
[INFO] Data processed: 1000 records
[ERROR] Failed to write to cache
[INFO] Processing completed
*/
```

**Extension Challenge**: Add a file logger implementation that writes to a file instead of the console!

```c
// File logger implementation
static FILE *log_file = NULL;

static void file_log_info(const char *message) {
    if (log_file) {
        fprintf(log_file, "[INFO] %s\n", message);
        fflush(log_file);
    }
}

static void file_log_warning(const char *message) {
    if (log_file) {
        fprintf(log_file, "[WARNING] %s\n", message);
        fflush(log_file);
    }
}

static void file_log_error(const char *message) {
    if (log_file) {
        fprintf(log_file, "[ERROR] %s\n", message);
        fflush(log_file);
    }
}

static Logger file_logger = {
    .log_info = file_log_info,
    .log_warning = file_log_warning,
    .log_error = file_log_error,
};

Logger *get_file_logger(const char *filename) {
    log_file = fopen(filename, "a");  // Append mode
    if (!log_file) {
        return NULL;
    }
    return &file_logger;
}

void close_file_logger(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
```

**Key Points**:
- This is the **Strategy Pattern** from object-oriented design
- The `process_data` function doesn't know or care which logger implementation it uses
- You can easily add new logger types (network, database, etc.) without changing existing code
- This is **exactly** how our UserInterface works in the Tic-Tac-Toe project!

</details>

---

## Next Steps

Continue to [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) →

← Previous: [Lesson 7: Header Files](LESSON_7_Header_Files.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics
> 🎯 **Prerequisites**: Complete [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) and [Lesson 10: The Challenge](LESSON_10_The_Challenge.md) before tackling this section. The exercises below combine function pointers with dynamic memory and more complex program structures.

Once you're comfortable with the core concepts and have completed Lessons 9-10, come back here to deepen your understanding with these advanced patterns and exercises.

---

### Quick Reference: Function Pointer Cheat Sheet
Use this as a handy reference when working with function pointers:

#### Declaration Patterns
```c
// Basic function pointer variable
return_type (*pointer_name)(param_types);

// Example: pointer to function taking two ints, returning int
int (*operation)(int, int);

// Array of function pointers
return_type (*array_name[])(param_types) = {func1, func2, func3};

// Example: array of void functions taking no parameters
void (*commands[])(void) = {cmd_help, cmd_quit, cmd_save};

// Function pointer as parameter
void caller(return_type (*callback)(param_types));

// Example: function taking a comparator
void sort(int *arr, int size, int (*compare)(int, int));

// Typedef for cleaner code
typedef return_type (*TypeName)(param_types);

// Example: create MathOp type
typedef int (*MathOp)(int, int);
MathOp operation = add;  // Much cleaner!
```

#### Struct with Function Pointers (Interface Pattern)
```c
// Define the interface
typedef struct Interface {
    void (*method1)(int arg);
    int  (*method2)(const char *str);
    void (*method3)(void);
} Interface;

// Implement the interface
static void impl_method1(int arg) { /* ... */ }
static int  impl_method2(const char *str) { /* ... */ }
static void impl_method3(void) { /* ... */ }

// Create instance with designated initializers
static Interface my_impl = {
    .method1 = impl_method1,
    .method2 = impl_method2,
    .method3 = impl_method3,
};

// Factory function
Interface *get_implementation(void) {
    return &my_impl;
}

// Usage
Interface *iface = get_implementation();
iface->method1(42);
int result = iface->method2("hello");
```

#### Common Mistakes Quick Reference

| ❌ Wrong                     | ✅ Correct           | Why                                              |
|-----------------------------|---------------------|--------------------------------------------------|
| `int *func(int);`           | `int (*func)(int);` | Without parens, it's a function returning `int*` |
| `ptr();` when `ptr == NULL` | `if (ptr) ptr();`   | Always check for NULL before calling             |
| Mismatched signatures       | Match exactly       | Undefined behavior if signatures differ          |

---

### Advanced Exercises
#### Exercise 1: Comparator Functions
**Goal**: Use function pointers with sorting algorithms.

**Prerequisites**: Understanding of arrays and basic algorithms.

**Task**: Write a generic bubble sort that accepts a comparator function.

```c
#include <stdio.h>

// TODO: Write a function 'bubble_sort' that takes:
//   - An array of integers
//   - The size of the array
//   - A comparator function pointer: int (*compare)(int a, int b)
//     The comparator should return:
//       < 0 if a should come before b
//       0 if a equals b
//       > 0 if a should come after b

// TODO: Write compare_ascending(int a, int b)

// TODO: Write compare_descending(int a, int b)

// TODO: Write compare_by_absolute_value(int a, int b)

int main(void) {
    int numbers[] = {-5, 2, -8, 0, 3, -1, 7};
    int size = 7;
    
    // TODO: Sort ascending and print
    // TODO: Sort descending and print
    // TODO: Sort by absolute value and print
    
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>

// Generic bubble sort with comparator
void bubble_sort(int *arr, int size, int (*compare)(int, int)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // Use the comparator to decide if we should swap
            if (compare(arr[j], arr[j + 1]) > 0) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Comparator for ascending order
int compare_ascending(int a, int b) {
    return a - b;  // Negative if a < b, positive if a > b
}

// Comparator for descending order
int compare_descending(int a, int b) {
    return b - a;  // Negative if b < a (i.e., a > b)
}

// Comparator by absolute value
int compare_by_absolute_value(int a, int b) {
    int abs_a = a < 0 ? -a : a;
    int abs_b = b < 0 ? -b : b;
    return abs_a - abs_b;
}

void print_array(const int *arr, int size, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    // Test 1: Ascending
    int numbers1[] = {-5, 2, -8, 0, 3, -1, 7};
    int size = 7;
    bubble_sort(numbers1, size, compare_ascending);
    print_array(numbers1, size, "Ascending");
    // Output: -8 -5 -1 0 2 3 7
    
    // Test 2: Descending
    int numbers2[] = {-5, 2, -8, 0, 3, -1, 7};
    bubble_sort(numbers2, size, compare_descending);
    print_array(numbers2, size, "Descending");
    // Output: 7 3 2 0 -1 -5 -8
    
    // Test 3: By absolute value
    int numbers3[] = {-5, 2, -8, 0, 3, -1, 7};
    bubble_sort(numbers3, size, compare_by_absolute_value);
    print_array(numbers3, size, "By absolute value");
    // Output: 0 -1 2 3 -5 7 -8
    
    return 0;
}
```

**Real-World Connection**:
- This is exactly how `qsort()` from `<stdlib.h>` works!
- The standard library version: `void qsort(void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *))`
- By passing different comparators, you can sort the same data in different ways

</details>

---

#### Exercise 2: Event System
**Goal**: Build a simple event dispatcher using function pointers.

**Prerequisites**: Understanding of structs, arrays, and static variables (Lesson 9 helpful).

**Task**: Create an event system where multiple listeners can register for events.

```c
#include <stdio.h>

#define MAX_LISTENERS 10

// TODO: Define an event handler type: void (*EventHandler)(const char *event_data)

// TODO: Create a struct to store registered event handlers

// TODO: Write functions:
//   - void register_listener(EventHandler handler)
//   - void trigger_event(const char *event_data)

// Example event handlers
void on_user_login(const char *username) {
    printf("[AUTH] User logged in: %s\n", username);
}

void on_user_login_analytics(const char *username) {
    printf("[ANALYTICS] Tracking login for: %s\n", username);
}

void on_user_login_notification(const char *username) {
    printf("[NOTIFICATION] Sending welcome email to: %s\n", username);
}

int main(void) {
    // TODO: Register all three handlers
    // TODO: Trigger the event with "alice@example.com"
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <string.h>

#define MAX_LISTENERS 10

// Event handler type
typedef void (*EventHandler)(const char *event_data);

// Event system state
static struct {
    EventHandler listeners[MAX_LISTENERS];
    int count;
} event_system = {.count = 0};

// Register a listener
void register_listener(EventHandler handler) {
    if (event_system.count < MAX_LISTENERS) {
        event_system.listeners[event_system.count++] = handler;
        printf("[SYSTEM] Listener registered (total: %d)\n", event_system.count);
    } else {
        printf("[SYSTEM] Error: Maximum listeners reached\n");
    }
}

// Trigger event - calls all registered listeners
void trigger_event(const char *event_data) {
    printf("\n[SYSTEM] Event triggered: %s\n", event_data);
    for (int i = 0; i < event_system.count; i++) {
        event_system.listeners[i](event_data);
    }
}

// Example event handlers
void on_user_login(const char *username) {
    printf("[AUTH] User logged in: %s\n", username);
}

void on_user_login_analytics(const char *username) {
    printf("[ANALYTICS] Tracking login for: %s\n", username);
}

void on_user_login_notification(const char *username) {
    printf("[NOTIFICATION] Sending welcome email to: %s\n", username);
}

int main(void) {
    printf("=== Event System Demo ===\n\n");
    
    // Register multiple handlers for the same event
    register_listener(on_user_login);
    register_listener(on_user_login_analytics);
    register_listener(on_user_login_notification);
    
    // Trigger the event - all handlers will be called
    trigger_event("alice@example.com");
    
    printf("\n--- Another event ---\n");
    trigger_event("bob@example.com");
    
    return 0;
}

/* Output:
=== Event System Demo ===

[SYSTEM] Listener registered (total: 1)
[SYSTEM] Listener registered (total: 2)
[SYSTEM] Listener registered (total: 3)

[SYSTEM] Event triggered: alice@example.com
[AUTH] User logged in: alice@example.com
[ANALYTICS] Tracking login for: alice@example.com
[NOTIFICATION] Sending welcome email to: alice@example.com

--- Another event ---
[SYSTEM] Event triggered: bob@example.com
[AUTH] User logged in: bob@example.com
[ANALYTICS] Tracking login for: bob@example.com
[NOTIFICATION] Sending welcome email to: bob@example.com
*/
```

**Key Points**:
- This is the **Observer Pattern** - multiple observers react to one event
- Used in GUI frameworks (button clicks), game engines (collision events), web servers (request middleware)
- Function pointers enable **decoupling**: the event trigger doesn't know about the listeners
- You can add/remove listeners at runtime without modifying the event triggering code

**Extension Ideas** (after Lesson 9):
- Use `malloc`/`free` for dynamic listener array
- Add `unregister_listener()` function
- Support different event types (login, logout, error, etc.)
- Pass more complex event data using structs
- Add priority levels for listener execution order

</details>

---

### Further Reading
Once you've mastered these concepts, explore:

- **Standard Library**: `qsort()` and `bsearch()` in `<stdlib.h>` use comparator function pointers
- **Signal Handling**: `signal()` in `<signal.h>` registers callback functions
- **Thread Creation**: `pthread_create()` takes a function pointer for the thread entry point
- **Dynamic Loading**: `dlsym()` returns function pointers from shared libraries

