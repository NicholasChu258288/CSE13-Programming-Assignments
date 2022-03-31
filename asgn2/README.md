#Assignment 2 - Numerical Integration

## Short Description
	The file mathlib.c containts functions that mimics some of the functions in the math library, math.h.
	The file functions.c uses mathlib.c's functions and formats them into specific functions for 
	integrate.c to integrate using the composite Simpson's 1/3 rule.

## Building
	make

## Running 
	./integrate (function) -p (low) -q (high) -n (partitions)
	
	Function is the option corresponding to the function that you want to integrate over.
	Low is the low end of the iterval you want to integrate over.
	High is the high end of the interval that you want to integrate over.
	Partitions is the number of desired partitions.

##Cleaning
	make clean

## Errors
	If you try calling some functions from functions.c some of the functions might return undefined depending
	on the input, specifically when you try square rooting a negative value or divide by 0.
	Somethings to note:
	Function a will be undefined and causes an error if 1-x^4 is less than 0.
	Function b will be undefined and causse an error when x equals 1.
	Functions g and h will be undefined and causes an error when x equals 0 since you can not divide by 0.
	Function j will be undefined when sin^2(x)+cos^2(x) is negative.
