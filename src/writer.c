#include <classparse.h>
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MINIMAL

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
    checked(write_cp(cf->constant_pool, cf->contant_pool_size, stream, target));
    checked(write16(stream, cf->access_flags, target));
    checked(write16(stream, ((uintptr_t) &cf->constant_pool[0]) - (uintptr_t) cf->name, target));
    return 1;
}

int write_cp(ConstantPool cp, uint16_t count, FILE *stream, int target)
{
    for (uint16_t i = 0; i < count; i++) {
        ConstantPoolEntry *entry = &cp[i];
        if (fwrite(&entry->tag, sizeof(uint8_t), 1, stream) != 1) {
            return 0;
        }
        switch (entry->tag) {
            case CONSTANT_Class:
                if (!write16(stream, (entry->info._class.name - &cp[0].info.utf8) + 1, target))
                    return 0;
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_Methodref:
                if (!write16(stream, (entry->info.member_ref.class_info - &cp[0].info._class) + 1, target) ||
                    !write16(stream, (entry->info.member_ref.name_and_type - &cp[0].info.name_and_type) + 1, target))
                    return 0;
                break;
            case CONSTANT_String:
                if (!write16(stream, (entry->info.string.string - &cp[0].info.utf8) + 1, target))
                    return 0;
                break;
            case CONSTANT_Integer:
            case CONSTANT_Float:
                if (!write32(stream, entry->info.int_float, target))
                    return 0;
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                if (!write32(stream, entry->info.long_double.high_bytes, target) ||
                    !write32(stream, entry->info.long_double.low_bytes, target))
                    return 0;
                i++;
                break;
            case CONSTANT_NameAndType:
                if (!write16(stream, (entry->info.name_and_type.name - &cp[0].info.utf8) + 1, target) ||
                    !write16(stream, (entry->info.name_and_type.descriptor - &cp[0].info.utf8) + 1, target))
                    return 0;
                break;
            case CONSTANT_Utf8: {
                uint16_t length = strlen(entry->info.utf8);
                if (!write16(stream, length, target) ||
                    fwrite(entry->info.utf8, sizeof(char), length, stream) != length)
                    return 0;
                break;
            }
            case CONSTANT_MethodHandle:
                if (fwrite(&entry->info.mh.reference_kind, sizeof(uint8_t), 1, stream) != 1 ||
                    !write16(stream, (entry->info.mh.member_ref - &cp[0].info.member_ref) + 1, target))
                    return 0;
                break;
            case CONSTANT_MethodType:
                if (!write16(stream, (entry->info.mt.descriptor - &cp[0].info.utf8) + 1, target))
                    return 0;
                break;
            case CONSTANT_Dynamic:
            case CONSTANT_InvokeDynamic:
                if (!write16(stream, entry->info.inv_dyn.bootstrap_method_attr_index, target) ||
                    !write16(stream, (entry->info.inv_dyn.name_and_type - &cp[0].info.name_and_type) + 1, target))
                    return 0;
                break;
            case CONSTANT_Module:
            case CONSTANT_Package:
                if (!write16(stream, (entry->info.mod_package.name - &cp[0].info.utf8) + 1, target))
                    return 0;
                break;
            default:
                fprintf(stderr, "Cannot write unknown constant pool tag: %d\n", entry->tag);
                return 0;
        }
    }
    return 1;
}

#endif
