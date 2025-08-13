MAIN: mov M1[r2][r7],LENGTH

 add r1,r2
LOOP: jmp END
 prn #-5
 sub r1 
 inc K

 mov M1[r3][r3],r3
 bne LOOP
 END: stop    
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22
M1: .mat [2][2] 1,2,3,4