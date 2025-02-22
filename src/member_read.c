#include <classparse.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern int read_16(FILE *stream, uint16_t *var);
extern int read_32(FILE *stream, uint32_t *var);
extern AttributeInfo *read_attributes(FILE *stream, ConstantPool pool, uint16_t length, void *declared_by);

Field *read_fields(uint16_t length)
{
    Field *fields = malloc(sizeof(Field) * length);
    for (size_t i = 0; i < length; i++) {

    }
    return fields;
}
