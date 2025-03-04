#ifndef CLASSPARSE_H
#define CLASSPARSE_H

/*
    This class contains definitions, macros and utility functions which can be used for various things.
    All of these constants are provided in little endian format(as opposed to ASM for example).
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// ===================================================== CONSTANTS

// Types of constant pool entries(tags):
#define CONSTANT_Class	            7
#define CONSTANT_Fieldref	        9
#define CONSTANT_Methodref	        10
#define CONSTANT_InterfaceMethodref	11
#define CONSTANT_String	            8
#define CONSTANT_Integer	        3
#define CONSTANT_Float	            4
#define CONSTANT_Long	            5
#define CONSTANT_Double	            6
#define CONSTANT_NameAndType	    12
#define CONSTANT_Utf8	            1
#define CONSTANT_MethodHandle	    15
#define CONSTANT_MethodType	        16
#define CONSTANT_Dynamic	        17
#define CONSTANT_InvokeDynamic	    18
#define CONSTANT_Module	            19
#define CONSTANT_Package	        20

// Types of access modifiers:
#define ACC_PUBLIC	     0x0001
#define ACC_PRIVATE      0x0002
#define ACC_PROTECTED    0x0004
#define ACC_STATIC       0x0008
#define ACC_FINAL	     0x0010
#define ACC_SUPER	     0x0020
#define ACC_SYNCHRONIZED 0x0020
#define ACC_BRIDGE       0x0040
#define ACC_VOLATILE     0x0040
#define ACC_TRANSIENT    0x0080
#define ACC_VARARGS      0x0080
#define ACC_NATIVE       0x0100
#define ACC_INTERFACE	 0x0200
#define ACC_ABSTRACT	 0x0400
#define ACC_STRICT       0x0800
#define ACC_SYNTHETIC	 0x1000
#define ACC_ANNOTATION	 0x2000
#define ACC_ENUM	     0x4000
#define ACC_MODULE	     0x8000

// Types of attributes
#define ATTR_UNKNOWN                                    0
#define ATTR_CONSTANT_VALUE                             1
#define ATTR_CODE                                       2
#define ATTR_STACK_MAP_TABLE                            3
#define ATTR_EXCEPTIONS                                 4
#define ATTR_INNER_CLASSES                              5
#define ATTR_ENCLOSING_METHOD                           6
#define ATTR_SYNTHETIC                                  7
#define ATTR_SIGNATURE                                  8
#define ATTR_SOURCE_FILE                                9
#define ATTR_SOURCE_DEBUG_EXTENSION                     10
#define ATTR_LINE_NUMBER_TABLE                          11
#define ATTR_LOCAL_VARIABLE_TABLE                       12
#define ATTR_LOCAL_VARIABLE_TYPE_TABLE                  13
#define ATTR_DEPRECATED                                 14  
#define ATTR_RUNTIME_VISIBLE_ANNOTATIONS                15
#define ATTR_RUNTIME_INVISIBLE_ANNOTATIONS              16
#define ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS      17
#define ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS    18
#define ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS           19
#define ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS         20
#define ATTR_ANNOTATION_DEFAULT                         21
#define ATTR_BOOTSTRAP_METHODS                          22
#define ATTR_METHOD_PARAMETERS                          23
#define ATTR_MODULE                                     24
#define ATTR_MODULE_PACKAGES                            25
#define ATTR_MODULE_MAIN_CLASS                          26
#define ATTR_NEST_HOST                                  27
#define ATTR_NEST_MEMBERS                               28
#define ATTR_RECORD                                     29
#define ATTR_PERMITTED_SUBCLASSES                       30

// ===================================================== INSTRUCTION OPCODES

#define AALOAD          0x32
#define AASTORE         0x53
#define ACONST_NULL     0x1
#define ALOAD           0x19
#define ALOAD_0         0x2A
#define ALOAD_1         0x2B
#define ALOAD_2         0x2C
#define ALOAD_3         0x2D
#define ARETURN         0xB0
#define ARRAYLENGTH     0xBE
#define ASTORE          0x3A
#define ASTORE_0        0x4B
#define ASTORE_1        0x4C
#define ASTORE_2        0x4D
#define ASTORE_3        0x4E
#define ATHROW          0xBF
#define BALOAD          0x33
#define BASTORE         0x54
#define BIPUSH          0x10
#define CALOAD          0x34
#define CASTORE         0x55
#define CHECKCAST       0xC0
#define D2F             0x90
#define D2I             0x8E
#define D2L             0x8F
#define DADD            0x63
#define DALOAD          0x31
#define DASTORE         0x52
#define DCMPG           0x98
#define DCMPL           0x97
#define DCONST_0        0xE
#define DCONST_1        0xF
#define DDIV            0x6F
#define DLOAD           0x18
#define DLOAD_0         0x26
#define DLOAD_1         0x27
#define DLOAD_2         0x28
#define DLOAD_3         0x29
#define DMUL            0x6B
#define DNEG            0x77
#define DREM            0x73
#define DRETURN         0xAF
#define DSTORE          0x39
#define DSTORE_0        0x47
#define DSTORE_1        0x48
#define DSTORE_2        0x49
#define DSTORE_3        0x4A
#define DSUB            0x67
#define DUP             0x59
#define DUPX1           0x5A
#define DUPX2           0x5B
#define DUP2            0x5C
#define DUP2_X1         0x5D
#define DUP2_X2         0x5E
#define F2D             0x8D
#define F2I             0x8B
#define F2L             0x8C
#define FADD            0x62
#define FALOAD          0x30
#define FASTORE         0x51
#define FCMPG           0x96
#define FCMPL           0x95
#define FCONST_0        0xB
#define FCONST_1        0xC
#define FCONST_2        0xD
#define FDIV            0x6E
#define FLOAD           0x17
#define FLOAD_0         0x22
#define FLOAD_1         0x23
#define FLOAD_2         0x24
#define FLOAD_3         0x25
#define FMUL            0x6A
#define FNEG            0x76
#define FREM            0x72
#define FRETURN         0xAE
#define FSTORE          0x38
#define FSTORE_0        0x43
#define FSTORE_1        0x44
#define FSTORE_2        0x45
#define FSTORE_3        0x46
#define FSUB            0x66
#define GETFIELD        0xB4
#define GETSTATIC       0xB2
#define GOTO            0xA7
#define GOTO_W          0xC8
#define I2B             0x91
#define I2C             0x92
#define I2D             0x87
#define I2F             0x86
#define I2L             0x85
#define I2S             0x93
#define IADD            0x60
#define IALOAD          0x2E
#define IAND            0x7E
#define IASTORE         0x4F
#define ICONST_M1       0x2
#define ICONST_0        0x3
#define ICONST_1        0x4
#define ICONST_2        0x5
#define ICONST_3        0x6
#define ICONST_4        0x7
#define ICONST_5        0x8
#define IDIV            0x6C
#define IF_ACMPEQ       0xA5
#define IF_ACMPNE       0xA6
#define IF_ICMPEQ       0x9F
#define IF_ICMPNE       0xA0
#define IF_ICMPLT       0xA1
#define IF_ICMPGE       0xA2
#define IF_ICMPGT       0xA3
#define IF_ICMPLE       0xA4
#define IFEQ            0x99
#define IFNE            0x9A
#define IFLT            0x9B
#define IFGE            0x9C
#define IFGT            0x9D
#define IFLE            0x9E
#define IFNONNULL       0xC7
#define IFNULL          0xC6
#define IINC            0x84
#define ILOAD           0x15
#define ILOAD_0         0x1A
#define ILOAD_1         0x1B
#define ILOAD_2         0x1C
#define ILOAD_3         0x1D
#define IMUL            0x68
#define INEG            0x74
#define INSTANCEOF      0xC1
#define INVOKEDYNAMIC   0xBA
#define INVOKEINTERFACE 0xB9
#define INVOKESPECIAL   0xB7
#define INVOKESTATIC    0xB8
#define INVOKEVIRTUAL   0xB6
#define IOR             0x80
#define IREM            0x70
#define IRETURN         0xAC
#define ISHL            0x78
#define ISHR            0x7A
#define ISTORE          0x36
#define ISTORE_0        0x3B
#define ISTORE_1        0x3C
#define ISTORE_2        0x3D
#define ISTORE_3        0x3E
#define ISUB            0x64
#define IUSHR           0x7C
#define IXOR            0x82
#define JSR             0xA8
#define JSR_W           0xC9
#define L2D             0x8A
#define L2F             0x89
#define L2I             0x88
#define LADD            0x61
#define LALOAD          0x2F
#define LAND            0x7F
#define LASTORE         0x50
#define LCMP            0x94
#define LCONST_0        0x9
#define LCONST_1        0xA
#define LDC             0x12
#define LDC_W           0x13
#define LDC2_W          0x14
#define LDIV            0x6D
#define LLOAD           0x16
#define LLOAD_0         0x1E
#define LLOAD_1         0x1F
#define LLOAD_2         0x20
#define LLOAD_3         0x21
#define LMUL            0x69
#define LNEG            0x75
#define LOOKUPSWITCH    0xAB
#define LOR             0x81
#define LREM            0x71
#define LRETURN         0xAD
#define LSHL            0x79
#define LSHR            0x7B
#define LSTORE          0x37
#define LSTORE_0        0x3F
#define LSTORE_1        0x40
#define LSTORE_2        0x41
#define LSTORE_3        0x42
#define LSUB            0x65
#define LUSHR           0x7D
#define LXOR            0x83
#define MONITORENTER    0xC2
#define MONITOREXIT     0xC3
#define MULTIANEWARRAY  0xC5
#define NEW             0xBB
#define NEWARRAY        0xBC
#define NOP             0x0
#define POP             0x57
#define POP2            0x58
#define PUTFIELD        0xB5
#define PUTSTATIC       0xB3
#define RET             0xA9
#define RETURN          0xB1
#define SALOAD          0x35
#define SASTORE         0x56
#define SIPUSH          0x11
#define SWAP            0x5F
#define TABLESWITCH     0xAA
#define WIDE            0xC4

// ===================================================== CONSTANT POOL 

typedef char *Utf8Info;

typedef struct {
    Utf8Info *name;
} ClassInfo;

typedef struct {
    Utf8Info *name;
    Utf8Info *descriptor;
} NameAndTypeInfo;

typedef struct {
    ClassInfo *class_info;
    NameAndTypeInfo *name_and_type;
} MemberRef;

typedef struct {
    Utf8Info *string;
} StringInfo;

typedef struct {
    uint32_t high_bytes;
    uint32_t low_bytes;
} LongDoubleInfo;

typedef uint32_t IntFloatInfo;

typedef struct {
    uint8_t reference_kind;
    MemberRef *member_ref;
} MethodHandleInfo;

typedef struct {
    Utf8Info *descriptor;
} MethodTypeInfo;

typedef struct {
    uint16_t bootstrap_method_attr_index;
    NameAndTypeInfo *name_and_type;
} DynamicInvokeDynamicInfo;

typedef struct {
    Utf8Info *name;
} ModulePackageInfo;

typedef struct {
    uint8_t tag;
    union {
        Utf8Info utf8;
        StringInfo string;
        NameAndTypeInfo name_and_type;
        MemberRef member_ref;
        ClassInfo _class;
        LongDoubleInfo long_double;
        IntFloatInfo int_float;
        MethodHandleInfo mh;
        MethodTypeInfo mt;
        DynamicInvokeDynamicInfo inv_dyn;
        ModulePackageInfo mod_package;
    } info;
} ConstantPoolEntry;

typedef ConstantPoolEntry *ConstantPool;

// ===================================================== ATTRIBUTES

struct _attribute_info;
typedef struct _attribute_info AttributeInfo;

typedef struct {
    ConstantPoolEntry *value;
} ConstantValueAttribute;

typedef struct {
    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_length;
    struct _exc_table {
        uint8_t *start_pc;
        uint8_t *end_pc;
        uint8_t *handler_pc;
        ClassInfo *catch_type;
    } *exception_table;
    uint16_t attributes_count;
    AttributeInfo *attributes;
} CodeAttribute;

// I don't like StackMapTable :p

typedef struct {
    uint16_t exception_length;
    ClassInfo *exceptions;
} ExceptionsAttribute;

typedef struct {
    uint16_t classes_length;
    struct inner_class_info {
        ClassInfo *inner_class_info;
        ClassInfo *outer_class_info;
        Utf8Info  *inner_name;
        uint16_t   inner_class_access_flags;
    } *classes;
} InnerClassesAttribute;

typedef struct {
    uint16_t bootstrap_method_count;
    struct bootstrap_method {
        MethodHandleInfo *bootstrap_method;
        uint16_t bootstrap_arg_length;
        ConstantPoolEntry **bootstrap_args;
    } *bootstrap_methods;
} BootstrapMethodsAttribute;

typedef struct {
    ClassInfo *host_class;
} NestHostAttribute;

typedef struct {
    uint16_t classes_length;
    ClassInfo **classes;
} NestMembersAttribute;

typedef struct {
    uint16_t classes_length;
    ClassInfo **classes;
} PermittedSubclassesAttribute;

typedef struct {} DeprecatedAttribute;

typedef struct {
    uint16_t line_number_table_length;
    struct line_number_table {
        uint8_t *start_pc;
        uint16_t line_number;
    } *line_number_table;
} LineNumberTableAttribute;

typedef struct {
    Utf8Info *sourcefile;
} SourceFileAttribute;

struct _attribute_info {
    Utf8Info attribute_name;
    uint32_t attribute_length;
    uint16_t synth_attribute_type; // Synthetically added, so identifying the attribute is easier. Not actually present in the bytecode
    union {
        ConstantValueAttribute constant_value;
        CodeAttribute code;
        InnerClassesAttribute inner_classes;
        BootstrapMethodsAttribute bootstrap_methods;
        NestHostAttribute nest_host;
        NestMembersAttribute nest_members;
        PermittedSubclassesAttribute permitted_subclasses;
        DeprecatedAttribute deprecated;
        LineNumberTableAttribute line_number_table;
        SourceFileAttribute sourcefile;
    } data;
};

// ===================================================== MEMBERS

typedef struct {
    uint16_t access_flags;
    char *descriptor;
    char *name;
    void *value;
    uint8_t field_size;
    uint16_t attribute_count;
    AttributeInfo *attributes;
} Field;

typedef struct {
    uint16_t access_flags;
    char *descriptor;
    char *name;
    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t *code;
    uint16_t attribute_count;
    AttributeInfo *attributes;
} Method;

#define NOCODE -1

typedef struct {
    uint32_t magic;
    uint16_t minor_version;
    uint16_t major_version;
    uint16_t access_flags;
    uint16_t contant_pool_size;
    ConstantPool constant_pool;
    char *name;
    char *super_name;
    uint16_t interface_count;
    char **interfaces;
    uint16_t field_count;
    Field *fields;
    uint16_t method_count;
    Method *methods;
    uint16_t attribute_count;
    AttributeInfo *attributes;
} ClassFile;

// ===================================================== FUNCTIONS

/**
 * Attempts to read a standard JVM class file object from the given stream.
 * The class file is read in big endian format, but the `class_file` structure
 * stores them in the native host endinness. The passing stream is not closed automatically.
 *
 * If the reading fails, the function will return NULL, with corresponding errno set.
 * Otherwise, a dynamically allocated pointer will be returned(don't forget to free it).
 * Please note that if function fails, all previously allocated resources will leak, so you're better just letting
 * the program crash!
 *
 * @param stream The passing stream, must not be NULL.
 */
