# Makefile for CPSC 411 Project

scanner-build: file.c file.h logger.c logger.h scanner.c scanner.h
	gcc -Wall -o scanner file.c scanner.c logger.c

scanner-test: scanner-build
	./scanner-tester.sh
