	.text
	.globl	func
	.type	func, @function
func:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$28, %esp
	pushl	%ebx

.BB0:
	pushl	%ecx
	pushl	%edx
	call	read
	popl	%edx
	popl	%ecx
	movl	%eax, %ebx
	movl	%ebx, -8(%ebp)
	pushl	%ecx
	pushl	%edx
	call	read
	popl	%edx
	popl	%ecx
	movl	%eax, %ebx
	movl	%ebx, -12(%ebp)
	movl	$0, -16(%ebp)
	jmp	.BB2

.BB1:
	movl	 -28(%ebp),%ebx
	movl	%ebx, %eax
	popl	 %ebx
	leave
	ret

.BB2:
	movl	 -16(%ebp),%ebx
	movl	 8(%ebp),%ecx
	movl	%ebx, edx
	cmpl	%ecx, %edx
	jlt	.BB3
	jmp	.BB4

.BB3:
	pushl	%ecx
	pushl	%edx
	call	print
	addl	$4, %esp
	popl	%edx
	popl	%ecx
	movl	 -8(%ebp),%ecx
	pushl	%ecx
	pushl	%edx
	call	print
	addl	$4, %esp
	popl	%edx
	popl	%ecx
	movl	 -12(%ebp),%edx
	movl	%ecx, ebx
	addl	%edx, %ebx
	movl	%eax, -24(%ebp)
	movl	%ebx, -24(%ebp)
	pushl	%ecx
	pushl	%edx
	call	print
	addl	$4, %esp
	popl	%edx
	popl	%ecx
	movl	%edx, -8(%ebp)
	movl	 -24(%ebp),%ebx
	movl	%ebx, -12(%ebp)
	movl	-16(%ebp), ebx
	addl	$1, %ebx
	movl	%eax, -16(%ebp)
	movl	%ebx, -16(%ebp)
	jmp	.BB2

.BB4:
	movl	 -8(%ebp),%ebx
	movl	 -12(%ebp),%ecx
	addl	%ecx, %ebx
	movl	%eax, -28(%ebp)
	movl	%ebx, -28(%ebp)
	jmp	.BB1
