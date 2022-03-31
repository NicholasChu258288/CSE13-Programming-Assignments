## Assignment 6 Huffman Encoding

## Short Description
	Takes input file and compresses the contents using Huffaman encoding and writing a compressed version to an outfile.
	This is done through the encoder, and the decoder takes an encoded file and decodes the contents to another specified
	output file.

## Building
	make all	Makes the encoder and decoder.
	make encode	Makes just the encoder.
	make decode	Makes just the decoding.

## Running
	./encode [hi:o:v] -h -i infile -o outfile -v	Uses Huffman encoding to encode an infile to an outfile, -h is for help
							and -v is for printing statistics like bytes that were writeen and read.
	./decode [hi:o:v] -h -i infile -o outfile -v	Decodes a compressed infile to an outfile, -h is to print help and -v
							is for printing statistics.

## Cleaning
	make clean

## Errros
	So far no mentioned errors in when using scan-build.