ClassFile *ReadFromStream(FILE *stream);

/**
 * Attempts to read a standard JVM class file object from the given pointer.
 * This will fail if the classfile is in big endian format. It is thus recommended to first convert the classfile
 * into a little endian format, as that will allow easy pointer support.
 *
 * If the reading fails, the function will return NULL, with corresponding errno set.
 * Otherwise, a dynamically allocated pointer will be returned(don't forget to free it).
 * Please note that if function fails, all previously allocated resources will leak, so you're better just letting
 * the program crash!
 *
 * @param ptr Pointer on where the reading should start.
 */
ClassFile *ReadFrom(uintptr_t ptr);

#define OUTPUT_LE 0
#define OUTPUT_BE 1

/**
 * Writes a parsed classfile into a writable stream.
 *
 * @param cf The classfile that will be written into a JVM-compatible structure.
 * @param stream The stream to write to.
 * @target Either OUTPUT_LE or OUTPUT_BE, resulting endianness of the classfile.
 */
int WriteToStream(ClassFile *cf, FILE *stream, int target);

/**
 * Frees given class file.
 * No error will appear if operation fails.
 *
 * @param cf Classfile to free.
 */
void FreeClassFile(ClassFile *cf);

/**
 * Locates a method with the given name from the given classfile.
 * If no such method is found, NULL is returned.
 */
