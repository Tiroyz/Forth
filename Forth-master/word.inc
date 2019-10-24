native 'drop', drop
  add rsp, 8
  jmp next

native 'swap', swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next

native 'dup', dup
  push qword [rsp]
  jmp next

native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next

native '-', minus
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next

native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next

native '/', div
     pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

native "%", mod
  pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'or', or
  pop rax
  or [rsp], rax
  jmp next

native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'lit', lit
    push qword [pc]
    add pc, 8
    jmp next

native 'unsetbranch', unsetbranch
  mov byte[was_branch], 0
  jmp next

native 'saveword', saveword
  pop rax
  mov  qword[here], rax
  xor eax, eax
  mov rax, here
  add rax, 8
  add qword[here], rax
  jmp next

native 'savenum', savenum
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, 8
  mov qword[here], rax
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next

native 'savemode', savemode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'branch0', branch0
  pop rax
  test rax, rax
  jnz .skip
  mov pc, [pc]
  jmp next
  .skip:
    add pc, 8
    jmp next

native 'branch', branch
    mov pc, [pc]
    jmp next

native 'docol', docol
  sub rstack, 8
  mov [rstack], pc
  add w, 8
  mov pc, w
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'create', create
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
  ;mov byte [rsi], 0
  ;inc rsi

  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  ; флаги
  inc rsi
  pop rax
  mov [rsi], al
  inc rsi

  mov [here], rsi
  jmp next


native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

native '>r', to_r
  pop rax
  rpush rax
  jmp next

native 'r>', from_r
  rpop rax
  push rax
  jmp next

native "r@", r_fetch
  push qword [rstack]
  jmp next

native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next

native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .nequals:
  xor rax, rax
  push rax
  jmp next

native 'rot', rot
  pop rax
  pop rdx
  pop rcx
  push rdx
  push rcx
  push rax
  jmp next

native '.', dot
  pop rdi
  call print_int
  jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

native '!', write
  pop rax
  pop rdx
  mov [rax], rdx
  jmp next

native '@', fetch
  pop rax
  push qword[rax]
  jmp next

native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native 'savemode_addr', savemode_addr
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native 'isimmediate', isimmediate
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'initcmd', initcmd
  pop rax
  push pc
  mov [stub], rax
  mov pc, stub
  mov w, [pc]
  pop pc
  jmp [w]

native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next

colon "'", tick, 1
  dq xt_buffer
  dq xt_read
  dq xt_drop
  dq xt_buffer
  dq xt_find, xt_dup
  branch0 .no_word
  dq xt_cfa
  dq xt_savemode
  branch0 .interpret
  dq xt_lit, xt_lit
  dq xt_comma
  dq xt_comma

  .interpret:
    dq xt_exit

    .no_word:
    dq xt_drop
    dq xt_lit, misfire, xt_prints
    dq xt_printnl
    dq xt_exit

colon 'double', double
  dq xt_dup
  dq xt_plus

  dq xt_exit

colon '>', greater
  dq xt_swap
  dq xt_less
  dq xt_exit

native 'read', read
  pop rdi
  call read_word
  push rdx
  jmp next

native 'save_lw', save_lw
  push last_word
  jmp next  

native 'prints', prints
  pop rdi
  call print_string
  jmp next

native 'printnl', printnl
  call print_newline
  jmp next

native 'parsei', parsei
  call parse_int
  push rax
  push rdx
  jmp next

native 'emit', emit
  pop rdi
  call print_char
  jmp next

native 'cfa', cfa
  pop rdi
	xor eax, eax
  add rdi, 8
  push rdi
  call string_length
  pop rdi
  add rax, 1
  add rax, 1
  add rdi, rax
  mov rax, rdi
  push rax
  jmp next

native 'find', find
  pop rdi
  xor eax, eax
  mov rsi, [last_word]

  .loop:
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi

    test rax, rax
    jnz .found
    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop

    xor eax, eax
    push rax
  jmp next

  .found:
    mov rax, rsi
  push rax
  jmp next

colon ':', colon
  .restart:
  dq xt_buffer
  dq xt_read

  branch0 .restart
  dq xt_lit, 0          
  dq xt_buffer
  dq xt_create
  dq xt_lit, 1, xt_savemode_addr
  dq xt_write

  dq xt_lit, docol_impl
  dq xt_comma

  dq xt_exit

colon ';', semicolon, 1
  dq xt_lit, 0, xt_savemode_addr
  dq xt_write
  dq xt_lit, xt_exit
  dq xt_comma
  dq xt_exit

  section .text
next:
  mov w, [pc]
  add pc, 8
  jmp [w]

native 'interpret' , interpret
dq docol_impl

main_loop:
    dq xt_buffer
    dq xt_read
    branch0 exit
    dq xt_buffer
    dq xt_find

    dq xt_savemode
    branch0 .interpreter

  .compiler_mode:
    dq xt_dup
    branch0 .number

    dq xt_cfa

    dq xt_isimmediate
    branch0 .notImmediate

    .immediate:
      dq xt_initcmd
      branch main_loop

    .notImmediate:
      dq xt_isbranch
      dq xt_comma
      branch main_loop

    .number:
      dq xt_drop
      dq xt_parsei
      branch0 .warn
      dq xt_wasbranch
      branch0 .lit

      dq xt_unsetbranch
      dq xt_savenum
      branch main_loop

      .lit:
      dq xt_lit, xt_lit
      dq xt_comma
      dq xt_comma

      branch main_loop


  .interpreter:
    dq xt_dup
    branch0 .interpret_number

    dq xt_cfa
    dq xt_initcmd
    branch main_loop

    .interpret_number:
      dq xt_drop
      dq xt_parsei
      branch0 .warn
      branch main_loop

  .warn:
    dq xt_drop
    dq xt_lit, misfire, xt_prints
    dq xt_printnl
    branch main_loop

exit:
  dq xt_bye

section .data
  stub: dq 0
  mode: dq 0
  was_branch: db 0
