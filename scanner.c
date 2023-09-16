#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "logger.h"
#include "file.h"

int cli_parser(scanner_main* scanner, int argc, char *argv[]) {
    if(argc < 1) {
        return -1;
    } else if(argc == 1) {
        LogError(__FUNCTION__, __LINE__, "Error parsing arguments");
        return 1;
    }
    
    char* filename = NULL;

    for (int i = 1; i < argc; i++) {
        if(i%2) {
            if(strcmp(argv[i], "-f") == 0) {
                if(i < argc) {
                    filename = argv[i+1];
                }
            }
        }
    }
    
    if(filename == NULL) {
        LogError(__FUNCTION__, __LINE__, "No file provided");
        goto end;
    }
    

    char *data = NULL;
    int32_t readbytes = read_file(filename, NULL, 0);
    printf("read in %d bytes\n", readbytes);
    data = malloc(readbytes*sizeof(char));
    read_file(filename, data, readbytes);
    printf("%s\n", data);
    free(data);

end:
    return 1;
}

int main(int argc, char *argv[]) {
    scanner_main scanner;
    cli_parser(&scanner, argc, argv);
    return 1;
}

