.entry hi
.extern something

mcro dosomething
     mov #5, r1
     sub #1, r1
     mov r1, r3

mcroend
;comment


hi: mov r1,r2
LOOP: cmp r1, something
     bne LOOP
     jmp dosomething
     stop
     ;comment
END
