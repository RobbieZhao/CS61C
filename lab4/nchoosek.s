.data
shouldbe0:	.asciiz "Should be 0, and it is: "
shouldbe1:	.asciiz "Should be 1, and it is: "
shouldbe4:	.asciiz "Should be 4, and it is: "
shouldbe6:	.asciiz "Should be 6, and it is: "

.text
main:
	la	$a0, shouldbe1
	jal	print_str
	li	$a0, 4
	li	$a1, 0
	jal	nchoosek		# evaluate C(4,0); should be 1
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe4
	jal	print_str
	li	$a0, 4
	li	$a1, 1
	jal	nchoosek		# evaluate C(4,1); should be 4
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe6
	jal	print_str
	li	$a0, 4
	li	$a1, 2
	jal	nchoosek		# evaluate C(4,2); should be 6
	move	$a0, $v0
	jal	print_int
	jal	print_newline
	
	la	$a0, shouldbe4
	jal	print_str
	li	$a0, 4
	li	$a1, 3
	jal	nchoosek		# evaluate C(4,3); should be 4
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe1
	jal	print_str
	li	$a0, 4
	li	$a1, 4
	jal	nchoosek		# evaluate C(4,4); should be 1
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe0
	jal	print_str
	li	$a0, 4
	li	$a1, 5
	jal	nchoosek		# evaluate C(4,5); should be 0
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	li	$v0, 10
	syscall

# calculate C($a0,$a1)
nchoosek:
	# prologue
	### YOUR CODE HERE ###
	addiu $sp, $sp, -4
	sw $ra, 0($sp)
    
	beq	$a1, $0, return1    # if k == 0, return 1
	beq	$a0, $a1, return1   # if n == k, return 1
	beq	$a0, $0, return0    # if n == 0, return 0
	blt	$a0, $a1, return0   # if n < k,  return 0

	addi	$a0, $a0, -1		# C(n,k) = C(n-1,k) + C(n-1,k-1)
	move	$s0, $a0
	move	$s1, $a1
	jal	nchoosek          # C(n-1,k)
	move	$s2, $v0        # $s2 = C(n-1,k)
	move	$a0, $s0
	addi	$a1, $s1, -1
	jal	nchoosek          # C(n-1,k-1)
	add	$v0, $v0, $s2     # v0 = C(n-1,k) + C(n-1,k-1)
	j	return
return0:
	move	$v0, $0
	j	return
return1:
	addi	$v0, $0, 1

return:
	# epilogue
	### YOUR CODE HERE ###
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra

print_int:
	li	$v0, 1
	syscall
	jr	$ra

print_str:
	li	$v0, 4
	syscall
	jr	$ra
	
print_newline:
	li	$a0, '\n'
	li	$v0, 11
	syscall
	jr	$ra
