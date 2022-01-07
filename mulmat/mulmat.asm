add $s2, $0, $0, $0, 0, 0        # $s2 = count = 0
add $t0, $0, $imm1, $0, 0, 0     # $t0 = j = 0
add $t1, $0, $imm1, $0, 0, 0     # $t1 = i = 0
add $s0, $0, $0, $0, 0, 0        # $s0 = 0 current sum
add $a0, $imm1, $0, $0, 4, 0 # $a0 = 4
CALC:
lw  $t2, $t0, $imm1, $0, 0x100, 0 # $t2 = mem[0x100+j]
lw  $s1, $t1, $imm1, $0, 0x110, 0 # $s1 = mem[0x110 +i]
mac $s0, $t2, $s1, $s0, 0, 0       # $s0 = a11 * b11 + $s0
add $t0, $t0, $0, $imm1, 1, 0     # j = j+1
add $t1, $t1, $0, $imm1, 4, 0     # i = i+4
blt $0, $t0, $a0, $imm2, 0, CALC # if j < $a0 (not end of calc)  jump to CALC              
sw $s0, $s2, $imm1, $0, 0x120, 0   # save $s0 to mem[0x120+count]
add $s2, $s2, $imm1, $0, 1, 0     # $s2 = count = count +1
bgt $0, $s2, $imm1, $imm2, 4, ROW_2 # if count > 4 jump to ROW_2 
beq $0, $s2, $imm1, $imm2, 4, NEW_ROW_2 # if count == 4 jump to NEW_ROW_2
add $t0, $0, $0, $0, 0, 0         #j = 0
add $t1, $s2, $0, $0, 0, 0        #i = count
add $s0, $0, $0, $0, 0, 0         #$s0 = 0
beq $0, $0, $0, $imm2, 0, CALC     #jump to CALC
NEW_ROW_2:
add $a0, $a0, $imm1, $0, 4, 0    #$a0 = $a0 + 4
ROW_2:
bgt $0, $s2, $imm1, $imm2, 8, ROW_3   # if count > 8 jump to ROW_3 
beq $0, $s2, $imm1, $imm2, 8, NEW_ROW_3 # if count == 8 jump to NEW_ROW_3
add $t0, $0, $0, $imm1, 4, 0      #j = 4
add $t1, $s2, $imm1, $0, -4, 0    #i = count - 4
add $s0, $0, $0, $0, 0, 0         #$s0 = 0
beq $0, $0, $0, $imm2, 0 CALC      #jump to CALC
NEW_ROW_3:
add $a0, $a0, $imm1, $0, 4, 0    #$a0 = $a0 + 4
ROW_3:
bgt $0, $s2, $imm1, $imm2, 12, ROW_4   # if count > 12 jump to ROW_4 
beq $0, $s2, $imm1, $imm2, 12, NEW_ROW_4 # if count == 12 jump to NEW_ROW_4
add $t0, $0, $0, $imm1, 8, 0      #j = 8
add $t1, $s2, $imm1, $0, -8, 0    #i = count - 8
add $s0, $0, $0, $0, 0, 0         #$s0 = 0
beq $0, $0, $0, $imm2, 0, CALC     #jump to CALC 
NEW_ROW_4:
add $a0, $a0, $imm1, $0, 4, 0    #$a0 = $a0 + 4
ROW_4:
bge $0, $s2, $imm1, $imm2, 16, FINISH # if count >= 16 jump to FINISH 
add $t0, $0, $0, $imm1, 12, 0     #j = 12
add $t1, $s2, $imm1, $0, -12, 0   #i = count - 12
add $s0, $0, $0, $0, 0, 0         #$s0 = 0
beq $0, $0, $0, $imm2, 0 CALC      #jump to CALC
FINISH:
halt