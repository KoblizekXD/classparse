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

Method *GetMethodByNameAndDescriptor(ClassFile *cf, const char *name, const char *descriptor)
{
    for (uint16_t i = 0; i < cf->method_count; i++) {
        if (strcmp(cf->methods[i].name, name) == 0 && strcmp(cf->methods[i].descriptor, descriptor) == 0) return &cf->methods[i];
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

char GetReturnType(Method *method)
{
    if (!method || !method->descriptor) return 0;
    char *descriptor = strchr(method->descriptor, ')') + 1;

    return descriptor[0];
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

const char *PRIMITIVE_DESC_TYPES = "BCDFIJSZ";

size_t GetParameterCount(Method *method)
{
    char *descriptor = strchr(method->descriptor, '(') + 1;
    size_t count = 0;
    size_t length = strrchr(method->descriptor, ')') - descriptor;
    if (length == 0) return 0;

    for (size_t i = 0; i < length; i++) {
        char c = descriptor[i];

        if (strchr(PRIMITIVE_DESC_TYPES, c)) {
            count++;
        } else if (c == 'L') {
            i++;
            while (descriptor[i] != ';') {
                i++;
            }
            count++;
        } else if (c == '[') {
            while (descriptor[i] == '[') {
                i++;
            }
            if (descriptor[i] == 'L') {
                while (descriptor[i] != ';') {
                    i++;
                }
            } else i++;
            count++;
            i--;
        }
    }

    return count;
}

size_t GetParameterSize(Method *method, uint16_t offset)
{
    char *descriptor = method->descriptor;
    size_t count = 0;
    size_t length = strlen(descriptor);

    for (size_t i = 0; i < length; i++) {
        char c = descriptor[i];

        if (strchr(PRIMITIVE_DESC_TYPES, c)) {
            if (count == offset) {
                switch (c) {
                    case 'D':
                    case 'J':
                        return 8;
                    case 'B':
                    case 'Z':
                    case 'C':
                        return 1;
                    case 'F':
                    case 'I':
                        return 4;
                    case 'S':
                        return 2;
                }
            }
            count++;
        } else if (c == 'L') {
            i++;
            while (descriptor[i] != ';') {
                i++;
            }
            if (count == offset) {
                return sizeof(void*);
            }
            count++;
        } else if (c == '[') {
            while (descriptor[i] == '[') {
                i++;
            }
            if (descriptor[i] == 'L') {
                while (descriptor[i] != ';') {
                    i++;
                }
            }
            if (count == offset) {
                return sizeof(void*);
            }
            count++;
            i--;
        }
    }

    return -1;
}
