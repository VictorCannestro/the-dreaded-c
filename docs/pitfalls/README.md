# C Pitfalls and How to Avoid Them

This guide covers common C mistakes drawn from real bugs found in the Tic-Tac-Toe project.
Each section is a dedicated document with detailed explanations, examples, and best practices.

> **Companion Guide:** [LEARNING_GUIDE.md](../learning_guides/LEARNING_GUIDE.md) covers core C language concepts.


## Sections

| File                                                              | Topics                                                                                          |
|-------------------------------------------------------------------|-------------------------------------------------------------------------------------------------|
| [Memory & Pointers](PITFALL_MEMORY_AND_POINTERS.md)               | Missing NULL include, array out of bounds, off-by-one errors, NULL dereference, `memset` misuse |
| [Function Declarations](PITFALL_FUNCTION_DECLARATIONS.md)         | Empty parameter list `()`, header guard typos                                                   |
| [Variables & Types](PITFALL_VARIABLES_AND_TYPES.md)               | Uninitialized variables, silent integer promotion                                               |
| [Operators & Control Flow](PITFALL_OPERATORS_AND_CONTROL_FLOW.md) | `=` vs `==`, missing `break` in `switch`                                                        |
| [Strings & Input](PITFALL_STRINGS_AND_INPUT.md)                   | Immutable string literals, `scanf` buffer overflow, ignored return values                       |
| [Standards & Conventions](PITFALL_STANDARDS_AND_CONVENTIONS.md)   | Missing newline at end of file                                                                  |



## Severity Key

| Icon | Meaning                                                                |
|------|------------------------------------------------------------------------|
| 🔴   | **Critical** — crashes, undefined behaviour, or security vulnerability |
| 🟡   | **Medium** — logic bug, silent failure, or standards violation         |



## Recommended Compiler Flags
Enable these to catch many pitfalls at compile time:

```bash
gcc -Wall -Wextra -Wpedantic -Werror       \
    -Wstrict-prototypes                    \
    -Wuninitialized                        \
    -Wformat=2                             \
    -Wnull-dereference                     \
    -Wimplicit-fallthrough                 \
    -Wwrite-strings                        \
    program.c -o program
```


## Debugging Tools Quick Reference

| Tool             | How to Enable               | Best For                           |
|------------------|-----------------------------|------------------------------------|
| AddressSanitizer | `gcc -fsanitize=address -g` | Buffer overflows, use-after-free   |
| MemorySanitizer  | `gcc -fsanitize=memory -g`  | Uninitialized reads                |
| Valgrind         | `brew install valgrind`     | Memory leaks, uninitialized memory |
| GDB              | `brew install gdb`          | Interactive crash analysis         |
| clang-tidy       | `brew install llvm`         | Static analysis                    |
| cppcheck         | `brew install cppcheck`     | Additional static analysis         |
