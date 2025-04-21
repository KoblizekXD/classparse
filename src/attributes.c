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
