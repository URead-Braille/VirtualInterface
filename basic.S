.global _start
.text
_start:
	mov $1, %rax
	mov $1, %rdi
	mov $message, %rsi
	mov $13, %rdx
	syscall
loop:
	jmp loop

message:
	.ascii "Hello, world\n"
