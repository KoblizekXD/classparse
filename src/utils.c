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

size_t GetFieldValueSize(Field *field)
{
    if (!field || !field->descriptor) {
        return 0;
    }
    
    char *descriptor = field->descriptor;
    
    switch (descriptor[0]) {
        case 'B': return 1;
        case 'C': return 2;
        case 'D': return 8;
        case 'F': return 4;
        case 'I': return 4;
        case 'J': return 8;
        case 'S': return 2;
        case 'Z': return 1; 
        case 'L': 
        case '[': 
            return sizeof(void*);
        default:
            return 0;
    }
}
