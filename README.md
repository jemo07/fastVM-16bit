# fastVM-16bit
An attempt to have a simple 16 bit CPU VM, based on a circular stack. 


It works, well, more like it will compile: https://godbolt.org/z/oncs7KabY 

A simple virtual machine implementation in C.

## Introduction

The virtual machine is a simple stack-based machine that runs code written in its own instruction set. This implementation uses an array to represent the stack, a separate pointer to keep track of the top of the stack, and an array of code words, along with a separate code pointer, to keep track of the current instruction being executed.
The goal of this VM is to leverage it as a portable means to develop Forth on top of it and be able to make it portable to any plaform for MCU's. 

## Instructions

The virtual machine supports the following instructions:

**DROP:** pops the top value off the stack
**EXIT:** terminates the program
**BRANCH:** jumps to the address stored on the top of the stack
**DUP:** duplicates the top value on the stack
**FETCH:** fetches the value stored at the address on the top of the stack and pushes it onto the stack
**LIT:** pushes the next code word onto the stack
**R_FROM:** pops a value from the return stack
**R_FETCH:** duplicates the top value on the return stack
**TO_R:** pushes the top value on the data stack onto the return stack
**STORE:** stores the value on the second item of the stack into the address specified by the top of the stack
**ENTER:** enters a subroutine
**OVER:** duplicates the second value on the stack
**AND:** performs a bitwise AND operation between the top two values on the stack
**ZERO_LESS:** sets the top value on the stack to 0 if it's equal to 0, otherwise 1
**SWAP:** swaps the top two values on the stack
**COND_BRANCH:** jumps to the address specified by the second value on the stack if the top value is equal to 0
**UM_PLUS:** adds the top two values on the stack

### Usage

Compile the code with a C compiler and run the binary with the name of the input file as an argument. The input file should contain the code to be executed by the virtual machine, with each instruction represented as a single unsigned short value.
```
$ gcc fastvm.c -o fastvm16
$ ./fastvm16 input.txt
```
