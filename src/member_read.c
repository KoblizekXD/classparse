#include <classparse.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern int read_16(FILE *stream, uint16_t *var);
extern int read_32(FILE *stream, uint32_t *var);
extern AttributeInfo *read_attributes(FILE *stream, ConstantPool pool, uint16_t length, void *declared_by);

Field *read_fields(FILE *stream, ConstantPool pool, uint16_t length)
{
    Field *fields = malloc(sizeof(Field) * length);
    uint16_t ui;
    for (size_t i = 0; i < length; i++) {
        read_16(stream, &fields[i].access_flags);
        read_16(stream, &ui);
        fields[i].name = pool[ui - 1].info.utf8;
        read_16(stream, &ui);
        fields[i].descriptor = pool[ui - 1].info.utf8;
        read_16(stream, &fields[i].attribute_count);
        fields[i].value = NULL;
        fields[i].attributes = read_attributes(stream, pool, fields[i].attribute_count, NULL);
    }
    return fields;
}

Method *read_methods(FILE *stream, ConstantPool pool, uint16_t length)
{
    Method *methods = malloc(sizeof(Method) * length);
    uint16_t ui;
    for (uint16_t i = 0; i < length; i++) {
        read_16(stream, &methods[i].access_flags);
        read_16(stream, &ui);
        methods[i].name = pool[ui - 1].info.utf8;
        read_16(stream, &ui);
        methods[i].descriptor = pool[ui - 1].info.utf8;
        read_16(stream, &methods[i].attribute_count);
        methods[i].attributes = read_attributes(stream, pool, methods[i].attribute_count, NULL);
    }
    return methods;
}
