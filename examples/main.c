#include <stdio.h>
#include <classparse.h>

int main()
{
    FILE* f = fopen("Main.class", "rb");
    ClassFile* cf = ReadFromStream(f);
    fclose(f);
    return 0;
}
