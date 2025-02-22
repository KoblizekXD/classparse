#include <classparse.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern int read_16(FILE *stream, uint16_t *var);
extern int read_32(FILE *stream, uint32_t *var);

uint8_t translate_attribute_name(char *str)
{
    if (strcmp(str, "ConstantValue") == 0) return ATTR_CONSTANT_VALUE;
    if (strcmp(str, "Code") == 0) return ATTR_CODE;
    if (strcmp(str, "StackMapTable") == 0) return ATTR_STACK_MAP_TABLE;
    if (strcmp(str, "Exceptions") == 0) return ATTR_EXCEPTIONS;
    if (strcmp(str, "InnerClasses") == 0) return ATTR_INNER_CLASSES;
    if (strcmp(str, "EnclosingMethod") == 0) return ATTR_ENCLOSING_METHOD;
    if (strcmp(str, "Synthetic") == 0) return ATTR_SYNTHETIC;
    if (strcmp(str, "Signature") == 0) return ATTR_SIGNATURE;
    if (strcmp(str, "SourceFile") == 0) return ATTR_SOURCE_FILE;
    if (strcmp(str, "SourceDebugExtension") == 0) return ATTR_SOURCE_DEBUG_EXTENSION;
    if (strcmp(str, "LineNumberTable") == 0) return ATTR_LINE_NUMBER_TABLE;
    if (strcmp(str, "LocalVariableTable") == 0) return ATTR_LOCAL_VARIABLE_TABLE;
    if (strcmp(str, "LocalVariableTypeTable") == 0) return ATTR_LOCAL_VARIABLE_TYPE_TABLE;
    if (strcmp(str, "Deprecated") == 0) return ATTR_DEPRECATED;
    if (strcmp(str, "RuntimeVisibleAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_ANNOTATIONS;
    if (strcmp(str, "RuntimeInvisibleAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_ANNOTATIONS;
    if (strcmp(str, "RuntimeVisibleParameterAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS;
    if (strcmp(str, "RuntimeInvisibleParameterAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS;
    if (strcmp(str, "RuntimeVisibleTypeAnnotations") == 0) return ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS;
    if (strcmp(str, "RuntimeInvisibleTypeAnnotations") == 0) return ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS;
    if (strcmp(str, "AnnotationDefault") == 0) return ATTR_ANNOTATION_DEFAULT;
    if (strcmp(str, "BootstrapMethods") == 0) return ATTR_BOOTSTRAP_METHODS;
    if (strcmp(str, "MethodParameters") == 0) return ATTR_METHOD_PARAMETERS;
    if (strcmp(str, "Module") == 0) return ATTR_MODULE;
    if (strcmp(str, "ModulePackages") == 0) return ATTR_MODULE_PACKAGES;
    if (strcmp(str, "ModuleMainClass") == 0) return ATTR_MODULE_MAIN_CLASS;
    if (strcmp(str, "NestHost") == 0) return ATTR_NEST_HOST;
    if (strcmp(str, "NestMembers") == 0) return ATTR_NEST_MEMBERS;
    if (strcmp(str, "Record") == 0) return ATTR_RECORD;
    if (strcmp(str, "PermittedSubclasses") == 0) return ATTR_PERMITTED_SUBCLASSES;
    
    return ATTR_UNKNOWN;
}

AttributeInfo *read_attributes(FILE *stream, ConstantPool pool, uint16_t length, void *declared_by)
{
    AttributeInfo *info = malloc(sizeof(AttributeInfo) * length);
    uint16_t ui;
    for (size_t i = 0; i < length; i++) {
        AttributeInfo *item = &info[i];
        read_16(stream, &ui);
        item->attribute_name = pool[ui - 1].info.utf8;
        item->synth_attribute_type = translate_attribute_name(item->attribute_name);
        read_32(stream, &item->attribute_length);
        switch (item->synth_attribute_type) {
            case ATTR_CONSTANT_VALUE: {
                read_16(stream, &ui);
                item->data.constant_value.value = &pool[ui - 1];
                break;
            }
            case ATTR_CODE: {
                read_16(stream, &item->data.code.max_stack);
                read_16(stream, &item->data.code.max_locals);
                read_32(stream, &item->data.code.code_length);
                item->data.code.code = malloc(sizeof(uint8_t) * item->data.code.code_length);
                fread(item->data.code.code, sizeof(uint8_t), item->data.code.code_length, stream);
                read_16(stream, &item->data.code.exception_table_length);
                item->data.code.exception_table = malloc(sizeof(struct _exc_table) * item->data.code.exception_table_length);
                for (uint16_t j = 0; j < item->data.code.exception_table_length; j++) {
                    read_16(stream, &ui);
                    item->data.code.exception_table[j].start_pc = item->data.code.code + (ui - 1);
                    read_16(stream, &ui);
                    item->data.code.exception_table[j].end_pc = item->data.code.code + (ui - 1);
                    read_16(stream, &ui);
                    item->data.code.exception_table[j].handler_pc = item->data.code.code + (ui - 1);
                    read_16(stream, &ui);
                    item->data.code.exception_table[j].catch_type = &pool[ui - 1].info._class;
                }
                read_16(stream, &item->data.code.attributes_count);
                read_attributes(stream, pool, item->data.code.attributes_count, &item->data.code);
                break;
            }
            case ATTR_STACK_MAP_TABLE:
                fseek(stream, item->attribute_length, SEEK_CUR);
                break;
            case ATTR_BOOTSTRAP_METHODS: {
                read_16(stream, &item->data.bootstrap_methods.bootstrap_method_count);
                item->data.bootstrap_methods.bootstrap_methods = malloc(sizeof(struct bootstrap_method) * item->data.bootstrap_methods.bootstrap_method_count);
                for (uint16_t j = 0; j < item->data.bootstrap_methods.bootstrap_method_count; j++) {
                    struct bootstrap_method *method = &item->data.bootstrap_methods.bootstrap_methods[j];
                    read_16(stream, &ui);
                    method->bootstrap_method = &pool[ui - 1].info.mh;
                    read_16(stream, &method->bootstrap_arg_length);
                    method->bootstrap_args = malloc(sizeof(ConstantPoolEntry*) * method->bootstrap_arg_length);
                    for (uint16_t k = 0; k < method->bootstrap_arg_length; k++) {
                        read_16(stream, &ui);
                        method->bootstrap_args[k] = &pool[ui - 1];
                    }
                }
                break;
            }
            case ATTR_NEST_HOST: { 
                read_16(stream, &ui);
                item->data.nest_host.host_class = &pool[ui - 1].info._class;
                break;
            }
            case ATTR_NEST_MEMBERS: {
                read_16(stream, &item->data.nest_members.classes_length);
                item->data.nest_members.classes = malloc(sizeof(ClassInfo*) * item->data.nest_members.classes_length);
                for (uint16_t j = 0; j < item->data.nest_members.classes_length; j++) {
                    read_16(stream, &ui);
                    item->data.nest_members.classes[j] = &pool[ui - 1].info._class;
                }
                break;
            }
            case ATTR_PERMITTED_SUBCLASSES: {
                read_16(stream, &item->data.permitted_subclasses.classes_length);
                item->data.permitted_subclasses.classes = malloc(sizeof(ClassInfo*) * item->data.permitted_subclasses.classes_length);
                for (uint16_t j = 0; j < item->data.permitted_subclasses.classes_length; j++) {
                    read_16(stream, &ui);
                    item->data.permitted_subclasses.classes[j] = &pool[ui - 1].info._class;
                }
                break;
            }
            case ATTR_DEPRECATED:
                break;
            case ATTR_SOURCE_FILE:
                read_16(stream, &ui);
                item->data.sourcefile.sourcefile = &pool[ui - 1].info.utf8;
                break;
            case ATTR_LINE_NUMBER_TABLE: {
                read_16(stream, &item->data.line_number_table.line_number_table_length);
                item->data.line_number_table.line_number_table = malloc(sizeof(struct line_number_table*) * item->data.line_number_table.line_number_table_length);
                for (uint16_t j = 0; j < item->data.line_number_table.line_number_table_length; j++) {
                    read_16(stream, &ui);
                    item->data.line_number_table.line_number_table[j].start_pc = ((CodeAttribute*) declared_by)->code + (ui - 1);
                    read_16(stream, &item->data.line_number_table.line_number_table[j].line_number);
                }
                break;
            }
            default:
                fseek(stream, item->attribute_length, SEEK_CUR);
                break;
        }
    }
    return info;
}
