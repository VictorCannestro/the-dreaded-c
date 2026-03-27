# C Learning Guide - Tic-Tac-Toe Project

This guide explains the key C concepts and software engineering practices used in this project, with explanations for 
people new to C.

---

## Lessons

| #  | Topic                                                                              | Description                                                                                         |
|----|------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| 0  | [Basic Types and Operators](LESSON_0_Basic_Types.md)                               | `int`, `char`, `float`, `double`, `void`, `stdint.h`, arithmetic, comparison, and logical operators |
| 1  | [Control Flow and Loops](LESSON_1_Control_Flow_And_Loops.md)                       | `if`, `else`, `switch`, `while`, `for`, `do-while`, `break`, `continue`, guard clauses              |
| 2  | [Arrays](LESSON_2_Arrays.md)                                                       | 0-indexing, 1D board as 2D grid, array-pointer decay, `memset`/`memcpy`                             |
| 3  | [Varieties of Named Constants](LESSON_3_Named_Constants.md)                        | `enum`, `const`, and `#define` — how each works, when to use each, all enums in this project        |
| 4  | [Functions](LESSON_4_Functions.md)                                                 | Declarations vs. definitions, return conventions, `const`, `static`                                 |
| 5  | [Pointers](LESSON_5_Pointers.md)                                                   | Addresses, `*` and `&`, passing by reference, `NULL` checks                                         |
| 6  | [Structs](LESSON_6_Structs.md)                                                     | `typedef struct`, `.` and `->` access, `GameState` and `Player`                                     |
| 7  | [Header Files](LESSON_7_Header_Files.md)                                           | `.h` vs `.c`, include guards, include order conventions                                             |
| 8  | [Function Pointers and Abstraction](LESSON_8_Function_Pointers_And_Abstraction.md) | Function pointer syntax, `UserInterface` pattern, swapping CLI/GUI                                  |
| 9  | [Memory Management](LESSON_9_Memory_Management.md)                                 | Stack vs. heap, `malloc`/`free`, memory errors, Valgrind, debugging                                 |
| 10 | [The Challenge 🐍](LESSON_10_The_Challenge.md)                                     | SDL Snake game analysis, bit packing, game loops, capstone project                                  |

---

## How to Use This Guide

1. Start with **Lesson 0** and work through in order — each lesson builds on the previous ones.
2. Each lesson includes code examples from this project and links back to the relevant source files.
3. After reading, explore the codebase:
   - [`src/board.c`](../../src/board.c) — Pure data structure operations
   - [`src/win_condition_calculator.c`](../../src/win_condition_calculator.c) — Algorithm with testable helpers
   - [`src/tictactoe.c`](../../src/tictactoe.c) — Module coordination
   - [`include/ui_interface.h`](../../include/ui_interface.h) — Function pointer abstraction
   - [`src/ui_cli.c`](../../src/ui_cli.c) — CLI implementation of the interface
   - [`tests/`](../../tests/) — See how functions are tested

---

## Additional Resources

- **Official C Standard**: ISO/IEC 9899:2018 (complex, formal)
- **Beej's Guide to C**: https://beej.us/guide/bgc/ (beginner-friendly)
- **K&R C**: "The C Programming Language" (classic textbook)
- **C Reference**: https://en.cppreference.com/w/c (quick lookup)
