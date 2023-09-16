#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// reads an entire file
int32_t read_file(char* filename, char* dest, int32_t dest_len);

// writes an entire file
int32_t write_file(char* filename, int32_t length, char* data);

#endif

