#include <classparse.h>
#include <stdint.h>

#ifndef STANDALONE

#include <stdlib.h>
#include <stdio.h>

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

        if (attr)
            method->code = &attr->data.code;
        else method->code = NULL;

    }
    return methods;
}

#else

extern void *malloc(size_t size);

#endif // STANDALONE

extern int read_16_ptr(void *stream, int *cursor, uint16_t *var);
extern int read_32_ptr(void *stream, int *cursor, uint32_t *var);
extern AttributeInfo *read_attributes_ptr(void *stream, int *cursor, ConstantPool pool, uint16_t length, void *declared_by);
AttributeInfo *read_attributes_ptr(void *stream, int *cursor, ConstantPool pool, uint16_t length, void *declared_by);

Field *read_fields_ptr(void *stream, int *cursor, ConstantPool pool, uint16_t length)
{
    Field *fields = malloc(sizeof(Field) * length);
    uint16_t ui;
    for (size_t i = 0; i < length; i++) {
        read_16_ptr(stream, cursor, &fields[i].access_flags);
        read_16_ptr(stream, cursor, &ui);
        fields[i].name = pool[ui - 1].info.utf8;
        read_16_ptr(stream, cursor, &ui);
        fields[i].descriptor = pool[ui - 1].info.utf8;
        fields[i].field_size = GetFieldValueSize(&fields[i]);
        read_16_ptr(stream, cursor, &fields[i].attribute_count);
        fields[i].value = NULL;
        fields[i].attributes = read_attributes_ptr(stream, cursor, pool, fields[i].attribute_count, NULL);
    }
    return fields;
}

Method *read_methods_ptr(void *stream, int *cursor, ConstantPool pool, uint16_t length)
{
    Method *methods = malloc(sizeof(Method) * length);
    for (uint16_t i = 0; i < length; i++) {
        Method *method = &methods[i];
        read_16_ptr(stream, cursor, &method->access_flags);
        uint16_t name_index, descriptor_index;
        read_16_ptr(stream, cursor, &name_index);
        method->name = pool[name_index - 1].info.utf8;
        read_16_ptr(stream, cursor, &descriptor_index);
        method->descriptor = pool[descriptor_index - 1].info.utf8;
        read_16_ptr(stream, cursor, &method->attribute_count);
        method->attributes = read_attributes_ptr(stream, cursor, pool, method->attribute_count, NULL);

        AttributeInfo *attr = GetAttributeBySyntheticIdentifier(method->attributes, method->attribute_count, ATTR_CODE);

        if (attr)
            method->code = &attr->data.code;
        else method->code = NULL;

    }
    return methods;
}
