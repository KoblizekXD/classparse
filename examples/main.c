#include <stdint.h>
#include <stdio.h>
#include <classparse.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Please provide a path to a classfile as a parameter!\n");
        return 0;
    }
    FILE* file = fopen(argv[1], "rb");
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Failed to seek file");
        fclose(file);
        return 1;
    }
    long file_size = ftell(file);
    if (file_size == -1) {
        perror("Failed to get file size");
        fclose(file);
        return 1;
    }

    rewind(file);
    uint8_t* buffer = (uint8_t*) malloc(file_size + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, file_size, file);
    size_t required = CalculateRequiredSize(buffer);
    printf("Bytes required to allocate %s: %zu bytes\n", argv[1], required);
    uint8_t cf_buffer[required];
    ClassFile *cf = ReadFrom(buffer, cf_buffer, required);
    printf("%s\n", *cf->attributes[0].data.sourcefile.sourcefile);
    fclose(file);
    free(buffer);
    return 0;
}
