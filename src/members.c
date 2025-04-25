#include "utils.h"
#include <classparse.h>
#include <stdint.h>

extern size_t sizeof_attributes(uint8_t **stream, ConstantPool pool, size_t attr_count);
extern AttributeInfo *read_attributes(uint8_t **stream, size_t attr_c, ClassFile *cf, uint8_t **allocation_ptr, CodeAttribute *code);

size_t sizeof_member(uint8_t **stream, ConstantPool pool)
{
    skip(stream, 6);
    uint16_t attr_c = read_u16_ptr(stream);
    return sizeof_attributes(stream, pool, attr_c) + (attr_c * sizeof(AttributeInfo));
}

void set_fields(uint8_t **stream, ClassFile *cf, uint8_t **allocation_ptr)
{
    cf->fields = (void*) *allocation_ptr;
    *allocation_ptr += (sizeof(Field) * cf->field_count);
    for (size_t i = 0; i < cf->field_count; i++) {
        Field *f = &cf->fields[i];
        f->access_flags = read_u16_ptr(stream);
        f->name = cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
        f->descriptor = cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
        f->attribute_count = read_u16_ptr(stream);
        f->attributes = read_attributes(stream, f->attribute_count, cf, allocation_ptr, NULL);
    }
}

void set_methods(uint8_t **stream, ClassFile *cf, uint8_t **allocation_ptr)
{
    cf->methods = (void*) *allocation_ptr;
    *allocation_ptr += (sizeof(Method) * cf->method_count);
    for (size_t i = 0; i < cf->method_count; i++) {
        Method *m = &cf->methods[i];
        m->access_flags = read_u16_ptr(stream);
        m->name = cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
        m->descriptor = cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
        m->attribute_count = read_u16_ptr(stream);
        m->attributes = read_attributes(stream, m->attribute_count, cf, allocation_ptr, NULL);
        AttributeInfo *i = GetAttributeBySyntheticIdentifier(m->attributes, m->attribute_count, ATTR_CODE);
        if (!i) m->code = NULL;
        else m->code = &i->data.code;
    }
}
