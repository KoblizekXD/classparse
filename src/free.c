#include <classparse.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void free_attribute_array(AttributeInfo *attributes, uint16_t length)
{
    if (attributes == NULL) return;
    for (uint16_t i = 0; i < length; i++) {
        AttributeInfo *item = &attributes[i];
        switch (item->synth_attribute_type) {
            case ATTR_CODE:
                free(item->data.code.code);
                free(item->data.code.exception_table);
                free_attribute_array(item->data.code.attributes, item->data.code.attributes_count);
                break;
            case ATTR_BOOTSTRAP_METHODS:
                for (uint16_t j = 0; j < item->data.bootstrap_methods.bootstrap_method_count; j++) {
                    free(item->data.bootstrap_methods.bootstrap_methods[j].bootstrap_args);
                }
                free(item->data.bootstrap_methods.bootstrap_methods);
                break;
            case ATTR_NEST_MEMBERS:
                free(item->data.nest_members.classes);
                break;
            case ATTR_PERMITTED_SUBCLASSES:
                free(item->data.permitted_subclasses.classes);
                break;
            case ATTR_LINE_NUMBER_TABLE:
                free(item->data.line_number_table.line_number_table);
                break;
            case ATTR_RUNTIME_INVISIBLE_ANNOTATIONS:
                for (size_t j = 0; j < item->data.invisible_annotations.annotations_length; j++) {
                    free(item->data.invisible_annotations.annotations[j].pairs);
                }
                free(item->data.invisible_annotations.annotations);
                break;
        }
    }
    free(attributes);
}

void FreeClassFile(ClassFile *cf)
{
    if (cf == NULL) return;
    for (uint16_t i = 0; i < cf->contant_pool_size; i++) {
        ConstantPoolEntry *entry = &cf->constant_pool[i];
        if (entry->tag == CONSTANT_Long || entry->tag == CONSTANT_Double)
            i++;
        if (entry->tag == CONSTANT_Utf8 && entry->info.utf8[0] != '\0')
            free(entry->info.utf8);
    }
    free(cf->constant_pool);
    free(cf->interfaces);
    for (uint16_t i = 0; i < cf->field_count; i++) {
        free(cf->fields[i].value);
        free_attribute_array(cf->fields[i].attributes, cf->fields[i].attribute_count);
    }
    free(cf->fields);
    for (uint16_t i = 0; i < cf->method_count; i++) {
        free_attribute_array(cf->methods[i].attributes, cf->methods[i].attribute_count);
    }
    free(cf->methods);
    free_attribute_array(cf->attributes, cf->attribute_count);

    free(cf);
}