Method *GetMethodByName(ClassFile *cf, const char *name);

/**
 * Locates a field with the given name from the given classfile.
 * If no such field is found, NULL is returned.
 */
Field *GetFieldByName(ClassFile *cf, const char *name);

/**
 * Locates the first found attribute with specified name from the provided array.
 * If no such attribute is found, NULL is returned.
 */
AttributeInfo *GetAttributeByName(AttributeInfo *attributes, uint16_t attribute_count, const char *name);

/**
 * Locates the first found attribute with specified synthetic identifier from the provided array.
 * If no such attribute is found, NULL is returned.
 */
AttributeInfo *GetAttributeBySyntheticIdentifier(AttributeInfo *attributes, uint16_t attribute_count, int id);

/**
 * Returns 1 if the attribute with given synthetic identifier is present in the array. 0 Otherwise.
 */
int HasAttributeWithId(AttributeInfo *attributes, uint16_t attribute_count, int id);

/**
 * Returns 1 if the attribute with given name is present in the array. 0 Otherwise.
 */
int HasAttributeWithName(AttributeInfo *attributes, uint16_t attribute_count, const char *name);

/**
 * Returns the size of the field's value in bytes.
 * This depends on the descriptor of the field:
 * B	byte	    1
 * C	char	    2
 * D	double	    8
 * F	float	    4
 * I	int         4
 * J	long	    8
 * S	short	    2
 * Z	boolean	    1
 * L    reference   4-8
 */
size_t GetFieldValueSize(Field *field);

size_t GetParameterCount(Method *method);

#endif // CLASSPARSE_H
