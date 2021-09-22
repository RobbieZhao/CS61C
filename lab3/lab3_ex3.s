	.file	1 "lab3_ex3.c"
	.section .mdebug.eabi32
	.previous
	.section .gcc_compiled_long32
	.previous
	.gnu_attribute 4, 1
	.text
	.align	2
	.globl	main
	.set	nomips16
	.ent	main
	.type	main, @function
main:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lui	$6,%hi(source)
	lw	$3,%lo(source)($6)
	beq	$3,$0,$L2
	lui	$7,%hi(dest)
	addiu	$7,$7,%lo(dest)
	addiu	$6,$6,%lo(source)
	move	$2,$0    # k = 0
$L3:
	addu	$5,$7,$2  # updates the pointer for destination
	addu	$4,$6,$2  # updates the pointer for source
	sw	$3,0($5)    # Stores the value (previously loaded from src) into dest.
	lw	$3,4($4)    # Loads the next source value
	addiu	$2,$2,4   # k = k + 1
	bne	$3,$0,$L3  # if source[k] != 0, continue the loop
$L2:
	move	$2,$0
	j	$31
	.end	main
	.size	main, .-main
	.globl	source
	.data
	.align	2
	.type	source, @object
	.size	source, 28
source:
	.word	3
	.word	1
	.word	4
	.word	1
	.word	5
	.word	9
	.word	0

	.comm	dest,40,4  # https://stackoverflow.com/questions/501105/what-does-comm-mean
	.ident	"GCC: (GNU) 4.4.1"
