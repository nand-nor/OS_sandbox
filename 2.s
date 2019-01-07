	.file	"2.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"oink! this much memory :( %d\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB1:
	.section	.text.startup,"ax",@progbits
.LHOTB1:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB16:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%ebx, %ebx
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L10:
	addl	$4096, %ebx
	cmpl	$4915200, %ebx
	je	.L2
.L3:
	movl	$4096, %edi
	call	malloc
	testq	%rax, %rax
	jne	.L10
.L2:
	movq	stderr(%rip), %rdi
	movl	%ebx, %edx
	movl	$.LC0, %esi
	xorl	%eax, %eax
	call	fprintf
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE16:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE1:
	.section	.text.startup
.LHOTE1:
	.ident	"GCC: (GNU) 5.4.0"
	.section	.note.GNU-stack,"",@progbits
