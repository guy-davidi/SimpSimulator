add $s0, $imm1, $0, $0, 1, 0        # $s0 = 1  now the code will write!
add $t1, $imm1, $0, $0, 0, 0        # $t1 = i = 0
START:
in $t0, $imm1, $0, $0, 17, 0          # $t0 = diskstatus
beq $0, $t0, $imm1, $imm2, 0, CHOOSE  # if diskstatus == 0 jump to CHOOSE
beq $0, $t0, $imm1, $imm2, 1, START   # if diskstatus == 1 jump to START
FINISH:
halt

CHOOSE:
beq $0, $s0, $imm1, $imm2, 1, READ     # if $s0 == 1 jump to READ
beq $0, $s0, $imm1, $imm2, 2, WRITE    # if $s0 == 2 jump to WRITE


READ:
mac $t2, $t1, $imm1, $0, 16, 0      # $t2 = buffer address = i * 16
out $0, $imm1, $0, $t2, 16, 0       # diskbuffer = $t2 = i * 16
out $0, $imm1, $0, $t1, 15, 0       # disksector = i
out $0, $imm1, $0, $imm2, 14, 1     # diskcmd = 1 -> read
out $0, $imm1, $0, $imm2, 14, 0     # diskcmd = 0 -> do nothing
add $t1, $t1, $imm1, $0, 1, 0        # i = i + 1 
blt $0, $t1, $imm1, $imm2, 8, START # if i < 8 jump to START
add $s0, $imm1, $0, $0, 2, 0        # $s0 = 2  now the code will write!
add $t1, $imm1, $0, $0, 0, 0        # $t1 = i = 0
beq $0, $0, $0, $imm1, START, 0     # jump to START

WRITE:
mac $t2, $t1, $imm1, $0, 16, 0        # $t2 = buffer address = i * 16       0   16  32
out $0, $imm1, $0, $t2, 16, 0         # diskbuffer = $t2 = i * 16           0   16  32
add $t1, $t1, $imm1, $0, 1, 0         # i = i + 1
out $0, $imm1, $0, $t1, 15, 0         # disksector = i                      1   2   3
out $0, $imm1, $0, $imm2, 14, 2       # diskcmd = 2 -> write
out $0, $imm1, $0, $imm2, 14, 0       # diskcmd = 0 -> do nothing
blt $0, $t1, $imm1, $imm2, 9, START   # if i < 8 jump to START
halt
out $0, $imm1, $0, $imm2, 1, 1        # irqenable1 = 1
out $0, $imm1, $0, $imm2, 1, 0        # irqenable1 = 0