  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
  	
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global delay
.global time2string

hexasc:
	andi $t0, $a0, 0x000f
	sltiu $t1, $t0, 10
	beq $t1, 1, onedig
	addi $t0, $t0, 7
onedig: 
	addi $v0, $t0, 0x30
	jr $ra
	
delay: 
	addi $t1, $0, 4450
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

	
	srl $t3, $t2, 1
	sll $t3, $t3, 1
	beq $t3, $t2, even
	j odd
even: 
	addi $t2, $0, 0x45
	j skipodd
odd:
	addi $t2, $0, 0x44
skipodd:
	sb $t2, 5($a0)
	
	addi $t2, $0, 0x00
	sb $t2, 6($a0)
	
	POP $a1
	POP $ra
	
	jr	$ra
	nop
	

