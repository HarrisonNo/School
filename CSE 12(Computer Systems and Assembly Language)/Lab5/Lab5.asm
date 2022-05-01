##########################################################################################################################
# Created by: Quiring, Harrison
#		hquiring
#		December 9, 2020
#
# Assignment: Lab 5: Functions and Graphics
#		CSE-12, Computer Systems and Assembly Language
#		UC Santa Cruz, Fall 2020
#
# Description: The program will implement functions that perform some primitive graphics operations on a small simulated display. These functions will clear the entire display to a color, display rectangular and diamond shapes using a memory-mapped bitmap graphics display tool in MARS.
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################################################################

## Macro that stores the value in %reg on the stack 
##  and moves the stack pointer.
.macro push(%reg)
	subi $sp $sp 4
	sw %reg 0($sp)
.end_macro 

# Macro takes the value on the top of the stack and 
#  loads it into %reg then moves the stack pointer.
.macro pop(%reg)
	lw %reg 0($sp)
	addi $sp $sp 4	
.end_macro

# Macro that takes as input coordinates in the format
# (0x00XX00YY) and returns 0x000000XX in %x and 
# returns 0x000000YY in %y
.macro getCoordinates(%input %x %y)
	srl %x, %input, 16 #Shifts right logically by 4 and places the result in %x, giving the correct x coordinates
	sll %y, %input, 16 #Shifts left logically by 4 and places the result in %y, effectively removing the x coordinates
	srl %y, %y, 16 #Shifts right logically by 4 and places the result in %y, giving the correct y cooridnates
.end_macro

# Macro that takes Coordinates in (%x,%y) where
# %x = 0x000000XX and %y= 0x000000YY and
# returns %output = (0x00XX00YY)
.macro formatCoordinates(%output %x %y)
	#Follows the equation of %output=FFFF0000+(2*%y+%x)
	add %y, %y, %y #Effectively multiplying %y by 2
	add %y, %y, %y #Effectively multiplying %y by 4
	add %y, %y, %y #Effectively multiplying %y by 8 
	add %y, %y, %y #Effectively multiplying %y by 16 
	add %y, %y, %y #Effectively multiplying %y by 32 
	add %y, %y, %y #Effectively multiplying %y by 64 
	add %y, %y, %y #Effectively multiplying %y by 128 in total 
	add %output, %x, %y 
	add %output, %output, %output #Effectively multiplying %output by 2
	add %output, %output, %output #Effectively multiplying %output by 4 in total, used to align it to 4
	addi %output, %output, 0xFFFF0000
.end_macro 


.data
originAddress: .word 0xFFFF0000

.text
j done #Ends program 
    
done: nop
	li $v0 10 
	syscall

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#  Subroutines defined below
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#*****************************************************
		#PSEUDOCODE#
#Load number of pixels to place
#Insert location of where memory mapped IO starts
#Start of loop
#Load color into address of pixel
#Increment pixel address by 1
##########################################
#Clear_bitmap: Given a color, will fill the bitmap display with that color.
#   Inputs:
#    $a0 = Color in format (0x00RRGGBB) 
#   Outputs:
#    No register outputs
#    Side-Effects: 
#    Colors the Bitmap display all the same color
#*****************************************************
clear_bitmap: nop
	#Input value is the hex value of the color, this chunk of code is from class 17 lecture, $t0 holds the input value(color value), $t1 holds the number of pixels to place, $t2 is the loop counter, $t3 is the location of the pixels
	move $t0, $a0
	li $t1, 0x4000 #How many pixels to place (128*128)
	add $t2, $zero, $zero #Loop counter
	la $t3, 0xFFFF0000 #Put location of where we are going to start
	
	place_pixels:
		beq $t2, $t1, place_pixelsexit
		sw $t0, ($t3)
		addi $t3, $t3, 4
		addi $t2, $t2, 1
		b place_pixels
	place_pixelsexit:
	
 	jr $ra
#*****************************************************
		#PSEUDOCODE#
