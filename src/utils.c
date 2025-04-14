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

char *GetInstructionName(uint8_t opcode)
{
    switch (opcode) {
        case NOP: return "NOP";
        case ACONST_NULL: return "ACONST_NULL";
        case ICONST_M1: return "ICONST_M1";
        case ICONST_0: return "ICONST_0";
        case ICONST_1: return "ICONST_1";
        case ICONST_2: return "ICONST_2";
        case ICONST_3: return "ICONST_3";
        case ICONST_4: return "ICONST_4";
        case ICONST_5: return "ICONST_5";
        case LCONST_0: return "LCONST_0";
        case LCONST_1: return "LCONST_1";
        case FCONST_0: return "FCONST_0";
        case FCONST_1: return "FCONST_1";
        case FCONST_2: return "FCONST_2";
        case DCONST_0: return "DCONST_0";
        case DCONST_1: return "DCONST_1";
        case BIPUSH: return "BIPUSH";
        case SIPUSH: return "SIPUSH";
        case LDC: return "LDC";
        case LDC_W: return "LDC_W";
        case LDC2_W: return "LDC2_W";
        case ILOAD: return "ILOAD";
        case LLOAD: return "LLOAD";
        case FLOAD: return "FLOAD";
        case DLOAD: return "DLOAD";
        case ALOAD: return "ALOAD";
        case ILOAD_0: return "ILOAD_0";
        case ILOAD_1: return "ILOAD_1";
        case ILOAD_2: return "ILOAD_2";
        case ILOAD_3: return "ILOAD_3";
        case LLOAD_0: return "LLOAD_0";
        case LLOAD_1: return "LLOAD_1";
        case LLOAD_2: return "LLOAD_2";
        case LLOAD_3: return "LLOAD_3";
        case FLOAD_0: return "FLOAD_0";
        case FLOAD_1: return "FLOAD_1";
        case FLOAD_2: return "FLOAD_2";
        case FLOAD_3: return "FLOAD_3";
        case DLOAD_0: return "DLOAD_0";
        case DLOAD_1: return "DLOAD_1";
        case DLOAD_2: return "DLOAD_2";
        case DLOAD_3: return "DLOAD_3";
        case ALOAD_0: return "ALOAD_0";
        case ALOAD_1: return "ALOAD_1";
        case ALOAD_2: return "ALOAD_2";
        case ALOAD_3: return "ALOAD_3";
        case IALOAD: return "IALOAD";
        case LALOAD: return "LALOAD";
        case FALOAD: return "FALOAD";
        case DALOAD: return "DALOAD";
        case AALOAD: return "AALOAD";
        case BALOAD: return "BALOAD";
        case CALOAD: return "CALOAD";
        case SALOAD: return "SALOAD";
        case ISTORE: return "ISTORE";
        case LSTORE: return "LSTORE";
        case FSTORE: return "FSTORE";
        case DSTORE: return "DSTORE";
        case ASTORE: return "ASTORE";
        case ISTORE_0: return "ISTORE_0";
        case ISTORE_1: return "ISTORE_1";
        case ISTORE_2: return "ISTORE_2";
        case ISTORE_3: return "ISTORE_3";
        case LSTORE_0: return "LSTORE_0";
        case LSTORE_1: return "LSTORE_1";
        case LSTORE_2: return "LSTORE_2";
        case LSTORE_3: return "LSTORE_3";
        case FSTORE_0: return "FSTORE_0";
        case FSTORE_1: return "FSTORE_1";
        case FSTORE_2: return "FSTORE_2";
        case FSTORE_3: return "FSTORE_3";
        case DSTORE_0: return "DSTORE_0";
        case DSTORE_1: return "DSTORE_1";
        case DSTORE_2: return "DSTORE_2";
        case DSTORE_3: return "DSTORE_3";
        case ASTORE_0: return "ASTORE_0";
        case ASTORE_1: return "ASTORE_1";
        case ASTORE_2: return "ASTORE_2";
        case ASTORE_3: return "ASTORE_3";
        case IASTORE: return "IASTORE";
        case LASTORE: return "LASTORE";
        case FASTORE: return "FASTORE";
        case DASTORE: return "DASTORE";
        case AASTORE: return "AASTORE";
        case BASTORE: return "BASTORE";
        case CASTORE: return "CASTORE";
        case SASTORE: return "SASTORE";
        case POP: return "POP";
        case POP2: return "POP2";
        case DUP: return "DUP";
        case DUP_X1: return "DUP_X1";
        case DUP_X2: return "DUP_X2";
        case DUP2: return "DUP2";
        case DUP2_X1: return "DUP2_X1";
        case DUP2_X2: return "DUP2_X2";
        case SWAP: return "SWAP";
        case IADD: return "IADD";
        case LADD: return "LADD";
        case FADD: return "FADD";
        case DADD: return "DADD";
        case ISUB: return "ISUB";
        case LSUB: return "LSUB";
        case FSUB: return "FSUB";
        case DSUB: return "DSUB";
        case IMUL: return "IMUL";
        case LMUL: return "LMUL";
        case FMUL: return "FMUL";
        case DMUL: return "DMUL";
        case IDIV: return "IDIV";
        case LDIV: return "LDIV";
        case FDIV: return "FDIV";
        case DDIV: return "DDIV";
        case IREM: return "IREM";
        case LREM: return "LREM";
        case FREM: return "FREM";
        case DREM: return "DREM";
        case INEG: return "INEG";
        case LNEG: return "LNEG";
        case FNEG: return "FNEG";
        case DNEG: return "DNEG";
        case ISHL: return "ISHL";
        case LSHL: return "LSHL";
        case ISHR: return "ISHR";
        case LSHR: return "LSHR";
        case IUSHR: return "IUSHR";
        case LUSHR: return "LUSHR";
        case IAND: return "IAND";
        case LAND: return "LAND";
        case IOR: return "IOR";
        case LOR: return "LOR";
        case IXOR: return "IXOR";
        case LXOR: return "LXOR";
        case IINC: return "IINC";
        case I2L: return "I2L";
        case I2F: return "I2F";
        case I2D: return "I2D";
        case L2I: return "L2I";
        case L2F: return "L2F";
        case L2D: return "L2D";
        case F2I: return "F2I";
        case F2L: return "F2L";
        case F2D: return "F2D";
        case D2I: return "D2I";
        case D2L: return "D2L";
        case D2F: return "D2F";
        case I2B: return "I2B";
        case I2C: return "I2C";
        case I2S: return "I2S";
        case LCMP: return "LCMP";
        case FCMPL: return "FCMPL";
        case FCMPG: return "FCMPG";
        case DCMPL: return "DCMPL";
        case DCMPG: return "DCMPG";
        case IFEQ: return "IFEQ";
        case IFNE: return "IFNE";
        case IFLT: return "IFLT";
        case IFGE: return "IFGE";
        case IFGT: return "IFGT";
        case IFLE: return "IFLE";
        case IF_ICMPEQ: return "IF_ICMPEQ";
        case IF_ICMPNE: return "IF_ICMPNE";
        case IF_ICMPLT: return "IF_ICMPLT";
        case IF_ICMPGE: return "IF_ICMPGE";
        case IF_ICMPGT: return "IF_ICMPGT";
        case IF_ICMPLE: return "IF_ICMPLE";
        case IF_ACMPEQ: return "IF_ACMPEQ";
        case IF_ACMPNE: return "IF_ACMPNE";
        case GETSTATIC: return "GETSTATIC";
        case PUTSTATIC: return "PUTSTATIC";
        case GETFIELD: return "GETFIELD";
        case PUTFIELD: return "PUTFIELD";
        case INVOKEVIRTUAL: return "INVOKEVIRTUAL";
        case INVOKESPECIAL: return "INVOKESPECIAL";
        case INVOKESTATIC: return "INVOKESTATIC";
        case INVOKEINTERFACE: return "INVOKEINTERFACE";
        case INVOKEDYNAMIC: return "INVOKEDYNAMIC";
        case NEW: return "NEW";
        case NEWARRAY: return "NEWARRAY";
        case ANEWARRAY: return "ANEWARRAY";
        case ARRAYLENGTH: return "ARRAYLENGTH";
        case ATHROW: return "ATHROW";
        case CHECKCAST: return "CHECKCAST";
        case INSTANCEOF: return "INSTANCEOF";
        case MONITORENTER: return "MONITORENTER";
        case MONITOREXIT: return "MONITOREXIT";
        case GOTO: return "GOTO";
        case JSR: return "JSR";
        case RET: return "RET";
        case TABLESWITCH: return "TABLESWITCH";
        case LOOKUPSWITCH: return "LOOKUPSWITCH";
        case IRETURN: return "IRETURN";
        case LRETURN: return "LRETURN";
        case FRETURN: return "FRETURN";
        case DRETURN: return "DRETURN";
        case ARETURN: return "ARETURN";
        case RETURN: return "RETURN";
        case WIDE: return "WIDE";
        case MULTIANEWARRAY: return "MULTIANEWARRAY";
        case IFNULL: return "IFNULL";
        case IFNONNULL: return "IFNONNULL";
        case GOTO_W: return "GOTO_W";
        case JSR_W: return "JSR_W";
        case BREAKPOINT: return "BREAKPOINT";
        case IMPDEP1: return "IMPDEP1";
        case IMPDEP2: return "IMPDEP2";
        default: return "UNKNOWN";
    }
}
