section .text

%macro ISR_NO_ERR_CODE 1
global isr%1
isr%1:
	push 0
	push %1
	jmp service_interrupt
%endmacro

%macro ISR_NO_ERR_CODE2 1
global isr%1
isr%1:
	jmp service_interrupt
%endmacro

%macro ISR_NO_ERR_CODE3 1
global isr%1
isr%1:
	jmp service_interrupt
%endmacro

%macro ISR_ERR_CODE 1
global isr%1
isr%1:
	push %1
	jmp service_interrupt
%endmacro

ISR_NO_ERR_CODE 0
ISR_NO_ERR_CODE 1
ISR_NO_ERR_CODE 2
ISR_NO_ERR_CODE 3
ISR_NO_ERR_CODE 4
ISR_NO_ERR_CODE 5
ISR_NO_ERR_CODE 6
ISR_NO_ERR_CODE 7
ISR_ERR_CODE 8
ISR_NO_ERR_CODE 9
ISR_ERR_CODE 10
ISR_ERR_CODE 11
ISR_ERR_CODE 12
ISR_ERR_CODE 13
ISR_ERR_CODE 14
ISR_NO_ERR_CODE 15
ISR_NO_ERR_CODE 16
ISR_ERR_CODE 17
ISR_NO_ERR_CODE 18
ISR_NO_ERR_CODE 19
ISR_NO_ERR_CODE 20
ISR_NO_ERR_CODE 21
ISR_NO_ERR_CODE 22
ISR_NO_ERR_CODE 23
ISR_NO_ERR_CODE 24
ISR_NO_ERR_CODE 25
ISR_NO_ERR_CODE 26
ISR_NO_ERR_CODE 27
ISR_NO_ERR_CODE 28
ISR_NO_ERR_CODE 29
ISR_ERR_CODE 30
ISR_NO_ERR_CODE 31
ISR_NO_ERR_CODE3 32

%macro ISR_FILL 0
	%assign i 33
	%rep 65 - 33
		ISR_NO_ERR_CODE i
		%assign i i+1
	%endrep
%endmacro

ISR_NO_ERR_CODE2 65

%macro ISR_FILL2 0
	%assign i 66
	%rep 256 - 66
		ISR_NO_ERR_CODE i
		%assign i i+1
	%endrep
%endmacro

ISR_FILL
ISR_FILL2

extern handle_interrupt

service_interrupt:
    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	xor rax, rax
	mov ax, ds
	push rax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	;mov ss, ax
	;mov gs, ax

	mov rdi, rsp
	call handle_interrupt

	pop rax

	mov ds, ax
	mov es, ax
	;mov ss, ax
	;mov gs, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16
	iretq