#Load the unformated coordinates into getCoordinates
#Load X and Y coordinates into formatCoordinates
#Load color into address of required pixel
####################################################
# draw_pixel:
#  Given a coordinate in $a0, sets corresponding value
#  in memory to the color given by $a1	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of pixel in format (0x00XX00YY)
#    $a1 = color of pixel in format (0x00RRGGBB)
#   Outputs:
#    No register outputs
#*****************************************************
draw_pixel: nop
	
	move $t0, $a0 #Stores the total coordinates in $t0
	move $t1, $a1 #Stores the color needed in $t1
	
	getCoordinates($t0 $t2 $t3) #Stores X coordinates in $t2, Y coordinates $t3
	formatCoordinates($t0 $t2 $t3) #Stores the memory address of the coordinates back into $t0
	
	sw $t1, ($t0) #Paints the color of $t1 into the address of $t0
	
	jr $ra
	
#*****************************************************
		#PSEUDOCODE#
#Load the unformated coordinates into getCoordinates
#Load X and Y coordinates into formatCoordinates
#Load color value of required address into $v0
#############################################
# get_pixel:
#  Given a coordinate, returns the color of that pixel	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of pixel in format (0x00XX00YY)
#   Outputs:
#    Returns pixel color in $v0 in format (0x00RRGGBB)
#*****************************************************
get_pixel: nop
	
	move $t0, $a0 #Stores the coordinates of the pixel in $t0
	
	getCoordinates($t0, $t1, $t2) #Stores X coordinates in $t1, Y coordinates $t2
	formatCoordinates($t0 $t1 $t2) #Stores the memory address of the coordinates back into $t0
	
	lw $v0, 0($t0) #Loads the color found in that pixel address into $v0
	
	jr $ra

#*****************************************************
		#PSEUDOCODE#
#Format coordinates with getCoordinates then formatCoordinates
#Start of height loop
#Start of width loop
#Load the required pixel address with an offset of the height and width counter
#Increment width loop counter
#Increment height loop counter
######################################
#draw_rect: Draws a rectangle on the bitmap display.
#	Inputs:
#		$a0 = coordinates of top left pixel in format (0x00XX00YY)
#		$a1 = width and height of rectangle in format (0x00WW00HH)
#		$a2 = color in format (0x00RRGGBB) 
#	Outputs:
#		No register outputs
#*****************************************************
draw_rect: nop
	
	move $t0, $a0 #Sets $t0 as the top left pixel in unformated coordinates
	move $t1, $a1 #Sets $t1 as the width/height of the rectangle in unformated coordinates
	move $t2, $a2 #Sets $t2 as the color of the rectangle
	
	getCoordinates($t0 $t3 $t4) #Sets $t3 as the x coordinate and $t4 as the y coordinate of the initial pixel, can reuse 3 and 4 and the next formatcoordinates
	formatCoordinates($t0 $t3 $t4)#Sets $t0 as the formated coordinates of the starting pixel
	move $t9, $t0 #Sets $t9 equal to starting pixel so that $t0 can be modified later
	getCoordinates($t1 $t3 $t4) #Sets $t3 as the max width and $t4 as the max height of the rectangle
	
	addi $t5, $zero, 0 #Loop counter for the height loop
	HeightLoop:
		#Exit Condition, needs to loop $t4 times, with $t7 being true/false, $t5 is counter
		slt $t7, $t4, $t5
		beq $t5, $t4 ExitHeightLoop
		beq $t2, 1, ExitHeightLoop
		
		addi $t6, $zero, 0 #Loop counter for the width loop
		WidthLoop:
			#Exit Condition, needs to loop $t3 times, with $t7 being true/false, $t6 is counter
			slt $t7, $t3, $t6
			beq $t3, $t6 ExitWidthLoop
			beq $t2, 1, ExitWidthLoop
			
			move $t8, $t6 #Temporarily sets $t8 to $t6 as otherwise formatcoordinates would mess with the values of the counter
			formatCoordinates($t0 $t5 $t8)
			subi $t0, $t0, 0xFFFF0000 #Removes the base of 0xFFFF0000 to then set it to the value of the starting pixel
			add $t0, $t0, $t9
			
			sw $t2, ($t0) #Paints the color of $t2 into the address of $t0
			
			addi $t6, $t6, 1 #Increment counter by 1 and loop widthloop
			j WidthLoop
		ExitWidthLoop:
		
		add $t7, $zero, 0 #Reset $t7 so it will not mess with HeightLoop
		addi $t5, $t5, 1 #Increment counter by 1 and loop heightloop
		j HeightLoop
	ExitHeightLoop:
	
 	jr $ra

