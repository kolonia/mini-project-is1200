  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0, 9
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
hexasc:
	andi $t0, $a0, 0x000f
	sltiu $t1, $t0, 10000
	beq $t1, 1, onedig
	nop
	addi $t0, $t0, 7
onedig: 
	addi $v0, $t0, 0x30
	jr $ra
	nop
	
delay: 
	addi $t1, $0, 40
while:
	beq $a0, $0, escape
	nop
	sub $a0, $a0, 1
	addi $t0, $0, 0
for:
	beq $t0, $t1, while
	nop
	addi $t0, $t0, 1
	j for
escape:
	jr $ra
	nop
	
time2string:
	move $t3, $a0
	PUSH $ra
	PUSH $a1
	
	andi $t2, $0, 0 #counter

notfinished:
	addi $t2, $t2, 1
	andi $a0, $a1, 0xf
	srl $a1, $a1, 4
	jal hexasc
	nop
	PUSH $v0
	bne $t2, 4, notfinished
	nop
	
	move $a0, $t3
	
	POP $t2
	sb $t2, 0($a0)
	POP $t2
	sb $t2, 1($a0)
	addi $t2, $0, 0x3A
	sb $t2, 2($a0)
	POP $t2
	sb $t2, 3($a0)
	POP $t2
	sb $t2, 4($a0)
	addi $t2, $0, 0x00
	sb $t2, 5($a0)
	
	POP $a1
	POP $ra
	
	jr	$ra
	nop
	
	 

