bits 64

global tl_coroutine_yield

CoroState:
.return_addr equ 0x00
.rsp         equ 0x08
.stack_base  equ 0x10
.stack_top   equ 0x18

section .text

tl_coroutine_yield:
	; yield value
	mov rax, rdx

	; return address
	mov r8, qword [rcx + CoroState.return_addr]

	; resume address
	pop qword [rcx + CoroState.return_addr]

	; save caller's registers
	push rbp
	push rbx
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15

	; restore stack
	xchg qword [rcx + CoroState.rsp], rsp

	; restore registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbx
	pop rbp

	jmp r8
