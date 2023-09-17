#include "file.h"
#include "logger.h"

int32_t read_file(char *filename, char *dest, int32_t dest_len) {
    int32_t status = -1, size = 0;
    FILE *file_pointer;

    LogDebug(__FUNCTION__, __LINE__, filename);

    file_pointer = fopen(filename, "r");

    if(file_pointer == NULL) {
        LogError(__FUNCTION__, __LINE__, "Couldn't open file");
        goto end;
    }

    fseek(file_pointer, 0L, SEEK_END);
    size = ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);
    
    if(size == 0) {
        LogError(__FUNCTION__, __LINE__, "File is empty");
        goto end;
    }

    status = size;

    if(dest == NULL) {
       LogDebug(__FUNCTION__, __LINE__, "No destination specified, getting size");
        goto end;
    }
    
    if(dest_len < size) {
        LogWarning(__FUNCTION__, __LINE__, "Destination not large enough");
        goto end;
    }
    
    memset(dest, 0, dest_len);

    fread(dest, size, 1, file_pointer);

end:
    if(file_pointer != NULL) {
        fclose(file_pointer);
    }

    return status;
}

int32_t write_file(char *filename, int32_t length, char *data) {
   int32_t status = -1;
   LogDebug(__FUNCTION__, __LINE__, "Writing file");

    return status;
}

