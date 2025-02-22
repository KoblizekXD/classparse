#include <classparse.h>
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int read_16(FILE *stream, uint16_t *var)
{
    uint16_t temp;
    if (fread(&temp, sizeof(uint16_t), 1, stream) != 1)
        return 0;
    *var = be16toh(temp);
    return 1;
}

int read_32(FILE *stream, uint32_t *var)
{
    uint32_t temp;
    if (fread(&temp, sizeof(uint32_t), 1, stream) != 1)
        return 0;
    *var = be32toh(temp);
    return 1;
}

#define read_8(SAVE_TO) if (fread(&SAVE_TO, sizeof(uint8_t), 1, stream) != 1)
#define checked_malloc(DEF, SIZE) temp_ptr = malloc(SIZE); if (temp_ptr == NULL) { errno = ENOMEM; return NULL; } DEF = temp_ptr

static ConstantPool read_cp(uint16_t count, FILE *stream)
{
    uint16_t ui;
    uint32_t ui32;
    void *temp_ptr;
    checked_malloc(ConstantPool cp, sizeof(ConstantPoolEntry) * count);
    for (uint16_t i = 0; i < count; i++) {
        ConstantPoolEntry *entry = &cp[i];
        read_8(entry->tag) {
            errno = ENOMEM;
            return NULL;
        }
        switch (entry->tag) {
            case CONSTANT_Class:
                read_16(stream, &ui);
                entry->info._class.name = &cp[ui - 1].info.utf8;
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_Methodref:
                read_16(stream, &ui);
                entry->info.member_ref.class_info = &cp[ui - 1].info._class;
                read_16(stream, &ui);
                entry->info.member_ref.name_and_type = &cp[ui - 1].info.name_and_type;
                break;
            case CONSTANT_String:
                read_16(stream, &ui);
                entry->info.string.string = &cp[ui - 1].info.utf8;
                break;
            case CONSTANT_Integer:
            case CONSTANT_Float:
                read_32(stream, &ui32);
                entry->info.int_float = ui32;
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                read_32(stream, &ui32);
                entry->info.long_double.high_bytes = ui32;
                read_32(stream, &ui32);
                entry->info.long_double.low_bytes = ui32;
                i++;
                break;
            case CONSTANT_NameAndType:
                read_16(stream, &ui);
                entry->info.name_and_type.name = &cp[ui - 1].info.utf8;
                read_16(stream, &ui);
                entry->info.name_and_type.descriptor = &cp[ui - 1].info.utf8;
                break;
            case CONSTANT_Utf8:
                read_16(stream, &ui);
                entry->info.utf8 = malloc(ui + 1);
                int result = fread(entry->info.utf8, sizeof(char), ui, stream);
                if (result != ui)
                    return NULL;
                entry->info.utf8[ui] = '\0';
                break;
            case CONSTANT_MethodHandle:
                read_8(entry->info.mh.reference_kind) return NULL;
                read_16(stream, &ui);
                entry->info.mh.member_ref = &cp[ui - 1].info.member_ref;
                break;
            case CONSTANT_MethodType:
                read_16(stream, &ui);
                entry->info.mt.descriptor = &cp[ui - 1].info.utf8;
                break;
            case CONSTANT_Dynamic:
            case CONSTANT_InvokeDynamic:
                read_16(stream, &ui);
                entry->info.inv_dyn.bootstrap_method_attr_index = ui;
                read_16(stream, &ui);
                entry->info.inv_dyn.name_and_type = &cp[ui - 1].info.name_and_type;
                break;
            case CONSTANT_Module:
            case CONSTANT_Package:
                read_16(stream, &ui);
                entry->info.mod_package.name = &cp[ui - 1].info.utf8;
                break;
            default:
                fprintf(stderr, "Cannot interpret constant pool tag: %d\n", entry->tag);
                return NULL;
        }
    }
    return cp;
}

ClassFile *ReadFromStream(FILE *stream)
{
    void *temp_ptr;
    if (stream == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    checked_malloc(ClassFile *cf, sizeof(ClassFile));
    read_32(stream, &cf->magic);
    if (cf->magic != 0xCAFEBABE) {
        errno = EIO;
        return NULL;
    }
    read_16(stream, &cf->minor_version);
    read_16(stream, &cf->major_version);
    read_16(stream, &cf->contant_pool_size);
    cf->contant_pool_size--;
    cf->constant_pool = read_cp(cf->contant_pool_size, stream);
    read_16(stream, &cf->access_flags);
    uint16_t classIndex;
    read_16(stream, &classIndex);
    cf->name = *cf->constant_pool[classIndex - 1].info._class.name;
    read_16(stream, &classIndex);
    if (classIndex == 0)
        cf->super_name = "Object";
    else cf->super_name = *cf->constant_pool[classIndex - 1].info._class.name;
    read_16(stream, &cf->interface_count);
    cf->interfaces = malloc(sizeof(char*) * cf->interface_count);
    for (size_t i = 0; i < cf->interface_count; i++) {
        read_16(stream, &classIndex);
        cf->interfaces[i] = *cf->constant_pool[classIndex - 1].info._class.name;
    }
    return cf;
}
