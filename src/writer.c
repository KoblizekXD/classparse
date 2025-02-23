#include <classparse.h>
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int write16(FILE *stream, uint16_t value, int target)
{
    uint16_t val;
    if (target == OUTPUT_BE)
        val = htobe16(value);
    else
        val = htole16(value);
    return fwrite(&val, sizeof(uint16_t), 1, stream) == 1;
}

int write32(FILE *stream, uint32_t value, int target)
{
    uint32_t val;
    if (target == OUTPUT_BE)
        val = htobe32(value);
    else
        val = htole32(value);
    return fwrite(&val, sizeof(uint32_t), 1, stream) == 1;
}

#define checked(x) if (x == 0) return 0

int write_cp(ConstantPool pool, uint16_t size, FILE *stream, int target);

int WriteToStream(ClassFile *cf, FILE *stream, int target)
{
    if ((target != 0 && target != 1) || stream == NULL || cf == NULL) {
        errno = EINVAL;
        return 0;
    }
    checked(write32(stream, cf->magic, target));
    checked(write16(stream, cf->minor_version, target));
    checked(write16(stream, cf->major_version, target));
    checked(write16(stream, cf->contant_pool_size, target));
    return 1;
}

