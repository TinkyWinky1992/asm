MAIN: mov M1[r2][r7],LENGTH
;r99 is not a valid operand and add need two operands
 add r99 
LOOP: jmp END
 prn #-5
 sub r1, r4
 inc K

 mov M1[r3][r3],r3
 bne LOOP
    ;here is a problem (END: label not exsits)
     stop    
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22
M1: .mat [2][2] 1,2,3,4