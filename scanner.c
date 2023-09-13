#include <stdio.h>
#include "scanner.h"
#include "logger.h"

int cli_parser(scanner_main* scanner, int argc, char *argv[]) {
    if(argc < 1) {
        return -1;
    } else if(argc == 1) {
        LogError(__FUNCTION__, __LINE__, "Error parsing arguments");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        LogDebug(__FUNCTION__, __LINE__, "got arg"); 
    }
    return 1;
}

int main(int argc, char *argv[]) {
    scanner_main scanner;
    LogWarning(__FUNCTION__, __LINE__, "Hello World");
    LogDebug(__FUNCTION__, __LINE__, "Hello World");
    cli_parser(&scanner, argc, argv);
    return 1;
}

