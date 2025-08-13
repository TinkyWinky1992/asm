MAIN: mov M1[r2][r7],r2
 add r2,STR
LOOP: jmp END
 prn #-5
 sub r1, r3
 inc K

 mov M1[r3][r3],r3
 bne LOOP
     stop
     
K: .data 22
M1: .mat [2][2] 1,2,3,4