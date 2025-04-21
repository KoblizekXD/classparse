#include <classparse.h>
#include <stdint.h>
#include "utils.h"

uint8_t translate_attribute_name(char *str)
{
    if (classparse_strcmp(str, "ConstantValue") == 0) return ATTR_CONSTANT_VALUE;
    if (classparse_strcmp(str, "Code") == 0) return ATTR_CODE;
    if (classparse_strcmp(str, "StackMapTable") == 0) return ATTR_STACK_MAP_TABLE;
    if (classparse_strcmp(str, "Exceptions") == 0) return ATTR_EXCEPTIONS;
    if (classparse_strcmp(str, "InnerClasses") == 0) return ATTR_INNER_CLASSES;
    if (classparse_strcmp(str, "EnclosingMethod") == 0) return ATTR_ENCLOSING_METHOD;
    if (classparse_strcmp(str, "Synthetic") == 0) return ATTR_SYNTHETIC;
    if (classparse_strcmp(str, "Signature") == 0) return ATTR_SIGNATURE;
    if (classparse_strcmp(str, "SourceFile") == 0) return ATTR_SOURCE_FILE;
    if (classparse_strcmp(str, "SourceDebugExtension") == 0) return ATTR_SOURCE_DEBUG_EXTENSION;
    if (classparse_strcmp(str, "LineNumberTable") == 0) return ATTR_LINE_NUMBER_TABLE;
    if (classparse_strcmp(str, "LocalVariableTable") == 0) return ATTR_LOCAL_VARIABLE_TABLE;
    if (classparse_strcmp(str, "LocalVariableTypeTable") == 0) return ATTR_LOCAL_VARIABLE_TYPE_TABLE;
    if (classparse_strcmp(str, "Deprecated") == 0) return ATTR_DEPRECATED;
    if (classparse_strcmp(str, "RuntimeVisibleAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_ANNOTATIONS;
    if (classparse_strcmp(str, "RuntimeInvisibleAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_ANNOTATIONS;
    if (classparse_strcmp(str, "RuntimeVisibleParameterAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS;
    if (classparse_strcmp(str, "RuntimeInvisibleParameterAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS;
    if (classparse_strcmp(str, "RuntimeVisibleTypeAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS;
    if (classparse_strcmp(str, "RuntimeInvisibleTypeAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS;
    if (classparse_strcmp(str, "AnnotationDefault") == 0) return ATTR_ANNOTATION_DEFAULT;
    if (classparse_strcmp(str, "BootstrapMethods") == 0) return ATTR_BOOTSTRAP_METHODS;
    if (classparse_strcmp(str, "MethodParameters") == 0) return ATTR_METHOD_PARAMETERS;
    if (classparse_strcmp(str, "Module") == 0) return ATTR_MODULE;
    if (classparse_strcmp(str, "ModulePackages") == 0) return ATTR_MODULE_PACKAGES;
    if (classparse_strcmp(str, "ModuleMainClass") == 0) return ATTR_MODULE_MAIN_CLASS;
    if (classparse_strcmp(str, "NestHost") == 0) return ATTR_NEST_HOST;
    if (classparse_strcmp(str, "NestMembers") == 0) return ATTR_NEST_MEMBERS;
    if (classparse_strcmp(str, "Record") == 0) return ATTR_RECORD;
    if (classparse_strcmp(str, "PermittedSubclasses") == 0) return ATTR_PERMITTED_SUBCLASSES;
    
    return ATTR_UNKNOWN;
}

size_t sizeof_annotation(uint8_t **stream);
size_t sizeof_annotation_kv_pair(uint8_t **stream);

size_t sizeof_annotation(uint8_t **stream)
{
    size_t total = 0;
    skip(stream, 2);
    uint16_t num = read_u16_ptr(stream);
    total += num * sizeof(AnnotationKVPair);
    for (size_t k = 0; k < num; k++) {
        skip(stream, 2);
        total += sizeof_annotation_kv_pair(stream);
    }
    return total;
}

size_t sizeof_annotation_kv_pair(uint8_t **stream)
{
    size_t total = 0;
    uint8_t tag = *(*stream)++;
    switch (tag) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 's':
            skip(stream, 2);
            break;
        case 'e':
            skip(stream, 4);
            break;
        case 'c':
            skip(stream, 2);
            break;
        case '@':
            total += sizeof(Annotation);
            total += sizeof_annotation(stream);
            break;
        case '[': {
            uint16_t ct = read_u16_ptr(stream);
            total += ct * sizeof(AnnotationKVPair);
            for (size_t i = 0; i < ct; i++) {
                total += sizeof_annotation_kv_pair(stream);
            }
            break;
        }
    }
    return total;
}

size_t sizeof_attributes(uint8_t **stream, ConstantPool pool, size_t attr_count)
{
    uint16_t ui;
    size_t total = 0;
    for (size_t i = 0; i < attr_count; i++) {
        ui = read_u16_ptr(stream);
        int synth = translate_attribute_name(pool[ui - 1].info.utf8);
        ui = read_u32_ptr(stream);
        switch (synth) {
            case ATTR_CODE: {
                skip(stream, 4);
                uint32_t code_length = read_u32_ptr(stream);
                total += code_length;
                skip(stream, code_length);
                ui = read_u16_ptr(stream);
                total += sizeof(struct _exc_table) * ui;
                skip(stream, 8 * ui);
                ui = read_u16_ptr(stream);
                total += (ui * sizeof(AttributeInfo));
                total += sizeof_attributes(stream, pool, ui);
                break;
            }
            case ATTR_NEST_MEMBERS:
            case ATTR_PERMITTED_SUBCLASSES: {
                ui = read_u16_ptr(stream);
                total += sizeof(ClassInfo*);
                break;
            }
            case ATTR_LINE_NUMBER_TABLE: {
                ui = read_u16_ptr(stream);
                total += sizeof(struct line_number_table) * ui;
                skip(stream, 4 * ui);
                break;
            }
            case ATTR_RUNTIME_INVISIBLE_ANNOTATIONS:
                ui = read_u16_ptr(stream);
                total += ui * sizeof(Annotation);
                for (size_t j = 0; j < ui; j++) {
                    total += sizeof_annotation(stream);
                }
                break;
            default:
                skip(stream, ui);
                break;
        }
    }
    return total;
}

// Read ATTR_C attributes from stream
AttributeInfo *read_attributes(uint8_t **stream, size_t attr_c, ClassFile *cf, uint8_t **allocation_ptr, CodeAttribute *code)
{
    AttributeInfo *start = (void*) *allocation_ptr;
    *allocation_ptr += (sizeof(AttributeInfo) * attr_c);
    for (size_t i = 0; i < attr_c; i++) {
        AttributeInfo *info = &start[i];
        info->attribute_name = cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
        info->synth_attribute_type = translate_attribute_name(info->attribute_name);
        info->attribute_length = read_u32_ptr(stream);
        switch (info->synth_attribute_type) {
            case ATTR_CONSTANT_VALUE:
                info->data.constant_value.value = &cf->constant_pool[read_u16_ptr(stream) - 1];
                break;
            case ATTR_NEST_HOST:
                info->data.nest_host.host_class = &cf->constant_pool[read_u16_ptr(stream) - 1].info._class;
                break;
            case ATTR_SOURCE_FILE:
                info->data.sourcefile.sourcefile = &cf->constant_pool[read_u16_ptr(stream) - 1].info.utf8;
                break;
            case ATTR_NEST_MEMBERS: {
                info->data.nest_members.classes_length = read_u16_ptr(stream);
                
                info->data.nest_members.classes = (ClassInfo**)*allocation_ptr;
                *allocation_ptr += sizeof(ClassInfo*) * info->data.nest_members.classes_length;
                
                for (uint16_t j = 0; j < info->data.nest_members.classes_length; j++) {
                    uint16_t ui = read_u16_ptr(stream);
                    info->data.nest_members.classes[j] = &cf->constant_pool[ui - 1].info._class;
                }
                break;
            }
            case ATTR_PERMITTED_SUBCLASSES: {
                info->data.permitted_subclasses.classes_length = read_u16_ptr(stream);
                
                info->data.permitted_subclasses.classes = (ClassInfo**)*allocation_ptr;
                *allocation_ptr += sizeof(ClassInfo*) * info->data.permitted_subclasses.classes_length;
                
                for (uint16_t j = 0; j < info->data.permitted_subclasses.classes_length; j++) {
                    uint16_t ui = read_u16_ptr(stream);
                    info->data.permitted_subclasses.classes[j] = &cf->constant_pool[ui - 1].info._class;
                }
                break;
            }
            case ATTR_LINE_NUMBER_TABLE:
                info->data.line_number_table.line_number_table_length = read_u16_ptr(stream);
                info->data.line_number_table.line_number_table = (void*) allocation_ptr;
                for (size_t j = 0; j < info->data.line_number_table.line_number_table_length; j++) {
                    info->data.line_number_table.line_number_table[j].start_pc = code->code + read_u16_ptr(stream);
                    info->data.line_number_table.line_number_table[j].line_number = read_u16_ptr(stream);
                }
                *allocation_ptr += sizeof(struct line_number_table) * info->data.line_number_table.line_number_table_length;
                break;
            case ATTR_BOOTSTRAP_METHODS:
                info->data.bootstrap_methods.bootstrap_method_count = read_u16_ptr(stream);
                info->data.bootstrap_methods.bootstrap_methods = (struct bootstrap_method*) *allocation_ptr;
                *allocation_ptr += sizeof(struct bootstrap_method) * info->data.bootstrap_methods.bootstrap_method_count;
                for (uint16_t j = 0; j < info->data.bootstrap_methods.bootstrap_method_count; j++) {
                    struct bootstrap_method *method = &info->data.bootstrap_methods.bootstrap_methods[j];
                    
                    uint16_t ui = read_u16_ptr(stream);
                    method->bootstrap_method = &cf->constant_pool[ui - 1].info.mh;
                    
                    method->bootstrap_arg_length = read_u16_ptr(stream);
                    method->bootstrap_args = (ConstantPoolEntry**)*allocation_ptr;
                    *allocation_ptr += sizeof(ConstantPoolEntry*) * method->bootstrap_arg_length;
                    
                    for (uint16_t k = 0; k < method->bootstrap_arg_length; k++) {
                        ui = read_u16_ptr(stream);
                        method->bootstrap_args[k] = &cf->constant_pool[ui - 1];
                    }
                }
                break;
            case ATTR_CODE: {
                info->data.code.max_stack = read_u16_ptr(stream);
                info->data.code.max_locals = read_u16_ptr(stream);
                info->data.code.code_length = read_u32_ptr(stream);
                info->data.code.code = *allocation_ptr;
                classparse_memcpy(info->data.code.code, *stream, info->data.code.code_length);
                *allocation_ptr += info->data.code.code_length;
                *stream += info->data.code.code_length;
                info->data.code.exception_table_length = read_u16_ptr(stream);
                info->data.code.exception_table = (void*) *allocation_ptr;
                for (size_t j = 0; j < info->data.code.exception_table_length; j++) {
                    info->data.code.exception_table[j].start_pc = info->data.code.code + read_u16_ptr(stream);
                    info->data.code.exception_table[j].end_pc = info->data.code.code + read_u16_ptr(stream);
                    info->data.code.exception_table[j].handler_pc = info->data.code.code + read_u16_ptr(stream);
                    uint16_t ui = read_u16_ptr(stream);
                    if (ui != 0)
                        info->data.code.exception_table[j].catch_type = &cf->constant_pool[ui - 1].info._class;
                    else 
                        info->data.code.exception_table[j].catch_type = NULL;
                }
                info->data.code.attributes_count = read_u16_ptr(stream);
                info->data.code.attributes = read_attributes(stream, info->data.code.attributes_count, cf, allocation_ptr, &info->data.code);
                break;
            }
            default:
                skip(stream, info->attribute_length);
                break;
        }
    }
    return start;
}
