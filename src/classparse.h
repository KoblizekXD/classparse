#ifndef CLASSPARSE_H
#define CLASSPARSE_H

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
#define ATTR_UNKNOWN                            0
#define CONSTANT_VALUE                          1
#define CODE                                    2
#define STACK_MAP_TABLE                         3
#define EXCEPTIONS                              4
#define INNER_CLASSES                           5
#define ENCLOSING_METHOD                        6
#define SYNTHETIC                               7
#define SIGNATURE                               8
#define SOURCE_FILE                             9
#define SOURCE_DEBUG_EXTENSION                  10
#define LINE_NUMBER_TABLE                       11
#define LOCAL_VARIABLE_TABLE                    12
#define LOCAL_VARIABLE_TYPE_TABLE               13
#define DEPRECATED                              14
#define RUNTIME_VISIBLE_ANNOTATIONS             15
#define RUNTIME_INVISIBLE_ANNOTATIONS           16
#define RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS   17
#define RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS 18
#define RUNTIME_VISIBLE_TYPE_ANNOTATIONS        19
#define RUNTIME_INVISIBLE_TYPE_ANNOTATIONS      20
#define ANNOTATION_DEFAULT                      21
#define BOOTSTRAP_METHODS                       22
#define METHOD_PARAMETERS                       23
#define MODULE                                  24
#define MODULE_PACKAGES                         25
#define MODULE_MAIN_CLASS                       26
#define NEST_HOST                               27
#define NEST_MEMBERS                            28
#define RECORD                                  29
#define PERMITTED_SUBCLASSES                    30

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
        uint16_t start_pc;
        uint16_t end_pc;
        uint16_t handler_pc;
        uint16_t catch_type;
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
        MethodHandleInfo **bootstrap_args;
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
    uint8_t has_superclass;
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
 */
ClassFile *ReadFromStream(FILE *stream);

/**
 * Attempts to read a standard JVM class file object from the given pointer.
 * The class file is read in big endian format, but the `class_file` structure
 * stores them in the native host endinness. The passing stream is not closed automatically.
 *
 * If the reading fails, the function will return NULL, with corresponding errno set.
 * Otherwise, a dynamically allocated pointer will be returned(don't forget to free it).
 */
ClassFile *ReadFrom(uintptr_t ptr);

/**
 * Frees given class file.
 * No error will appear if operation fails.
 */
void FreeClassFile(ClassFile *cf);

#endif // CLASSPARSE_H
