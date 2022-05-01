##########################################################################################################################
# Created by: Quiring, Harrison
#		hquiring
#		December 2, 2020
#
# Assignment: Lab 4: Searching HEX
#		CSE-12, Computer Systems and Assembly Language
#		UC Santa Cruz, Fall 2020
#
# Description: The program will accept up to 8 program arguments in HEX format. The numbers will range from 0x000 up to 0xFFF. These numbers will be converted to decimals. The maximum number will be printed on screen in decimal format.
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################################################################
##PSEUDOCODE##
#Print first prompt "Program arguments:"
#Starting at the initial address of the program arguments 
#Print the program argument
#Increment the address by 4 to reach the address of the next program argument
#Loop previous statements until print all of the program arguments
#Print "Integer values:"
#Large loop that cycles through each program argument
#Smaller nested loop that takes the value of each individual character in a program argument
#Subtract the value of the character to convert the ascii value to hex
#Series of math equations that turn the hex value into decimal
#Add the character's decimal value to the other characters decimal values to see total value for that program argument
#Check to see if that new program argument value is the new max
#Print "Max value:"
#Print the maximum program argument value after comparing each one earlier
.data
	PrArg: .asciiz "Program arguments:\n"
	Space: .asciiz " "
	IntVal: .asciiz "Integer values:\n"
	NewLine: .asciiz "\n"
	MaxVal: .asciiz "Maximum value:\n"
.text

	#Sets $t0 to the number of arguments entered
	move $t0, $a0
	#Sets $t5 to the address of arguments entered
	move $t5, $a1
	#Prints out initial statement of "Program arguments:"
	li $v0, 4
	la $a0, PrArg
	syscall
	
	#Sets ArgumentPrintLoop counter
	addi $t1, $zero, 1
	ArgumentPrintLoop: #$t1 is the tracking register(can resuse), $t2 is true/false(can reuse), $t0 is the max
		#Exit condition, checking to see if $t1>$t0, uses $t2 as true/false
		slt $t2, $t0, $t1
		beq $t2, 1, ExitArgumentPrintLoop
		#need a for loop that prints all of the program arguments
		li $v0, 4
		lw $a0, 0($t5)
		syscall
		li $v0, 4
		la $a0, Space
		syscall
		#Can go to next argument by incrementing by 4
		addi $t5, $t5 4
		#Increment counter by 1 and jump back to loop
		addi $t1, $t1, 1
		j ArgumentPrintLoop
	ExitArgumentPrintLoop:
	move $t5, $a1
	
	li $v0, 4
	la $a0, NewLine
	syscall
	#Goes to new line and prints statement of "Integer Values:"
	li $v0, 4
	la $a0, NewLine
	syscall
	li $v0, 4
	la $a0, IntVal
	syscall
	
	#Sets ReadIndividualProArg counter
	addi $t1, $zero, 1
	addi $t2, $zero, 0
	#Loop that reads each byte of a program argument
	CycleProArg: #$t1 is the tracking register(can resuse), $t2 is true/false(can reuse), $t0 is the max, $t5 is tracking which arg
		#Exit Condition, needs to loop $t0 times
		slt $t2, $t0, $t1
		beq $t2, 1, ExitCycleProArg
		
		#Loop that calculates how many characters are in an argument 
		#Sets the CalcMaxProArg counter, starts at 0
		addi $t3, $zero, 0
		lw $t4, 0($t5)
		CalcMaxProArg: #$t3 is the counting/tracking register that is needed later, $t4 is the char
			#Exit condition, needs to loop until it hits null(which is equal to 0)
			lb $a0, 0($t4)
			beq $a0, 0, ExitCalcMaxProArg
			
			#Increment counter by 1 and $t4 by 1 then loop
			addi $t3, $t3 1
			addi $t4, $t4 1
			j CalcMaxProArg
		ExitCalcMaxProArg:
		
		#Loop that actually does the math to calculate the value of a program argument
		lw $t4, 0($t5)
		addi $t2, $zero, 2
		CalcValueProArg: #Uses $t2 as the counter(can reuse) need to reset at end for callee loop, $t4 is used as the word loader, $t3 is the max, $t6 is true/false, $t9 is the final value, $t7 is a temp value, $t8 is also a temp value
			#Exit condition, checking to see if $t3>=$t2, uses $t6 as true/false
			slt $t6, $t3, $t2
			beq $t3, $t2 ExitCalcProArg
			beq $t6, 1, ExitCalcProArg
			
			#Need to convert value of $t4 from ascii to hex value
			lb $s1, 2($t4)
			#If s1 is greater than 64 
			addi $t7, $zero, 64
			slt $t6, $t7, $s1
			beq $t6, 1, GreaterThanF
			addi $s1, $s1, -48
			j ExitAsciiCon
			GreaterThanF:
			addi $s1, $s1, -55
			ExitAsciiCon:
			
			#Equation needed is $t9+=$a0*16^(4-$t2), setting the exponent here
			add $t7, $zero, $t3
			sub $t7, $t7, $t2 
			addi $t7, $t7, -1
			#Exponent logic
			addi $t8, $zero, 1 #Sets $t8 to 1 in order to multiply and not get 0
			beq $t7, 0, PowerOfZero
			beq $t7, 1, PowerOfOne
			beq $t7, 2, PowerOfTwo
			PowerOfTwo:
			addi $t7, $zero, 16
			mul $t8, $t8, $t7
			
			PowerOfOne:
			addi $t7, $zero, 16
			mul $t8, $t8, $t7
			
			PowerOfZero:
			addi $t7, $zero, 1
			mul $t8, $t8, $t7
			
			
			mul $t8, $t8, $s1
			add $t9, $t9, $t8
			
		
			#Increment counter by 1 and $t4 by 1 then loop
			addi $t4, $t4, 1
			addi $t2, $t2, 1
			j CalcValueProArg
		ExitCalcProArg:
		
		###Check for max value here###
		#checking to see if $t9>$s2, uses $t6 as true/false
		slt $t6, $s2, $t9
		beq $t6, 0, ExitMaxCheck
		add $s2, $zero, $t9
		ExitMaxCheck:
		##############################
		
		#Resetting some variables
		addi $t2, $zero, 0
		li $v0, 1
		move $a0, $t9
		syscall
		#Reset $t9 for next arg
		addi $t9, $zero, 0
		#Printing a space to separate the values
		li $v0, 4
		la $a0, Space
		syscall
	
		#End of ReadIndividualProArg loop
		#Can go to next argument by incrementing by 4
		addi $t5, $t5 4
		#Increment counter by 1 and jump back to loop
		addi $t1, $t1, 1
		j CycleProArg
	ExitCycleProArg:
	
	#Prints max value prompt and value stored at $s2
	li $v0, 4
	la $a0, NewLine
	syscall
	li $v0, 4
	la $a0, NewLine
	syscall
	li $v0, 4
	la $a0, MaxVal
	syscall
	li $v0, 1
	move $a0, $s2
	syscall
	li $v0, 4
	la $a0, NewLine
	syscall		
		
	#Ends program
	li $v0, 10
	syscall
