# Makefile for CPSC 411 Project
common-files: file.c file.h logger.c logger.h

scanner-build: scanner.c scanner.h common-files
	gcc -Wall -o scanner -DMODE_SCANNER file.c scanner.c logger.c

scanner-test: scanner-build
	./scanner-tester.sh

parser-build: parser.c parser.h cminus.y common-files
	bison -do pr.c cminus.y
	gcc -o parser -Wall -DMODE_PARSER scanner.c logger.c file.c parser.c pr.c ast.c

parser-debug: parser.c parser.h cminus.y common-files
	bison -dvto pr.c cminus.y
	gcc -o parser -Wall -DMODE_PARSER scanner.c logger.c file.c parser.c pr.c ast.c

parser-test: parser-build
	./parser-tester.sh
