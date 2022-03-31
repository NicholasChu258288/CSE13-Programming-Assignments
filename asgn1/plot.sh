#!/bin/bash

#Creating all needed temperary .dat files for plotting.
>/tmp/seqlen.dat
>/tmp/seqmax.dat
>/tmp/seqlenhis.dat

for (( n=2; n < 10001; n++))
do

#Step 1: Putting Collatz sequence of n into the temp .dat file collatz.dat.
make clean && make collatz 
./collatz -n $n > /tmp/collatz.dat

#Step 2: Getting length of Collatz sequence and along with n, adding it to
#seqlen.dat.
printf "%d " $n >> /tmp/seqlen.dat
wc -l < /tmp/collatz.dat >> /tmp/seqlen.dat

#Step 3: Getting the max value of the Collatz sequence and along with n,
#adding it to seqmax.dat.
printf "%d " $n >> /tmp/seqmax.dat
sort -n -r /tmp/collatz.dat | head -n 1 >> /tmp/seqmax.dat

#Step 4: Getting the frequency of lengths of the produced collatz sequences and along with n, adding it to seqlenhis.dat.
wc -l < /tmp/collatz.dat >> /tmp/seqlenhis.dat
done

sort -n /tmp/seqlenhis.dat > /tmp/seqlenhis1.dat
uniq -c /tmp/seqlenhis1.dat | sed 's/^ *//' > /tmp/seqlenhis2.dat

gnuplot <<END
	set terminal pdf
	set output "CollatzSequenceLengths.pdf"
	set title "Collatz Sequence Lengths"
	set xlabel "n"
	set ylabel "length"
	plot "/tmp/seqlen.dat" with dots title ""
END

gnuplot <<END
	set terminal pdf
	set output "MaximumCollatzSequence.pdf"
	set title "Maximum Collatz Sequence Value"
	set xlabel "n"
	set ylabel "value"
	set yrange [0:100000]
	plot "/tmp/seqmax.dat" with dots title ""
END

gnuplot <<END
	set terminal pdf
	set style data histogram
	set style histogram cluster gap 1
	set style fill solid
	set output "CollatzSequenceLengthHistogram.pdf"
	set title "Collatz Sequence Length Histogram"
	set xlabel "length"
	set xrange [0:225]
	set xtics (0,25,50,75,100,125,150,175,200,225)
	set ylabel "frequency"
	set yrange [0:200]
	set ytics (0,25,50,75,100,125,150,175,200)
	plot "/tmp/seqlenhis2.dat" using 1 title ""
END
