### 1. What do the .data, .word, .text directives mean (i.e. what do you use them for)?

- `.data` marks the start of the static data segment
- `.text` marks the start of the code segment
- `.word` stores the static data
  - 9 is the parameter for the fib function
  - no idea what 2,4,6,8 is for

### 2. How do you set a breakpoint in MARS? Set a breakpoint on line 14 and run to it. What is the instruction address? Has line 14 executed yet?

- toggle the checkbox
- instruction address is 0x00400020
- the instruction hasn't been executed yet. The PC points at the address of the instruction that's about to execute

### 3. Once at a breakpoint, how do you continue to execute your code? How do you step through your code? Run the code to completion.

- `Run -> Go` to continue executing
- `Run -> Step` to step through the code

### 4. Find the "Run I/O" window. What number did the program output? Which fib number is this?

- 34
- the 9th (0-indexed) fib number

### 5. At what address is n stored in memory? Try finding this by (1) looking at the Data Segment and (2) looking at the machine code (Code column in the Text Segment).

- the address is 0x10010010

### 6. Without using the "Edit" tab, have the program calculate the 13th fib number by manually modifying this memory location before execution. You may find it helpful to uncheck the "Hexadecimal Values" box at the bottom of the Data Segment.

- 233

### 7. How do you view and modify the contents of a register? Reset the simulation (Run-->Reset or F12) and now calculate the 13th fib number by (1) breaking at a well-chosen spot, (2) modifying a single register, and then (3) unsetting the breakpoint.

- set a breakpoint at the start of fib
- change $t3 to 13

### 8. Lines 19 and 21 use the syscall instruction. What is it and how do you use it? (Hint: look in Help)

- Issue a system call: execute the system call specified by value in $v0
- `1` means print integer in `$a0`
- `10` means exit
- [SYSCALL functions available in MARS](https://courses.missouristate.edu/KenVollmar/MARS/Help/SyscallHelp.html)
