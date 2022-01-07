add $t0, $imm1, $0, $0, -1, 0                # $t0 = x = -1
add $t1, $imm1, $0, $0, -1, 0                # $t1 = y = -1
out $0, $imm1, $0, $imm2, 21, 255           # monitordata = 255
add $s0, $imm1, $0, $0, 128, 0              # $s0 = center = 128
lw $s2, $imm1, $0, $0, 0x100, 0             # $s2 = R
mac $s1, $s2, $s2, $0, 0, 0                 # $s1 = R^2
Y_LOOP:
add $t1, $t1, $imm1, $0, 1, 0               # $t1 = y = y + 1
add $t0, $imm1, $0, $0, -1, 0               # $t0 = x = -1
X_LOOP:
add $t0, $t0, $imm1, $0, 1, 0               # $t0 = x = x + 1 
sub $a1, $t1, $s0, $0, 0, 0                 # $a1 = y-center y distance 
sub $a0, $t0, $s0, $0, 0, 0                 # $a0 = x-center x distance  
mac $a0, $a0, $a0, $0, 0, 0                 # $a0 = ($a0)^2  (x distance)^2
mac $a1, $a1, $a1, $0, 0, 0                 # $a1 = ($a1)^2  (y distance)^2
add $a0, $a0, $a1, $0, 0, 0                 # $a0 = dist^2
ble $0, $a0, $s1, $imm1, WHITE, 0           # if dist^2 <= R^2 jump to WHITE
blt $0, $t0, $imm2, $imm1, X_LOOP, 256      # if x < 256 jump to X_LOOP
blt $0, $t1, $imm2, $imm1, Y_LOOP, 256      # if y < 256 jump to Y_LOOP
halt
WHITE:
mac $a1, $t1, $imm1, $0, 256, 0             # $a1 = y * 256
add $a1, $a1, $t0, $0, 0, 0                 # $a1 = x + (y*256) offset
out $0, $imm1, $0, $a1, 20, 0               # monitoraddr = $a1
out $0, $imm1, $0, $imm2, 22, 1             # monitorcmd  = 1
out $0, $imm1, $0, $imm2, 22, 0             # monitorcmd  = 0 
blt $0, $t0, $imm2, $imm1, X_LOOP, 256      # if x < 256 jump to X_LOOP
blt $0, $t1, $imm2, $imm1, Y_LOOP, 256      # if y < 256 jump to Y_LOOP
halt