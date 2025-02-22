#include <classparse.h>
#include <stdint.h>
#include <string.h>

Method *GetMethodByName(ClassFile *cf, const char *name)
{
    for (uint16_t i = 0; i < cf->method_count; i++) {
        if (strcmp(cf->methods[i].name, name) == 0) return &cf->methods[i];
    }
    return NULL;
}

Field *GetFieldByName(ClassFile *cf, const char *name)
{
    for (uint16_t i = 0; i < cf->field_count; i++) {
        if (strcmp(cf->fields[i].name, name) == 0) return &cf->fields[i];
    }
    return NULL;
}

AttributeInfo *GetAttributeByName(AttributeInfo *attributes, uint16_t attribute_count, const char *name)
{
    for (uint16_t i = 0; i < attribute_count; i++) {
        if (strcmp(attributes[i].attribute_name, name) == 0) return &attributes[i];
    }
    return NULL;
}

AttributeInfo *GetAttributeBySyntheticIdentifier(AttributeInfo *attributes, uint16_t attribute_count, int id)
{
    for (uint16_t i = 0; i < attribute_count; i++) {
        if (attributes[i].synth_attribute_type == id) return &attributes[i];
    }
    return NULL;
}

int HasAttributeWithId(AttributeInfo *attributes, uint16_t attribute_count, int id)
{
    for (uint16_t i = 0; i < attribute_count; i++) {
        if (attributes[i].synth_attribute_type == id) return 1;
    }
    return 0;
}

int HasAttributeWithName(AttributeInfo *attributes, uint16_t attribute_count, const char *name)
{
    for (uint16_t i = 0; i < attribute_count; i++) {
        if (strcmp(attributes[i].attribute_name, name) == 0) return 1;
    }
    return 0;
}
