#include "utils.h"
#include <classparse.h>
#include <stdint.h>

extern void read_constant_pool(uint8_t **stream, ConstantPoolEntry *buffer, size_t buffer_count, char *string_buffer);
extern AttributeInfo *read_attributes(uint8_t **stream, size_t attr_c, ClassFile *cf, uint8_t **allocation_ptr, CodeAttribute *code);
extern size_t string_size(uint8_t **stream, uint16_t count);

extern void set_fields(uint8_t **stream, ClassFile *cf, uint8_t **allocation_ptr);
extern void set_methods(uint8_t **stream, ClassFile *cf, uint8_t **allocation_ptr);

ClassFile *ReadFrom(void *ptr, void *buffer, size_t buffer_size)
{
    uint8_t *stream = ptr;
    uint8_t *allocation_cursor = (uint8_t*) buffer + sizeof(ClassFile);
    ClassFile *cf = buffer;
    uint32_t magic = read_u32_ptr(&stream);
    if (magic != 0xCAFEBABE) return NULL;
    cf->minor_version = read_u16_ptr(&stream);
    cf->major_version = read_u16_ptr(&stream);
    cf->constant_pool_size = read_u16_ptr(&stream) - 1;
    uint8_t *str_stream_copy = stream;
    uint8_t *string_pool = (((uint8_t*) buffer) + buffer_size - string_size(&str_stream_copy, cf->constant_pool_size));
    read_constant_pool(&stream, (void*) allocation_cursor, cf->constant_pool_size, (void*) string_pool);
    cf->constant_pool = (void*) allocation_cursor;
    allocation_cursor += (cf->constant_pool_size * sizeof(ConstantPoolEntry));
    cf->access_flags = read_u16_ptr(&stream);
    uint16_t some_index = read_u16_ptr(&stream); 
    cf->name = *cf->constant_pool[some_index - 1].info._class.name;
    some_index = read_u16_ptr(&stream);
    cf->super_name = *cf->constant_pool[some_index - 1].info._class.name;
    cf->interface_count = read_u16_ptr(&stream);
    cf->interfaces = (char**) allocation_cursor;
    for (size_t i = 0; i < cf->interface_count; i++) {
        some_index = read_u16_ptr(&stream);
        char *name_ptr = *cf->constant_pool[some_index - 1].info._class.name;
        *(char **)allocation_cursor = name_ptr;
        allocation_cursor += sizeof(char*);
    }
    cf->field_count = read_u16_ptr(&stream);
    set_fields(&stream, cf, &allocation_cursor);
    cf->method_count = read_u16_ptr(&stream);
    set_methods(&stream, cf, &allocation_cursor);
    cf->attribute_count = read_u16_ptr(&stream);
    cf->attributes = read_attributes(&stream, cf->attribute_count, cf, &allocation_cursor, NULL);
    return cf;
}
