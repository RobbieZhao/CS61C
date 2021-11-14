.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	$a0, shouldbe31
	jal	print_str
	lui	$a0, 0x8000	# should be 31
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x8000
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe16
	jal	print_str
	lui	$a0, 0x0001	# should be 16
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x0001
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe0
	jal	print_str
	li	$a0, 1		# should be 0
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	li	$a0, 1
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldben1
	jal	print_str
	move	$a0, $0		# should be -1
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	move	$a0, $0
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	li	$v0, 10
	syscall

first1posshift:
	### YOUR CODE HERE ###
	addiu  $sp, $sp, -4        # 4 bytes to store s0 on the stack
	sw     $s0, 0($sp)         # store s0 because we will be using s0 

	bne    $a0, $0, nonzero    # if (n == 0)
	addiu  $v0, $zero, -1      #   return -1 
	j      return

nonzero:
	addiu  $s0, $zero, 31     # pos = 31

loop:
	ble    $a0, $zero, after # while (n > 0) {
	sll    $a0, $a0, 1        #   n = n << 1;
	addiu  $s0, $s0, -1       #   pos --;
	j      loop               # }

after:
	addiu  $v0, $s0, 0
	lw     $s0, 0($sp)

return:
	addiu  $sp, $sp, 4
	jr     $ra

first1posmask:
	### YOUR CODE HERE ###
	addiu  $sp, $sp, -8             # 8 bytes to store s0 and s1 on the stack	
	sw     $s0, 0($sp)              # store s0 because we will be using s0 
	sw     $s1, 4($sp)              # store s1 because we will be using s1

	bne    $a0, $0, nonzero         # if (n == 0)
	addiu  $v0, $zero, -1           #   return -1 
	j      return

nonzero:
  addiu  $s0, $zero, 31           # pos = 31
	addiu  $s1, $zero, 0x80000000   # mask = 0x80000000;

loop:
	and    $t0, $s0, $s1            # mask & n
	addiu  $t1, $0, 1               # 1
	beq    $t0, $t1, after          # while (mask & n != 1) {
	addiu  $s0, $s0, -1             #   pos--;
	srl    $s1, $s1, 1              #   mask = mask >> 1;
	j      loop                     # }

after:
	addiu  $v0, $s0, 0
	lw     $s0, 0($sp)
  lw     $s1, 4($sp)

return:
	addiu  $sp, $sp, 8
	jr     $ra

print_int:
	move	$a0, $v0
	li	$v0, 1
	syscall
	jr	$ra

print_str:
	li	$v0, 4
	syscall
	jr	$ra

print_space:
	li	$a0, ' '
	li	$v0, 11
	syscall
	jr	$ra

print_newline:
	li	$a0, '\n'
	li	$v0, 11
	syscall
	jr	$ra
