	out $zero, $zero, $imm2, $imm1, 1, 2		# enable irq2
	sll $sp, $imm1, $imm2, $zero, 1, 11		# set $sp = 1 << 11 = 2048
	out $zero, $imm1, $zero, $imm2, 6, L3		# set irqhandler as L3
	lw $a0, $zero, $imm2, $zero, 0, 64		# get x from address 64
	jal $ra, $zero, $zero, $imm2, 0, fib		# calc $v0 = fib(x)
	sw $zero, $zero, $imm2, $v0, 0, 65		# store fib(x) in 65
	halt $zero, $zero, $zero, $zero, 0, 0		# halt
fib:
	add $sp, $sp, $imm2, $zero, 0, -3		# adjust stack for 3 items
	sw $zero, $sp, $imm2, $s0, 0, 2			# save $s0
	sw $zero, $sp, $imm2, $ra, 0, 1			# save return address
	sw $zero, $sp, $imm2, $a0, 0, 0			# save argument
	bgt $zero, $a0, $imm1, $imm2, 1, L1		# jump to L1 if x > 1
	add $v0, $a0, $zero, $zero, 0, 0		# otherwise, fib(x) = x, copy input
	beq $zero, $zero, $zero, $imm2, 0, L2		# jump to L2
L1:
	sub $a0, $a0, $imm2, $zero, 0, 1		# calculate x - 1
	jal $ra, $zero, $zero, $imm2, 0, fib		# calc $v0=fib(x-1)
	add $s0, $v0, $imm2, $zero, 0, 0		# $s0 = fib(x-1)
	lw $a0, $sp, $imm2, $zero, 0, 0			# restore $a0 = x
	sub $a0, $a0, $imm2, $zero, 0, 2		# calculate x - 2
	jal $ra, $zero, $zero, $imm2, 0, fib		# calc fib(x-2)
	add $v0, $v0, $s0, $zero, 0, 0			# $v0 = fib(x-2) + fib(x-1)
	lw $a0, $sp, $imm2, $zero, 0, 0			# restore $a0
	lw $ra, $sp, $imm2, $zero, 0, 1			# restore $ra
	lw $s0, $sp, $imm2, $zero, 0, 2			# restore $s0
L2:
	add $sp, $sp, $imm2, $zero, 0, 3		# pop 3 items from stack
	add $t0, $a0, $zero, $zero, 0, 0		# $t0 = $a0
	sll $t0, $t0, $imm2, $zero, 0, 16		# $t0 = $t0 << 16
	add $t0, $t0, $v0, $zero, 0, 0			# $t0 = $t0 + $v0
	out $zero, $zero, $imm2, $t0, 0, 10		# write $t0 to display
	beq $zero, $zero, $zero, $ra, 0, 0		# and return
L3:
	in $t1, $zero, $imm2, $zero, 0, 9		# read leds register into $t1
	sll $t1, $t1, $imm2, $zero, 0, 1		# left shift led pattern to the left
	or $t1, $t1, $imm2, $zero, 0, 1			# lit up the rightmost led
	out $zero, $zero, $imm2, $t1, 0, 9		# write the new led pattern
	out $zero, $zero, $imm2, $imm1, 255, 21		# set pixel color to white
	out $zero, $zero, $imm2, $imm1, 1, 22		# draw pixel
	in $t0, $zero, $imm2, $zero, 0, 20		# read pixel address
	add $t0, $t0, $imm2, $zero, 0, 257		# $t0 += 257
	out $zero, $zero, $imm2, $t0, 0, 20		# update address
	out $zero, $zero, $imm2, $zero, 0, 5		# clear irq2 status
	reti $zero, $zero, $zero, $zero, 0, 0		# return from interrupt
	.word 64 7
