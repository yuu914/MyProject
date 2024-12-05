@team-05
.data
    msg1:   .asciz "*****Input ID*****\n"
    msg2:   .asciz "**Please Enter Member 1 ID **\n"
    msg3:   .asciz "**Please Enter Member 2 ID **\n"
    msg4:   .asciz "**Please Enter Member 3 ID **\n"
    msg5:   .asciz "**Please Enter Command **\n"
    msg6:   .asciz "*****Print Team Member ID and ID Summation*****\n"
    msg7:   .asciz "ID Summation = %d\n"
    msg8:   .asciz "*****End Print*****\n"
    newline:  .asciz "\n"
    fmtint: .asciz "%d"
    finl:   .asciz "%d\n"

    @id1:    .word 0
    @id2:    .word 0
    @id3:    .word 0
    @idsum:  .word 0

    .text
    .global id
    .global print
    @.global id1
    @.global id2
    @.global id3
    @.global idsum

    id:
        stmfd sp!,{fp,lr}

        ldr r0, =msg1
        bl printf

        ldr r0, =msg2
        bl printf

        ldr r0, =fmtint
        ldr r1, =id1    @address of id1
        bl scanf

        ldr r0, =msg3
        bl printf

        ldr r0, =fmtint
        ldr r1, =id2    @address of id2
        bl scanf

        ldr r0, =msg4
        bl printf

        ldr r0, =fmtint
        ldr r1, =id3    @address of id3
        bl scanf

        ldr r1, =id1    @address of id1
        ldr r1, [r1]    @valude of id1
        ldr r2, =id2
        ldr r2, [r2]
        ldr r3, =id3
        ldr r3, [r3]
        addeq r2, r2, r1
        addeq r3, r3, r2
        ldr r4, =idsum
        str r3, [r4]

        ldr r0, =msg5
        bl printf

        bl getchar
        bl getchar
        cmp r0, #'p'
        it eq
        beq true
        it ne
        bne false

    true:
        ldr r0, =msg6
        bl printf

        ldr r0, =finl
        ldr r1, =id1
	ldr r1, [r1] @ ******corrected*****
        @ldr r1, [r1], r2
        bl printf
        @sub r1,r1,r2

        ldr r0, =finl
        ldr r1, =id2
	ldr r1, [r1] @ *****corrected*****
        @ldr r1, [r1], #4
        bl printf
        @sub r1,r1, #4

        ldr r0, =finl
        ldr r1, =id3
        mov r1, r1, ror #16
        mov r1, r1, ror #16
        mov r2, r1
        mov r1, r1, lsl #2
        mov r1, r1, asr #2
        mov r1, r2
        ldr r1, [r1]
        bl printf

        ldr r0, =newline
        bl printf

        ldr r0, =msg7
        ldr r1, =idsum
        ldr r1,[r1]
        bl printf


        ldr r0,=msg8
        bl printf

        b false

    false:
        ldmfd sp!,{fp,lr}
        mov pc,lr