#***********************************************
# draw_diamond:
#  Draw diamond of given height peaking at given point.
#  Note: Assume given height is odd.
#-----------------------------------------------------
	#PSEUDOCODE#
# draw_diamond(height, base_point_x, base_point_y)
# 	for (dy = 0; dy <= h; dy++)
# 		y = base_point_y + dy
#
# 		if dy <= h/2
# 			x_min = base_point_x - dy
# 			x_max = base_point_x + dy
# 		else
# 			x_min = base_point_x - floor(h/2) + (dy - ceil(h/2)) = base_point_x - h + dy
# 			x_max = base_point_x + floor(h/2) - (dy - ceil(h/2)) = base_point_x + h - dy
#
#   		for (x=x_min; x<=x_max; x++) 
# 			draw_diamond_pixels(x, y)
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of top point of diamond in format (0x00XX00YY)
#    $a1 = height of the diamond (must be odd integer)
#    $a2 = color in format (0x00RRGGBB)
#   Outputs:
#    No register outputs
#***************************************************
draw_diamond: nop
	#Width x int start at 0
	#Max width check if 2x+1 equals height
	#Width ends back at 0, use 2 separate loops one increasing the size of width, the other decreasing, in between have the max width?
	#One variable dedicated to shift left
	
	getCoordinates($a0, $t1, $t2) #Stores X coordinates in $t1, Y coordinates $t2
	formatCoordinates($t0 $t1 $t2) #Stores the memory address of the coordinates into $t0 
	getCoordinates($a0, $t1, $t2) #Restores base X coordinates in $t1, Y coordinates $t2
	
	addi $t7, $zero, 0#Sets $t7 as x_min
	addi $t8, $zero, 0#Sets $t8 as x_max
	
	addi $t4, $zero, 2 #Sets $t4 to use as a dividing register, can use again later
	subi $t3, $a1, 1
	div $t3, $t3, $t4 #Effectively sets $t3 to 1/2 of the max height
	addi $t4, $zero, 0 #Reseting $t4
	addi $t5, $zero, 0 #Loop counter for the height loop
	ddHeightLoop:
		#Exit Condition, checking to see if $t5>$a1, uses $t4 as true/false
		slt $t4, $a1, $t5
		beq $t4, 1, ExitddHeightLoop
		
		add $t6, $t2, $t5 #Sets $t6 equal to y=base_y + dy
		
		#If statement, checking to see if $t5>$t3, uses $t4 as true/false
		slt $t4, $t3, $t5
		beq $t4, 1, ElseHeightCheck
			sub $t7, $t1, $t5
			add $t8, $t1, $t5
			j ExitHeightCheck
		ElseHeightCheck:
			sub $t7, $t1, $a1
			add $t7, $t7, $t5
			
			add $t8, $t1, $a1
			sub $t8, $t8, $t5
		ExitHeightCheck:
		addi $t4, $zero, 0 #Reseting $t4
		
		add $t9, $zero, $t7  #Loop counter for nested height loop
		NestedHeightLoop:
			#Exit Condition, checking to see if $t9>$t8, uses $t4 as true/false
			slt $t4, $t8, $t9
			beq $t4, 1, ExitNestedHeightLoop
			
			move $t4, $t6 #Copying $t5 as formatcoordinates changes it
			formatCoordinates($t0 $t9 $t6) #Stores the memory address of the coordinates into $t0, $t9 represents the x value and $t6 represents the y value
			move $t6, $t4 #Resetting $t5 to original value
			addi $t4, $zero, 0 #Resetting $t4
			
			sw $a2, ($t0) #Paints the color of $a2 into the address of $t0
		
			addi $t9, $t9, 1
			j NestedHeightLoop
		ExitNestedHeightLoop:
		addi $t4, $zero, 0 #Reseting $t4
	
		addi $t5, $t5, 1 #Increment counter by 1 and loop heightloop
		j ddHeightLoop
	ExitddHeightLoop:

	jr $ra
