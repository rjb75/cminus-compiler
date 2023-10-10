# Makefile for CPSC 411 Project

scanner-build: file.c file.h logger.c logger.h scanner.c scanner.h
	gcc -Wall -o scanner file.c scanner.c logger.c

scanner-run:
	./scanner -f test.txt

scanner-prod:
	gcc -Wall -D LOG_LEVEL=0 -o scanner file.c scanner.c logger.c

