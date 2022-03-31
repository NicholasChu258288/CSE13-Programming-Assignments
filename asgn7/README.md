#Assignment 7: Author Identification
	
## Short Destription
	The program takes an anonymous text and using a data base of authors with matched texts, analyze the linguistic styles and patterns of the texts and to try and match the anonymous text with the most likely authors. 

## Build
	Typing in "make all" should create "indentify" which is what you will run.

## Running
	./identify [d:n:k:l:emch]

## Command-line Options
	-d      Path to data base, default is lib.db.
	-n      Specify path the noise file, default is noise.txt.
	-k      Specify number of key matches, default is 5.
	-l      Specify number of nose words to use, default is 100.
	-e      Sets distance metric to Euclidean distance, is the default metric.
	-m      Sets metric to Manhattan distance.
	-c      Sets distacne to cosine distance.
	-h      Displays usage.

## Errors
	To check for errors tpye: "scan-build make" and it should show any existing errors.
	Errors found by scan-build:
	Running scan-build with make produced no errors.

	To check for memeory leaks use: "valgrind ./identify [d:n:k:l:emch]"
	Memory leaks detected by valgrind: 
	After testing identify using the small data base it produced no memory, leaks.
	It would be safe to assume it should work fine when working with other data bases as well.

	Errors in identify:
	The distances calculated by identify do not exactly match the provided sample program.
	Top authors printed are still the same but still have the wrong distances, accurate only up to the thousandth and ten thousandth place.
	My best guess to the reason of this issue might be due to an issue with my regex maybe not picking up very sepcific cases, but so far I but have not been able to confim it and don't know where the exact issue is.
