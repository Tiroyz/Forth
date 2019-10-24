%include 'com.inc'
%include 'macro.inc'
%include 'word.inc'

section .data
  here: dq forth_mem
  stack_start: dq 0


section .bss
  resq 1023
  rstack_start: resq 1
  forth_mem: resq 65536
  input_buf: resb 1024
  user_buf: resb 1024

const here, [here]

global _start

section .data
  last_word: dq link

section .rodata
misfire: db "bad effort, try again", 10, 0

section .text
_start:
  mov rstack, rstack_start
  mov stack, stack_start
  mov qword[mode], 0
  mov pc, xt_interpret
  cmp qword [stack_start], 0
  je  .first
  mov rsp, [stack_start]
  jmp next
  .first:
    mov [stack_start], rsp
  jmp next


