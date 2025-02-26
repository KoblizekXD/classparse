#include <stdio.h>
#include <classparse.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Please provide a path to a classfile as a parameter!\n");
        return 0;
    }
    FILE* f = fopen(argv[1], "rb");
    ClassFile* cf = ReadFromStream(f);
    fclose(f);
    FreeClassFile(cf);
    return 0;
}
