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
        fields[i].field_size = GetFieldValueSize(&fields[i]);
        read_16(stream, &fields[i].attribute_count);
        fields[i].value = NULL;
        fields[i].attributes = read_attributes(stream, pool, fields[i].attribute_count, NULL);
    }
    return fields;
}

Method *read_methods(FILE *stream, ConstantPool pool, uint16_t length)
{
    Method *methods = malloc(sizeof(Method) * length);
    for (uint16_t i = 0; i < length; i++) {
        Method *method = &methods[i];
        read_16(stream, &method->access_flags);
        uint16_t name_index, descriptor_index;
        read_16(stream, &name_index);
        method->name = pool[name_index - 1].info.utf8;
        read_16(stream, &descriptor_index);
        method->descriptor = pool[descriptor_index - 1].info.utf8;
        read_16(stream, &method->attribute_count);
        method->attributes = read_attributes(stream, pool, method->attribute_count, NULL);

        AttributeInfo *attr = GetAttributeBySyntheticIdentifier(method->attributes, method->attribute_count, ATTR_CODE);

        if (attr) {
            method->code = attr->data.code.code;
            method->code_length = attr->data.code.code_length;
            method->max_stack = attr->data.code.max_stack;
            method->max_locals = attr->data.code.max_locals;
        } else {
            method->code = NULL;
            method->code_length = method->max_stack = method->max_locals = NOCODE;
        }

    }
    return methods;
}
