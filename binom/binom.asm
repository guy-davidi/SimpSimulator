sll $sp, $imm1, $imm2, $zero, 1, 11		    # set $sp = 1 << 11 = 2048
lw $a0, $imm1, $0, $0, 0x100, 0             # a0 = n = mem[0x100]
lw $a1, $imm1, $0, $0, 0x101, 0             # a1 = k = mem[0x101]
jal $ra, $zero, $zero, $imm2, 0, BINOM		# calc $v0 = BINOM[k,n]
sw $zero, $zero, $imm2, $v0, 0, 0x102		# store BINOM in 0x102
halt
BINOM:    
add $sp, $sp, $imm2, $zero, 0, -4		    # adjust stack for 4 items            
sw $zero, $sp, $imm2, $a1, 0, 3			    # save $a1
sw $zero, $sp, $imm2, $a0, 0, 2			    # save $a0
sw $zero, $sp, $imm2, $ra, 0, 1			    # save return address
sw $zero, $sp, $imm2, $s0, 0, 0			    # save $s0
sub $t0, $a1, $a0, $0, 0, 0                 # $t0 = k-n         [$t0 != 0 only if k != n]
add $t1, $a1, $0, $0, 0 , 0                 # $t1 = k           [$t1 != 0 only if k != 0]
mac $t1, $t1, $t0, $0, 0, 0                 # $t1 = $t0 * $t1   [$t1 = 0 only if k == n or k == 0]
bne $0, $t1, $imm1, $imm2, 0, REC_SET       # if $t1 != 0 jump to REC_SET
add $v0, $0, $imm1, $0, 1, 0                # $v0 = 1;
beq $zero, $zero, $zero, $imm2, 0, FINISH	# jump to FINISH
REC_SET:               
add $a0, $a0, $imm1, $0, -1, 0              # n = n -1
add $a1, $a1, $imm1, $0, -1, 0              # k = k - 1
jal $ra, $0, $0, $imm1, BINOM, 0            # calc $v0 = BINOM [k-1, n-1]
add $s0, $v0, $0, $0, 0, 0                  # $s0 = BINOM[k-1,n-1]
add $a1, $a1, $imm1, $0, 1, 0               # k = k + 1
jal $ra, $zero, $zero, $imm2, 0, BINOM      # calc v0 = BINOM [k, n-1]
add $v0, $v0, $s0, $0, 0, 0                 # $v0 = BIN[k,n-1]+BIN[k-1,n-1]
lw $a1, $sp, $imm2, $0, 0, 3			    # restore $a1
lw $a0, $sp, $imm2, $0, 0, 2			    # restore $a0
lw $ra, $sp, $imm2, $0, 0, 1			    # restore return address
lw $s0, $sp, $imm2, $0, 0, 0			    # restore $s0
FINISH:
add $sp, $sp, $imm2, $zero, 0, 4		    # pop 4 items from stack
beq $zero, $zero, $zero, $ra, 0, 0		    # and return