// Utilities for peeking & other essential utility operations.
#include <classparse.h>
#include <stddef.h>
#include <stdint.h>
#include "utils.h"

// Reads from the beginning of constant pool, returns the size required for string buffer in the classfile, this includes null terminators.
size_t string_size(uint8_t **stream, uint16_t count)
{
    size_t total = 0;
    for (size_t i = 0; i < count; i++) {
        size_t tag = *(*stream)++;
         switch (tag) {
            case CONSTANT_Class:
                skip(stream, 2);
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_Methodref:
                skip(stream, 4);
                break;
            case CONSTANT_String:
                skip(stream, 2);
                break;
            case CONSTANT_Integer:
            case CONSTANT_Float:
                skip(stream, 4);
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                skip(stream, 8);
                i++;
                break;
            case CONSTANT_NameAndType:
                skip(stream, 4);
                break;
            case CONSTANT_Utf8: {
                uint16_t size = read_u16_ptr(stream);
                skip(stream, size);
                total += size + 1; // bytes + \0
                break;
            }
            case CONSTANT_MethodHandle:
                skip(stream, 3);
                break;
            case CONSTANT_MethodType:
                skip(stream, 2);
                break;
            case CONSTANT_Dynamic:
            case CONSTANT_InvokeDynamic:
                skip(stream, 4);
                break;
            case CONSTANT_Module:
            case CONSTANT_Package:
                skip(stream, 2);
                break;
            default:
                return 0;
        }
    }
    return total;
}

void read_constant_pool(uint8_t **stream, ConstantPoolEntry *buffer, size_t buffer_count, char *string_buffer)
{
    size_t stroffset = 0;
    uint16_t ui;
    uint32_t ui32;
    for (size_t i = 0; i < buffer_count; i++) {
        ConstantPoolEntry *entry = &buffer[i];
        entry->tag = *(*stream)++;
        switch (entry->tag) {
            case CONSTANT_Class:
                ui = read_u16_ptr(stream);
                entry->info._class.name = &buffer[ui - 1].info.utf8;
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_Methodref:
                ui = read_u16_ptr(stream);
                entry->info.member_ref.class_info = &buffer[ui - 1].info._class;
                ui = read_u16_ptr(stream);
                entry->info.member_ref.name_and_type = &buffer[ui - 1].info.name_and_type;
                break;
            case CONSTANT_String:
                ui = read_u16_ptr(stream);
                entry->info.string.string = &buffer[ui - 1].info.utf8;
                break;
            case CONSTANT_Integer:
            case CONSTANT_Float:
                ui32 = read_u32_ptr(stream);
                entry->info.int_float = ui32;
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                ui32 = read_u32_ptr(stream);
                entry->info.long_double.high_bytes = ui32;
                ui32 = read_u32_ptr(stream);
                entry->info.long_double.low_bytes = ui32;
                i++;
                break;
            case CONSTANT_NameAndType:
                ui = read_u16_ptr(stream);
                entry->info.name_and_type.name = &buffer[ui - 1].info.utf8;
                ui = read_u16_ptr(stream);
                entry->info.name_and_type.descriptor = &buffer[ui - 1].info.utf8;
                break;
            case CONSTANT_Utf8: {
                ui = read_u16_ptr(stream);
                entry->info.utf8 = string_buffer + stroffset;
                for (size_t j = 0; j < ui; j++) {
                    string_buffer[stroffset] = *(*stream);
                    stroffset++;
                    (*stream)++;
                }
                entry->info.utf8[ui] = '\0';
                stroffset++;
                break;
            }
            case CONSTANT_MethodHandle:
                entry->info.mh.reference_kind = *(*stream)++;
                ui = read_u16_ptr(stream);
                entry->info.mh.member_ref = &buffer[ui - 1].info.member_ref;
                break;
            case CONSTANT_MethodType:
                ui = read_u16_ptr(stream);
                entry->info.mt.descriptor = &buffer[ui - 1].info.utf8;
                break;
            case CONSTANT_Dynamic:
            case CONSTANT_InvokeDynamic:
                ui = read_u16_ptr(stream);
                entry->info.inv_dyn.bootstrap_method_attr_index = ui;
                ui = read_u16_ptr(stream);
                entry->info.inv_dyn.name_and_type = &buffer[ui - 1].info.name_and_type;
                break;
            case CONSTANT_Module:
            case CONSTANT_Package:
                ui = read_u16_ptr(stream);
                entry->info.mod_package.name = &buffer[ui - 1].info.utf8;
                break;
            default:
                return;
        }
    }
}

extern size_t sizeof_attributes(uint8_t **stream, ConstantPool pool, size_t attr_count);
extern size_t sizeof_member(uint8_t **stream, ConstantPool pool);

size_t CalculateRequiredSize(uint8_t *ptr)
{
    uint8_t *local_copy = ptr;
    size_t total = sizeof(ClassFile);
    local_copy += 8; 
    uint16_t cpsize = read_u16_ptr(&local_copy) - 1;
    uint8_t *another_copy = local_copy;
    size_t strbuf_len = string_size(&another_copy, cpsize);
    char string_buffer[strbuf_len];
    ConstantPoolEntry pool[cpsize];
    total += (sizeof(ConstantPoolEntry) * cpsize) + strbuf_len;
    read_constant_pool(&local_copy, pool, cpsize, string_buffer);
    local_copy += 6;
    uint16_t if_count = read_u16_ptr(&local_copy);
    local_copy += (sizeof(uint16_t) * if_count);
    total += if_count * sizeof(char*);
    uint16_t member_count = read_u16_ptr(&local_copy);
    total += sizeof(Field) * member_count;
    for (size_t i = 0; i < member_count; i++)
        total += sizeof_member(&local_copy, pool);
    member_count = read_u16_ptr(&local_copy);
    total += sizeof(Method) * member_count;
    for (size_t i = 0; i < member_count; i++)
        total += sizeof_member(&local_copy, pool);
    member_count = read_u16_ptr(&local_copy);
    total += sizeof_attributes(&local_copy, pool, member_count);
    total += member_count * sizeof(AttributeInfo);
    return total;
}
