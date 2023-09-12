#include <stdio.h>
#include "scanner.h"

int cli_parser(scanner_main* scanner, int argc, char *argv[]) {
    if(argc < 1) {
        printf("Error: couldn't read arguments\n");
        return -1;
    } else if(argc == 1) {
        printf("Debug: no arguments provided\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        printf("arg\t %s\n", argv[i]);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    scanner_main scanner;
    printf("Hello World\n");
    cli_scanner(&scanner, argc, argv);
    return 1;
}
