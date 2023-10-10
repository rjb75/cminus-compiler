#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// reads an entire file
int32_t read_file(char* filename, char* dest, int32_t dest_len);

// opens a file for reading and writing
int32_t open_file(char* filename, FILE** file_pointer);

// writes a string to a file
int32_t write_file(FILE* file_pointer, char* data);

// closes an open file
int32_t close_file(FILE** file_pointer);

#endif

