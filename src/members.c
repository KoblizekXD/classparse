#include "utils.h"
#include <classparse.h>
#include <stdint.h>

extern size_t sizeof_attributes(uint8_t **stream, ConstantPool pool, size_t attr_count);

size_t sizeof_member(uint8_t **stream, ConstantPool pool)
{
    skip(stream, 6);
    uint16_t attr_c = read_u16_ptr(stream);
    return sizeof_attributes(stream, pool, attr_c) + (attr_c * sizeof(AttributeInfo));
}
